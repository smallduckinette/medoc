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

#include "LanguageOptionsPanel.h"

#include <wx/config.h>

LanguageOptionsPanel::LanguageOptionsPanel(wxWindow * parent):
  wxPanel(parent, wxID_ANY),
  m_tesseractDataPath(new wxTextCtrl(this, wxID_ANY, wxEmptyString))
{
  wxGridSizer * grid = new wxGridSizer(2, 3);
  grid->Add(new wxStaticText(this, wxID_NEW, _("Tesseract data path")), 0, wxALIGN_CENTER_VERTICAL);
  grid->Add(m_tesseractDataPath, 0, wxEXPAND);

  wxBoxSizer * vbox = new wxBoxSizer(wxVERTICAL);
  vbox->Add(grid, 0, wxEXPAND | wxALL, 7);
  
  SetSizer(vbox);
  vbox->SetSizeHints(this);  
}

void LanguageOptionsPanel::loadConfig()
{
  wxConfig config(_("medoc"));
  
  m_tesseractDataPath->SetValue(config.Read(_("TesseractDataPath"), _("/usr/share/tesseract-ocr/tessdata/")));
}

void LanguageOptionsPanel::saveConfig()
{
  wxConfig config(_("medoc"));
  
  config.Write(_("TesseractDataPath"), m_tesseractDataPath->GetValue());  
}
