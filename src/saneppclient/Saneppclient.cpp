#include <iostream>

#include "sanepp/Sanepp.h"
#include "sanepp/Device.h"

int main()
{
  sanepp::Sanepp sanepp;
  std::vector<std::shared_ptr<sanepp::Device> > devices = sanepp.listDevices();
  for(const std::shared_ptr<sanepp::Device> & device : devices)
  {
    std::cout << device << std::endl;
  }

  return 0;
}
