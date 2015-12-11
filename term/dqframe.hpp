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
#include <wx/config.h>
#include <wx/slider.h>
#include <math.h>
#include <vector>
#include <libserialport.h>
#include "dqdraw.hpp"

class dqFrame : public wxFrame
{
public:
    dqFrame(const wxString& title);
private:
    dqDraw *dqdraw;
};
