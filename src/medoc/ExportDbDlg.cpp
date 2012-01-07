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

#include <algorithm>
#include <wx/mstream.h>
#include "MedocDb.h"
#include "LoginDlg.h"
#include "ImageUtils.h"
#include "MedocConfig.h"
#include "Ocr.h"


BEGIN_EVENT_TABLE(ExportDbDlg, wxDialog)
  EVT_BUTTON(ID_BUTTON_EXPORT, ExportDbDlg::onExport)
  EVT_BUTTON(ID_BUTTON_CANCEL, ExportDbDlg::onCancel)
END_EVENT_TABLE()


ExportDbDlg::ExportDbDlg(wxWindow * parent,
                         const std::vector<wxImage> & images):
  wxDialog(parent, wxID_NEW, _("Export to database")),
  m_images(images),
  m_title(new wxTextCtrl(this, wxID_NEW)),
  m_calendar(new wxCalendarCtrl(this, wxID_NEW)),
  m_languages(new wxChoice(this, wxID_NEW))
{
  MedocConfig config;
  for(const MedocConfig::Language & item : config.getLanguages())
  {
    m_languages->Append(item.m_language);
  }
  if(m_languages->GetCount() > 0)
  {
    m_languages->SetSelection(0);
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
    LoginDlg loginDlg(this);
    if(loginDlg.ShowModal() == wxID_OK)
    {
      if(m_medocDb.checkUser(loginDlg.getLogin(),
                             loginDlg.getPassword()))
      {
        wxString tesseractLanguage;
        wxString postgresLanguage;
        getCurrentLanguage(tesseractLanguage, postgresLanguage);
        m_medocDb.createDocument(m_title->GetValue(),
                                 m_calendar->GetDate(),
                                 postgresLanguage,
                                 processImages(tesseractLanguage),
                                 loginDlg.getPassword());
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

std::vector<MedocDb::File> ExportDbDlg::processImages(const wxString & tesseractLanguage) const
{
  Ocr ocr(tesseractLanguage);
  
  std::vector<MedocDb::File> files;
  
  for(const wxImage & image : m_images)
  {
    files.push_back
      (MedocDb::File
       (processImage(image), 
        processImage(scale(image, 80)),
        ocr.recognize(image)));
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

void ExportDbDlg::getCurrentLanguage(wxString & tesseractLanguage, wxString & postgresLanguage)
{
  wxString currentLanguage = m_languages->GetStringSelection();
  MedocConfig config;
  std::vector<MedocConfig::Language> languages = config.getLanguages();
  auto it = std::find_if(languages.begin(),
                         languages.end(),
                         [&currentLanguage](const MedocConfig::Language & language) {return language.m_language == currentLanguage;});
  if(it != languages.end())
  {
    tesseractLanguage = it->m_tesseractLanguage;
    postgresLanguage = it->m_postgresLanguage;
  }
  else
  {
    throw std::runtime_error("Bad current selection");
  }
}
