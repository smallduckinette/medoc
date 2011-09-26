#ifndef __SANEPP_SANEPP_H__
#define __SANEPP_SANEPP_H__

#include <memory>
#include <vector>

namespace sanepp
{
  class Device;

  class Sanepp
  {
  public:
    Sanepp();
    ~Sanepp();
    
    std::vector<std::shared_ptr<Device> > listDevices() const;
  };
}

#endif
