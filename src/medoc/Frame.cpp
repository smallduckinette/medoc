#include "Frame.h"

#include "ImagePanel.h"
#include "PageInfo.h"
#include "ScannerDlg.h"
#include "DatabaseDlg.h"
#include "ExportDbDlg.h"


BEGIN_EVENT_TABLE(Frame, wxFrame)
  EVT_MENU(ID_FILE_QUIT, Frame::onQuit)
  EVT_MENU(ID_IMPORT_FILE, Frame::onImportFile)
  EVT_MENU(ID_IMPORT_DEVICE, Frame::onImportDevice)
  EVT_MENU(ID_EXPORT_DB, Frame::onExportDb)
  EVT_MENU(ID_CONFIG_DB, Frame::onConfigureDatabase)
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
  menuFile->Append(ID_FILE_QUIT, _("E&xit"));
  
  wxMenu * menuImport = new wxMenu;
  menuImport->Append(ID_IMPORT_FILE, _("From &file..."));
  menuImport->Append(ID_IMPORT_DEVICE, _("From &device..."));
  
  wxMenu * menuExport = new wxMenu;
  menuExport->Append(ID_EXPORT_FILE, _("To &file..."));
  menuExport->Append(ID_EXPORT_DB, _("To &database..."));
  menuExport->AppendSeparator();
  menuExport->Append(ID_CONFIG_DB, _("&Configure database...&"));
  
  wxMenuBar * menuBar = new wxMenuBar;
  menuBar->Append(menuFile, _("&File"));
  menuBar->Append(menuImport, _("&Import"));
  menuBar->Append(menuExport, _("&Export"));

  SetMenuBar(menuBar);
  
  // Frame
  wxBoxSizer * sizer = new wxBoxSizer(wxHORIZONTAL);
  sizer->Add(m_imageList, 0, wxEXPAND);
  sizer->Add(m_imagePanel, 1, wxEXPAND);

  // Show
  SetSizer(sizer);
  Show(true);
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
      m_imageList->Append(names.Item(i), new PageInfo(paths.Item(i)));
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
        m_imageList->Append(_("Scanned image"), new PageInfo(image));
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

void Frame::onImageSelected(wxCommandEvent & event)
{
  m_imagePanel->setImage(static_cast<PageInfo *>(event.GetClientObject())->getImage());
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
