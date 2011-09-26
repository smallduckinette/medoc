#ifndef __SANEPP_DEVICE_H__
#define __SANEPP_DEVICE_H__

#include <string>
#include <ostream>

namespace sanepp
{
  class Device
  {
  public:
    Device(const std::string & name,
           const std::string & vendor,
           const std::string & model,
           const std::string & type);

    std::string getName() const;
    std::string getVendor() const;
    std::string getModel() const;
    std::string getType() const;

  private:
    std::string m_name;
    std::string m_vendor;
    std::string m_model;
    std::string m_type;
  };

  std::ostream & operator<<(std::ostream & stream, const Device & device);
}

#endif
