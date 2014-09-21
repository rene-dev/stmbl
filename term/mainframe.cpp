#include "mainframe.hpp"

MainFrame::MainFrame(const wxString& title) : wxFrame(NULL, wxID_ANY, title){
    connected = false;
    wxBoxSizer *mainsizer = new wxBoxSizer(wxHORIZONTAL);
    wxSplitterWindow *mainsplitter = new wxSplitterWindow(this,wxID_ANY,wxDefaultPosition, wxSize(1024,768),wxSP_LIVE_UPDATE|wxSP_3DSASH);
    wxImage::AddHandler(new wxGIFHandler);
    
    mainsplitter->SetSashGravity(0);
    mainsplitter->SetMinimumPaneSize(100);
    mainsizer->Add(mainsplitter, 1,wxEXPAND,0);
    
    Bind(wxEVT_IDLE,&MainFrame::MainFrame::OnIdle, this, wxID_ANY);
    
    //oben
    wxPanel *top = new wxPanel(mainsplitter, wxID_ANY);
    wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *leiste = new wxBoxSizer(wxHORIZONTAL);
    choose_port = new wxChoice (top, wxID_ANY);
    connectbutton = new wxButton(top, wxID_ANY, wxT("Connect"));
    clear = new wxButton(top, wxID_ANY, wxT("Clear"));
    refresh = new wxButton(top, wxID_ANY, wxT("Refresh"));
    refresh->Bind(wxEVT_BUTTON, &MainFrame::OnRefresh, this, wxID_ANY);
    connectbutton->Bind(wxEVT_BUTTON, &MainFrame::OnConnect, this, wxID_ANY);
    clear->Bind(wxEVT_BUTTON, &MainFrame::OnClear, this, wxID_ANY);
    listports();
    leiste->Add(choose_port, 0,wxALIGN_LEFT|wxALL,3);
    leiste->Add(connectbutton,0,wxALIGN_LEFT|wxALL,3);
    leiste->Add(refresh,0,wxALIGN_LEFT|wxALL,3);
    leiste->Add(clear,0,wxALIGN_LEFT|wxALL,3);
    topsizer->Add(leiste);
    drawpanel = new BasicDrawPane((wxFrame*)top);
    topsizer->Add(drawpanel, 1,wxEXPAND,0);
    top->SetSizer(topsizer);
    
    //unten
    wxPanel *bottom = new wxPanel(mainsplitter, wxID_ANY);
    text = new wxTextCtrl((wxFrame*)bottom,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_MULTILINE|wxTE_READONLY);
    textinput = new wxTextCtrl((wxFrame*)bottom,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_PROCESS_ENTER);
    textinput->Bind(wxEVT_TEXT_ENTER, &MainFrame::OnInput, this, wxID_ANY);
    wxBoxSizer *bottomsizer = new wxBoxSizer(wxVERTICAL);
    bottomsizer->Add(text, 1,wxEXPAND|wxALL,3);
    bottomsizer->Add(textinput, 0,wxEXPAND|wxALL,3);
    bottom->SetSizer(bottomsizer);
    
    mainsplitter->SplitHorizontally(top, bottom,400);
    this->SetSizer(mainsizer);
    mainsizer->SetSizeHints(this);
}

void MainFrame::OnIdle(wxIdleEvent& evt){
    int ret;
    if(connected){
        ret = sp_nonblocking_read(port, buf, bufsize);
        if(ret > 0){
            buf[ret] = 0;
            for (int i=0; i<ret; i++) {
                if ((buf[i]>>7)) {
                    drawpanel->plotvalue(((int)buf[i])+128/2);
                }else{
                    text->AppendText(buf[i]);
                }
            }
            //std::cout << buf;
            
        }
        wxMilliSleep(3);//örks
    }
    //wxWakeUpIdle();
    evt.RequestMore();
}

void MainFrame::OnRefresh(wxCommandEvent& WXUNUSED(event)){
    listports();
}

void MainFrame::listports(){
    if (!ports) {
        sp_free_port_list(ports);
    }
    if(sp_list_ports(&ports) == SP_OK){
        choose_port->Clear();
        for (int i = 0; ports[i]; i++) {
            choose_port->Append(sp_get_port_description(ports[i]));
        }
    }
}

void MainFrame::OnClear(wxCommandEvent& WXUNUSED(event)){
    text->Clear();
}

void MainFrame::OnConnect(wxCommandEvent& WXUNUSED(event)){
    if(connected)
        disconnect();
    else
        connect();
    //TODO: free port, check op port noch da
}

void MainFrame::connect(){
    port = ports[choose_port->GetSelection()];
    if(sp_open(port, SP_MODE_WRITE) == SP_OK){//port da und lässt sich öffnen
        sp_set_baudrate(port,38400);
        sp_set_bits(port, 8);
        sp_set_stopbits(port, 1);
        sp_set_parity(port, SP_PARITY_NONE);
        sp_set_xon_xoff(port, SP_XONXOFF_DISABLED);
        sp_set_flowcontrol(port, SP_FLOWCONTROL_NONE);
        connected = true;
        connectbutton->SetLabel(wxT("Disonnect"));
        refresh->Disable();
        choose_port->Disable();
    }else{
        wxMessageBox( wxT("Fehler beim Öffnen"), wxT("Error"), wxICON_EXCLAMATION);
    }
}

void MainFrame::disconnect(){
    if(sp_close(port) == SP_OK){
        connected = false;
        connectbutton->SetLabel(wxT("Connect"));
        refresh->Enable();
        choose_port->Enable();
    }

}

void MainFrame::OnInput(wxCommandEvent& event){
    if(connected){
        //std::cout << textinput->GetValue();
        int ret1 = sp_nonblocking_write(port, textinput->GetValue().mb_str(), textinput->GetValue().mb_str().length());
        int ret2 = sp_nonblocking_write(port, "\r", 1);
        if(ret1 != textinput->GetValue().mb_str().length() || ret2!=1){
            wxMessageBox( wxT("Fehler beim senden"), wxT("Error"), wxICON_EXCLAMATION);
            disconnect();
        }
    }else{
        wxMessageBox( wxT("Nicht verbunden"), wxT("Error"), wxICON_EXCLAMATION);
    }
    textinput->Clear();
}
