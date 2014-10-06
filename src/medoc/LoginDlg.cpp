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

#include "LoginDlg.h"

LoginDlg::LoginDlg(wxWindow * parent):
  wxDialog(parent, wxID_NEW, _("Login")),
  m_login(new wxTextCtrl(this, wxID_NEW, wxEmptyString)),
  m_password(new wxTextCtrl(this, 
                            wxID_NEW, 
                            wxEmptyString,
                            wxDefaultPosition,
                            wxDefaultSize,
                            wxTE_PASSWORD))
{
  wxGridSizer * grid = new wxGridSizer(2, 3, 3);
  grid->Add(new wxStaticText(this, wxID_NEW, _("Login")), 0, wxALIGN_CENTER_VERTICAL);
  grid->Add(m_login, 0, wxEXPAND);    
  grid->Add(new wxStaticText(this, wxID_NEW, _("Password")), 0, wxALIGN_CENTER_VERTICAL);
  grid->Add(m_password, 0, wxEXPAND);

  wxBoxSizer * vbox = new wxBoxSizer(wxVERTICAL);
  vbox->Add(grid, 0, wxEXPAND | wxALL, 7);  
  vbox->Add(CreateStdDialogButtonSizer(wxOK | wxCANCEL), 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 7);

  m_login->SetFocus();

  SetSizer(vbox);
  vbox->SetSizeHints(this);
}

wxString LoginDlg::getLogin() const
{
  return m_login->GetValue();
}

wxString LoginDlg::getPassword() const
{
  return m_password->GetValue();
}
