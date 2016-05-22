#include "mainframe.hpp"

using std::cout;
using std::endl;
using std::string;
using std::to_string;

ServoFrame::ServoFrame(const wxString& title) : wxFrame(NULL, wxID_ANY, title){
   ports = 0;
    currentID = wxID_LOWEST;
    addr = -1;
	connected = false;
	histpos = 0;
	wxBoxSizer *mainsizer = new wxBoxSizer(wxHORIZONTAL);
	wxSplitterWindow *mainsplitter = new wxSplitterWindow(this,wxID_ANY,wxDefaultPosition, wxSize(1024,700),wxSP_LIVE_UPDATE|wxSP_3DSASH);
	wxImage::AddHandler(new wxGIFHandler);

	mainsplitter->SetSashGravity(0);
	mainsplitter->SetMinimumPaneSize(100);
	mainsizer->Add(mainsplitter, 1,wxEXPAND,0);

    timer = new wxTimer(this, wxID_ANY);
    Bind(wxEVT_TIMER,&ServoFrame::OnTimer,this,wxID_ANY);

	//top
	wxPanel *top = new wxPanel(mainsplitter, wxID_ANY);
	wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *leiste = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *channelleiste = new wxBoxSizer(wxHORIZONTAL);
	choose_port = new wxChoice (top, wxID_ANY);

	connectbutton = new wxButton(top, wxID_ANY, wxT("&Connect"));
    connectbutton->Disable();
	clear = new wxButton(top, wxID_ANY, wxT("Clear"));
	refresh = new wxButton(top, wxID_ANY, wxT("&Refresh"));
	reset = new wxButton(top, wxID_ANY, wxT("Reset Fault"));
	reset->Disable();
	uhu = new wxRadioButton(top,wxID_ANY, "UHU");
	stmbl = new wxRadioButton(top, wxID_ANY,"STMBL");
	stmbl->SetValue(true);

	refresh->Bind(wxEVT_BUTTON, &ServoFrame::OnRefresh, this, wxID_ANY);
	connectbutton->Bind(wxEVT_BUTTON, &ServoFrame::OnConnect, this, wxID_ANY);
	clear->Bind(wxEVT_BUTTON, &ServoFrame::OnClear, this, wxID_ANY);
	reset->Bind(wxEVT_BUTTON, &ServoFrame::OnReset, this, wxID_ANY);
	listports();
	leiste->Add(choose_port, 0,wxALIGN_LEFT|wxALL,3);
	leiste->Add(connectbutton,0,wxALIGN_LEFT|wxALL,3);
	leiste->Add(refresh,0,wxALIGN_LEFT|wxALL,3);
	leiste->Add(clear,0,wxALIGN_LEFT|wxALL,3);
	leiste->Add(reset,0,wxALIGN_LEFT|wxALL,3);
	leiste->Add(uhu,0,wxALIGN_LEFT|wxALL,3);
	leiste->Add(stmbl,0,wxALIGN_LEFT|wxALL,3);
	topsizer->Add(leiste);
	drawpanel = new BasicDrawPane((wxFrame*)top,8);

    topsizer->Add(drawpanel, 1,wxEXPAND,0);
    wxArrayString waves;
    waves.push_back("0");
    waves.push_back("net0.cmd");
    waves.push_back("net0.fb");
    waves.push_back("pid0.pwm_cmd");
    waves.push_back("pos_minus0.out");


    //channels
    channelstartID = currentID-1;//next ID
    for(int i = 0;i<drawpanel->channels;i++){
        wxBoxSizer *channelsizer = new wxBoxSizer(wxVERTICAL);
        channelchoice.push_back(new wxComboBox (top, ++currentID, _T(""), wxDefaultPosition, wxDefaultSize, 0, NULL, wxTE_PROCESS_ENTER));
        channelchoice.back()->Bind(wxEVT_COMBOBOX,&ServoFrame::OnChannelChange, this, currentID);
        channelchoice.back()->Bind(wxEVT_TEXT_ENTER,&ServoFrame::OnChannelChange, this, currentID);
        channelchoice.back()->Set(waves);
        channelchoice.back()->SetSelection(0);

        wxPanel *c_panel;
        c_panel = new wxPanel(top, wxID_NEW, wxPoint(150, 20), wxSize(20, 20), wxBORDER_NONE);
        c_panel->SetBackgroundColour(drawpanel->pen[i].GetColour());

        wxBoxSizer *sizer1 = new wxBoxSizer(wxHORIZONTAL);
        sizer1->Add(c_panel, 0,wxALIGN_LEFT|wxALL,3);
        sizer1->Add(channelchoice.back(), 1,wxALIGN_LEFT|wxALL,3);
        channelsizer->Add(sizer1);
        channelleiste->Add(channelsizer);
    }

	topsizer->Add(channelleiste);

    top->SetSizer(topsizer);

	//bottom
	wxPanel *bottom = new wxPanel(mainsplitter, wxID_ANY);
	text = new wxTextCtrl((wxFrame*)bottom,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_MULTILINE|wxTE_READONLY);
	textinput = new wxTextCtrl((wxFrame*)bottom,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_PROCESS_ENTER);
	textinput->Bind(wxEVT_TEXT_ENTER, &ServoFrame::OnInput, this, wxID_ANY);
	textinput->Bind(wxEVT_KEY_DOWN, &ServoFrame::OnKeyDown, this, wxID_ANY);

    //use monospace font
    wxFont font = textinput->GetFont();
    font = wxFont(font.GetPointSize(), wxTELETYPE, font.GetStyle(), font.GetWeight(), font.GetUnderlined());
    textinput->SetFont(font);
    text->SetFont(font);

	wxBoxSizer *bottomsizer = new wxBoxSizer(wxVERTICAL);
	bottomsizer->Add(text, 1,wxEXPAND|wxALL,3);
	bottomsizer->Add(textinput, 0,wxEXPAND|wxALL,3);
	bottom->SetSizer(bottomsizer);

	mainsplitter->SplitHorizontally(top, bottom,500);
	this->SetSizer(mainsizer);
	mainsizer->SetSizeHints(this);
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
    int channel = (int)((event.GetId()-channelstartID-2));
    wxString value = event.GetString();
    string df = "term0.wave";
    df += to_string(channel);
    df += " = ";
    df += value;
    send(df);
}

