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

#ifndef __MEDOC_SCANNER_H__
#define __MEDOC_SCANNER_H__

#include <vector>
#include <memory>
#include <wx/wx.h>
#include <sane/sane.h>

class ScannerOption;

class ScannerDlg : public wxDialog
{
public:
  ScannerDlg(wxWindow * parent);
  ~ScannerDlg();
  
  std::vector<wxImage> getImages() const;
  
protected:
  enum
    {
      ID_BUTTON_SCAN_MANY = wxID_HIGHEST + 1,
      ID_BUTTON_SCAN_SINGLE,
      ID_BUTTON_CANCEL
    };
  
  void onScanMany(wxCommandEvent &);
  void onScanSingle(wxCommandEvent &);
  void onCancel(wxCommandEvent &);
  

  DECLARE_EVENT_TABLE();
  
private:
  void onScan(bool many);
  
  SANE_Handle m_handle;
  std::vector<wxImage> m_images;
  
  std::vector<std::shared_ptr<ScannerOption> > m_options;

  wxGauge * m_gauge;
};

#endif
