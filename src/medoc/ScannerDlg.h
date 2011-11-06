#ifndef __MEDOC_SCANNER_H__
#define __MEDOC_SCANNER_H__

#include <vector>
#include <memory>
#include <wx/wx.h>
#include <sane/sane.h>

class ScannerOption;

class ScannerDlg : public wxDialog
{
public:
  ScannerDlg(wxWindow * parent);
  ~ScannerDlg();
  
  std::vector<wxImage> getImages() const;
  
protected:
  enum
    {
      ID_BUTTON_SCAN_MANY = wxID_HIGHEST + 1,
      ID_BUTTON_SCAN_SINGLE,
      ID_BUTTON_CANCEL
    };
  
  void onScanMany(wxCommandEvent &);
  void onScanSingle(wxCommandEvent &);
  void onCancel(wxCommandEvent &);
  

  DECLARE_EVENT_TABLE();
  
private:
  void onScan(bool many);
  
  SANE_Handle m_handle;
  std::vector<wxImage> m_images;
  
  std::vector<std::shared_ptr<ScannerOption> > m_options;
};

#endif
