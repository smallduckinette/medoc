#ifndef __MEDOC_FRAME_H__
#define __MEDOC_FRAME_H__

#include <wx/wx.h>

class Frame : public wxFrame
{
public:
  Frame();

protected:
  enum
    {
      ID_FILE_QUIT = wxID_HIGHEST + 1
    };

  void onQuit(wxCommandEvent &);

  DECLARE_EVENT_TABLE();
};

#endif
