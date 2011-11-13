// Copyright 2011 small_duck
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "ImagePanel.h"


ImagePanel::ImagePanel(wxWindow * parent):
  wxScrolledWindow(parent)
{
  SetScrollRate(10, 10);
}

void ImagePanel::clear()
{
  m_image = wxImage();
  m_bitmap = wxBitmap();
  Refresh();
}

void ImagePanel::setImage(const wxImage & image)
{
  m_image = image;
  m_bitmap = wxBitmap(m_image);
  Refresh();
}

void ImagePanel::zoomIn()
{
}

void ImagePanel::zoomOut()
{
}

void ImagePanel::zoomFit()
{
  if(m_image.GetWidth() > 0)
  {
    double width = GetSize().GetWidth();
    double scale = width / m_image.GetWidth();
    wxImage scaledImage(m_image);
    scaledImage.Rescale(m_image.GetWidth() * scale, 
                        m_image.GetHeight() * scale,
                        wxIMAGE_QUALITY_HIGH);
    m_bitmap = wxBitmap(scaledImage);
    Refresh();
  }
}

void ImagePanel::resetZoom()
{
}

void ImagePanel::OnDraw(wxDC & dc)
{
  dc.DrawBitmap(m_bitmap, 0, 0, false);
  SetVirtualSize(m_bitmap.GetWidth(), m_image.GetHeight());
}
