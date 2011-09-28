#include "Frame.h"

#include "ImagePanel.h"


BEGIN_EVENT_TABLE(Frame, wxFrame)
  EVT_MENU(ID_FILE_QUIT, Frame::onQuit)
END_EVENT_TABLE()


Frame::Frame():
  wxFrame(nullptr, 
          wxID_ANY,
          _("Medoc"), 
          wxDefaultPosition,
          wxSize(800, 600))
{
  // Menus
  wxMenu * menuFile = new wxMenu;
  menuFile->Append(ID_FILE_QUIT, _("E&xit"));

  wxMenuBar * menuBar = new wxMenuBar;
  menuBar->Append(menuFile, _("&File"));

  SetMenuBar(menuBar);

  // Frame
  wxBoxSizer * sizer = new wxBoxSizer(wxHORIZONTAL);
  ImagePanel * imagePanel = new ImagePanel(this, wxImage(_("P1010568.JPG")));
  sizer->Add(imagePanel, 1, wxEXPAND);

  // Show
  SetSizer(sizer);
  Show(true);
}

void Frame::onQuit(wxCommandEvent &)
{
  Close();
}
