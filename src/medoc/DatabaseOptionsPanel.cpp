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

#include "MedocConfig.h"

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
                        5432)),
  m_name(new wxTextCtrl(this, wxID_NEW, wxEmptyString)),
  m_login(new wxTextCtrl(this, wxID_NEW)),
  m_password(new wxTextCtrl(this, wxID_NEW, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD))
{
  wxGridSizer * grid = new wxGridSizer(2, 3, 3);
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
  
  wxBoxSizer * vbox = new wxBoxSizer(wxVERTICAL);
  vbox->Add(grid, 0, wxEXPAND | wxALL, 7);
  
  SetSizer(vbox);
  vbox->SetSizeHints(this);
}

void DatabaseOptionsPanel::loadConfig()
{
  MedocConfig config;
  
  m_host->SetValue(config.getHost());
  m_port->SetValue(config.getPort());
  m_name->SetValue(config.getName());
  m_login->SetValue(config.getLogin());
  m_password->SetValue(config.getPassword());
}

void DatabaseOptionsPanel::saveConfig()
{
  MedocConfig config;
  
  config.setHost(m_host->GetValue());
  config.setPort(m_port->GetValue());
  config.setName(m_name->GetValue());
  config.setLogin(m_login->GetValue());
  config.setPassword(m_password->GetValue());
}
