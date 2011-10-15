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
  
  int iValue;
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
