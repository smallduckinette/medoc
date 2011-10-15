#ifndef __MEDOC_PAGEINFO_H__
#define __MEDOC_PAGEINFO_H__

#include <wx/wx.h>

class PageInfo : public wxClientData
{
public:
  PageInfo(const wxString & file);
  
  wxImage getImage() const;

private:
  wxImage m_image;
};

#endif