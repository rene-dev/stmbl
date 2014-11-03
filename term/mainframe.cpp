#include "mainframe.hpp"

MainFrame::MainFrame(const wxString& title) : wxFrame(NULL, wxID_ANY, title){
    addr = -1;
	connected = false;
	histpos = 0;
	config = new wxConfig("Servoterm");
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
    wxBoxSizer *channelleiste = new wxBoxSizer(wxHORIZONTAL);
	choose_port = new wxChoice (top, wxID_ANY);
    
	connectbutton = new wxButton(top, wxID_ANY, wxT("&Connect"));
	clear = new wxButton(top, wxID_ANY, wxT("Clear"));
	refresh = new wxButton(top, wxID_ANY, wxT("&Refresh"));
	uhu = new wxRadioButton(top,wxID_ANY, "UHU");
	stmbl = new wxRadioButton(top, wxID_ANY,"STMBL");
	stmbl->SetValue(true);

	refresh->Bind(wxEVT_BUTTON, &MainFrame::OnRefresh, this, wxID_ANY);
	connectbutton->Bind(wxEVT_BUTTON, &MainFrame::OnConnect, this, wxID_ANY);
	clear->Bind(wxEVT_BUTTON, &MainFrame::OnClear, this, wxID_ANY);
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
    for(int i = 0;i<drawpanel->channels;i++){
        channelchoice.push_back(new wxChoice (top, wxID_ANY));
        channelchoice.back()->SetClientData(new int(i));
        channelchoice.back()->Bind(wxEVT_CHOICE,&MainFrame::OnChannelChange, this, wxID_ANY);
        channelchoice.back()->Append(wxT("-"));
        channelchoice.back()->Append(wxT("1"));
        channelchoice.back()->Append(wxT("2"));
        channelchoice.back()->Append(wxT("3"));
        channelleiste->Add(channelchoice.back(), 0,wxALIGN_LEFT|wxALL,3);
    }
	topsizer->Add(channelleiste);
    
    top->SetSizer(topsizer);

	//unten
	wxPanel *bottom = new wxPanel(mainsplitter, wxID_ANY);
	text = new wxTextCtrl((wxFrame*)bottom,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_MULTILINE|wxTE_READONLY);
	textinput = new wxTextCtrl((wxFrame*)bottom,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_PROCESS_ENTER);
	textinput->Bind(wxEVT_TEXT_ENTER, &MainFrame::OnInput, this, wxID_ANY);
	textinput->Bind(wxEVT_KEY_DOWN, &MainFrame::OnKeyDown, this, wxID_ANY);
	wxBoxSizer *bottomsizer = new wxBoxSizer(wxVERTICAL);
	bottomsizer->Add(text, 1,wxEXPAND|wxALL,3);
	bottomsizer->Add(textinput, 0,wxEXPAND|wxALL,3);
	bottom->SetSizer(bottomsizer);

	mainsplitter->SplitHorizontally(top, bottom,400);
	this->SetSizer(mainsizer);
	mainsizer->SetSizeHints(this);
}

void MainFrame::OnKeyDown(wxKeyEvent& event){
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
	}
	else{
		event.Skip();
	}
}

void MainFrame::OnChannelChange(wxCommandEvent& event){
    std::cout << *(int*)event.GetClientData() << "->" << event.GetSelection() << std::endl;
}

void MainFrame::OnIdle(wxIdleEvent& evt){
	int ret;
	if(connected){
		ret = sp_nonblocking_read(port, buf, bufsize);
		if(ret > 0){
			buf[ret] = 0;
			if(uhu->GetValue()){
                for (int i=0; i<ret; i++) {
                    if ((buf[i]>>7)) {
                        drawpanel->plotvalue(((int)buf[i])+128/2);
                    }else{
                        text->AppendText((char)buf[i]);
                    }
                }
            }else if(stmbl->GetValue()){
                for (int i=0; i<ret; i++){
                    if(addr != -1){
                        std::cout << "addr:" << addr;
                        addr = -1;
                        if (addr == 127) {
                            drawpanel->plotvalue(values);
                        }else{
                            values[addr] = (int)buf[i];
                            std::cout << " data:" << (int)buf[i] << std::endl;
                        }
                        //drawpanel->plotvalue(values);
                    }else if ((buf[i]>>7)) {
                        //int values[] = {((int)buf[i])+128/2,((int)buf[i])+128/2,((int)buf[i])+128/2,((int)buf[i])+128/2};
                        if((int)buf[i]-128 != 0 && addr == -1){
                            addr = (int)buf[i]-128;
                        }
                        //drawpanel->plotvalue(((int)buf[i])+128/2,1);
                        //drawpanel->plotvalue(((int)buf[i])+128/2,2);
                        //drawpanel->plotvalue(((int)buf[i])+128/2,3);
                    }else{
                        text->AppendText((char)buf[i]);
                    }
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

void MainFrame::OnClear(wxCommandEvent& WXUNUSED(event)){
	text->Clear();
}

void MainFrame::OnConnect(wxCommandEvent& WXUNUSED(event)){
	if(connected)
		disconnect();
	else
		connect();
}

void MainFrame::connect(){
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
		textinput->SetFocus();
	}else{
		wxMessageBox( wxT("Fehler beim Öffnen"), wxT("Error"), wxICON_EXCLAMATION);
		listports();
	}
}

void MainFrame::disconnect(){
	if(sp_close(port) == SP_OK){
		connected = false;
		connectbutton->SetLabel(wxT("&Connect"));
		refresh->Enable();
		choose_port->Enable();
	}
}

void MainFrame::OnInput(wxCommandEvent& event){
	if(connected){
		if((history.size()==0 || history.back() != textinput->GetValue()) && textinput->GetValue() != wxEmptyString){
			history.push_back(textinput->GetValue());
		}
		histpos = history.size();
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
