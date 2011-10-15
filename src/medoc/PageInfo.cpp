#include "PageInfo.h"



PageInfo::PageInfo(const wxString & file):
  m_image(file)
{
}

PageInfo::PageInfo(const wxImage & image):
  m_image(image)
{
}

wxImage PageInfo::getImage() const
{
  return m_image;
}
