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

#include "MedocConfig.h"


namespace
{
  wxString pdfDpi(_("PdfDpi"));
  wxString dbHost(_("DbHost"));
  wxString dbPort(_("DbPort"));
  wxString dbName(_("DbName"));
  wxString dbLogin(_("DbLogin"));
  wxString dbPassword(_("DbPassword"));
  wxString ocrEnabled(_("OcrEnabled"));
  wxString tesseractDataPath(_("TesseractDataPath"));
}


MedocConfig::MedocConfig():
  m_config(_("medoc"))
{
}

int MedocConfig::getDpi()
{
  return m_config.Read(pdfDpi, 150);
}

void MedocConfig::setDpi(int dpi)
{
  m_config.Write(pdfDpi, dpi);
}

wxString MedocConfig::getHost()
{
  return m_config.Read(dbHost, _("localhost"));
}

int MedocConfig::getPort()
{
  return m_config.Read(dbPort, 5432);
}

wxString MedocConfig::getName()
{
  return m_config.Read(dbName, _("medoc"));
}

wxString MedocConfig::getLogin()
{
  return m_config.Read(dbLogin, _(""));
}

wxString MedocConfig::getPassword()
{
  return m_config.Read(dbPassword, _(""));
}

void MedocConfig::setHost(const wxString & host)
{
  m_config.Write(dbHost, host);
}

void MedocConfig::setPort(int port)
{
  m_config.Write(dbPort, port);
}

void MedocConfig::setName(const wxString & name)
{
  m_config.Write(dbName, name);
}

void MedocConfig::setLogin(const wxString & login)
{
  m_config.Write(dbLogin, login);
}

void MedocConfig::setPassword(const wxString & password)
{
  m_config.Write(dbPassword, password);
}

bool MedocConfig::isOcrEnabled()
{
  return m_config.Read(ocrEnabled, true);
}

wxString MedocConfig::getTesseractDataPath()
{
  return m_config.Read(tesseractDataPath, _("/usr/share/tesseract-ocr/tessdata/"));
}

void MedocConfig::setOcrEnabled(bool value)
{
  m_config.Write(ocrEnabled, value);
}

void MedocConfig::setTesseractDataPath(const wxString & dataPath)
{
  m_config.Write(tesseractDataPath, dataPath);
}

std::vector<MedocConfig::Language> MedocConfig::getLanguages()
{
  if(m_config.HasGroup(_("Languages")))
  {    
    std::vector<Language> languages;
    
    wxString initialPath = m_config.GetPath();
    m_config.SetPath(_("/Languages"));
    
    wxString group;
    long index;
    bool hasGroup = m_config.GetFirstGroup(group, index);
    while(hasGroup)
    {
      wxString tesseractValue = m_config.Read(_("/Languages/") + group + _("/Tesseract"));
      wxString postgresValue = m_config.Read(_("/Languages/") + group + _("/Postgres"));
      languages.push_back({group, tesseractValue, postgresValue});
      hasGroup = m_config.GetNextGroup(group, index);
    }
    
    m_config.SetPath(initialPath);    
    return languages;
  }
  else
  {
    return {{_("English"), _("eng"), _("english_safe")}, {_("French"), _("fra"), _("french_safe")}};
  }
}

void MedocConfig::setLanguages(const std::vector<Language> & languages)
{
  m_config.DeleteGroup(_("Languages"));
  
  for(const Language & item : languages)
  {
    wxString key = _("Languages/") + item.m_language;
    m_config.Write(key + _T("/Tesseract"), item.m_tesseractLanguage);
    m_config.Write(key + _T("/Postgres"), item.m_postgresLanguage);
  }
}
