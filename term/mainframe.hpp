#pragma once

#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/listctrl.h>


class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString& title);
};

class SearchResultsList: public wxListCtrl{
public:
    SearchResultsList(wxWindow* parent);
    wxString OnGetItemText(long item, long column) const;
};