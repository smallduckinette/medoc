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

#include "DatabaseDlg.h"

#include "MedocDb.h"


BEGIN_EVENT_TABLE(DatabaseDlg, wxDialog)
  EVT_BUTTON(ID_BUTTON_TEST, DatabaseDlg::onTest)
END_EVENT_TABLE()


DatabaseDlg::DatabaseDlg(wxWindow * parent):
  wxDialog(parent, wxID_NEW, _("Database configuration")),
  m_host(new wxTextCtrl(this, wxID_NEW, _("localhost"))),
  m_port(new wxTextCtrl(this, wxID_NEW, _("5432"))),
  m_name(new wxTextCtrl(this, wxID_NEW, _("medoc"))),
  m_login(new wxTextCtrl(this, wxID_NEW)),
  m_password(new wxTextCtrl(this, wxID_NEW, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD)),
  m_account(new wxTextCtrl(this, wxID_NEW))
{
  wxGridSizer * grid = new wxGridSizer(2);
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
  vbox->Add(new wxButton(this, ID_BUTTON_TEST, _("Test connection")), 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 7);
  vbox->Add(CreateStdDialogButtonSizer(wxOK | wxCANCEL), 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 7);

  SetSizer(vbox);
  vbox->SetSizeHints(this);

  SetSize(400, wxDefaultCoord);
}

DbConfig DatabaseDlg::getDbConfig() const
{
  long port = 0;
  m_port->GetValue().ToLong(&port);
  return DbConfig(m_host->GetValue(),
                  port,
                  m_name->GetValue(),
                  m_login->GetValue(),
                  m_password->GetValue(),
                  m_account->GetValue());
}

void DatabaseDlg::onTest(wxCommandEvent &)
{
  try
  {
    MedocDb medocDb(getDbConfig());
    wxMessageDialog message(this, _("Connection successful"));
    message.ShowModal();
  }
  catch(const std::exception & e)
  {
    std::ostringstream str;
    str << "Connection failed: " << e.what();
    wxMessageDialog message(this, wxString(str.str().c_str(), wxConvUTF8));
    message.ShowModal();    
  }
}
