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

#include "ExportDbDlg.h"

#include <wx/mstream.h>
#include "MedocDb.h"


BEGIN_EVENT_TABLE(ExportDbDlg, wxDialog)
  EVT_BUTTON(ID_BUTTON_EXPORT, ExportDbDlg::onExport)
  EVT_BUTTON(ID_BUTTON_CANCEL, ExportDbDlg::onCancel)
END_EVENT_TABLE()


ExportDbDlg::ExportDbDlg(wxWindow * parent,
                         const Config & config,
                         const std::vector<wxImage> & images):
  wxDialog(parent, wxID_NEW, _("Export to database")),
  m_config(config),
  m_images(images),
  m_medocDb(config.getDbConfig()),
  m_title(new wxTextCtrl(this, wxID_NEW)),
  m_calendar(new wxCalendarCtrl(this, wxID_NEW)),
  m_languages(new wxChoice(this, wxID_NEW))
{
  for(const wxString & language : m_medocDb.getLanguages())
  {
    m_languages->Append(language);
  }
  
  wxGridSizer * gbox = new wxGridSizer(2, 7);
  gbox->Add(new wxStaticText(this, wxID_NEW, _("Title")),
            0,
            wxALIGN_CENTER_VERTICAL);
  gbox->Add(m_title, 0, wxEXPAND);
  gbox->Add(new wxStaticText(this, wxID_NEW, _("Language")),
            0,
            wxALIGN_CENTER_VERTICAL);
  gbox->Add(m_languages, 0, wxEXPAND);
  
  wxBoxSizer * vbox = new wxBoxSizer(wxVERTICAL);
  vbox->Add(gbox, 0, wxEXPAND | wxALL, 7);
  vbox->Add(m_calendar, 1, wxEXPAND | wxBOTTOM | wxLEFT | wxRIGHT, 7);

  wxBoxSizer * hbox = new wxBoxSizer(wxHORIZONTAL);
  hbox->Add(new wxButton(this, ID_BUTTON_EXPORT, _("Export")), 1, wxEXPAND | wxRIGHT, 3);
  hbox->Add(new wxButton(this, ID_BUTTON_CANCEL, _("Cancel")), 1, wxEXPAND | wxLEFT, 3);
  vbox->Add(hbox, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 7);

  SetSizer(vbox);
  vbox->SetSizeHints(this);
}

void ExportDbDlg::onExport(wxCommandEvent &)
{
  if(m_title->GetValue().IsEmpty())
  {
    wxMessageDialog message(this, _("Please enter a title for your document"));
    message.ShowModal();    
  }
  else if(m_languages->GetSelection() == wxNOT_FOUND)
  {
    wxMessageDialog message(this, _("Please select a language for your document"));
    message.ShowModal();
  }
  else
  {
    wxPasswordEntryDialog passwordDlg(this, _T("Please enter your passphrase"));
    if(passwordDlg.ShowModal() == wxID_OK)
    {
      if(m_medocDb.checkUser(m_config.getDbConfig().getAccount(),
                             passwordDlg.GetValue()))
      {
        m_medocDb.createDocument(m_title->GetValue(),
                                 m_calendar->GetDate(),
                                 m_languages->GetStringSelection(),
                                 processImages(),
                                 passwordDlg.GetValue());
      }
      else
      {
        wxMessageDialog message(this, _("Bad key"));
        message.ShowModal();        
      }
    }
    EndModal(wxID_OK);
  }
}

void ExportDbDlg::onCancel(wxCommandEvent &)
{
  EndModal(wxID_CANCEL);
}

std::vector<MedocDb::File> ExportDbDlg::processImages() const
{
  std::vector<MedocDb::File> files;
  
  for(const wxImage & image : m_images)
  {
    double ratio = std::max(image.GetHeight() / 80, image.GetWidth() / 80);
    
    files.push_back
      (MedocDb::File
       (processImage(image), 
        processImage(image.Scale(image.GetWidth() / ratio, image.GetHeight() / ratio, wxIMAGE_QUALITY_HIGH))));
  }
  
  return files;
}

std::string ExportDbDlg::processImage(const wxImage & image) const
{
  wxMemoryOutputStream memStream;
  image.SaveFile(memStream, _("image/jpeg"));
  std::vector<char> buffer(memStream.GetLength());
  memStream.CopyTo(&buffer.at(0), memStream.GetLength());
  std::string stringBuf;
  std::copy(buffer.begin(),
            buffer.end(),
            std::back_inserter(stringBuf));
  return stringBuf;
}
