#include "DatabaseDlg.h"


BEGIN_EVENT_TABLE(DatabaseDlg, wxDialog)
  EVT_BUTTON(ID_BUTTON_OK, DatabaseDlg::onOk)
  EVT_BUTTON(ID_BUTTON_CANCEL, DatabaseDlg::onCancel)
  EVT_BUTTON(ID_BUTTON_TEST, DatabaseDlg::onTest)
END_EVENT_TABLE()


DatabaseDlg::DatabaseDlg(wxWindow * parent):
  wxDialog(parent, wxID_NEW, _("Database configuration")),
  m_host(new wxTextCtrl(this, wxID_NEW, _("localhost"))),
  m_port(new wxTextCtrl(this, wxID_NEW, _("5432"))),
  m_name(new wxTextCtrl(this, wxID_NEW, _("medoc"))),
  m_login(new wxTextCtrl(this, wxID_NEW)),
  m_password(new wxTextCtrl(this, wxID_NEW, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD))
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

  wxBoxSizer * vbox = new wxBoxSizer(wxVERTICAL);
  vbox->Add(grid, 0, wxEXPAND | wxALL, 7);
  vbox->Add(new wxButton(this, ID_BUTTON_TEST, _("Test connection")), 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 7);
  wxBoxSizer * hbox = new wxBoxSizer(wxHORIZONTAL);
  hbox->Add(new wxButton(this, ID_BUTTON_OK, _("Ok")), 1, wxEXPAND | wxRIGHT, 3);
  hbox->Add(new wxButton(this, ID_BUTTON_CANCEL, _("Cancel")), 1, wxEXPAND | wxLEFT, 3);
  vbox->Add(hbox, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 7);

  SetSizer(vbox);
  vbox->SetSizeHints(this);

  SetSize(400, wxDefaultCoord);
}

void DatabaseDlg::onOk(wxCommandEvent &)
{
  EndModal(wxID_OK);
}

void DatabaseDlg::onCancel(wxCommandEvent &)
{
  EndModal(wxID_CANCEL);
}

void DatabaseDlg::onTest(wxCommandEvent &)
{
}
