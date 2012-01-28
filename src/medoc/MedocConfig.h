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

#ifndef __MEDOC_MEDOCCONFIG_H__
#define __MEDOC_MEDOCCONFIG_H__

#include <vector>
#include <wx/wx.h>
#include <wx/config.h>

class MedocConfig
{
public:
  MedocConfig();

  // Pdf
  int getDpi();

  void setDpi(int dpi);

  // Database
  wxString getHost();
  int getPort();
  wxString getName();
  wxString getLogin();
  wxString getPassword();

  void setHost(const wxString & host);
  void setPort(int port);
  void setName(const wxString & name);
  void setLogin(const wxString & login);
  void setPassword(const wxString & password);
  
  // Tesseract
  bool isOcrEnabled();
  wxString getTesseractDataPath();
  
  void setOcrEnabled(bool value);
  void setTesseractDataPath(const wxString & dataPath);

  // Languages
  struct Language
  {
    wxString m_language;
    wxString m_tesseractLanguage;
    wxString m_postgresLanguage;
  };

  std::vector<Language> getLanguages();
  void setLanguages(const std::vector<Language> & languages);

  // Default device
  wxString getDefaultDevice();
  void setDefaultDevice(const wxString & device);
  
  // Device config
  wxString getDeviceConfig(const wxString & device, const wxString & category);
  void setDeviceConfig(const wxString & device, const wxString & category, const wxString & value);
  
private:
  wxConfig m_config;
};

#endif
