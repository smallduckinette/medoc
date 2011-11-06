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
    if(optionsFactory.isValidOption(description))
    {
      m_options.push_back(optionsFactory.create(this, m_handle, index, description));
    }
    ++index;
  }
  
  wxGridSizer * gridSizer = new wxGridSizer(2);
  std::for_each(m_options.begin(),
                m_options.end(),
                std::bind(&ScannerOption::append, std::placeholders::_1, gridSizer));
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
    
  wxBoxSizer * vbox = new wxBoxSizer(wxVERTICAL);
  vbox->Add(gridSizer, 1, wxEXPAND | wxALL, 7);
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
  for(auto option : m_options)
  {
    option->setOption();
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
    wxProgressDialog progress(_("Scanning..."), 
                              message,
                              parameters.bytes_per_line * parameters.lines);
    progress.Update(0);
    SANE_Int length;  
    while(sane_read(m_handle, &buffer[0], buffer.size(), &length) == SANE_STATUS_GOOD)
    {
      data.insert(data.end(), buffer.begin(), buffer.begin() + length);
      progress.Update(data.size());
    }
    
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
