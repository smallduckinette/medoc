#include "Sanepp.h"

#include <iostream>
#include <sane/sane.h>

#include "Exception.h"

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

void sanepp::Sanepp::listDevices()
{
  const SANE_Device ** device_list;
  checkStatus(sane_get_devices(&device_list, SANE_FALSE));
  while(*device_list)
  {
    std::cout << "* "
              << (*device_list)->name << " "
              << (*device_list)->vendor << " "
              << (*device_list)->model << " "
              << (*device_list)->type << std::endl;
    ++device_list;
  }
}
