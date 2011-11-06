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

ImagePanel::ImagePanel(wxWindow * parent, const wxImage & image):
  wxScrolledWindow(parent),
  m_image(image)
{
  SetScrollRate(10, 10);
}

void ImagePanel::clear()
{
  m_image = wxBitmap();
  Refresh();
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
