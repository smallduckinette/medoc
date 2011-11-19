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

#include "PdfOptionsPanel.h"

PdfOptionsPanel::PdfOptionsPanel(wxWindow * parent):
  wxPanel(parent, wxID_NEW),
  m_dpi(new wxTextCtrl(this, wxID_NEW))
{
  wxGridSizer * grid = new wxGridSizer(2, 3);
  grid->Add(new wxStaticText(this, wxID_NEW, _("Import DBI")), 0, wxALIGN_CENTER_VERTICAL);
  grid->Add(m_dpi, 0, wxEXPAND);  
  
  wxBoxSizer * vbox = new wxBoxSizer(wxVERTICAL);
  vbox->Add(grid, 0, wxEXPAND | wxALL, 7);
  
  SetSizer(vbox);
  vbox->SetSizeHints(this);
}
