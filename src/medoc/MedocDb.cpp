#include "MedocDb.h"


MedocDb::MedocDb(const std::string & host,
                 int port,
                 const std::string & dbName,
                 const std::string & login,
                 const std::string & password)
{
  try
  {
    std::ostringstream str;
    str << "host=" << host << " "
        << "port=" << port << " "
        << "dbname=" << dbName << " "
        << "user=" << login << " "
        << "password=" << password;
    m_dbConn.reset(new pqxx::connection(str.str()));
  }
  catch(const pqxx::pqxx_exception & e)
  {
    throw std::runtime_error(e.base().what());
  }
}
