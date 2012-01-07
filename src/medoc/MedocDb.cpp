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

#include "MedocDb.h"

#include "MedocConfig.h"

namespace pqxx
{
  template<> struct PQXX_LIBEXPORT string_traits<wxString>
  {
    static const char * name() { return "wxString"; }
    static bool has_null() { return false; }
    static bool is_null(const wxString &) { return false; }
    static wxString null() { internal::throw_null_conversion(name()); return wxString(); }
    static void from_string(const char Str[], wxString & Obj) { Obj = wxString(Str, wxConvUTF8); }
    static PGSTD::string to_string(const wxString & Obj) { return std::string(Obj.mb_str(wxConvUTF8)); }
  };
  
  template<> struct PQXX_LIBEXPORT string_traits<wxDateTime>
  {
    static const char * name() { return "wxDateTime"; }
    static bool has_null() { return false; }
    static bool is_null(const wxDateTime &) { return false; }
    static wxDateTime null() { internal::throw_null_conversion(name()); return wxDateTime(); }
    static void from_string(const char Str[], wxDateTime & Obj) { Obj.ParseDateTime(wxString(Str, wxConvUTF8)); }
    static PGSTD::string to_string(const wxDateTime & Obj) { return std::string(Obj.FormatISODate().mb_str(wxConvUTF8)) + "T" + std::string(Obj.FormatISOTime().mb_str(wxConvUTF8)); }
  };
}

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
        m_languages.push_back(tuple[0].as<wxString>());
      }
    }
    
    static void prepare(pqxx::connection & dbConn)
    {
      dbConn.prepare("getLanguages", "select * from getLanguages()");
    }
    
  private:
    std::vector<wxString> & m_languages;
  };

  class CheckUser : public pqxx::transactor<pqxx::transaction<> >
  {
  public:
    CheckUser(const wxString & account,
              const wxString & key,
              bool & isValid):
      m_account(account),
      m_key(key),
      m_isValid(isValid)
    {
    }

    void operator()(argument_type & T)
    {
      pqxx::result result = T.prepared("checkUser")(m_account)(m_key).exec();
      m_isValid = result[0][0].as<bool>();
    }
    
    static void prepare(pqxx::connection & dbConn)
    {
      dbConn.prepare
        ("checkUser", "select * from checkUser($1, $2)")
        ("text", pqxx::prepare::treat_string)
        ("text", pqxx::prepare::treat_string);
    }

  private:
    wxString m_account;
    wxString m_key;
    bool & m_isValid;
  };

  class CreateDocument : public pqxx::transactor<pqxx::transaction<> >
  {
  public:
    CreateDocument(const wxString & title,
                   const wxDateTime & dateTime,
                   const wxString & language,
                   const std::vector<MedocDb::File> files,
                   const wxString & password):
      m_title(title),
      m_dateTime(dateTime),
      m_language(language),
      m_files(files),
      m_password(password)
    {
    }
    
    void operator()(argument_type & T)
    {
      pqxx::result result = 
        T.prepared("createDocument")
        (m_title)
        (m_dateTime)
        (m_password).exec();
      int documentId = result[0][0].as<int>();
      for(const MedocDb::File & file : m_files)
      {
        T.prepared("addFile")
          (documentId)
          (m_title)
          (m_language)
          (file.m_thumb)
          (file.m_image)
          (m_password).exec();
      }
    }

    static void prepare(pqxx::connection & dbConn)
    {
      dbConn.prepare("createDocument", "select * from createDocument($1, $2, $3)")
        ("text", pqxx::prepare::treat_string)
        ("date", pqxx::prepare::treat_string)
        ("text", pqxx::prepare::treat_string);
      dbConn.prepare("addFile", "select * from addFile($1, $2, $3, $4, $5, $6)")
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
    wxString m_language;
    std::vector<MedocDb::File> m_files;
    wxString m_password;
  };
}


MedocDb::MedocDb()
{
  try
  {
    MedocConfig config;
    wxString str;
    str << _("host=") << config.getHost() << _(" ")
        << _("port=") << config.getPort() << _(" ")
        << _("dbname=") << config.getName() << _(" ")
        << _("user=") << config.getLogin() << _(" ")
        << _("password=") << config.getPassword();
    m_dbConn.reset(new pqxx::connection(str.mb_str(wxConvUTF8)));
    GetLanguages::prepare(*m_dbConn);
    CheckUser::prepare(*m_dbConn);
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

bool MedocDb::checkUser(const wxString & account,
                        const wxString & key) const
{
  bool isValid;
  m_dbConn->perform(CheckUser(account,
                              key,
                              isValid));
  return isValid;
}

MedocDb::File::File(const std::string & image,
                    const std::string & thumb):
  m_image(image),
  m_thumb(thumb)
{
}

void MedocDb::createDocument(const wxString & title,
                             const wxDateTime & dateTime,
                             const wxString & language,
                             const std::vector<File> & files,
                             const wxString & password)
{
  m_dbConn->perform(CreateDocument(title,
                                   dateTime,
                                   language,
                                   files,
                                   password));
}
