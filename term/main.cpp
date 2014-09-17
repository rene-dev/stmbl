#include <wx/wx.h>
#include <wx/aboutdlg.h>
#include <wx/filedlg.h>

#include "mainframe.hpp"

class Servoterm: public wxApp
{
public:
    bool OnInit();
private:
    MainFrame* frame;
    void OnAbout(wxCommandEvent& WXUNUSED(event));
};

IMPLEMENT_APP(Servoterm)

bool Servoterm::OnInit()
{
    frame = new MainFrame(wxT("Servoterm"));
    wxMenuBar *menuBar = new wxMenuBar;
    
    wxMenu *helpMenu = new wxMenu;
    menuBar->Append(helpMenu, "&Help" );
    helpMenu->Append(wxID_ABOUT);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &Servoterm::OnAbout, this, wxID_ABOUT);

    frame->SetMenuBar( menuBar );

    //frame->CreateStatusBar();
    //frame->SetStatusText("Statuskram und so");
 
    frame->Show(TRUE);
    return TRUE;
}

void Servoterm::OnAbout(wxCommandEvent& WXUNUSED(event)){
    /*
    wxAboutDialogInfo aboutInfo;
    aboutInfo.SetName("Servoterm");
    aboutInfo.SetVersion("git");
    aboutInfo.SetDescription(_("Servoterm"));
    aboutInfo.SetCopyright("(C) 2010-2014");
    aboutInfo.SetWebSite("https://github.com/rene-dev/stmbl");
    aboutInfo.AddDeveloper("Crinq");
    aboutInfo.AddDeveloper("Rene-dev");
    wxAboutBox(aboutInfo);
    */
}