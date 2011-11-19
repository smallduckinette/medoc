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

#include "DatabaseOptionsPanel.h"

#include <wx/config.h>

DatabaseOptionsPanel::DatabaseOptionsPanel(wxWindow * parent):
  wxPanel(parent, wxID_NEW),
  m_host(new wxTextCtrl(this, wxID_NEW, wxEmptyString)),
  m_port(new wxSpinCtrl(this, 
                        wxID_NEW, 
                        wxEmptyString, 
                        wxDefaultPosition,
                        wxDefaultSize,
                        wxSP_ARROW_KEYS,
                        1,
                        65535,
                        2345)),
  m_name(new wxTextCtrl(this, wxID_NEW, wxEmptyString)),
  m_login(new wxTextCtrl(this, wxID_NEW)),
  m_password(new wxTextCtrl(this, wxID_NEW, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD)),
  m_account(new wxTextCtrl(this, wxID_NEW))
{
  wxGridSizer * grid = new wxGridSizer(2, 3);
  grid->Add(new wxStaticText(this, wxID_NEW, _("Db host")), 0, wxALIGN_CENTER_VERTICAL);
  grid->Add(m_host, 0, wxEXPAND);
  grid->Add(new wxStaticText(this, wxID_NEW, _("Db port")), 0, wxALIGN_CENTER_VERTICAL);
  grid->Add(m_port, 0, wxEXPAND);
  grid->Add(new wxStaticText(this, wxID_NEW, _("Db name")), 0, wxALIGN_CENTER_VERTICAL);
  grid->Add(m_name, 0, wxEXPAND);
  grid->Add(new wxStaticText(this, wxID_NEW, _("Db login")), 0, wxALIGN_CENTER_VERTICAL);
  grid->Add(m_login, 0, wxEXPAND);
  grid->Add(new wxStaticText(this, wxID_NEW, _("Db password")), 0, wxALIGN_CENTER_VERTICAL);
  grid->Add(m_password, 0, wxEXPAND);
  grid->Add(new wxStaticText(this, wxID_NEW, _("Medoc account")), 0, wxALIGN_CENTER_VERTICAL);
  grid->Add(m_account, 0, wxEXPAND);
  
  wxBoxSizer * vbox = new wxBoxSizer(wxVERTICAL);
  vbox->Add(grid, 0, wxEXPAND | wxALL, 7);
  
  SetSizer(vbox);
  vbox->SetSizeHints(this);
}

void DatabaseOptionsPanel::loadConfig()
{
  wxConfig config(_("medoc"));
  
  m_host->SetValue(config.Read(_("DbHost"), _("localhost")));
  m_port->SetValue(config.Read(_("DbPort"), 2345));
  m_name->SetValue(config.Read(_("DbName"), _("medoc")));
  m_login->SetValue(config.Read(_("DbLogin"), _("")));
  m_password->SetValue(config.Read(_("DbPassword"), _("")));
}

void DatabaseOptionsPanel::saveConfig()
{
  wxConfig config(_("medoc"));
  
  config.Write(_("DbHost"), m_host->GetValue());
  config.Write(_("DbPort"), m_port->GetValue());
  config.Write(_("DbName"), m_name->GetValue());
  config.Write(_("DbLogin"), m_login->GetValue());
  config.Write(_("DbPassword"), m_password->GetValue());
}
