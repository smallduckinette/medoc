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
#include <memory>
#include <wx/wx.h>

class GalleryPanel : public wxScrolledWindow
{
public:
  GalleryPanel(wxWindow * parent, 
               wxWindowID id,
               int cols,
               int imageSize);
  
  int addImage(const wxImage & image);
  wxImage getImage(int index) const;
  std::vector<wxImage> getAllImages() const;
  void clear();
  
  void upSelection();
  void downSelection();
  void eraseSelection();
  
protected:
  void OnDraw(wxDC & dc);
  
  void onSelect(wxMouseEvent & event);

  DECLARE_EVENT_TABLE();
  
private:
  template<typename ITERATOR>
  void moveSelection(ITERATOR begin, ITERATOR end);
  void unselectAll();
  
  struct GalleryItem
  {
    GalleryItem(const wxImage & image,
                const wxBitmap & bitmap);
    
    void select();
    void deselect();
    void invertSelection();
    bool isSelected() const;

    wxImage m_image;
    wxBitmap m_bitmap;
    
  private:
    bool m_selected;
  };
  
  int m_orientation;
  int m_cols;
  int m_imageSize;
  typedef std::vector<std::shared_ptr<GalleryItem> > BitmapsT;
  BitmapsT m_bitmaps;
};

#endif
