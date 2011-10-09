#include "ScannerOptionImpl.h"


ScannerOptionStringChoice::ScannerOptionStringChoice(wxWindow * parent,
                                                     SANE_String_Const name,
                                                     SANE_String_Const title,
                                                     SANE_String_Const desc,
                                                     const SANE_String_Const * string_list):
  m_name(name),
  m_title(new wxStaticText(parent, wxID_ANY, wxString(title, wxConvUTF8))),
  m_options(new wxChoice(parent, wxID_ANY))
{
  while(*string_list)
  {
    m_options->Append(wxString(*string_list, wxConvUTF8));
    ++string_list;
  }
}

void ScannerOptionStringChoice::append(wxSizer * sizer)
{
  sizer->Add(m_title, 0, wxEXPAND);
  sizer->Add(m_options, 1, wxEXPAND);
}

ScannerOptionIntRange::ScannerOptionIntRange(wxWindow * parent,
                                             SANE_String_Const name,
                                             SANE_String_Const title,
                                             SANE_String_Const desc,
                                             const SANE_Range * range):
  m_name(name),
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
  label << wxString(name, wxConvUTF8) << _(" [") << m_min << _(" ") << m_max << _("]");

  m_title = new wxStaticText(parent, wxID_ANY, label);
}

void ScannerOptionIntRange::append(wxSizer * sizer)
{
  sizer->Add(m_title, 0, wxEXPAND);
  sizer->Add(m_input, 1, wxEXPAND);
}
