#ifndef __MEDOC_IMAGEPANEL_H__
#define __MEDOC_IMAGEPANEL_H__

#include <wx/wx.h>

class ImagePanel : public wxScrolledWindow
{
public:
  ImagePanel(wxWindow * parent, const wxImage & image);
  
protected:
  void OnDraw(wxDC & dc);

private:
  wxBitmap m_image;
};

#endif
