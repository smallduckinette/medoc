#ifndef __MEDOC_SCANNEROPTION_H__
#define __MEDOC_SCANNEROPTION_H__

#include <wx/wx.h>

class ScannerOption
{
public:
  virtual ~ScannerOption();
  
  virtual void append(wxSizer * sizer) = 0;
  virtual void setOption() = 0;
};

#endif
