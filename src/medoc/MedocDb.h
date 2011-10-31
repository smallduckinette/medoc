#ifndef __MEDOC_MEDOCDB_H__
#define __MEDOC_MEDOCDB_H__

#include <pqxx/pqxx>
#include <wx/wx.h>
#include "Config.h"

class MedocDb
{
public:
  MedocDb(const DbConfig & dbConfig);
  
  std::vector<wxString> getLanguages() const;
  bool checkUser(const wxString & account,
                 const wxString & key) const;
  
  struct File
  {
    File(const std::string & image,
         const std::string & thumb);

    std::string m_image;
    std::string m_thumb;
  };

  void createDocument(const wxString & title,
                      const wxDateTime & dateTime,
                      const wxString & language,
                      const std::vector<File> & files,
                      const wxString & password);
  
private:
  std::unique_ptr<pqxx::connection> m_dbConn;
};

#endif
