// Copyright 2011 small_duck
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.


#include "Frame.h"

#include "ImagePanel.h"
#include "PageInfo.h"
#include "ScannerDlg.h"
#include "DatabaseDlg.h"
#include "ExportDbDlg.h"
#include "OptionsDlg.h"


BEGIN_EVENT_TABLE(Frame, wxFrame)
  EVT_MENU(ID_FILE_CLEAR, Frame::onClear)
  EVT_MENU(ID_FILE_OPTIONS, Frame::onOptions)
  EVT_MENU(ID_FILE_QUIT, Frame::onQuit)
  EVT_MENU(ID_IMPORT_FILE, Frame::onImportFile)
  EVT_MENU(ID_IMPORT_DEVICE, Frame::onImportDevice)
  EVT_MENU(ID_EXPORT_DB, Frame::onExportDb)
  EVT_MENU(ID_CONFIG_DB, Frame::onConfigureDatabase)
  EVT_MENU(ID_VIEW_ZOOMIN, Frame::onZoomIn)
  EVT_MENU(ID_VIEW_ZOOMOUT, Frame::onZoomOut)
  EVT_MENU(ID_VIEW_ZOOMFIT, Frame::onZoomFit)
  EVT_MENU(ID_VIEW_ZOOMRESET, Frame::onZoomReset)
  EVT_LISTBOX(ID_IMAGE_LIST, Frame::onImageSelected)
END_EVENT_TABLE()


Frame::Frame():
  wxFrame(nullptr, 
          wxID_ANY,
          _("Medoc"), 
          wxDefaultPosition,
          wxSize(800, 600)),
  m_imageList(new wxListBox(this, ID_IMAGE_LIST)),
  m_imagePanel(new ImagePanel(this))
{
  // Menus
  wxMenu * menuFile = new wxMenu;
  menuFile->Append(ID_FILE_CLEAR, _("&Clear"));
  menuFile->Append(ID_FILE_OPTIONS, _("&Options"));
  menuFile->AppendSeparator();
  menuFile->Append(ID_FILE_QUIT, _("E&xit"));
  
  wxMenu * menuImport = new wxMenu;
  menuImport->Append(ID_IMPORT_FILE, _("From &file..."));
  menuImport->Append(ID_IMPORT_DEVICE, _("From &device..."));
  
  wxMenu * menuExport = new wxMenu;
  menuExport->Append(ID_EXPORT_FILE, _("To &file..."));
  menuExport->Append(ID_EXPORT_DB, _("To &database..."));
  menuExport->AppendSeparator();
  menuExport->Append(ID_CONFIG_DB, _("&Configure database...&"));

  wxMenu * menuView = new wxMenu;
  menuView->Append(ID_VIEW_ZOOMIN, _("Zoom &in"));
  menuView->Append(ID_VIEW_ZOOMOUT, _("Zoom &out"));
  menuView->Append(ID_VIEW_ZOOMFIT, _("Zoom to &fit"));
  menuView->Append(ID_VIEW_ZOOMRESET, _("&Reset zoom"));
  
  wxMenuBar * menuBar = new wxMenuBar;
  menuBar->Append(menuFile, _("&File"));
  menuBar->Append(menuImport, _("&Import"));
  menuBar->Append(menuExport, _("&Export"));
  menuBar->Append(menuView, _("&View"));
  
  SetMenuBar(menuBar);
  
  // Frame
  wxBoxSizer * sizer = new wxBoxSizer(wxHORIZONTAL);
  sizer->Add(m_imageList, 0, wxEXPAND);
  sizer->Add(m_imagePanel, 1, wxEXPAND);

  // Show
  SetSizer(sizer);
  Show(true);
}

void Frame::onClear(wxCommandEvent &)
{
  m_imageList->Clear();
  m_imagePanel->clear();
}

void Frame::onOptions(wxCommandEvent &)
{
  OptionsDlg optionsDlg(this);
  optionsDlg.ShowModal();
}

