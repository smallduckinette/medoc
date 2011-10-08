#include "PageInfo.h"



PageInfo::PageInfo(const wxString & file):
  m_image(file)
{
}

wxImage PageInfo::getImage() const
{
  return m_image;
}
