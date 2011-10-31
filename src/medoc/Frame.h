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
      ID_FILE_QUIT = wxID_HIGHEST + 1,
      ID_IMPORT_FILE,
      ID_IMPORT_DEVICE,
      ID_EXPORT_FILE,
      ID_EXPORT_DB,
      ID_CONFIG_DB,
      ID_IMAGE_LIST
    };
  
  void onQuit(wxCommandEvent &);
  void onImportFile(wxCommandEvent &);
  void onImportDevice(wxCommandEvent &);
  void onExportFile(wxCommandEvent &);
  void onExportDb(wxCommandEvent &);
  void onConfigureDatabase(wxCommandEvent &);

  void onImageSelected(wxCommandEvent & event);
  
  DECLARE_EVENT_TABLE();

private:
  std::vector<wxImage> getImages() const;

  wxListBox * m_imageList;
  ImagePanel * m_imagePanel;
  Config m_config;
};

#endif
