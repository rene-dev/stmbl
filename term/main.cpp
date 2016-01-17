#include <wx/wx.h>
#include <wx/filedlg.h>

#include <fstream>

#include "mainframe.hpp"

class Servoterm: public wxApp
{
public:
    bool OnInit();
    void OnOpen(wxCommandEvent& WXUNUSED(event));
    void OnWrite(wxCommandEvent& WXUNUSED(event));
    void OnRead(wxCommandEvent& WXUNUSED(event));
    void OnSave(wxCommandEvent& WXUNUSED(event));
private:
    ServoFrame* servoframe;
    wxString filename;
    int writeID;
    int readID;
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
    filename = "";
    writeID = wxNewId();
    readID = wxNewId();

    menuBar->Append(fileMenu, "&File");
    fileMenu->Append(wxID_OPEN);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &Servoterm::OnOpen, this, wxID_OPEN);
    //fileMenu->Append(wxID_SAVE);
    //Bind(wxEVT_COMMAND_MENU_SELECTED, &Servoterm::OnSave, this, wxID_SAVE);
    fileMenu->Append(writeID, "&Write");
    Bind(wxEVT_COMMAND_MENU_SELECTED, &Servoterm::OnWrite, this, writeID);
    fileMenu->Append(readID, "&Read");
    Bind(wxEVT_COMMAND_MENU_SELECTED, &Servoterm::OnRead, this, readID);

    wxMenu *helpMenu = new wxMenu;
    menuBar->Append(helpMenu, "&Help" );

    servoframe->SetMenuBar( menuBar );

    //frame->CreateStatusBar();
    //frame->SetStatusText("Statuskram und so");

    servoframe->Show(TRUE);
    return TRUE;
}

void Servoterm::OnRead(wxCommandEvent& WXUNUSED(event)){
    servoframe->send("conf");
}

void Servoterm::OnWrite(wxCommandEvent& WXUNUSED(event)){
    ifstream infile(filename);
    string line;
    while (std::getline(infile, line))
    {
        servoframe->send(line);
    }

}

void Servoterm::OnSave(wxCommandEvent& WXUNUSED(event)){

}

void Servoterm::OnOpen(wxCommandEvent& WXUNUSED(event)){
    wxFileDialog openFileDialog(servoframe, wxEmptyString, SRCDIR"/../conf", "","*", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;
    filename = openFileDialog.GetPath();
    servoframe->SetTitle(filename);
}
