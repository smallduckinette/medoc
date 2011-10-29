#include "ExportDbDlg.h"

#include "MedocDb.h"


ExportDbDlg::ExportDbDlg(wxWindow * parent,
                         const Config & config):
  wxDialog(parent, wxID_NEW, _("Export to database")),
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
