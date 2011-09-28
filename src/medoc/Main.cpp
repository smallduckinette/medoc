#include <wx/wx.h>

#include "Frame.h"

class App : public wxApp
{
  bool OnInit();
};

IMPLEMENT_APP(App);

bool App::OnInit()
{
  wxInitAllImageHandlers();
  
  SetTopWindow(new Frame());
  
  return true;
}
