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
#include <vector>
#include <libserialport.h>

#define CLAMP(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))

class dqDraw : public wxPanel
{
public:
    dqDraw(wxFrame* parent);
    void paintEvent(wxPaintEvent & evt);
    void render(wxDC& dc);
    void mouseEvent(wxMouseEvent & evt);
    void scrollEvent(wxMouseEvent & evt);
    wxPoint mousepos;
    bool drag;
    wxCoord w,h;
    wxStaticText *text;
    
    //motor model
    double Omega;
    double Id, Iq, Iabs;
    double Ud, Uq, Uabs;
    double M_perm;
    double M_rel;
    double M_ges;
    void SetCurrent(double d, double q);
    void Update();
    void SetVoltage(double d, double q);
    
    double P,Psi,Ld,Lq,Rs;
    
    double In,Un;
    
    double Pm,Pe;
    
    double scale;
    double iscale;

private:
};
