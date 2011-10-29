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
};

#endif
