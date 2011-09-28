#include "ImagePanel.h"


BEGIN_EVENT_TABLE(ImagePanel, wxScrolledWindow)
  EVT_PAINT(ImagePanel::onPaint)
END_EVENT_TABLE()


ImagePanel::ImagePanel(wxWindow * parent, const wxImage & image):
  wxScrolledWindow(parent),
  m_image(image)
{
  SetScrollRate(10, 10);
}

void ImagePanel::onPaint(wxPaintEvent &)
{
  wxPaintDC dc(this);
  DoPrepareDC(dc);
  dc.DrawBitmap(m_image, 0, 0, false);
  SetVirtualSize(m_image.GetWidth(), m_image.GetHeight());
}
