#include "Frame.h"

BEGIN_EVENT_TABLE(Frame, wxFrame)
  EVT_MENU(ID_FILE_QUIT, Frame::onQuit)
END_EVENT_TABLE()

Frame::Frame():
  wxFrame(nullptr, 
          wxNewId(), 
          _("Medoc"), 
          wxDefaultPosition,
          wxSize(800, 600))
{
  wxMenu * menuFile = new wxMenu;
  menuFile->Append(ID_FILE_QUIT, _("E&xit"));

  wxMenuBar * menuBar = new wxMenuBar;
  menuBar->Append(menuFile, _("&File"));

  SetMenuBar(menuBar);

  Show(true);
}

void Frame::onQuit(wxCommandEvent &)
{
  Close();
}
