#include <wx/wx.h>
#include <wx/aboutdlg.h>
#include <wx/filedlg.h>

#include <fstream>

#include "mainframe.hpp"
#include "dqframe.hpp"

class Servoterm: public wxApp
{
public:
    bool OnInit();
    void OnOpen(wxCommandEvent& WXUNUSED(event));
    void OnSend(wxCommandEvent& WXUNUSED(event));
private:
    ServoFrame* servoframe;
    wxString filename;
    dqFrame* dqframe;
    void OnAbout(wxCommandEvent& WXUNUSED(event));
};

IMPLEMENT_APP(Servoterm)

using std::cout;
using std::endl;
using std::ifstream;
using std::string;

bool Servoterm::OnInit()
{
    servoframe = new ServoFrame(wxT("Servoterm"));
    dqframe = new dqFrame(wxT("dq"));
    wxMenuBar *menuBar = new wxMenuBar;
    wxMenu *fileMenu = new wxMenu;
    filename = "";
    
    menuBar->Append(fileMenu, "&File");
    fileMenu->Append(wxID_OPEN);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &Servoterm::OnOpen, this, wxID_OPEN);
    //fileMenu->Append(wxID_ANY, "&Send\tCTRL+S");
    //Bind(wxEVT_COMMAND_MENU_SELECTED, &Servoterm::OnSend, this, wxID_ANY);
    
    wxMenu *helpMenu = new wxMenu;
    menuBar->Append(helpMenu, "&Help" );
    helpMenu->Append(wxID_ABOUT);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &Servoterm::OnAbout, this, wxID_ABOUT);

    servoframe->SetMenuBar( menuBar );

    //frame->CreateStatusBar();
    //frame->SetStatusText("Statuskram und so");
 
    servoframe->Show(TRUE);
    dqframe->Show(TRUE);
    return TRUE;
}

void Servoterm::OnSend(wxCommandEvent& WXUNUSED(event)){
    
}

void Servoterm::OnOpen(wxCommandEvent& WXUNUSED(event)){
    wxFileDialog openFileDialog(servoframe, wxEmptyString, SRCDIR"/..", "","*", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;
    ifstream infile(openFileDialog.GetPath());
    string line;
    while (std::getline(infile, line))
    {
        if(!servoframe->send(line))
            break;
    }
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