#include "basicdrawpane.hpp"

const wxPen BasicDrawPane::pen[] = {*wxBLACK_PEN, *wxRED_PEN, *wxBLUE_PEN, *wxGREEN_PEN};

BasicDrawPane::BasicDrawPane(wxFrame* parent, int ch) : wxPanel(parent){
    time = 0;
    diff = 0;
    Bind(wxEVT_PAINT, &BasicDrawPane::paintEvent, this);
    xpos = 0;
    channels = ch;

    for (int i = 0; i<channels; i++) {
        data.push_back(std::vector<float>());
        for (int j = 0; j<1024; j++) {
            data[i].push_back(0);
        }
    }
}

void BasicDrawPane::Clear(){
    for (int i = 0; i<channels; i++) {
        for (auto &d : data[i]){
            d = 0;
        }
    }
    xpos = 0;
    Refresh();
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

void BasicDrawPane::plotvalue(float values[])
{
    diff = wxGetUTCTimeMillis()-time;
    time = wxGetUTCTimeMillis();
    //std::cout << "data:" << value << std::endl;
    //data.at(xpos) += 0.1;
    for (int i = 0; i<channels; i++) {
        data[i].at(xpos) = (float)values[i];
    }
    xpos = (xpos+1)%data[0].size();
    Refresh();
    //oder
    //Update();
}

void BasicDrawPane::plotvalue(float value)
{
    diff = wxGetUTCTimeMillis()-time;
    time = wxGetUTCTimeMillis();
    //std::cout << "data:" << value << std::endl;
    //data.at(xpos) += 0.1;
    data[0].at(xpos) = (float)value;
    xpos = (xpos+1)%data[0].size();
    Refresh();
    //oder
    //Update();
}

/*
 * Here we do the actual rendering. I put it in a separate
 * method so that it can work no matter what type of DC
 * (e.g. wxPaintDC or wxClientDC) is used.
 */
void BasicDrawPane::render(wxDC&  dc)
{
    wxCoord w,h;
    dc.GetSize(&w, &h);
    
    // ursprung oben links
    // draw some text
    
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    
    dc.SetPen(*wxBLACK_PEN);
    dc.DrawText(wxString::Format(wxT("%i"),diff*50), 40, 60);
    dc.DrawLine( 40, 60, 40+50, 60 );
    
    


    for (int i = 0; i<channels; i++) {
        dc.SetPen(pen[i]);
        x = 0;
        y = h/2;
        xstep = w/(data[i].size()-1);
        for(auto point : data[i]){
            xold = x;
            yold = y;
            y = h/2-point*h/2;
            dc.DrawLine( xold-xstep, yold, x, y );
            x += xstep;
        }
    }
    
    //mittellinie
    dc.SetPen(*wxGREY_PEN);
    dc.DrawLine( 0, h/2, w, h/2 );
    // Look at the wxDC docs to learn how to draw other stuff
}
