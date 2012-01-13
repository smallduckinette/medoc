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

#include "ScannerDlg.h"

#include <stdexcept>
#include <algorithm>
#include <wx/progdlg.h>
#include "ScannerOptionFactory.h"
#include "ScannerOption.h"

BEGIN_EVENT_TABLE(ScannerDlg, wxDialog)
  EVT_BUTTON(ID_BUTTON_SCAN_MANY, ScannerDlg::onScanMany)
  EVT_BUTTON(ID_BUTTON_SCAN_SINGLE, ScannerDlg::onScanSingle)
  EVT_BUTTON(ID_BUTTON_CANCEL, ScannerDlg::onCancel)
END_EVENT_TABLE()

namespace
{
  void checkStatus(SANE_Status status)
  {
    if(status != SANE_STATUS_GOOD)
    {
      throw std::runtime_error(sane_strstatus(status));
    }    
  }
}

ScannerDlg::ScannerDlg(wxWindow * parent):
  wxDialog(parent, wxID_ANY, _("Import from device"))
{
  SANE_Int version_code;  
  checkStatus(sane_init(&version_code, nullptr));
  
  const SANE_Device ** device_list;
  checkStatus(sane_get_devices(&device_list, SANE_FALSE));
  if(!*device_list)
  {
    throw std::runtime_error("No devices found");
  }
  
  SetTitle(_("Import from device ") + wxString((*device_list)->model, wxConvUTF8));
  
  checkStatus(sane_open((*device_list)->name, &m_handle));
  
  ScannerOptionFactory optionsFactory;
  int index = 1;
  while(const SANE_Option_Descriptor * description = sane_get_option_descriptor(m_handle, index))
  {
    wxString groupName;
    if(optionsFactory.isGroup(description, groupName))
    {
      m_options.push_back(std::make_pair(groupName, OptionsCollT()));
    }
    else if(optionsFactory.isValidOption(description))
    {
      if(m_options.empty())
      {
        m_options.push_back(std::make_pair(_(""), OptionsCollT()));
      }
      m_options.back().second.push_back(optionsFactory.create(this, m_handle, index, description));
    }
    ++index;
  }
  
  wxBoxSizer * hbox = new wxBoxSizer(wxHORIZONTAL);
  hbox->Add(new wxButton(this, ID_BUTTON_SCAN_MANY, _T("Scan many")),
            1, 
            wxEXPAND | wxRIGHT, 
            3);
  hbox->Add(new wxButton(this, ID_BUTTON_SCAN_SINGLE, _T("Scan single")),
            1, 
            wxEXPAND | wxLEFT | wxRIGHT, 
            3);
  hbox->Add(new wxButton(this, ID_BUTTON_CANCEL, _T("Cancel")),
            1, 
            wxEXPAND | wxLEFT, 
            3);
  
  m_gauge = new wxGauge(this, wxID_ANY, 100);
  
  wxBoxSizer * vbox = new wxBoxSizer(wxVERTICAL);

  for(auto optionList : m_options)
  {
    wxGridSizer * gridSizer = new wxGridSizer(2);
    std::for_each(optionList.second.begin(),
                  optionList.second.end(),
                  std::bind(&ScannerOption::append, std::placeholders::_1, gridSizer));
    
    wxStaticBoxSizer * staticSizer = new wxStaticBoxSizer(wxVERTICAL, this, optionList.first);
    staticSizer->Add(gridSizer, 0, wxEXPAND);
    vbox->Add(staticSizer, 0, wxEXPAND | wxBOTTOM | wxRIGHT | wxLEFT, 7);
  }

  vbox->Add(m_gauge, 0, wxEXPAND | wxBOTTOM | wxRIGHT | wxLEFT, 7);
  vbox->Add(hbox, 0, wxEXPAND | wxBOTTOM | wxRIGHT | wxLEFT, 7);
  
  SetSizer(vbox);
  vbox->SetSizeHints(this);
}

std::vector<wxImage> ScannerDlg::getImages() const
{
  return m_images;
}

ScannerDlg::~ScannerDlg()
{
  sane_close(m_handle);
  sane_exit();
}

void ScannerDlg::onScanMany(wxCommandEvent &)
{
  onScan(true);
}

void ScannerDlg::onScanSingle(wxCommandEvent &)
{
  onScan(false);
}

void ScannerDlg::onScan(bool many)
{
  for(auto optionList : m_options)
  {
    for(auto option : optionList.second)
    {
      option->setOption();
    }
  }

  int imageNb = 0;
  
  while(sane_start(m_handle) == SANE_STATUS_GOOD && (many || imageNb == 0))
  {
    ++imageNb;
    SANE_Parameters parameters;
    checkStatus(sane_get_parameters(m_handle, &parameters));

    std::vector<SANE_Byte> data;
    data.reserve(parameters.bytes_per_line * parameters.lines);
    std::vector<SANE_Byte> buffer(4096);
    wxString message;
    message << _("Scanning page ") << imageNb;
    m_gauge->SetValue(0);
    m_gauge->SetRange(parameters.bytes_per_line * parameters.lines);
    wxTheApp->Yield();
    SANE_Int length;  
    while(sane_read(m_handle, &buffer[0], buffer.size(), &length) == SANE_STATUS_GOOD)
    {
      data.insert(data.end(), buffer.begin(), buffer.begin() + length);
      m_gauge->SetValue(data.size());
      wxTheApp->Yield();
    }
    
    m_gauge->SetValue(parameters.bytes_per_line * parameters.lines);
    wxTheApp->Yield();

    wxImage image(parameters.pixels_per_line, data.size() / parameters.bytes_per_line);
    std::copy(data.begin(), data.end(), image.GetData());
    m_images.push_back(image);
  }
  
  EndModal(imageNb > 0 ? wxID_OK : wxID_CANCEL);
}

void ScannerDlg::onCancel(wxCommandEvent &)
{
  EndModal(wxID_CANCEL);
}
