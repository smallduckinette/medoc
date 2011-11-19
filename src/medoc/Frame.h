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

#ifndef __MEDOC_FRAME_H__
#define __MEDOC_FRAME_H__

#include <vector>
#include <wx/wx.h>
#include "Config.h"

class ImagePanel;

class Frame : public wxFrame
{
public:
  Frame();

protected:
  enum
    {
      ID_FILE_CLEAR = wxID_HIGHEST + 1,
      ID_FILE_OPTIONS,
      ID_FILE_QUIT,
      ID_IMPORT_FILE,
      ID_IMPORT_DEVICE,
      ID_EXPORT_FILE,
      ID_EXPORT_DB,
      ID_CONFIG_DB,
      ID_IMAGE_LIST,
      ID_VIEW_ZOOMIN,
      ID_VIEW_ZOOMOUT,
      ID_VIEW_ZOOMFIT,
      ID_VIEW_ZOOMRESET
    };
  
  void onClear(wxCommandEvent &);
  void onOptions(wxCommandEvent &);
  void onQuit(wxCommandEvent &);
  void onImportFile(wxCommandEvent &);
  void onImportDevice(wxCommandEvent &);
  void onExportFile(wxCommandEvent &);
  void onExportDb(wxCommandEvent &);
  void onConfigureDatabase(wxCommandEvent &);
  void onZoomIn(wxCommandEvent &);
  void onZoomOut(wxCommandEvent &);
  void onZoomFit(wxCommandEvent &);
  void onZoomReset(wxCommandEvent &);

  void onImageSelected(wxCommandEvent & event);
  
  DECLARE_EVENT_TABLE();
  
private:
  void onImageSelected(int index);
  std::vector<wxImage> getImages() const;
  
  wxListBox * m_imageList;
  ImagePanel * m_imagePanel;
  Config m_config;
};

#endif
