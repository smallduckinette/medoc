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

#ifndef __MEDOC_MEDOCDB_H__
#define __MEDOC_MEDOCDB_H__

#include <pqxx/pqxx>
#include <wx/wx.h>

class MedocDb
{
public:
  MedocDb();
  
  std::vector<wxString> getLanguages() const;
  bool checkUser(const wxString & account,
                 const wxString & key) const;
  
  struct File
  {
    File(const std::string & image,
         const std::string & thumb,
         const wxString & contents);
    
    std::string m_image;
    std::string m_thumb;
    wxString m_contents;
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