void Frame::onQuit(wxCommandEvent &)
{
  Close();
}

void Frame::onImportFile(wxCommandEvent &)
{
  wxFileDialog fileDialog(this,
                          _("Import file..."),
                          _(""),
                          _(""),
                          _("Image files (*.jpg;*.bmp;*.png)|*.jpg;*.JPG;*.bmp;*.BMP;*.png;*.PNG|All files (*.*)|*.*"),
                          wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE);
  if(fileDialog.ShowModal() == wxID_OK)
  {
    wxArrayString names;
    wxArrayString paths;
    fileDialog.GetFilenames(names);
    fileDialog.GetPaths(paths);
    assert(names.GetCount() == paths.GetCount());
    for(size_t i = 0; i < names.GetCount(); ++i)
    {
      int imageCount = wxImage::GetImageCount(paths.Item(i));
      for(int imageIndex = 0; imageIndex < imageCount; ++imageIndex)
      {
        onImageSelected(m_imageList->Append(names.Item(i), new PageInfo(wxImage(paths.Item(i), wxBITMAP_TYPE_ANY, imageIndex))));
      }
    }
  }
  Layout();
}

void Frame::onImportDevice(wxCommandEvent &)
{
  try
  {
    ScannerDlg scannerDlg(this);
    if(scannerDlg.ShowModal() == wxID_OK)
    {
      for(const wxImage & image : scannerDlg.getImages())
      {
        onImageSelected(m_imageList->Append(_("Scanned image"), new PageInfo(image)));
      }
    }
  }
  catch(const std::exception & e)
  {
    wxMessageDialog message(this, wxString(e.what(), wxConvUTF8));
    message.ShowModal();
  }
  Layout();
}

void Frame::onExportDb(wxCommandEvent &)
{
  bool retry = true;
  while(retry)
  {
    try
    {
      ExportDbDlg exportDbDlg(this, m_config, getImages());
      exportDbDlg.ShowModal();
      retry = false;
    }
    catch(const std::exception & e)
    {
      wxMessageDialog message(this, wxString(e.what(), wxConvUTF8));
      message.ShowModal();
      DatabaseDlg databaseDlg(this);
      if(databaseDlg.ShowModal() == wxID_OK)
      {
        m_config.setDbConfig(databaseDlg.getDbConfig());
      }
      else
      {
        retry = false;
      }
    }
  }
}

void Frame::onConfigureDatabase(wxCommandEvent &)
{
  DatabaseDlg databaseDlg(this);
  if(databaseDlg.ShowModal() == wxID_OK)
  {
    m_config.setDbConfig(databaseDlg.getDbConfig());
  }
}

void Frame::onZoomIn(wxCommandEvent &)
{
  m_imagePanel->zoomIn();
}

void Frame::onZoomOut(wxCommandEvent &)
{
  m_imagePanel->zoomOut();
}

void Frame::onZoomFit(wxCommandEvent &)
{
  m_imagePanel->zoomFit();
}

void Frame::onZoomReset(wxCommandEvent &)
{
  m_imagePanel->resetZoom();
}

void Frame::onImageSelected(wxCommandEvent & event)
{
  if(event.IsSelection())
  {
    m_imagePanel->setImage(static_cast<PageInfo *>(event.GetClientObject())->getImage());  
  }
}

void Frame::onImageSelected(int index)
{
  m_imageList->SetSelection(index);
  m_imagePanel->setImage(static_cast<PageInfo *>(m_imageList->GetClientObject(index))->getImage());  
}

std::vector<wxImage> Frame::getImages() const
{
  std::vector<wxImage> images;
  for(size_t i = 0; i < m_imageList->GetCount(); ++i)
  {
    images.push_back(static_cast<PageInfo *>(m_imageList->GetClientObject(i))->getImage());
  }
  return images;
}
