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

private:
  SANE_Handle m_handle;
  
  std::vector<std::shared_ptr<ScannerOption> > m_options;
};

#endif