int ServoFrame::send(const string& s,bool h){
    if(!connected)
        connect();
    if(connected){
        if(h){//history
            if((history.size()==0 || history.back() != s) && !s.empty()){
                history.push_back(s);
            }
            histpos = history.size();
        }
        txqueue.push(s);
    }
    return 1;
}

void ServoFrame::OnTimer(wxTimerEvent& evt){
	int ret;
	if(connected){
        if(!txqueue.empty()){
            int ret1 = sp_nonblocking_write(port, txqueue.front().c_str(), txqueue.front().length());
            int ret2 = sp_nonblocking_write(port, "\n", 1);
            if(ret1 != txqueue.front().length() || ret2!=1){
                wxMessageBox( wxT("Error while sending"), wxT("Error"), wxICON_EXCLAMATION);
                disconnect();
            }
            txqueue.pop();
        }
		ret = sp_nonblocking_read(port, buf, bufsize);
		if(ret > 0){
			buf[ret] = 0;
			if(uhu->GetValue()){
                for (int i=0; i<ret; i++) {
                    if ((buf[i]>>7)) {
                        values[0] = (float)(buf[i]-128-64)/64;
                        drawpanel->plotvalue(values);
                    }else{
                        text->AppendText(wxString::FromAscii(buf[i] & 0x7f));
                    }
                }

            }else if(stmbl->GetValue()){
                for (int i=0; i<ret; i++){
                    if(addr >= 0){
                        values[addr++] = (buf[i]-128) / 128.0;
                        if(addr == 8){
                            drawpanel->plotvalue(values);
                            addr = -1;
                        }
                    }else if (buf[i] == 0xff) {
                        addr = 0;
                    }else{
                        text->AppendText(wxString::FromAscii(buf[i] & 0x7f));
                    }
                }
            }
        }else if(ret < 0){//disconnect on failure
            disconnect();
        }
	}
}

void ServoFrame::OnRefresh(wxCommandEvent& WXUNUSED(event)){
	listports();
}

void ServoFrame::OnReset(wxCommandEvent& WXUNUSED(event)){
    send("fault0.reset = 1");
    send("fault0.reset = 0");
}

void ServoFrame::listports(){
    int selection = 0;
    string description;
	if (ports) {
		sp_free_port_list(ports);
	}
	if(sp_list_ports(&ports) == SP_OK){
		wxString str;
		choose_port->Clear();
        if(ports){
            for (int i = 0; ports[i]; i++) {
                description = sp_get_port_description(ports[i]);
                choose_port->Append(description);
                if(description.find("STMBL") != std::string::npos){
                    selection = i;
                }
            }
            choose_port->SetSelection(selection);
            connectbutton->Enable();
        }else{
            connectbutton->Disable();
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
	if(sp_open(port, SP_MODE_READ_WRITE) == SP_OK){//port available and can be opened
		sp_set_baudrate(port,38400);
		sp_set_bits(port, 8);
		sp_set_stopbits(port, 1);
		sp_set_parity(port, SP_PARITY_NONE);
		sp_set_xon_xoff(port, SP_XONXOFF_DISABLED);
		sp_set_flowcontrol(port, SP_FLOWCONTROL_NONE);
		connected = true;
		connectbutton->SetLabel(wxT("&Disonnect"));
		refresh->Disable();
		reset->Enable();
		choose_port->Disable();
        uhu->Disable();
        stmbl->Disable();
		textinput->SetFocus();
        timer->Start(50);
	}else{
		wxMessageBox( wxT("Cannot open port"), wxT("Error"), wxICON_EXCLAMATION);
		listports();
	}
}

void ServoFrame::disconnect(){
	if(sp_close(port) == SP_OK){
		connected = false;
		connectbutton->SetLabel(wxT("&Connect"));
		refresh->Enable();
		reset->Disable();
		choose_port->Enable();
        uhu->Enable();
        stmbl->Enable();
        timer->Stop();
	}
    listports();
}

void ServoFrame::OnInput(wxCommandEvent& event){
    string s =string(textinput->GetValue().mb_str());
    send(s,true);
	textinput->Clear();
}
