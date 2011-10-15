#include "ScannerDlg.h"

#include <stdexcept>
#include <algorithm>
#include "ScannerOptionFactory.h"
#include "ScannerOption.h"

BEGIN_EVENT_TABLE(ScannerDlg, wxDialog)
  EVT_BUTTON(ID_BUTTON_SCAN, ScannerDlg::onScan)
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
  
  int nbOptions = sane_get_option_descriptor(m_handle, 0)->size;
  ScannerOptionFactory optionsFactory;
  
  for(int i = 1; i < nbOptions; ++i)
  {
    const SANE_Option_Descriptor * description = sane_get_option_descriptor(m_handle, i);
    if(optionsFactory.isValidOption(description))
    {
      m_options.push_back(optionsFactory.create(this, m_handle, i, description));
    }
  }
  
  
  wxGridSizer * gridSizer = new wxGridSizer(2);
  std::for_each(m_options.begin(),
                m_options.end(),
                std::bind(&ScannerOption::append, std::placeholders::_1, gridSizer));
  wxBoxSizer * hbox = new wxBoxSizer(wxHORIZONTAL);
  hbox->Add(new wxButton(this, ID_BUTTON_SCAN, _T("Scan")),
            1, 
            wxEXPAND | wxRIGHT, 
            3);
  hbox->Add(new wxButton(this, ID_BUTTON_CANCEL, _T("Cancel")),
            1, 
            wxEXPAND | wxLEFT, 
            3);
    
  wxBoxSizer * vbox = new wxBoxSizer(wxVERTICAL);
  vbox->Add(gridSizer, 1, wxEXPAND | wxALL, 7);
  vbox->Add(hbox, 0, wxEXPAND | wxBOTTOM | wxRIGHT | wxLEFT, 7);
  
  SetSizer(vbox);
  vbox->SetSizeHints(this);
}

wxImage ScannerDlg::getImage() const
{
  return m_image;
}

ScannerDlg::~ScannerDlg()
{
  sane_close(m_handle);
  sane_exit();
}

void ScannerDlg::onScan(wxCommandEvent &)
{
  checkStatus(sane_start(m_handle));
  
  SANE_Parameters parameters;
  checkStatus(sane_get_parameters(m_handle, &parameters));
  
  m_image.Create(parameters.pixels_per_line, parameters.lines);
  
  size_t currentPosition = 0;
  SANE_Int length;
  while(sane_read(m_handle, m_image.GetData() + currentPosition, 4096, &length) == SANE_STATUS_GOOD)
  {
    currentPosition += length;
  }

  std::cout << currentPosition << std::endl;

  EndModal(wxID_OK);
}

void ScannerDlg::onCancel(wxCommandEvent &)
{
  EndModal(wxID_CANCEL);
}
