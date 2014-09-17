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
#include <math.h>
#include <vector>
#include <libserialport.h>

class BasicDrawPane : public wxPanel
{
public:
    BasicDrawPane(wxFrame* parent);
    void paintEvent(wxPaintEvent & evt);
    void paintNow();
    void plotvalue(int);
    void render(wxDC& dc);
private:
    std::vector<float> data;
    int x,y,xold,yold,xpos,xstep;
};

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString& title);
private:
    wxButton *connect;
    wxButton *refresh;
    wxButton *clear;
    struct sp_port **ports;
    struct sp_port *port;
    wxChoice *choose_port;
    bool connected;
    void OnConnect(wxCommandEvent& WXUNUSED(event));
    void OnRefresh(wxCommandEvent& WXUNUSED(event));
    void OnClear(wxCommandEvent& WXUNUSED(event));
    void OnInput(wxCommandEvent& event);
    void OnIdle(wxIdleEvent& evt);
    void listports();
    static const int bufsize = 10;
    char buf[MainFrame::bufsize+1];
    wxTextCtrl *text;
    wxTextCtrl *textinput;
    BasicDrawPane *drawpanel;
};
