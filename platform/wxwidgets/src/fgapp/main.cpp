/*****************************************************************************
 * Project:  Feographia
 * Purpose:  The application to work with the biblical text
 * Author:   NikitaFeodonit, nfeodonit@yandex.com
 *****************************************************************************
 *   Copyright (c) 2017-2018 NikitaFeodonit
 *
 *    This file is part of the Feographia project.
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published
 *    by the Free Software Foundation, either version 3 of the License,
 *    or (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *    See the GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program. If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/

// Based on the code from:
// http://mrl.nyu.edu/~ajsecord/downloads/wxAGG-1.1.tgz

#include <iostream>

#include <wx/app.h>
#include <wx/filedlg.h>
#include <wx/frame.h>
#include <wx/intl.h>
#include <wx/menu.h>
#include <wx/msgdlg.h>

#include "hgkamva/platform/wxwidgets/HgKamvaWxWindow.h"

namespace fg
{
//////// class MainFrame ////////

/// Main window of the application.
class MainFrame : public wxFrame
{
public:
  /// Create a new window.
  MainFrame(wxFrame* parent,
      const wxString& title = wxT(""),
      const wxPoint& pos = wxDefaultPosition,
      const wxSize& size = wxDefaultSize);

  /// Clean up resources owned by the window.
  virtual ~MainFrame();

  /// Initialize the main window
  void init(int argc, wxChar** argv);

  /// Change the status bar message.
  void updateStatus(const wxString& s);

protected:
  /// Exit event
  void onExit(wxCommandEvent& event);

  /// "About" event
  void onAbout(wxCommandEvent& event);

  /// File menu events
  void onFileMenu(wxCommandEvent& event);

  /// Window close event.
  void onClose(wxCloseEvent& event);

  /// Window paint event.
  void onPaint(wxPaintEvent& event);

  /// Open a file
  void open();

  /// Setup the status bar and menu bar.
  void initStandardGUI();

private:
  hg::HgKamvaWxWindow mHgKamva;  ///< The AGG bitmap display panel

  wxMenuBar* mMenuBar;  ///< Menu bar
  wxMenu* mFileMenu;  ///< File menu
  wxMenu* mHelpMenu;  ///< Help menu

protected:
  DECLARE_EVENT_TABLE()  /// Allocate wxWidgets storage for event handlers
};  // class MainFrame

// Declare the event handlers for the menu items, etc.
BEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_MENU(wxID_EXIT, MainFrame::onExit)
EVT_MENU(wxID_ABOUT, MainFrame::onAbout)
EVT_MENU(wxID_OPEN, MainFrame::onFileMenu)
EVT_CLOSE(MainFrame::onClose)
END_EVENT_TABLE()

MainFrame::MainFrame(wxFrame* parent,
    const wxString& title,
    const wxPoint& pos,
    const wxSize& size)
    : wxFrame(parent, wxID_ANY, title, pos, size)
    , mHgKamva(this)
    , mMenuBar(nullptr)
    , mFileMenu(nullptr)
    , mHelpMenu(nullptr)
{
  // All menus are deleted by wxMenuBar
  mFileMenu = new wxMenu(wxT(""), wxMENU_TEAROFF);
  mFileMenu->Append(wxID_OPEN, _("&Open\tCtrl-O"), _("Open..."));
  mFileMenu->Append(wxID_EXIT, _("&Quit\tCtrl-Q"), _("Quit the application"));

  mHelpMenu = new wxMenu(wxT(""), wxMENU_TEAROFF);
  mHelpMenu->Append(wxID_ABOUT, _("About"), _("About"));

  // Deleted by wxFrame
  mMenuBar = new wxMenuBar;
  mMenuBar->Append(mFileMenu, _("&File"));
  mMenuBar->Append(mHelpMenu, _("&Help"));

  CreateStatusBar(2);
  SetMenuBar(mMenuBar);
  updateStatus(_("Feographia status bar."));
}

MainFrame::~MainFrame() {}

void MainFrame::onClose(wxCloseEvent& WXUNUSED(event))
{
  Destroy();
}

void MainFrame::onFileMenu(wxCommandEvent& event)
{
  switch(event.GetId()) {
    case wxID_OPEN:
      open();
      break;

    default:
      assert(!"Unknown event ID");
      break;
  }
}

void MainFrame::onExit(wxCommandEvent& WXUNUSED(event))
{
  Close(TRUE);
}

void MainFrame::onAbout(wxCommandEvent& WXUNUSED(event))
{
  wxMessageBox(
      _("Example of combining wxWidgets and the Anti-Grain Geometry renderer."),
      _("About Feographia"));
}

void MainFrame::updateStatus(const wxString& s)
{
  if(GetStatusBar())
    SetStatusText(s);
}

void MainFrame::open()
{
  // Commented because of compile error :
  // ‘wxOPEN’ (wxMULTIPLE, wxFILE_MUST_EXIST) was not declared in this scope

  // Get a file or a set of files.
  //  wxFileDialog dlg(this, _("Open"), wxT(""), wxT(""), wxT("*"),
  //      wxOPEN | wxMULTIPLE | wxFILE_MUST_EXIST);
  //
  //  if (dlg.ShowModal() == wxID_CANCEL)
  //    return;
  //
  //  wxArrayString files;
  //  dlg.GetFilenames(files);
  //
  //  if (files.GetCount() == 0)
  //    return;
  //
  //  // Do something intelligent with the files.
  //  wxString msg;
  //  for (int i = 0; i < (int) files.GetCount(); ++i) {
  //    msg += files[i] + (i == files.GetCount() - 1 ? wxT("") : wxT("\n"));
  //  }
  //  wxMessageBox(msg, _("Selected files"), wxICON_INFORMATION | wxOK, this);
}

//////// class Application ////////

/// The top-most interface to wxWidgets.
class Application : public wxApp
{
public:
  Application();

  /// wxWidgets calls this to initialize the user interface
  bool OnInit();

protected:
  MainFrame* mFrame;
};  // class Application

Application::Application()
    : mFrame(nullptr)
{
  // empty
}

// Application initialization.
bool Application::OnInit()
{
  // Create a new window.
  mFrame = new MainFrame(
      (wxFrame*) nullptr, _("Feographia"), wxDefaultPosition, wxSize(640, 480));

  //SetTopWindow(frame);
  mFrame->Show(true);

  // Can do something application-specific here with the wxApp member
  // variables argc and argv.

  return true;
}

}  // namespace fg

//////// main() function ////////

#if defined(__WINDOWS__) && defined(ATTACH_WX_CONSOLE)

// GUI app in Windows with console.
// Attaching a console and writing std outputs to console.
// https://forums.wxwidgets.org/viewtopic.php?t=34374
IMPLEMENT_APP_NO_MAIN(fg::Application)

// Application entry.
int main(int argc, char* argv[])
{
  // Get HINSTANCE of current application.
  std::cout << "Console is attached\n";
  HINSTANCE hInstance = GetModuleHandle(nullptr);
  // Get command line.
  wxCmdLineArgType lpCmdLine = (char*) GetCommandLine();

  // Create GUI window.
  return wxEntry(hInstance, nullptr, lpCmdLine, SW_SHOWNORMAL);
}

#else  // !(defined(__WINDOWS__) && defined(ATTACH_WX_CONSOLE))

// "allows wxWindows to dynamically create an instance of the application
//  object at the appropriate point in wxWindows initialization"
IMPLEMENT_APP(fg::Application)

#endif  // defined(__WINDOWS__) && defined(ATTACH_WX_CONSOLE)
