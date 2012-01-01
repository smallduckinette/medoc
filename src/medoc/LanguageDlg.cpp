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


#include "LanguageDlg.h"



LanguageDlg::LanguageDlg(wxWindow * parent):
  wxDialog(parent, wxID_ANY, _("Add language")),
  m_language(new wxTextCtrl(this, wxID_ANY, wxEmptyString)),
  m_tesseractLanguage(new wxTextCtrl(this, wxID_ANY, wxEmptyString)),
  m_postgresLanguage(new wxTextCtrl(this, wxID_ANY, wxEmptyString))
{
  wxGridSizer * grid = new wxGridSizer(2, 3);
  grid->Add(new wxStaticText(this, wxID_NEW, _("Language")), 0, wxALIGN_CENTER_VERTICAL);
  grid->Add(m_language, 0, wxEXPAND);    
  grid->Add(new wxStaticText(this, wxID_NEW, _("Tesseract language")), 0, wxALIGN_CENTER_VERTICAL);
  grid->Add(m_tesseractLanguage, 0, wxEXPAND);
  grid->Add(new wxStaticText(this, wxID_NEW, _("Postgresql language")), 0, wxALIGN_CENTER_VERTICAL);
  grid->Add(m_postgresLanguage, 0, wxEXPAND);
  
  wxBoxSizer * vbox = new wxBoxSizer(wxVERTICAL);
  vbox->Add(grid, 0, wxEXPAND | wxALL, 7);  
  vbox->Add(CreateStdDialogButtonSizer(wxOK | wxCANCEL), 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 7);

  SetSizer(vbox);
  vbox->SetSizeHints(this);  
}

wxString LanguageDlg::getLanguage() const
{
  return m_language->GetValue();
}

wxString LanguageDlg::getTesseractLanguage() const
{
  return m_tesseractLanguage->GetValue();
}

wxString LanguageDlg::getPostgresLanguage() const
{
  return m_postgresLanguage->GetValue();
}
