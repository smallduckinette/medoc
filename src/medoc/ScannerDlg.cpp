#include "ScannerDlg.h"

#include <stdexcept>

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
  try
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
    
    int nbOptions = sane_get_option_descriptor(m_handle, 0)->size;
    
    for(int i = 1; i < nbOptions; ++i)
    {
      const SANE_Option_Descriptor * description = sane_get_option_descriptor(m_handle, i);
      std::cout << description->name << std::endl;
      std::cout << description->title << std::endl;
      std::cout << description->desc << std::endl;
    }
  }
  catch(const std::exception & e)
  {
    wxMessageDialog message(parent, wxString(e.what(), wxConvUTF8));
    message.ShowModal();
  }
}

ScannerDlg::~ScannerDlg()
{
  sane_close(m_handle);
  sane_exit();
}
