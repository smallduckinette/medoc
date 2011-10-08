#ifndef __MEDOC_SCANNER_H__
#define __MEDOC_SCANNER_H__

#include <wx/wx.h>
#include <sane/sane.h>

class ScannerDlg : public wxDialog
{
public:
  ScannerDlg(wxWindow * parent);
  ~ScannerDlg();

private:
  SANE_Handle m_handle;
};

#endif
