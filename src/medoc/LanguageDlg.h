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

#ifndef __MEDOC_LANGUAGEDLG_H__
#define __MEDOC_LANGUAGEDLG_H__

#include <wx/wx.h>

class LanguageDlg : public wxDialog
{
public:
  LanguageDlg(wxWindow * parent);

  wxString getLanguage() const;
  wxString getTesseractLanguage() const;
  wxString getPostgresLanguage() const;

private:
  wxTextCtrl * m_language;
  wxTextCtrl * m_tesseractLanguage;
  wxTextCtrl * m_postgresLanguage;
};

#endif
