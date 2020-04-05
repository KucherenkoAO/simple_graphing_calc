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
#include "FunctionGraphPanel.h"



class FunctionGraphFrame : public wxFrame {
public:
    FunctionGraphFrame(std::string expr, const wxString& title, const wxPoint& pos, const wxSize& size);
private:
    void OnExit(wxCommandEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnPaint(wxPaintEvent& event);

    wxPanel* ControlPanel;
    FunctionGraphPanel* GraphPanel;

    enum {
        ID_CONTROL_PANEL,
        ID_GRAPH_PANEL,
    };
};

