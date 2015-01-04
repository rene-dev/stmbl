#include "dqframe.hpp"

using std::cout;
using std::endl;
using std::string;
using std::to_string;

dqFrame::dqFrame(const wxString& title) : wxFrame(NULL, wxID_ANY, title){
    dqdraw = new dqDraw(this);
    
    //topsizer->Add(drawpanel, 1,wxEXPAND,0);
}
