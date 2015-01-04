#include "dqframe.hpp"

using std::cout;
using std::endl;
using std::string;
using std::to_string;

dqFrame::dqFrame(const wxString& title) : wxFrame(NULL, wxID_ANY, title){
    wxBoxSizer *mainsizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *datasizer = new wxBoxSizer(wxHORIZONTAL);
    dqdraw = new dqDraw(this);
    mainsizer->Add(dqdraw, 1,wxEXPAND,0);
    mainsizer->Add(datasizer, 1,wxEXPAND,0);
    
    //topsizer->Add(drawpanel, 1,wxEXPAND,0);
}
