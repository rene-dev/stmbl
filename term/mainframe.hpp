#pragma once

#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/listctrl.h>
#include <wx/bitmap.h>
#include <wx/artprov.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/choice.h>
#include <wx/arrstr.h>
#include <wx/button.h>
#include <wx/config.h>
#include <math.h>
#include <vector>
#include <queue>
#include <libserialport.h>
#include "basicdrawpane.hpp"

class ServoFrame : public wxFrame
{
public:
    ServoFrame(const wxString& title);
    int send(const std::string& s,bool h = false);
private:
    wxButton *connectbutton;
    wxButton *refresh;
    wxButton *clear;
    wxButton *reset;
    wxRadioButton *uhu;
    wxRadioButton *stmbl;
    wxTimer * timer;
    struct sp_port **ports;
    struct sp_port *port;
    wxChoice *choose_port;
    std::vector<wxComboBox *> channelchoice;
    bool connected;
    void OnConnect(wxCommandEvent& WXUNUSED(event));
    void OnRefresh(wxCommandEvent& WXUNUSED(event));
    void OnReset(wxCommandEvent& WXUNUSED(event));
    void OnClear(wxCommandEvent& WXUNUSED(event));
    void OnInput(wxCommandEvent& event);
    void OnChannelChange(wxCommandEvent& event);
    void OnPosChange(wxCommandEvent& event);
    void OnGainChange(wxCommandEvent& event);
    void OnKeyDown(wxKeyEvent& event);
    void OnTimer(wxTimerEvent& evt);
    void connect();
    void disconnect();
    void listports();
    static const int bufsize = 20000;
    unsigned char buf[ServoFrame::bufsize+1];
    wxTextCtrl *text;
    wxTextCtrl *textinput;
    wxWindowID currentID;
    wxWindowID channelstartID;//StartID of channel config
    BasicDrawPane *drawpanel;
    std::vector<wxString> history;
    int histpos;
    int addr;
    float values[16];
    std::queue<std::string> txqueue;
};
