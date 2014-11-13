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
#include <wx/slider.h>
#include <math.h>
#include <vector>
#include <libserialport.h>
#include "basicdrawpane.hpp"

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString& title);
private:
    wxButton *connectbutton;
    wxButton *refresh;
    wxButton *clear;
    wxRadioButton *uhu;
    wxRadioButton *stmbl;
    wxConfig *config;
    struct sp_port **ports;
    struct sp_port *port;
    wxChoice *choose_port;
    std::vector<wxChoice *> channelchoice;
    std::vector<wxSlider *> channelpos;
    std::vector<wxSlider *> channelgain;
    bool connected;
    void OnConnect(wxCommandEvent& WXUNUSED(event));
    void OnRefresh(wxCommandEvent& WXUNUSED(event));
    void OnClear(wxCommandEvent& WXUNUSED(event));
    void OnInput(wxCommandEvent& event);
    void OnChannelChange(wxCommandEvent& event);
    void OnPosChange(wxCommandEvent& event);
    void OnGainChange(wxCommandEvent& event);
    void OnKeyDown(wxKeyEvent& event);
    void OnIdle(wxIdleEvent& evt);
    void send(std::string& s);
    void connect();
    void disconnect();
    void listports();
    static const int bufsize = 1000;
    unsigned char buf[MainFrame::bufsize+1];
    wxTextCtrl *text;
    wxTextCtrl *textinput;
    wxWindowID currentID;
    wxWindowID channelstartID;//StartID of channel config
    BasicDrawPane *drawpanel;
    std::vector<wxString> history;
    int histpos;
    int addr;
    float values[8];
};
