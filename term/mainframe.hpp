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
#include <libserialport.h>


class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString& title);
private:
    wxButton *connect;
    wxButton *refresh;
    struct sp_port **ports;
    struct sp_port *port;
    wxChoice *choose_port;
    bool connected;
    void OnConnect(wxCommandEvent& WXUNUSED(event));
    void OnRefresh(wxCommandEvent& WXUNUSED(event));
    void listports();
};
