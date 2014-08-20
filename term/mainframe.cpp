#include "mainframe.hpp"

class BasicDrawPane : public wxPanel
{
    
public:
    BasicDrawPane(wxFrame* parent);
    
    void paintEvent(wxPaintEvent & evt);
    void paintNow();
    
    void render(wxDC& dc);
};

BasicDrawPane::BasicDrawPane(wxFrame* parent) : wxPanel(parent){
    Bind(wxEVT_PAINT, &BasicDrawPane::paintEvent, this);
}

/*
 * Called by the system of by wxWidgets when the panel needs
 * to be redrawn. You can also trigger this call by
 * calling Refresh()/Update().
 */

void BasicDrawPane::paintEvent(wxPaintEvent & evt)
{
    wxPaintDC dc(this);
    render(dc);
}

/*
 * Alternatively, you can use a clientDC to paint on the panel
 * at any time. Using this generally does not free you from
 * catching paint events, since it is possible that e.g. the window
 * manager throws away your drawing when the window comes to the
 * background, and expects you will redraw it when the window comes
 * back (by sending a paint event).
 *
 * In most cases, this will not be needed at all; simply handling
 * paint events and calling Refresh() when a refresh is needed
 * will do the job.
 */
void BasicDrawPane::paintNow()
{
    wxClientDC dc(this);
    render(dc);
}

/*
 * Here we do the actual rendering. I put it in a separate
 * method so that it can work no matter what type of DC
 * (e.g. wxPaintDC or wxClientDC) is used.
 */
void BasicDrawPane::render(wxDC&  dc)
{
    // ursprung oben links
    // draw some text
    //dc.DrawText(wxT("Testing"), 40, 60);
    
    dc.SetPen( wxPen( wxColor(0,0,0), 3 ) ); // black line, 3 pixels thick
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    
    dc.DrawLine( 0, 0, 100, 100 ); // draw line across the rectangle
    
    // Look at the wxDC docs to learn how to draw other stuff
}

void MainFrame::OnRefresh(wxCommandEvent& WXUNUSED(event)){
    listports();
}

void MainFrame::listports(){
    if(sp_list_ports(&ports) == SP_OK){
        choose_port->Clear();
        for (int i = 0; ports[i]; i++) {
            choose_port->Append(sp_get_port_name(ports[i]));//TODO: sp_get_port_description
        }
    }
    //TODO: free ports
}

void MainFrame::OnConnect(wxCommandEvent& WXUNUSED(event)){
    wxString s = choose_port->GetString(choose_port->GetSelection());
    if(sp_get_port_by_name(s.ToUTF8().data(), &port) == SP_OK){
        std::cout << "yo";
        if(sp_open(port, SP_MODE_WRITE) == SP_OK)
            std::cout << "connected";
            
    }
    //TODO: free port
}

MainFrame::MainFrame(const wxString& title) : wxFrame(NULL, wxID_ANY, title){
    connected = false;
    wxBoxSizer *mainsizer = new wxBoxSizer(wxHORIZONTAL);
    wxSplitterWindow *mainsplitter = new wxSplitterWindow(this,wxID_ANY,wxDefaultPosition, wxSize(1024,768),wxSP_LIVE_UPDATE|wxSP_3DSASH);
    wxImage::AddHandler(new wxGIFHandler);
    
    mainsplitter->SetSashGravity(0);
    mainsplitter->SetMinimumPaneSize(100);
    mainsizer->Add(mainsplitter, 1,wxEXPAND,0);
    
    connect = new wxButton(this, wxID_ANY, wxT("Connect"));
    refresh = new wxButton(this, wxID_ANY, wxT("Refresh"));
    refresh->Bind(wxEVT_BUTTON, &MainFrame::OnRefresh, this, wxID_ANY);
    connect->Bind(wxEVT_BUTTON, &MainFrame::OnConnect, this, wxID_ANY);
    
    
    //oben
    wxPanel *top = new wxPanel(mainsplitter, wxID_ANY);
    wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *leiste = new wxBoxSizer(wxHORIZONTAL);
    choose_port = new wxChoice (top, wxID_ANY);
    listports();
    leiste->Add(choose_port, 0,wxALIGN_LEFT|wxALL,3);
    leiste->Add(connect,0,wxALIGN_LEFT|wxALL,3);
    leiste->Add(refresh,0,wxALIGN_LEFT|wxALL,3);
    topsizer->Add(leiste);
    topsizer->Add(new BasicDrawPane((wxFrame*)top), 1,wxEXPAND,0);
    top->SetSizer(topsizer);
    
    //unten
    wxPanel *bottom = new wxPanel(mainsplitter, wxID_ANY);
    wxBoxSizer *bottomsizer = new wxBoxSizer(wxHORIZONTAL);
    bottomsizer->Add(new wxTextCtrl::wxTextCtrl((wxFrame*)bottom,-1), 1,wxALIGN_BOTTOM,0);
    bottom->SetSizer(bottomsizer);
    
    mainsplitter->SplitHorizontally(top, bottom,400);
    this->SetSizer(mainsizer);
    mainsizer->SetSizeHints(this);
}
