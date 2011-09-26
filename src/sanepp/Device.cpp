#include "Device.h"

sanepp::Device::Device(const std::string & name,
                       const std::string & vendor,
                       const std::string & model,
                       const std::string & type):
  m_name(name),
  m_vendor(vendor),
  m_model(model),
  m_type(type)
{
}

std::string sanepp::Device::getName() const
{
  return m_name;
}

std::string sanepp::Device::getVendor() const
{
  return m_vendor;
}

std::string sanepp::Device::getModel() const
{
  return m_model;
}

std::string sanepp::Device::getType() const
{
  return m_type;
}

std::ostream & sanepp::operator<<(std::ostream & stream, const Device & device)
{
  stream << device.getName() << " "
         << device.getVendor() << " "
         << device.getModel() << " "
         << device.getType();
  return stream;
}
