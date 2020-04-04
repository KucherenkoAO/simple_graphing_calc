#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <string>
#include <vector>
#include <stack>
#include <memory>
#include "RPN.h"

class GraphFrame : public wxFrame {
public:
    GraphFrame(std::string expr, const wxString& title, const wxPoint& pos, const wxSize& size);
    void DrawFunction();
private:
    std::string expression;
    double scale = 1;
    void OnExit(wxCommandEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnPaint(wxPaintEvent& event);
    void OnScaleEnter(wxCommandEvent& event);

    wxPanel* ControlPanel;
    wxPanel* DrawPanel;
    wxStaticText* LabelScale;
    wxTextCtrl* TextScale;

    enum {
        ID_CONTROL_PANEL,
        ID_GRAPH_PANEL,
        ID_TEXT_SCALE,
    };

    struct {
        wxPoint scaleLabel = {10, 25};
        wxPoint scaleField = {100, 20};
    } position;
};

