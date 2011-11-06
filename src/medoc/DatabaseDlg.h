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

#ifndef __MEDOC_DATABASEDLG_H__
#define __MEDOC_DATABASEDLG_H__

#include <wx/wx.h>
#include "Config.h"

class DatabaseDlg : public wxDialog
{
public:
  DatabaseDlg(wxWindow * parent);
  
  DbConfig getDbConfig() const;
  
protected:
  enum
    {
      ID_BUTTON_OK = wxID_HIGHEST + 1,
      ID_BUTTON_CANCEL,
      ID_BUTTON_TEST
    };
  
  void onOk(wxCommandEvent &);
  void onCancel(wxCommandEvent &);
  void onTest(wxCommandEvent &);

  DECLARE_EVENT_TABLE();  
  
private:
  wxTextCtrl * m_host;
  wxTextCtrl * m_port;
  wxTextCtrl * m_name;
  wxTextCtrl * m_login;
  wxTextCtrl * m_password;
  wxTextCtrl * m_account;
};

#endif
