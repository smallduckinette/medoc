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

#include "GalleryPanel.h"

#include "ImageUtils.h"


GalleryPanel::GalleryPanel(wxWindow * parent,
                           int cols,
                           int imageSize):
  wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxSize((imageSize + 20) * cols, 0)),
  m_cols(cols),
  m_imageSize(imageSize)
{
  SetScrollRate(10, 10);
}

void GalleryPanel::addImage(const wxImage & image)
{
  m_bitmaps.push_back(scale(image, m_imageSize));
  Refresh();
}

void GalleryPanel::clear()
{
  m_bitmaps.clear();
  Refresh();
}

void GalleryPanel::OnDraw(wxDC & dc)
{
  wxBrush backgroundBrush = dc.GetBackground();
  backgroundBrush.SetColour(*wxWHITE);
  dc.SetBackground(backgroundBrush);
  dc.Clear();
  
  SetVirtualSize(m_cols * (m_imageSize + 20),
                 (ceil(static_cast<double>(m_bitmaps.size()) / m_cols)) * (m_imageSize + 20));
  
  int x = 0;
  int y = 0;
  for(const wxBitmap & bitmap : m_bitmaps)
  {
    int posx = x * (m_imageSize + 20) + (m_imageSize / 2 + 10) - bitmap.GetWidth() / 2;
    int posy = y * (m_imageSize + 20) + (m_imageSize / 2 + 10) - bitmap.GetHeight() / 2;

    dc.DrawBitmap(bitmap, posx, posy, false);
    ++x;
    if(x >= m_cols)
    {
      x = 0;
      ++y;
    }
  }
}
