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

#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <wx/scrolwin.h>

#include "ImageUtils.h"

BEGIN_EVENT_TABLE(GalleryPanel, wxScrolledWindow)
  EVT_LEFT_DOWN(GalleryPanel::onSelect)
END_EVENT_TABLE()


GalleryPanel::GalleryPanel(wxWindow * parent,
                           wxWindowID id,
                           int cols,
                           int imageSize):
  wxScrolledWindow(parent, id, wxDefaultPosition, wxSize((imageSize + 20) * cols + 20, 0)),
  m_cols(cols),
  m_imageSize(imageSize)
{
  SetScrollRate(10, 10);
}

int GalleryPanel::addImage(const wxImage & image)
{
  m_bitmaps.push_back(std::make_shared<GalleryItem>(image, scale(image, m_imageSize)));
  Refresh();
  return (m_bitmaps.size() - 1);
}

wxImage GalleryPanel::getImage(int index) const
{
  if(index >= 0 && index < static_cast<int>(m_bitmaps.size()))
  {
    return m_bitmaps.at(index)->m_image;
  }
  else
  {
    throw std::runtime_error("Bad index");
  }
}

std::vector<wxImage> GalleryPanel::getAllImages() const
{
  std::vector<wxImage> images;
  std::transform(m_bitmaps.begin(),
                 m_bitmaps.end(),
                 std::back_inserter(images),
                 std::bind(&GalleryItem::m_image, std::placeholders::_1));
  return images;
}

void GalleryPanel::clear()
{
  m_bitmaps.clear();
  Refresh();
}

void GalleryPanel::upSelection()
{
  moveSelection(m_bitmaps.begin(), m_bitmaps.end());
}

void GalleryPanel::downSelection()
{
  moveSelection(m_bitmaps.rbegin(), m_bitmaps.rend());
}

void GalleryPanel::eraseSelection()
{
  m_bitmaps.erase(std::remove_if(m_bitmaps.begin(), 
                                 m_bitmaps.end(),
                                 [](const std::shared_ptr<GalleryItem> & item) { return item->isSelected(); }),
                  m_bitmaps.end());
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
  for(const std::shared_ptr<GalleryItem> & item : m_bitmaps)
  {
    const wxBitmap & bitmap = item->m_bitmap;
    int posx = x * (m_imageSize + 20) + (m_imageSize / 2 + 10) - bitmap.GetWidth() / 2;
    int posy = y * (m_imageSize + 20) + (m_imageSize / 2 + 10) - bitmap.GetHeight() / 2;
    
    if(item->isSelected())
    {
      dc.DrawRoundedRectangle(x * (m_imageSize + 20) + 5,
                              y * (m_imageSize + 20) + 5,
                              m_imageSize + 10,
                              m_imageSize + 10,
                              3);
    }
    dc.DrawBitmap(bitmap, posx, posy, false);
    ++x;
    if(x >= m_cols)
    {
      x = 0;
      ++y;
    }
  }
}

void GalleryPanel::onSelect(wxMouseEvent & event)
{
  int posx;
  int posy;
  CalcUnscrolledPosition(event.GetX(), event.GetY(), &posx, &posy);
  
  size_t imagex = std::floor(static_cast<double>(posx) / (m_imageSize + 20));
  size_t imagey = std::floor(static_cast<double>(posy) / (m_imageSize + 20));
  size_t imageIndex = imagex + imagey * m_cols;
  
  if(!event.ControlDown())
  {
    unselectAll();
  }
  
  if(imageIndex < m_bitmaps.size())
  {
    if(event.ControlDown())
    {
      m_bitmaps.at(imageIndex)->invertSelection();
    }
    else
    {
      m_bitmaps.at(imageIndex)->select();
    }
    wxCommandEvent commandEvent(wxEVT_COMMAND_LISTBOX_SELECTED, GetId());
    commandEvent.SetInt(imageIndex);
    commandEvent.SetExtraLong(1);
    GetEventHandler()->ProcessEvent(commandEvent);
  }
  
  event.Skip();
  Refresh();
}

template<typename ITERATOR>
void GalleryPanel::moveSelection(ITERATOR begin, ITERATOR end)
{
  ITERATOR it = begin;
  for(; it != end; ++it)
  {
    if((*it)->isSelected()
       && it != begin
       && !(*(it - 1))->isSelected())
    {
      std::swap(*it, *(it - 1));
    }
  }
  Refresh();  
}

void GalleryPanel::unselectAll()
{
  std::for_each(m_bitmaps.begin(),
                m_bitmaps.end(),
                std::bind(&GalleryItem::deselect, std::placeholders::_1));
}

GalleryPanel::GalleryItem::GalleryItem(const wxImage & image,
                                       const wxBitmap & bitmap):
  m_image(image),
  m_bitmap(bitmap),
  m_selected(false)
{
}

void GalleryPanel::GalleryItem::select()
{
  m_selected = true;
}

void GalleryPanel::GalleryItem::deselect()
{
  m_selected = false;
}

void GalleryPanel::GalleryItem::invertSelection()
{
  m_selected = !m_selected;
}

bool GalleryPanel::GalleryItem::isSelected() const
{
  return m_selected;
}
