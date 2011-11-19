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

#include "OptionsDlg.h"

BEGIN_EVENT_TABLE(OptionsDlg, wxDialog)
  EVT_BUTTON(wxID_OK, OptionsDlg::onOk)
END_EVENT_TABLE()

OptionsDlg::OptionsDlg(wxWindow * parent):
  wxDialog(parent, wxID_ANY, _("Options")),
  m_options(new wxNotebook(this, wxID_ANY)),
  m_dbPanel(new DatabaseOptionsPanel(m_options)),
  m_pdfPanel(new PdfOptionsPanel(m_options))
{
  m_options->AddPage(m_dbPanel, _("Database"));
  m_options->AddPage(m_pdfPanel, _("PDF"));
  
  wxBoxSizer * vbox = new wxBoxSizer(wxVERTICAL);
  vbox->Add(m_options, 1, wxEXPAND | wxALL, 7);  
  vbox->Add(CreateStdDialogButtonSizer(wxOK | wxCANCEL), 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 7);

  m_dbPanel->loadConfig();
  m_pdfPanel->loadConfig();

  SetSizer(vbox);
}

void OptionsDlg::onOk(wxCommandEvent & event)
{
  m_dbPanel->saveConfig();
  m_pdfPanel->saveConfig();
  event.Skip();
}
