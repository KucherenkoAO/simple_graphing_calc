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
    void OnSize(wxSizeEvent& event);
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


struct ElementGeometry {
    wxPoint position;
    wxSize size;
};


const ElementGeometry enterLabel { wxPoint(25, 30), wxSize(150, 40) };
const ElementGeometry enterField { wxPoint(150, 20), wxSize(250, 35) };
const ElementGeometry answerLabel { wxPoint(25, 80), wxSize(150, 40) };
const ElementGeometry answerField { wxPoint(150, 70), wxSize(250, 35) };







