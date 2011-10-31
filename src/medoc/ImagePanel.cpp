#include "ImagePanel.h"


ImagePanel::ImagePanel(wxWindow * parent):
  wxScrolledWindow(parent)
{
  SetScrollRate(10, 10);
}

ImagePanel::ImagePanel(wxWindow * parent, const wxImage & image):
  wxScrolledWindow(parent),
  m_image(image)
{
  SetScrollRate(10, 10);
}

void ImagePanel::setImage(const wxImage & image)
{
  m_image = wxBitmap(image);
  Refresh();
}

void ImagePanel::OnDraw(wxDC & dc)
{
  dc.DrawBitmap(m_image, 0, 0, false);
  SetVirtualSize(m_image.GetWidth(), m_image.GetHeight());
}
