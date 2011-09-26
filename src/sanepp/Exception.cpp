#include "Exception.h"



sanepp::SaneppException::SaneppException(const std::string & msg):
  std::runtime_error(msg)
{
}
