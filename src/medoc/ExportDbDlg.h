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

#ifndef __MEDOC_EXPORTDBDLG_H__
#define __MEDOC_EXPORTDBDLG_H__

#include <wx/wx.h>
#include <wx/calctrl.h>
#include "MedocDb.h"

class ExportDbDlg : public wxDialog
{
public:
  ExportDbDlg(wxWindow * parent,
              const std::vector<wxImage> & images);
  
protected:
  enum
    {
      ID_BUTTON_EXPORT = wxID_HIGHEST + 1,
      ID_BUTTON_CANCEL
    };
  
  void onExport(wxCommandEvent &);

  DECLARE_EVENT_TABLE();
  
private:
  std::vector<MedocDb::File> processImages(const wxString & tesseractLanguage) const;
  std::string processImage(const wxImage & image) const;
  void getCurrentLanguage(wxString & tesseractLanguage, wxString & postgresLanguage);

  std::vector<wxImage> m_images;
  MedocDb m_medocDb;
  
  wxTextCtrl * m_title;
  wxChoice * m_languages;
  wxCalendarCtrl * m_calendar;
};

#endif
