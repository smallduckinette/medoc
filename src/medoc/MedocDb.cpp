#include "MedocDb.h"


namespace
{
  class GetLanguages : public pqxx::transactor<pqxx::transaction<> >
  {
  public:
    GetLanguages(std::vector<wxString> & languages):
      m_languages(languages)
    {
    }
    
    void operator()(argument_type & T)
    {
      pqxx::result result = T.prepared("getLanguages").exec();
      for(const pqxx::result::tuple & tuple : result)
      {
        m_languages.push_back(wxString(tuple[0].c_str(), wxConvUTF8));
      }
    }
    
    static void prepare(pqxx::connection & dbConn)
    {
      dbConn.prepare("getLanguages", "select * from getLanguages()");
    }
    
  private:
    std::vector<wxString> & m_languages;
  };

  class CreateDocument : public pqxx::transactor<pqxx::transaction<> >
  {
  public:
    CreateDocument(const wxString & title,
                   const wxDateTime & dateTime,
                   const std::vector<MedocDb::File> files,
                   const wxString & password):
      m_title(title),
      m_dateTime(dateTime),
      m_files(files),
      m_password(password)
    {
    }

    void operator()(argument_type & T)
    {
      pqxx::result result = T.prepared("createDocument").exec();
      //for(const pqxx::result::tuple & tuple : result)
      {
        //m_languages.push_back(wxString(tuple[0].c_str(), wxConvUTF8));
      }
    }

    static void prepare(pqxx::connection & dbConn)
    {
      dbConn.prepare("createDocument", "select * from createDocument()")
        ("text", pqxx::prepare::treat_string)
        ("date", pqxx::prepare::treat_string)
        ("text", pqxx::prepare::treat_string);
      dbConn.prepare("addFile", "select * from addFile()")
        ("integer", pqxx::prepare::treat_direct)
        ("text", pqxx::prepare::treat_string)
        ("regconfig", pqxx::prepare::treat_string)
        ("bytea", pqxx::prepare::treat_binary)
        ("bytea", pqxx::prepare::treat_binary)
        ("text", pqxx::prepare::treat_string);
    }    

  private:
    wxString m_title;
    wxDateTime m_dateTime;
    std::vector<MedocDb::File> m_files;
    wxString m_password;
  };
}


MedocDb::MedocDb(const wxString & host,
                 int port,
                 const wxString & dbName,
                 const wxString & login,
                 const wxString & password)
{
  try
  {
    wxString str;
    str << _("host=") << host << _(" ")
        << _("port=") << port << _(" ")
        << _("dbname=") << dbName << _(" ")
        << _("user=") << login << _(" ")
        << _("password=") << password;
    m_dbConn.reset(new pqxx::connection(str.mb_str(wxConvUTF8)));
    GetLanguages::prepare(*m_dbConn);
    CreateDocument::prepare(*m_dbConn);
  }
  catch(const pqxx::pqxx_exception & e)
  {
    throw std::runtime_error(e.base().what());
  }
}

std::vector<wxString> MedocDb::getLanguages() const
{
  std::vector<wxString> languages;
  m_dbConn->perform(GetLanguages(languages));
  return languages;
}

void MedocDb::createDocument(const wxString & title,
                             const wxDateTime & dateTime,
                             const std::vector<File> & files,
                             const wxString & password)
{
  m_dbConn->perform(CreateDocument(title,
                                   dateTime,
                                   files,
                                   password));
}
