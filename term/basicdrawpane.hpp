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

class BasicDrawPane : public wxPanel
{
public:
    BasicDrawPane(wxFrame* parent, int ch);
    void paintEvent(wxPaintEvent & evt);
    void paintNow();
    void plotvalue(float[]);
    void Clear();
    int channels;
    static const wxPen pen[];
private:
    wxLongLong time;
    std::vector<std::vector<float>> data;
    double x,y,xpos,xstep;
};
