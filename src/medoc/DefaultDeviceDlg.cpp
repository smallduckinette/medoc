// Copyright 2012 small_duck
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

#include "DefaultDeviceDlg.h"

#include <stdexcept>
#include <sane/sane.h>

#include "MedocConfig.h"

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


BEGIN_EVENT_TABLE(DefaultDeviceDlg, wxDialog)
  EVT_BUTTON(wxID_OK, DefaultDeviceDlg::onOk)
END_EVENT_TABLE()


DefaultDeviceDlg::DefaultDeviceDlg(wxWindow * parent):
  wxDialog(parent, wxID_ANY, _("Set default device")),
  _devices(new wxListBox(this, wxID_ANY))
{
  SANE_Int version_code;  
  checkStatus(sane_init(&version_code, nullptr));
  
  const SANE_Device ** device_list;
  checkStatus(sane_get_devices(&device_list, SANE_FALSE));
  
  MedocConfig config;

  while(*device_list)
  {
    wxString deviceName((*device_list)->name, wxConvUTF8);
    int index = _devices->Append(wxString((*device_list)->model, wxConvUTF8), 
                                 new wxStringClientData(deviceName));
    if(deviceName == config.getDefaultDevice())
    {
      _devices->SetSelection(index);
    }
    ++device_list;
  }
  
  sane_exit();
  
  wxBoxSizer * vbox = new wxBoxSizer(wxVERTICAL);
  vbox->Add(_devices, 1, wxEXPAND);
  vbox->Add(CreateStdDialogButtonSizer(wxOK | wxCANCEL), 0, wxEXPAND | wxALL, 7);  
  
  SetSizer(vbox);
  vbox->SetSizeHints(this);  
}


void DefaultDeviceDlg::onOk(wxCommandEvent & event)
{
  int index = _devices->GetSelection();
  if(index != wxNOT_FOUND)
  {
    MedocConfig config;
    config.setDefaultDevice(dynamic_cast<wxStringClientData *>(_devices->GetClientObject(index))->GetData());
  }
  event.Skip();
}
