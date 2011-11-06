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

#include "ScannerOptionImpl.h"


ScannerOptionStringChoice::ScannerOptionStringChoice(wxWindow * parent,
                                                     const SANE_Handle & handle,
                                                     int index,
                                                     SANE_String_Const title,
                                                     SANE_String_Const desc,
                                                     const SANE_String_Const * string_list):
  m_handle(handle),
  m_index(index),
  m_title(new wxStaticText(parent, wxID_ANY, wxString(title, wxConvUTF8))),
  m_options(new wxChoice(parent, wxID_ANY))
{
  while(*string_list)
  {
    m_options->Append(wxString(*string_list, wxConvUTF8));
    ++string_list;
  }
  
  char cValue[255];
  sane_control_option(m_handle, index, SANE_ACTION_GET_VALUE, cValue, nullptr);
  m_options->SetStringSelection(wxString(cValue, wxConvUTF8));
}

void ScannerOptionStringChoice::append(wxSizer * sizer)
{
  sizer->Add(m_title, 0, wxEXPAND);
  sizer->Add(m_options, 1, wxEXPAND);
}

void ScannerOptionStringChoice::setOption()
{
  sane_control_option(m_handle, 
                      m_index, 
                      SANE_ACTION_SET_VALUE, 
                      const_cast<char *>(m_options->GetStringSelection().utf8_str().data()), 
                      nullptr);
}

ScannerOptionIntRange::ScannerOptionIntRange(wxWindow * parent,
                                             const SANE_Handle & handle,
                                             int index,
                                             SANE_String_Const title,
                                             SANE_String_Const desc,
                                             const SANE_Range * range):
  m_handle(handle),
  m_index(index),
  m_min(range->min),
  m_max(range->max),
  m_quant(range->quant),
  m_input(new wxTextCtrl(parent, 
                         wxID_ANY, 
                         wxEmptyString, 
                         wxDefaultPosition, 
                         wxDefaultSize, 
                         0,
                         wxTextValidator(wxFILTER_NUMERIC)))
{
  wxString label;
  label << wxString(title, wxConvUTF8) << _(" [") << m_min << _(" ") << m_max << _("]");
  m_title = new wxStaticText(parent, wxID_ANY, label);
  
  SANE_Int iValue;
  sane_control_option(m_handle, index, SANE_ACTION_GET_VALUE, &iValue, nullptr);
  wxString value;
  value << iValue;
  m_input->SetValue(value);
}

void ScannerOptionIntRange::append(wxSizer * sizer)
{
  sizer->Add(m_title, 0, wxEXPAND);
  sizer->Add(m_input, 1, wxEXPAND);
}

void ScannerOptionIntRange::setOption()
{
  SANE_Int value;
  long lValue;
  m_input->GetValue().ToLong(&lValue);
  value = lValue;
  sane_control_option(m_handle, 
                      m_index, 
                      SANE_ACTION_SET_VALUE, 
                      &value,
                      nullptr);   
}

ScannerOptionBool::ScannerOptionBool(wxWindow * parent,
                                     const SANE_Handle & handle,
                                     int index,
                                     SANE_String_Const title,
                                     SANE_String_Const desc):
  m_handle(handle),
  m_index(index),
  m_title(new wxStaticText(parent,
                           wxID_ANY,
                           wxString(title, wxConvUTF8))),
  m_input(new wxCheckBox(parent,
                         wxID_ANY, 
                         wxEmptyString))
{
  SANE_Bool bValue;
  sane_control_option(m_handle, index, SANE_ACTION_GET_VALUE, &bValue, nullptr);
  m_input->SetValue(bValue);  
}

void ScannerOptionBool::append(wxSizer * sizer)
{
  sizer->Add(m_title, 0, wxEXPAND);
  sizer->Add(m_input, 1, wxEXPAND);
}

void ScannerOptionBool::setOption()
{
  SANE_Bool value = m_input->GetValue() ? SANE_TRUE : SANE_FALSE;
  sane_control_option(m_handle, 
                      m_index, 
                      SANE_ACTION_SET_VALUE, 
                      &value,
                      nullptr);  
}
