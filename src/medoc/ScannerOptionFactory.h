#ifndef __MEDOC_SCANNEROPTIONFACTORY_H__
#define __MEDOC_SCANNEROPTIONFACTORY_H__

#include <memory>
#include <sane/sane.h>
#include <wx/wx.h>

class ScannerOption;

class ScannerOptionFactory
{
public:
  std::shared_ptr<ScannerOption> create(wxWindow * wxWindow,
                                        const SANE_Handle & handle,
                                        int index,
                                        const SANE_Option_Descriptor * descriptor) const;
  bool isValidOption(const SANE_Option_Descriptor * descriptor) const;
};

#endif
