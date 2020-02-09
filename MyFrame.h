#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class MyFrame: public wxFrame
{
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

private:
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnTextEnter(wxCommandEvent& event);

    wxPanel* GlobalPanel;
    wxStaticText* LabelEnter;
    wxStaticText* LabelAnswer;
    wxTextCtrl* TextEnter;
    wxTextCtrl* TextAnswer;
};


enum {
    ID_GLOBAL_PANEL,
    ID_TEXT_ENTER,
    ID_TEXT_ANSWER,
};
