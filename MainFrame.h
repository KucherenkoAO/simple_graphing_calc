#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "FunctionGraphPanel.h"


class MainFrame: public wxFrame
{
public:
    MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

private:
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnTextEnter(wxCommandEvent& event);
    void OnHistoryButton(wxCommandEvent& event);
    void OnDrawButton(wxCommandEvent& event);
    void OnPressEnter(wxCommandEvent& event);

    wxPanel* GlobalPanel;
    FunctionGraphPanel* DrawPanel;
    wxStaticText* LabelEnter;
    wxStaticText* LabelAnswer;
    wxTextCtrl* TextEnter;
    wxTextCtrl* TextAnswer;
    wxButton* HistoryButton;
    wxTextCtrl* TextHistory;
    wxButton* DrawButton;
};


enum {
    ID_GLOBAL_PANEL,
    ID_TEXT_ENTER,
    ID_TEXT_ANSWER,
    ID_SAVE_HISTORY_BUTTON,
    ID_DRAW_BUTTON,
};


struct {
    wxPoint enterLabel = {25, 25};
    wxPoint enterField = {150, 20};
    wxPoint answerLabel = {25, 75};
    wxPoint answerField = {150, 70};
    wxPoint historyButton = {25, 120};
    wxPoint graphButton = {140, 120};
    wxPoint historyField = {25, 180};
} position;




