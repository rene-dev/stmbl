#include "mainframe.hpp"

using std::cout;
using std::endl;
using std::string;
using std::to_string;

ServoFrame::ServoFrame(const wxString& title) : wxFrame(NULL, wxID_ANY, title){
    currentID = wxID_LOWEST;
    addr = -1;
	connected = false;
	histpos = 0;
	config = new wxConfig("Servoterm");
    
    
    wxBoxSizer *sizermain = new wxBoxSizer(wxHORIZONTAL);
    wxSplitterWindow *splittermain = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxSize(1024, 512),wxSP_LIVE_UPDATE|wxSP_3DSASH);
    splittermain->SetSashGravity(0.5);
    splittermain->SetMinimumPaneSize(100); // Smalest size the
    sizermain->Add(splittermain, 1,wxEXPAND,0);
    
    wxPanel *pnl_up = new wxPanel(splittermain, wxID_ANY, wxDefaultPosition, wxSize(1024, 256));
    
    wxBoxSizer *sizer_up =new wxBoxSizer(wxVERTICAL);
    pnl_up->SetSizer(sizer_up);
    wxBoxSizer *sizer_leiste = new wxBoxSizer(wxHORIZONTAL);

    choose_port = new wxChoice (pnl_up, wxID_ANY);
    connectbutton = new wxButton(pnl_up, wxID_ANY, wxT("&Connect"));
    clear = new wxButton(pnl_up, wxID_ANY, wxT("Clear"));
    refresh = new wxButton(pnl_up, wxID_ANY, wxT("&Refresh"));
    uhu = new wxRadioButton(pnl_up,wxID_ANY, "UHU");
    stmbl = new wxRadioButton(pnl_up, wxID_ANY,"STMBL");
    stmbl->SetValue(true);
    
    refresh->Bind(wxEVT_BUTTON, &ServoFrame::OnRefresh, this, wxID_ANY);
    connectbutton->Bind(wxEVT_BUTTON, &ServoFrame::OnConnect, this, wxID_ANY);
    clear->Bind(wxEVT_BUTTON, &ServoFrame::OnClear, this, wxID_ANY);
    listports();
    
    sizer_leiste->Add(choose_port, 0,wxALIGN_LEFT|wxALL,2);
    sizer_leiste->Add(connectbutton,0,wxALIGN_LEFT|wxALL,2);
    sizer_leiste->Add(refresh,0,wxALIGN_LEFT|wxALL,2);
    sizer_leiste->Add(clear,0,wxALIGN_LEFT|wxALL,2);
    sizer_leiste->Add(uhu,0,wxALIGN_LEFT|wxALL,2);
    sizer_leiste->Add(stmbl,0,wxALIGN_LEFT|wxALL,2);
    
    //wxTextCtrl* df = new wxTextCtrl(pnl_up, wxID_ANY);
    drawpanel = new BasicDrawPane((wxFrame*)pnl_up,0);
    sizer_up->Add(sizer_leiste, 0, wxEXPAND | wxALL, 0);
    sizer_up->Add(drawpanel, 1, wxEXPAND | wxALL, 0);
    sizer_up->SetSizeHints(pnl_up);
    
    
    wxPanel *pnl_down = new wxPanel(splittermain, wxID_ANY);
    
    wxBoxSizer *sizersub = new wxBoxSizer(wxHORIZONTAL);
    pnl_down->SetSizer(sizersub);
    
    
    wxPanel *pnl_left = new wxPanel(pnl_down, wxID_ANY);
    wxPanel *pnl_right = new wxPanel(pnl_down, wxID_ANY, wxDefaultPosition, wxSize(100, 100));
    sizersub->Add(pnl_left, 1, wxEXPAND, 0);
    sizersub->Add(pnl_right, 0, wxEXPAND, 0);
    

    wxBoxSizer *consolesizer = new wxBoxSizer(wxVERTICAL);
    wxTextCtrl *console = new wxTextCtrl(pnl_left, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
    wxTextCtrl *command = new wxTextCtrl(pnl_left, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    console->AppendText("console");
    command->AppendText("command");
    command->Bind(wxEVT_TEXT_ENTER, &ServoFrame::OnInput, this, wxID_ANY);
    command->Bind(wxEVT_KEY_DOWN, &ServoFrame::OnKeyDown, this, wxID_ANY);
    consolesizer->Add(console, 1,wxEXPAND|wxALL,0);
    consolesizer->Add(command, 0,wxEXPAND|wxALL,0);
    pnl_left->SetSizer(consolesizer);
    
    splittermain->SplitHorizontally(pnl_up, pnl_down);

    this->SetSizer(sizermain);
    sizermain->SetSizeHints(this);
    
    
	/*wxBoxSizer *mainsizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *subsizer = new wxBoxSizer(wxVERTICAL);
	wxSplitterWindow *mainsplitter = new wxSplitterWindow(this,wxID_ANY,wxDefaultPosition, wxSize(1024,768),wxSP_LIVE_UPDATE|wxSP_3DSASH);
    wxSplitterWindow *subsplitter = new wxSplitterWindow(this);
	wxImage::AddHandler(new wxGIFHandler);

	mainsplitter->SetSashGravity(0);
	mainsplitter->SetMinimumPaneSize(100);
	mainsizer->Add(mainsplitter, 1,wxEXPAND,0);
    subsplitter->SetSashGravity(0.0);
    subsplitter->SetMinimumPaneSize(100);
    subsizer->Add(subsplitter, 1, wxEXPAND, 0);

    timer = new wxTimer(this, wxID_ANY);
    Bind(wxEVT_TIMER,&ServoFrame::OnTimer,this,wxID_ANY);

	//oben
	wxPanel *top = new wxPanel(mainsplitter, wxID_ANY);
	wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *leiste = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *channelleiste = new wxBoxSizer(wxHORIZONTAL);
	choose_port = new wxChoice (top, wxID_ANY);
    
	connectbutton = new wxButton(top, wxID_ANY, wxT("&Connect"));
	clear = new wxButton(top, wxID_ANY, wxT("Clear"));
	refresh = new wxButton(top, wxID_ANY, wxT("&Refresh"));
	uhu = new wxRadioButton(top,wxID_ANY, "UHU");
	stmbl = new wxRadioButton(top, wxID_ANY,"STMBL");
	stmbl->SetValue(true);

	refresh->Bind(wxEVT_BUTTON, &ServoFrame::OnRefresh, this, wxID_ANY);
	connectbutton->Bind(wxEVT_BUTTON, &ServoFrame::OnConnect, this, wxID_ANY);
	clear->Bind(wxEVT_BUTTON, &ServoFrame::OnClear, this, wxID_ANY);
	listports();
	leiste->Add(choose_port, 0,wxALIGN_LEFT|wxALL,3);
	leiste->Add(connectbutton,0,wxALIGN_LEFT|wxALL,3);
	leiste->Add(refresh,0,wxALIGN_LEFT|wxALL,3);
	leiste->Add(clear,0,wxALIGN_LEFT|wxALL,3);
	leiste->Add(uhu,0,wxALIGN_LEFT|wxALL,3);
	leiste->Add(stmbl,0,wxALIGN_LEFT|wxALL,3);
	topsizer->Add(leiste);
	drawpanel = new BasicDrawPane((wxFrame*)top,4);
    
    topsizer->Add(drawpanel, 1,wxEXPAND,0);
    wxArrayString waves;
    waves.push_back("-");
    waves.push_back("net0.cmd");
    waves.push_back("net0.fb");
    waves.push_back("pid0.pwm_cmd");
    waves.push_back("pos_minus0.out");
    

    //channels
    channelstartID = currentID-1;//next ID
    

    top->SetSizer(topsizer);

	//unten
    wxPanel *left = new wxPanel(subsplitter, wxID_ANY);
	text = new wxTextCtrl((wxFrame*)left,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_MULTILINE|wxTE_READONLY);
	textinput = new wxTextCtrl((wxFrame*)left,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_PROCESS_ENTER);
	textinput->Bind(wxEVT_TEXT_ENTER, &ServoFrame::OnInput, this, wxID_ANY);
	textinput->Bind(wxEVT_KEY_DOWN, &ServoFrame::OnKeyDown, this, wxID_ANY);
	wxBoxSizer *leftsizer = new wxBoxSizer(wxVERTICAL);
    leftsizer->Add(text);
    leftsizer->Add(textinput);
    
	//leftsizer->Add(text, 1,wxEXPAND|wxALL,3);
	//leftsizer->Add(textinput, 0,wxEXPAND|wxALL,3);
	left->SetSizer(leftsizer);
    
    wxPanel *right = new wxPanel(subsplitter, wxID_ANY);
    text2 = new wxTextCtrl((wxFrame*)right,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_MULTILINE|wxTE_READONLY);
    wxBoxSizer *rightsizer = new wxBoxSizer(wxVERTICAL);
    rightsizer->Add(text2);
    //rightsizer->Add(text, 1,wxEXPAND|wxALL,3);
    //rightsizer->Add(textinput, 0,wxEXPAND|wxALL,3);
    right->SetSizer(rightsizer);
    
    
    for(int i = 0;i<drawpanel->channels;i++){
        wxBoxSizer *channelsizer = new wxBoxSizer(wxVERTICAL);
        channelchoice.push_back(new wxChoice (top, ++currentID));
        channelchoice.back()->Bind(wxEVT_CHOICE,&ServoFrame::OnChannelChange, this, currentID);
        channelchoice.back()->Set(waves);
        channelchoice.back()->SetSelection(0);
        cout << channelchoice.back()->GetId() << endl;
        
        channelpos.push_back(new wxSlider(top, ++currentID, 0, -100, 100));
        channelpos.back()->Bind(wxEVT_SLIDER,&ServoFrame::OnChannelChange, this, currentID);
        
        channelgain.push_back(new wxSlider(top, ++currentID, 10, 1, 100));
        channelgain.back()->Bind(wxEVT_SLIDER,&ServoFrame::OnChannelChange, this, currentID);
        
        wxPanel *c_panel;
        c_panel = new wxPanel(top, wxID_NEW, wxPoint(150, 20), wxSize(20, 20), wxBORDER_NONE);
        c_panel->SetBackgroundColour(drawpanel->pen[i].GetColour());
        
        wxBoxSizer *sizer1 = new wxBoxSizer(wxHORIZONTAL);
        wxBoxSizer *sizer2 = new wxBoxSizer(wxHORIZONTAL);
        wxBoxSizer *sizer3 = new wxBoxSizer(wxHORIZONTAL);
        sizer1->Add(c_panel, 0,wxALIGN_LEFT|wxALL,3);
        sizer1->Add(channelchoice.back(), 1,wxALIGN_LEFT|wxALL,3);
        sizer2->Add(channelpos.back(), 1,wxALIGN_LEFT|wxALL,3);
        sizer3->Add(channelgain.back(), 1,wxALIGN_LEFT|wxALL,3);
        channelsizer->Add(sizer1);
        channelsizer->Add(sizer2);
        channelsizer->Add(sizer3);
        channelleiste->Add(channelsizer);
    }
    rightsizer->Add(channelleiste);

    
    subsplitter->SplitVertically(left, right, 500);
	mainsplitter->SplitHorizontally(top, subsplitter,100);
	this->SetSizer(mainsizer);
    mainsplitter->SetSizer(subsizer);
	mainsizer->SetSizeHints(this);*/
}

void ServoFrame::OnKeyDown(wxKeyEvent& event){
	if(event.GetKeyCode() == 315){//up
		if(histpos > 0 && history.size()>0){
			textinput->SetValue(history.at(--histpos));
			textinput->SetInsertionPointEnd();
		}
	}
	else if(event.GetKeyCode() == 317){//down
		if(histpos < history.size()-1 && history.size()>0){
			textinput->SetValue(history.at(++histpos));
			textinput->SetInsertionPointEnd();
		}else if(histpos < history.size()){
			histpos++;
			textinput->SetValue(wxT(""));
		}
    }else if (event.GetKeyCode() == 9){}//tab
	else{
		event.Skip();
	}
}

void ServoFrame::OnChannelChange(wxCommandEvent& event){
    int param = (event.GetId()-channelstartID-2)%3;
    int channel = (int)((event.GetId()-channelstartID-2)/3);
    string params[] = {"term0.wave","term0.offset","term0.gain"};
    wxString value;
    switch (param) {
        case 0://wave
            value = event.GetSelection()==0?"0":event.GetString();
            break;
        case 1://offset
        case 2://gain
            value = to_string(event.GetSelection()/10.0f);
            break;
        default:
            break;
    }
    //cout << "change " << channel << " " << param << " " << value << endl;

    string df = params[param];
    df += to_string(channel);
    df += " = ";
    df += value;
    send(df);
}

int ServoFrame::send(string& s,bool h){
    cout << s << endl;
    if(connected){
        if(h){//history
            if((history.size()==0 || history.back() != s) && !s.empty()){
                history.push_back(s);
            }
            histpos = history.size();
        }
        int ret1 = sp_nonblocking_write(port, s.c_str(), s.length());
        int ret2 = sp_nonblocking_write(port, "\r", 1);
        if(ret1 != s.length() || ret2!=1){
            wxMessageBox( wxT("Fehler beim senden"), wxT("Error"), wxICON_EXCLAMATION);
            disconnect();
            return 0;
        }
    }else{
        wxMessageBox( wxT("Nicht verbunden"), wxT("Error"), wxICON_EXCLAMATION);
        return 0;
    }
    return 1;
}

void ServoFrame::OnTimer(wxTimerEvent& evt){
	int ret;
	if(connected){
		ret = sp_nonblocking_read(port, buf, bufsize);
		if(ret > 0){
			buf[ret] = 0;
			if(uhu->GetValue()){
                for (int i=0; i<ret; i++) {
                    if ((buf[i]>>7)) {
                        drawpanel->plotvalue((buf[i]+128/2) / 64.0);
                    }else{
                        text->AppendText((char)buf[i]);
                    }
                }
            }else if(stmbl->GetValue()){
                for (int i=0; i<ret; i++){
                    if(addr >= 0){
                        values[addr++] = (buf[i]-128) / 128.0;
                        if(addr == 4){
                            drawpanel->plotvalue(values);
                            addr = -1;
                        }
                    }else if (buf[i] == 0xff) {
                        addr = 0;
                    }else{
                        text->AppendText((char)buf[i]);
                    }
                }
            }
		}
	}
}

void ServoFrame::OnRefresh(wxCommandEvent& WXUNUSED(event)){
	listports();
}

void ServoFrame::listports(){
	if (!ports) {
		sp_free_port_list(ports);
	}
	if(sp_list_ports(&ports) == SP_OK){
		wxString str;
		choose_port->Clear();
		for (int i = 0; ports[i]; i++) {
			choose_port->Append(sp_get_port_description(ports[i]));
			if(config->Read("port", &str) && sp_get_port_description(ports[i]) == str){
				choose_port->SetSelection(i);
			}
		}
	}
}

void ServoFrame::OnClear(wxCommandEvent& WXUNUSED(event)){
	text->Clear();
    drawpanel->Clear();
}

void ServoFrame::OnConnect(wxCommandEvent& WXUNUSED(event)){
	if(connected)
		disconnect();
	else
		connect();
}

void ServoFrame::connect(){
	if(choose_port->IsEmpty())
        return;
    port = ports[choose_port->GetSelection()];
	if(sp_open(port, SP_MODE_READ_WRITE) == SP_OK){//port da und lässt sich öffnen
		wxString str;
		str = sp_get_port_description(port);
		config->Write("port", str);
		config->Flush();
		sp_set_baudrate(port,38400);
		sp_set_bits(port, 8);
		sp_set_stopbits(port, 1);
		sp_set_parity(port, SP_PARITY_NONE);
		sp_set_xon_xoff(port, SP_XONXOFF_DISABLED);
		sp_set_flowcontrol(port, SP_FLOWCONTROL_NONE);
		connected = true;
		connectbutton->SetLabel(wxT("&Disonnect"));
		refresh->Disable();
		choose_port->Disable();
        uhu->Disable();
        stmbl->Disable();
		textinput->SetFocus();
        timer->Start(50);
	}else{
		wxMessageBox( wxT("Fehler beim Öffnen"), wxT("Error"), wxICON_EXCLAMATION);
		listports();
	}
}

void ServoFrame::disconnect(){
	if(sp_close(port) == SP_OK){
		connected = false;
		connectbutton->SetLabel(wxT("&Connect"));
		refresh->Enable();
		choose_port->Enable();
        uhu->Enable();
        stmbl->Enable();
        timer->Stop();
	}
}

void ServoFrame::OnInput(wxCommandEvent& event){
    string s =string(textinput->GetValue().mb_str());
    send(s,true);
	textinput->Clear();
}
