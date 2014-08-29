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

#include <vector>
#include <algorithm>
#include <functional>

#include "LanguageDlg.h"
#include "MedocConfig.h"


BEGIN_EVENT_TABLE(LanguageOptionsPanel, wxPanel)
  EVT_BUTTON(ID_BUTTON_ADD, LanguageOptionsPanel::onAdd)
  EVT_BUTTON(ID_BUTTON_REMOVE, LanguageOptionsPanel::onRemove)
  EVT_LIST_ITEM_SELECTED(ID_LIST_LANGUAGES, LanguageOptionsPanel::onSelectionChange)
  EVT_LIST_ITEM_DESELECTED(ID_LIST_LANGUAGES, LanguageOptionsPanel::onSelectionChange)
END_EVENT_TABLE()


LanguageOptionsPanel::LanguageOptionsPanel(wxWindow * parent):
  wxPanel(parent, wxID_ANY),
  m_isOcrEnabled(new wxCheckBox(this, wxID_ANY, wxEmptyString)),
  m_tesseractDataPath(new wxTextCtrl(this, wxID_ANY, wxEmptyString)),
  m_languages(new wxListCtrl(this, ID_LIST_LANGUAGES, wxDefaultPosition, wxDefaultSize, wxLC_REPORT)),
  m_addBtn(new wxButton(this, ID_BUTTON_ADD, _("Add"))),
  m_removeBtn(new wxButton(this, ID_BUTTON_REMOVE, _("Remove")))
{
  m_languages->InsertColumn(0, _T("Language"));
  m_languages->InsertColumn(1, _T("Tesseract"));
  m_languages->InsertColumn(2, _T("Postgresql"));
  
  wxGridSizer * grid = new wxGridSizer(2, 3);
  grid->Add(new wxStaticText(this, wxID_NEW, _("Enable OCR")), 0, wxALIGN_CENTER_VERTICAL);
  grid->Add(m_isOcrEnabled, 0, wxEXPAND);
  grid->Add(new wxStaticText(this, wxID_NEW, _("Tesseract data path")), 0, wxALIGN_CENTER_VERTICAL);
  grid->Add(m_tesseractDataPath, 0, wxEXPAND);
  
  wxBoxSizer * vhbox = new wxBoxSizer(wxVERTICAL);
  vhbox->Add(m_addBtn, 0);
  vhbox->Add(m_removeBtn, 0);
  
  wxBoxSizer * hbox = new wxBoxSizer(wxHORIZONTAL);
  hbox->Add(vhbox, 0, wxRIGHT, 3);
  hbox->Add(m_languages, 1, wxEXPAND);
  
  wxBoxSizer * vbox = new wxBoxSizer(wxVERTICAL);
  vbox->Add(grid, 0, wxEXPAND | wxALL, 7);
  vbox->Add(hbox, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 7);
  
  SetSizer(vbox);
  vbox->SetSizeHints(this);  
}

void LanguageOptionsPanel::loadConfig()
{
  MedocConfig config;
  m_isOcrEnabled->SetValue(config.isOcrEnabled());
  m_tesseractDataPath->SetValue(config.getTesseractDataPath());
  
  for(const MedocConfig::Language & item : config.getLanguages())
  {
    long itemId = m_languages->InsertItem(m_languages->GetItemCount(), _(""));
    m_languages->SetItem(itemId, 0, item.m_language);
    m_languages->SetItem(itemId, 1, item.m_tesseractLanguage);
    m_languages->SetItem(itemId, 2, item.m_postgresLanguage);    
  }
}

void LanguageOptionsPanel::saveConfig()
{
  MedocConfig config;
  config.setOcrEnabled(m_isOcrEnabled->GetValue());
  config.setTesseractDataPath(m_tesseractDataPath->GetValue());
  
  std::vector<MedocConfig::Language> languages;
  long itemId = -1;
  do
  {
    itemId = m_languages->GetNextItem(itemId);
    if(itemId != -1)
    {
      wxListItem listItem;
      listItem.SetId(itemId);
      listItem.SetColumn(0);
      m_languages->GetItem(listItem);
      wxString key = listItem.GetText();
      listItem.SetColumn(1);
      m_languages->GetItem(listItem);
      wxString tesseractValue = listItem.GetText();
      listItem.SetColumn(2);
      m_languages->GetItem(listItem);
      wxString postgresValue = listItem.GetText();
      languages.push_back({key, tesseractValue, postgresValue});
    }
  } 
  while(itemId != -1);
  config.setLanguages(languages);
}

void LanguageOptionsPanel::onAdd(wxCommandEvent &)
{
  LanguageDlg languageDlg(this);
  if(languageDlg.ShowModal() == wxID_OK)
  {
    long itemId = m_languages->InsertItem(m_languages->GetItemCount(), _(""));
    m_languages->SetItem(itemId, 0, languageDlg.getLanguage());
    m_languages->SetItem(itemId, 1, languageDlg.getTesseractLanguage());
    m_languages->SetItem(itemId, 2, languageDlg.getPostgresLanguage());
  }
  wxListEvent emptyListEvent;
  onSelectionChange(emptyListEvent);
}

void LanguageOptionsPanel::onRemove(wxCommandEvent &)
{
  std::vector<long> selection;
  long itemId = -1;
  do
  {
    itemId = m_languages->GetNextItem(itemId,
                                      wxLIST_NEXT_ALL,
                                      wxLIST_STATE_SELECTED);
    if(itemId != -1)
      selection.push_back(itemId);
  } 
  while(itemId != -1);
  
  std::for_each(selection.rbegin(),
                selection.rend(),
                std::bind(&wxListCtrl::DeleteItem, m_languages, std::placeholders::_1));
  wxListEvent emptyListEvent;
  onSelectionChange(emptyListEvent);
}

void LanguageOptionsPanel::onSelectionChange(wxListEvent & event)
{
  m_removeBtn->Enable(m_languages->GetSelectedItemCount() > 0);
}
