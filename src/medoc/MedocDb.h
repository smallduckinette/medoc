#ifndef __MEDOC_MEDOCDB_H__
#define __MEDOC_MEDOCDB_H__

#include <pqxx/pqxx>
#include <wx/wx.h>

class MedocDb
{
public:
  MedocDb(const wxString & host,
          int port,
          const wxString & dbName,
          const wxString & login,
          const wxString & password);
  
  std::vector<wxString> getLanguages() const;
  
  struct File
  {
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
