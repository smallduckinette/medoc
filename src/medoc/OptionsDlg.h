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

#ifndef __MEDOC_OPTIONSDLG_H__
#define __MEDOC_OPTIONSDLG_H__

#include <wx/wx.h>
#include <wx/notebook.h>

#include "DatabaseOptionsPanel.h"
#include "PdfOptionsPanel.h"

class OptionsDlg : public wxDialog
{
public:
  OptionsDlg(wxWindow * parent);
  
private:
  wxNotebook * m_options;
  DatabaseOptionsPanel * m_dbPanel;
  PdfOptionsPanel * m_pdfPanel;
};

#endif
