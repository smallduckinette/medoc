#ifndef __MEDOC_SCANNEROPTIONIMPL_H__
#define __MEDOC_SCANNEROPTIONIMPL_H__

#include <sane/sane.h>
#include "ScannerOption.h"

class ScannerOptionStringChoice : public ScannerOption
{
public:
  ScannerOptionStringChoice(wxWindow * parent,
                            SANE_String_Const name,
                            SANE_String_Const title,
                            SANE_String_Const desc,
                            const SANE_String_Const * string_list);
  
  void append(wxSizer * sizer);

private:
  SANE_String_Const m_name;
  
  wxStaticText * m_title;
  wxChoice * m_options;
};

class ScannerOptionIntRange : public ScannerOption
{
public:
  ScannerOptionIntRange(wxWindow * parent,
                        SANE_String_Const name,
                        SANE_String_Const title,
                        SANE_String_Const desc,
                        const SANE_Range * range);

  void append(wxSizer * sizer);

private:
  SANE_String_Const m_name;
  int m_min;
  int m_max;
  int m_quant;
  
  wxStaticText * m_title;
  wxTextCtrl * m_input;
};

#endif
