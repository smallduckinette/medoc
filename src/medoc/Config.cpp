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

#include "Config.h"


DbConfig::DbConfig()
{
}

DbConfig::DbConfig(const wxString & host,
                   int port,
                   const wxString & dbName,
                   const wxString & login,
                   const wxString & password,
                   const wxString & account):
  m_host(host),
  m_port(port),
  m_dbName(dbName),
  m_login(login),
  m_password(password),
  m_account(account)
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

wxString DbConfig::getAccount() const
{
  return m_account;
}

void Config::setDbConfig(const DbConfig & dbConfig)
{
  m_dbConfig = dbConfig;
}

DbConfig Config::getDbConfig() const
{
  return m_dbConfig;
}
