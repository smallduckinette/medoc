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

#ifndef __MEDOC_SCANNEROPTION_H__
#define __MEDOC_SCANNEROPTION_H__

#include <wx/wx.h>

class ScannerOption
{
public:
  virtual ~ScannerOption();
  
  virtual void append(wxSizer * sizer) = 0;
  virtual void setOption() = 0;
};

#endif
