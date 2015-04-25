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
  wxScrolledWindow(parent),
  m_scale(1)
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
  m_scale *= 1.1;
  setBitmap();
}

void ImagePanel::zoomOut()
{
  m_scale *= 0.9;
  setBitmap();
}

void ImagePanel::zoomFit()
{
  if(m_image.GetWidth() > 0)
  {
    double width = GetSize().GetWidth();
    m_scale = width / m_image.GetWidth();
    setBitmap();
  }
}

void ImagePanel::resetZoom()
{
  m_scale = 1;
  setBitmap();
}

void ImagePanel::OnDraw(wxDC & dc)
{
  if(m_bitmap.IsOk())
  {
    dc.DrawBitmap(m_bitmap, 0, 0, false);
    SetVirtualSize(m_bitmap.GetWidth(), m_image.GetHeight());
  }
}

void ImagePanel::setBitmap()
{
  wxImage scaledImage(m_image);
  scaledImage.Rescale(m_image.GetWidth() * m_scale, 
                      m_image.GetHeight() * m_scale,
                      wxIMAGE_QUALITY_HIGH);
  m_bitmap = wxBitmap(scaledImage);
  Refresh();
}  
