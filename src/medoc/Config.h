#ifndef __MEDOC_CONFIG_H__
#define __MEDOC_CONFIG_H__

#include <wx/wx.h>

class DbConfig
{
public:
  DbConfig();
  DbConfig(const wxString & host,
           int port,
           const wxString & dbName,
           const wxString & login,
           const wxString & password,
           const wxString & account);
           
  wxString getHost() const;
  int getPort() const;
  wxString getDbName() const;
  wxString getLogin() const;
  wxString getPassword() const;
  wxString getAccount() const;

private:
  wxString m_host;
  int m_port;
  wxString m_dbName;
  wxString m_login;
  wxString m_password;
  wxString m_account;
};

class Config
{
public:
  void setDbConfig(const DbConfig & dbConfig);
  DbConfig getDbConfig() const;

private:
  DbConfig m_dbConfig;
};

#endif
