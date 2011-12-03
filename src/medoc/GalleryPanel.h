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

#ifndef __MEDOC_GALLERYPANEL_H__
#define __MEDOC_GALLERYPANEL_H__

#include <vector>
#include <wx/wx.h>

class GalleryPanel : public wxScrolledWindow
{
public:
  GalleryPanel(wxWindow * parent, 
               wxWindowID id,
               int cols,
               int imageSize);
  
  void addImage(const wxImage & image);
  void clear();
  
protected:
  void OnDraw(wxDC & dc);
  
  void onSelect(wxMouseEvent & event);

  DECLARE_EVENT_TABLE();
  
private:
  int m_orientation;
  int m_cols;
  int m_imageSize;
  std::vector<wxBitmap> m_bitmaps;
};

#endif
