#include "Config.h"


DbConfig::DbConfig()
{
}

DbConfig::DbConfig(const wxString & host,
                   int port,
                   const wxString & dbName,
                   const wxString & login,
                   const wxString & password):
  m_host(host),
  m_port(port),
  m_dbName(dbName),
  m_login(login),
  m_password(password)
{
}

wxString DbConfig::getHost() const
{
  return m_host;
}

int DbConfig::getPort() const
{
  return m_port;
}

wxString DbConfig::getDbName() const
{
  return m_dbName;
}

wxString DbConfig::getLogin() const
{
  return m_login;
}

wxString DbConfig::getPassword() const
{
  return m_password;
}

void Config::setDbConfig(const DbConfig & dbConfig)
{
  m_dbConfig = dbConfig;
}

DbConfig Config::getDbConfig() const
{
  return m_dbConfig;
}
