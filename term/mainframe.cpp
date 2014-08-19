#include "mainframe.hpp"
#include <wx/bitmap.h>
#include <wx/artprov.h>
#include <wx/stattext.h>
#include <math.h>

class BasicDrawPane : public wxPanel
{
    
public:
    BasicDrawPane(wxFrame* parent);
    
    void paintEvent(wxPaintEvent & evt);
    void paintNow();
    
    void render(wxDC& dc);
    
    // some useful events
    /*
     void mouseMoved(wxMouseEvent& event);
     void mouseDown(wxMouseEvent& event);
     void mouseWheelMoved(wxMouseEvent& event);
     void mouseReleased(wxMouseEvent& event);
     void rightClick(wxMouseEvent& event);
     void mouseLeftWindow(wxMouseEvent& event);
     void keyPressed(wxKeyEvent& event);
     void keyReleased(wxKeyEvent& event);
     */
    
    DECLARE_EVENT_TABLE()
};

/*
 class MyApp: public wxApp
 {
 bool OnInit();
 
 wxFrame *frame;
 BasicDrawPane * drawPane;
 public:
 
 };
 
 IMPLEMENT_APP(MyApp)
 
 
 bool MyApp::OnInit()
 {
 wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
 frame = new wxFrame((wxFrame *)NULL, -1,  wxT("Hello wxDC"), wxPoint(50,50), wxSize(800,600));
 
 drawPane = new BasicDrawPane( (wxFrame*) frame );
 sizer->Add(drawPane, 1, wxEXPAND);
 
 frame->SetSizer(sizer);
 frame->SetAutoLayout(true);
 
 frame->Show();
 return true;
 }
 */
BEGIN_EVENT_TABLE(BasicDrawPane, wxPanel)
// some useful events
/*
 EVT_MOTION(BasicDrawPane::mouseMoved)
 EVT_LEFT_DOWN(BasicDrawPane::mouseDown)
 EVT_LEFT_UP(BasicDrawPane::mouseReleased)
 EVT_RIGHT_DOWN(BasicDrawPane::rightClick)
 EVT_LEAVE_WINDOW(BasicDrawPane::mouseLeftWindow)
 EVT_KEY_DOWN(BasicDrawPane::keyPressed)
 EVT_KEY_UP(BasicDrawPane::keyReleased)
 EVT_MOUSEWHEEL(BasicDrawPane::mouseWheelMoved)
 */

// catch paint events
EVT_PAINT(BasicDrawPane::paintEvent)

END_EVENT_TABLE()


// some useful events
/*
 void BasicDrawPane::mouseMoved(wxMouseEvent& event) {}
 void BasicDrawPane::mouseDown(wxMouseEvent& event) {}
 void BasicDrawPane::mouseWheelMoved(wxMouseEvent& event) {}
 void BasicDrawPane::mouseReleased(wxMouseEvent& event) {}
 void BasicDrawPane::rightClick(wxMouseEvent& event) {}
 void BasicDrawPane::mouseLeftWindow(wxMouseEvent& event) {}
 void BasicDrawPane::keyPressed(wxKeyEvent& event) {}
 void BasicDrawPane::keyReleased(wxKeyEvent& event) {}
 */

BasicDrawPane::BasicDrawPane(wxFrame* parent) :
wxPanel(parent)
{
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
    
    // draw a circle
    //dc.SetBrush(*wxGREEN_BRUSH); // green filling
    //dc.SetPen( wxPen( wxColor(255,0,0), 5 ) ); // 5-pixels-thick red outline
    //dc.DrawCircle( wxPoint(200,100), 25 /* radius */ );
    
    // draw a rectangle
    //dc.SetBrush(*wxBLUE_BRUSH); // blue filling
    //dc.SetPen( wxPen( wxColor(255,175,175), 10 ) ); // 10-pixels-thick pink outline
    //dc.DrawRectangle( 300, 100, 400, 200 );
    
    // draw a line
    int z = 10;
    float modul = 12;
    
    float r = modul*z/2;
    float p = modul*M_PI;
    float rf = r-1.25*modul; //fußradius
    float rk = r+modul; //kopfradius
    
    float ewinkel = (3.0f*sqrt(4.0f*z-1.0f))/(2.0f*z-5.0f);
    
    float xmid = r+15;
    float ymid = r+15;
    
    dc.SetPen( wxPen( wxColor(0,0,0), 2 ) ); // black line, 3 pixels thick
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    
    //dc.DrawCircle( wxPoint(xmid,ymid), r /* radius */ );
    //dc.DrawCircle( wxPoint(xmid,ymid), rk /* radius */ );
    dc.DrawCircle( wxPoint(xmid,ymid), rf /* radius */ );

    float i,oldx=0,oldy=0;
    float scale = 1;//400;
    float xoff = xmid;
    float yoff = ymid+rf;
    for (i = 0; i < M_PI/*ewinkel*/ ; i+=0.05) {
        float x = (rf*cos(i)+i*sin(i))*rf*i+xoff;
        float y = (rf*sin(i)+i*cos(i))*rf*i+yoff;
        if(oldx != 0 && oldy != 0)
            dc.DrawLine( oldx, oldy, x, y);
        oldx = x;
        oldy = y;
    }
    //dc.DrawLine( 0, 0, 100, 100 ); // draw line across the rectangle
    
    // Look at the wxDC docs to learn how to draw other stuff
}

MainFrame::MainFrame(const wxString& title) : wxFrame(NULL, wxID_ANY, title){
    wxBoxSizer *mainsizer = new wxBoxSizer(wxVERTICAL);
    wxSplitterWindow *mainsplitter = new wxSplitterWindow(this,wxID_ANY,wxDefaultPosition, wxSize(1024,768),wxSP_LIVE_UPDATE|wxSP_3DSASH);
    wxImage::AddHandler(new wxGIFHandler);
    
    mainsplitter->SetSashGravity(0);
    mainsplitter->SetMinimumPaneSize(100);
    mainsizer->Add(mainsplitter, 1,wxEXPAND,0);
    
    //left
    wxPanel *leftpanel = new wxPanel(mainsplitter, wxID_ANY);
    wxBoxSizer *leftsizer = new wxBoxSizer(wxVERTICAL);
    //leftsizer->Add(new SearchResultsList(leftpanel), 1,wxEXPAND,0);
    leftpanel->SetSizer(leftsizer);
    
    //right
    wxPanel *rightpanel = new wxPanel(mainsplitter, wxID_ANY);
    wxBoxSizer *rightsizer = new wxBoxSizer(wxVERTICAL);
    rightsizer->Add(new BasicDrawPane((wxFrame*)rightpanel), 1,wxEXPAND,0);


    rightpanel->SetSizer(rightsizer);
    
    mainsplitter->SplitVertically(leftpanel, rightpanel,200);
    this->SetSizer(mainsizer);
    mainsizer->SetSizeHints(this);

}
