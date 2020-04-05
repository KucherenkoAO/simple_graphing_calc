#include <iostream>
#include <exception>
#include "Node.h"
#include "FunctionGraphFrame.h"

#define DEBUGVAR(x) std::cout << #x << " = " << (x) << std::endl

static const char * about_str = "About";


FunctionGraphFrame::FunctionGraphFrame(std::string expr, const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append( menuHelp, "&Help" );
    SetMenuBar( menuBar );
    CreateStatusBar();
    SetStatusText( "y = " + expr );

    Bind(wxEVT_COMMAND_MENU_SELECTED, &FunctionGraphFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &FunctionGraphFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_SIZE, &FunctionGraphFrame::OnSize, this);

    GraphPanel = new FunctionGraphPanel(expr, this, ID_GRAPH_PANEL, wxPoint(0, 0), {GetSize().GetWidth() - 10, GetSize().GetHeight() - 110}, wxTAB_TRAVERSAL | wxFULL_REPAINT_ON_RESIZE);
    ControlPanel = new wxPanel(this, ID_CONTROL_PANEL, {0, GetSize().GetHeight() - 100}, {GetSize().GetWidth(), 100}, wxTAB_TRAVERSAL);
}


void FunctionGraphFrame::OnExit(wxCommandEvent& event)
{
    Close( true );
}


void FunctionGraphFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox(about_str, "About graph", wxOK | wxICON_INFORMATION);
}


void FunctionGraphFrame::OnSize(wxSizeEvent& event)
{
    GraphPanel->SetSize(event.GetSize().GetWidth() - 10, event.GetSize().GetHeight() - 110);
    ControlPanel->SetSize(0, event.GetSize().GetHeight() - 100, event.GetSize().GetWidth(), 100);
    event.Skip();
}








