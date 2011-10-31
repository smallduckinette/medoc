#ifndef __MEDOC_EXPORTDBDLG_H__
#define __MEDOC_EXPORTDBDLG_H__

#include <wx/wx.h>
#include <wx/calctrl.h>
#include "Config.h"
#include "MedocDb.h"

class ExportDbDlg : public wxDialog
{
public:
  ExportDbDlg(wxWindow * parent,
              const Config & config,
              const std::vector<wxImage> & images);
  
protected:
  enum
    {
      ID_BUTTON_EXPORT = wxID_HIGHEST + 1,
      ID_BUTTON_CANCEL
    };
  
  void onExport(wxCommandEvent &);
  void onCancel(wxCommandEvent &);

  DECLARE_EVENT_TABLE();
  
private:
  std::vector<MedocDb::File> processImages() const;
  std::string processImage(const wxImage & image) const;

  Config m_config;
  std::vector<wxImage> m_images;
  MedocDb m_medocDb;
  
  wxTextCtrl * m_title;
  wxCalendarCtrl * m_calendar;
  wxChoice * m_languages;
};

#endif
