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
    void OnWrite(wxCommandEvent& WXUNUSED(event));
    void OnRead(wxCommandEvent& WXUNUSED(event));
    void OnSave(wxCommandEvent& WXUNUSED(event));
    void OnDq(wxCommandEvent& WXUNUSED(event));
private:
    ServoFrame* servoframe;
    wxString filename;
    dqFrame* dqframe;
    int writeID;
    int readID;
    int dqID;
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
    wxMenuBar *menuBar = new wxMenuBar;
    wxMenu *fileMenu = new wxMenu;
    wxMenu *viewMenu = new wxMenu;
    filename = "";
    writeID = wxNewId();
    readID = wxNewId();
    dqID = wxNewId();
    
    menuBar->Append(fileMenu, "&File");
    fileMenu->Append(wxID_OPEN);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &Servoterm::OnOpen, this, wxID_OPEN);
    //fileMenu->Append(wxID_SAVE);
    //Bind(wxEVT_COMMAND_MENU_SELECTED, &Servoterm::OnSave, this, wxID_SAVE);
    fileMenu->Append(writeID, "&Write");
    Bind(wxEVT_COMMAND_MENU_SELECTED, &Servoterm::OnWrite, this, writeID);
    fileMenu->Append(readID, "&Read");
    Bind(wxEVT_COMMAND_MENU_SELECTED, &Servoterm::OnRead, this, readID);
    
    menuBar->Append(viewMenu, "&View");
    viewMenu->Append(dqID, "&DQsim");
    Bind(wxEVT_COMMAND_MENU_SELECTED, &Servoterm::OnDq, this, dqID);
    
    wxMenu *helpMenu = new wxMenu;
    menuBar->Append(helpMenu, "&Help" );
    helpMenu->Append(wxID_ABOUT);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &Servoterm::OnAbout, this, wxID_ABOUT);

    servoframe->SetMenuBar( menuBar );

    //frame->CreateStatusBar();
    //frame->SetStatusText("Statuskram und so");
 
    servoframe->Show(TRUE);
    return TRUE;
}

void Servoterm::OnDq(wxCommandEvent& WXUNUSED(event)){
    dqframe = new dqFrame(wxT("dq"));
    dqframe->Show(TRUE);
}

void Servoterm::OnRead(wxCommandEvent& WXUNUSED(event)){

}

void Servoterm::OnWrite(wxCommandEvent& WXUNUSED(event)){
    ifstream infile(filename);
    string line;
    while (std::getline(infile, line))
    {
        if(!servoframe->send(line))
            break;
    }

}

void Servoterm::OnSave(wxCommandEvent& WXUNUSED(event)){
    
}

void Servoterm::OnOpen(wxCommandEvent& WXUNUSED(event)){
    wxFileDialog openFileDialog(servoframe, wxEmptyString, SRCDIR"/..", "","*", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;
    filename = openFileDialog.GetPath();
    servoframe->SetTitle(filename);
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