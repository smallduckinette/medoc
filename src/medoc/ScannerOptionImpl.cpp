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

#include "MedocConfig.h"


ScannerOptionStringChoice::ScannerOptionStringChoice(wxWindow * parent,
                                                     const SANE_Handle & handle,
                                                     int index,
                                                     const wxString & deviceName,
                                                     const wxString & title,
                                                     const wxString & desc,
                                                     const SANE_String_Const * string_list):
  m_handle(handle),
  m_index(index),
  m_deviceName(deviceName),
  m_optionTitle(title),
  m_title(new wxStaticText(parent, wxID_ANY, title)),
  m_options(new wxChoice(parent, wxID_ANY))
{
  while(*string_list)
  {
    m_options->Append(wxString(*string_list, wxConvUTF8));
    ++string_list;
  }
  
  MedocConfig config;
  wxString configValue = config.getDeviceConfig(deviceName, title);
  if(!configValue.IsEmpty())
  {
    m_options->SetStringSelection(configValue);
  }
  else
  {
    char cValue[255];
    sane_control_option(m_handle, index, SANE_ACTION_GET_VALUE, cValue, nullptr);
    m_options->SetStringSelection(wxString(cValue, wxConvUTF8));
  }
}

void ScannerOptionStringChoice::append(wxSizer * sizer)
{
  sizer->Add(m_title, 0, wxALIGN_CENTER_VERTICAL);
  sizer->Add(m_options, 1, wxEXPAND);
}

void ScannerOptionStringChoice::setOption()
{
  sane_control_option(m_handle, 
                      m_index, 
                      SANE_ACTION_SET_VALUE, 
                      const_cast<char *>(m_options->GetStringSelection().utf8_str().data()), 
                      nullptr);
  MedocConfig config;
  config.setDeviceConfig(m_deviceName, m_optionTitle, m_options->GetStringSelection());
}

ScannerOptionIntRange::ScannerOptionIntRange(wxWindow * parent,
                                             const SANE_Handle & handle,
                                             int index,
                                             const wxString & deviceName,
                                             const wxString & title,
                                             const wxString & desc,
                                             const SANE_Range * range):
  m_handle(handle),
  m_index(index),
  m_min(range->min),
  m_max(range->max),
  m_quant(range->quant),
  m_deviceName(deviceName),
  m_optionTitle(title),
  m_input(new wxTextCtrl(parent, 
                         wxID_ANY, 
                         wxEmptyString, 
                         wxDefaultPosition, 
                         wxDefaultSize, 
                         0,
                         wxTextValidator(wxFILTER_NUMERIC)))
{
  wxString label;
  label << title << _(" [") << m_min << _(" ") << m_max << _("]");
  m_title = new wxStaticText(parent, wxID_ANY, label);
  
  MedocConfig config;
  wxString configValue = config.getDeviceConfig(deviceName, title);
  if(!configValue.IsEmpty())
  {
    m_input->SetValue(configValue);
  }
  else
  {  
    SANE_Int iValue = 0;
    sane_control_option(m_handle, index, SANE_ACTION_GET_VALUE, &iValue, nullptr);
    wxString value;
    value << iValue;
    m_input->SetValue(value);
  }
}

void ScannerOptionIntRange::append(wxSizer * sizer)
{
  sizer->Add(m_title, 0, wxALIGN_CENTER_VERTICAL);
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
  MedocConfig config;
  config.setDeviceConfig(m_deviceName, m_optionTitle, m_input->GetValue());
}

ScannerOptionInt::ScannerOptionInt(wxWindow * parent,
                                   const SANE_Handle & handle,
                                   int index,
                                   const wxString & deviceName,
                                   const wxString & title,
                                   const wxString & desc):
  m_handle(handle),
  m_index(index),
  m_deviceName(deviceName),
  m_optionTitle(title),
  m_input(new wxTextCtrl(parent, 
                         wxID_ANY, 
                         wxEmptyString, 
                         wxDefaultPosition, 
                         wxDefaultSize, 
                         0,
                         wxTextValidator(wxFILTER_NUMERIC)))
{
  m_title = new wxStaticText(parent, wxID_ANY, title);
  
  MedocConfig config;
  wxString configValue = config.getDeviceConfig(deviceName, title);
  if(!configValue.IsEmpty())
  {
    m_input->SetValue(configValue);
  }
  else
  {  
    SANE_Int iValue;
    sane_control_option(m_handle, index, SANE_ACTION_GET_VALUE, &iValue, nullptr);
    wxString value;
    value << iValue;
    m_input->SetValue(value);
  }
}

void ScannerOptionInt::append(wxSizer * sizer)
{
  sizer->Add(m_title, 0, wxALIGN_CENTER_VERTICAL);
  sizer->Add(m_input, 1, wxEXPAND);
}

void ScannerOptionInt::setOption()
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
  MedocConfig config;
  config.setDeviceConfig(m_deviceName, m_optionTitle, m_input->GetValue());
}

ScannerOptionBool::ScannerOptionBool(wxWindow * parent,
                                     const SANE_Handle & handle,
                                     int index,
                                     const wxString & deviceName,
                                     const wxString & title,
                                     const wxString & desc):
  m_handle(handle),
  m_index(index),
  m_deviceName(deviceName),
  m_optionTitle(title),
  m_title(new wxStaticText(parent,
                           wxID_ANY,
                           title)),
  m_input(new wxCheckBox(parent,
                         wxID_ANY, 
                         wxEmptyString))
{
  MedocConfig config;
  wxString configValue = config.getDeviceConfig(deviceName, title);
  if(!configValue.IsEmpty())
  {
    m_input->SetValue(configValue == _("true"));
  }
  else
  {  
    SANE_Bool bValue;
    sane_control_option(m_handle, index, SANE_ACTION_GET_VALUE, &bValue, nullptr);
    m_input->SetValue(bValue);  
  }
}

void ScannerOptionBool::append(wxSizer * sizer)
{
  sizer->Add(m_title, 0, wxALIGN_CENTER_VERTICAL);
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
  MedocConfig config;
  config.setDeviceConfig(m_deviceName, m_optionTitle, m_input->GetValue() ? _("true") : _("false"));
}

ScannerOptionGroup::ScannerOptionGroup(wxWindow * parent,
                                       const SANE_Handle & handle,
                                       int index,
                                       const wxString & deviceName,
                                       const wxString & title,
                                       const wxString & desc):
  m_handle(handle),
  m_index(index),
  m_deviceName(deviceName),
  m_optionTitle(title),
  m_title(new wxStaticText(parent,
                           wxID_ANY,
                           title)),
  m_description(new wxStaticText(parent,
                                 wxID_ANY, 
                                 desc))
{
}

void ScannerOptionGroup::append(wxSizer * sizer)
{
  sizer->Add(m_title, 0, wxALIGN_CENTER_VERTICAL);
  sizer->Add(m_description, 1, wxEXPAND);
}

void ScannerOptionGroup::setOption()
{
}
