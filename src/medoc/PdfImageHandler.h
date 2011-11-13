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

#ifndef __MEDOC_PDFIMAGEHANDLER_H__
#define __MEDOC_PDFIMAGEHANDLER_H__

#include <wx/wx.h>

class PdfImageHandler : public wxImageHandler
{
public:
  PdfImageHandler();

  virtual int GetImageCount(wxInputStream & stream);
  virtual bool LoadFile(wxImage * image, wxInputStream & stream, bool verbose=true, int index=-1);
  virtual bool SaveFile(wxImage * image, wxOutputStream & stream, bool verbose=true);
  
protected:
  bool DoCanRead(wxInputStream & stream);
};

#endif
