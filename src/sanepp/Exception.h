#ifndef __SANEPP_EXCEPTION_H__
#define __SANEPP_EXCEPTION_H__

#include <stdexcept>

namespace sanepp
{
  class SaneppException : public std::runtime_error
  {
  public:
    SaneppException(const std::string & msg);
  };
}

#endif
