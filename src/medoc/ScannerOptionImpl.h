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

#ifndef __MEDOC_SCANNEROPTIONIMPL_H__
#define __MEDOC_SCANNEROPTIONIMPL_H__

#include <sane/sane.h>
#include "ScannerOption.h"

class ScannerOptionStringChoice : public ScannerOption
{
public:
  ScannerOptionStringChoice(wxWindow * parent,
                            const SANE_Handle & handle,
                            int index,
                            SANE_String_Const title,
                            SANE_String_Const desc,
                            const SANE_String_Const * string_list);
  
  void append(wxSizer * sizer);
  void setOption();
  
private:
  SANE_Handle m_handle;
  int m_index;
  
  wxStaticText * m_title;
  wxChoice * m_options;
};

class ScannerOptionIntRange : public ScannerOption
{
public:
  ScannerOptionIntRange(wxWindow * parent,
                        const SANE_Handle & handle,
                        int index,
                        SANE_String_Const title,
                        SANE_String_Const desc,
                        const SANE_Range * range);

  void append(wxSizer * sizer);
  void setOption();

private:
  SANE_Handle m_handle;
  int m_index;
  int m_min;
  int m_max;
  int m_quant;
  
  wxStaticText * m_title;
  wxTextCtrl * m_input;
};

class ScannerOptionBool : public ScannerOption
{
public:
  ScannerOptionBool(wxWindow * parent,
                    const SANE_Handle & handle,
                    int index,
                    SANE_String_Const title,
                    SANE_String_Const desc);
  
  void append(wxSizer * sizer);
  void setOption();

private:
  SANE_Handle m_handle;
  int m_index;

  wxStaticText * m_title;
  wxCheckBox * m_input;
};

#endif
