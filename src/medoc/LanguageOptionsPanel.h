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


#ifndef __MEDOC_LANGUAGEOPTIONSPANEL_H__
#define __MEDOC_LANGUAGEOPTIONSPANEL_H__

#include <wx/wx.h>

class LanguageOptionsPanel : public wxPanel
{
public:
  LanguageOptionsPanel(wxWindow * parent);

  void loadConfig();
  void saveConfig();
  
protected:
  enum
    {
      ID_LIST_LANGUAGES = wxID_HIGHEST + 1,
      ID_BUTTON_ADD,
      ID_BUTTON_REMOVE
    };
  
  void onAdd(wxCommandEvent &);
  void onRemove(wxCommandEvent &);
  void onSelectionChange(wxListEvent & event);

  DECLARE_EVENT_TABLE();
  
private:
  wxCheckBox * m_isOcrEnabled;
  wxTextCtrl * m_tesseractDataPath;
  wxListCtrl * m_languages;
  wxButton * m_addBtn;
  wxButton * m_removeBtn;
};

#endif
