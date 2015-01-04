#include "dqdraw.hpp"

dqDraw::dqDraw(wxFrame* parent) : wxPanel(parent){
    Bind(wxEVT_PAINT, &dqDraw::paintEvent, this);
    Bind(wxEVT_MOTION, &dqDraw::mouseEvent, this);
    
    scale = 3;
    In = 2;
    Un = 150;
    
    Rs = 7.6;
    Omega = 0;
    Psi = 0.5411;
    P = 4;
    Ld = 0.0041;
    Lq = 0.0041;
}

void dqDraw::Update()
{
    Uabs = sqrt(Ud*Ud + Uq*Uq);
    Iabs = sqrt(Id*Id + Iq*Iq);
    
    // Permanentmagnet-, Reluktanz- und Gesamtmoment
    //
    M_perm = 1.5f * P *  Psi * Iq;
    M_rel  = 1.5f * P * (Ld - Lq) * Id * Iq;
    M_ges  = M_rel + M_perm;
}


void dqDraw::SetCurrent(double d, double q)
{
    double OmegaPsi = Omega * Psi;
    double Xd   = Omega * Ld;
    double Xq   = Omega * Lq;
    
    Id = d;
    Iq = q;
    Ud =           -Iq * Xq + Id * Rs;
    Uq = OmegaPsi + Id * Xd + Iq * Rs;
    
    Update();
}

void dqDraw::SetVoltage(double d, double q)
{
    double OmegaPsi = Omega * Psi;
    double Xd   = Omega * Ld;
    double Xq   = Omega * Lq;
    
    Ud = d;
    Uq = q;
    Id = (Rs * d + Xq * (q - OmegaPsi)) / (Rs * Rs + Xd * Xq);
    Iq = (Rs * (q - OmegaPsi) - Xd * d) / (Rs * Rs + Xd * Xq);
    
    Update();
}

void dqDraw::mouseEvent(wxMouseEvent & evt)
{
    if(evt.LeftIsDown() && evt.Dragging()){
        double x,y;
        x = (evt.GetX()-w/2)*scale;
        y = (h/2-evt.GetY())*scale;
        //std::cout << x << " " << y << std::endl;
        SetVoltage(x,y);
        //SetCurrent(x, y);
    }

    //Update();
    Refresh();
    evt.Skip();
}

void dqDraw::paintEvent(wxPaintEvent & evt)
{
    wxPaintDC dc(this);
    render(dc);
}

void dqDraw::render(wxDC&  dc)
{
    dc.GetSize(&w, &h);
    
    // ursprung oben links
    
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.SetPen(*wxRED_PEN);
    dc.DrawLine( w/2, h/2, Ud/scale+w/2, h/2-Uq/scale);
    dc.DrawCircle(w/2, h/2, Un*scale);
    dc.SetPen(*wxBLUE_PEN);
    dc.DrawLine( w/2, h/2, Id/scale+w/2, h/2-Iq/scale);
    dc.DrawCircle(w/2, h/2, In*scale);

    dc.SetPen(*wxGREY_PEN);
    dc.DrawLine( 0, h/2, w, h/2 );
    dc.DrawLine( w/2, 0, w/2, h );
}