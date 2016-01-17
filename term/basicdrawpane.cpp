#include "basicdrawpane.hpp"
#include <wx/graphics.h>

const wxPen BasicDrawPane::pen[] = {
    *wxBLACK_PEN,
    *wxRED_PEN,
    *wxBLUE_PEN,
    *wxGREEN_PEN,
    wxPen(wxColour(255, 128, 0)),
    wxPen(wxColour(128, 128, 64)),
    wxPen(wxColour(128, 64, 128)),
    wxPen(wxColour(64, 128, 128))
};

BasicDrawPane::BasicDrawPane(wxFrame* parent, int ch) : wxPanel(parent){
    time = wxGetUTCTimeMillis();
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
    wxCoord w,h;
    dc.GetSize(&w, &h);
    wxGraphicsContext *gc = wxGraphicsContext::Create( dc );
    if (gc)
    {
        for (int i = 0; i<channels; i++) {
            wxGraphicsPath path = gc->CreatePath();
            gc->SetPen(pen[i]);
            x = 0;
            y = h/2;
            xstep = (double)w/(double)(data[i].size()-1);
            path.MoveToPoint(x, y);
            for(auto point : data[i]){
                y = h/2-point*h/2;
                path.AddLineToPoint(x, y);
                x += xstep;
            }
            gc->StrokePath(path);
        }

        //center line
        wxGraphicsPath path = gc->CreatePath();
        gc->SetPen(*wxGREY_PEN);
        path.MoveToPoint(0, h/2);
        path.AddLineToPoint(w, h/2);
        path.CloseSubpath();
        gc->StrokePath(path);
        delete gc;
    }
}

void BasicDrawPane::plotvalue(float values[])
{
    for (int i = 0; i<channels; i++) {
        data[i].at(xpos) = (float)values[i];
    }
    xpos = ((int)xpos+1)%data[0].size();
    Refresh();
}
