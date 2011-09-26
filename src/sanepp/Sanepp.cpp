#include "Sanepp.h"

#include <sane/sane.h>

#include "Exception.h"
#include "Device.h"

namespace
{
  void checkStatus(SANE_Status status)
  {
    if(status != SANE_STATUS_GOOD)
    {
      throw sanepp::SaneppException(sane_strstatus(status));
    }    
  }
}

sanepp::Sanepp::Sanepp()
{
  SANE_Int version_code;
  
  checkStatus(sane_init(&version_code, nullptr));
}

sanepp::Sanepp::~Sanepp()
{
  sane_exit();
}

std::vector<std::shared_ptr<sanepp::Device> > sanepp::Sanepp::listDevices() const
{
  std::vector<std::shared_ptr<Device> > devices;
  
  const SANE_Device ** device_list;
  checkStatus(sane_get_devices(&device_list, SANE_FALSE));
  while(*device_list)
  {
    devices.push_back(std::shared_ptr<Device>(new Device((*device_list)->name,
                                                         (*device_list)->vendor,
                                                         (*device_list)->model,
                                                         (*device_list)->type)));

    ++device_list;
  }
  
  return devices;
}
