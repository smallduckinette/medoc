#ifndef __MEDOC_MEDOCDB_H__
#define __MEDOC_MEDOCDB_H__

#include <pqxx/pqxx>

class MedocDb
{
public:
  MedocDb(const std::string & host,
          int port,
          const std::string & dbName,
          const std::string & login,
          const std::string & password);
  
private:
  std::unique_ptr<pqxx::connection> m_dbConn;
};

#endif
