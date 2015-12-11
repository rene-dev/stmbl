#include "dqframe.hpp"

using std::cout;
using std::endl;
using std::string;
using std::to_string;

dqFrame::dqFrame(const wxString& title) : wxFrame(NULL, wxID_ANY, title){
    wxPanel *panel = new wxPanel(this, wxID_ANY);
    wxBoxSizer *mainsizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *datasizer = new wxBoxSizer(wxVERTICAL);
    wxStaticText *text = new wxStaticText(panel, wxID_ANY, wxEmptyString, wxPoint(25, 80));
    dqdraw = new dqDraw((wxFrame*)panel);
    dqdraw->text = text;
    
    mainsizer->Add(dqdraw, 1,wxEXPAND,0);
    datasizer->SetMinSize(120, 0);
    mainsizer->Add(datasizer, 0,wxRIGHT | wxALL,3);
    datasizer->Add(text, 1,wxEXPAND | wxALL,3);

    panel->SetSizer(mainsizer);
    //wxTextCtrl* text = new wxTextCtrl(rightpanel,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_MULTILINE|wxTE_READONLY);
    //datasizer->Add(text, 0,wxEXPAND,0);
    
}
