#include <iostream>
#include <exception>
#include "Node.h"
#include "DrawFunctionGraphics.h"

#define DEBUGVAR(x) std::cout << #x << " = " << (x) << std::endl

static const char * about_str = "About";

GraphFrame::GraphFrame(std::string expr, const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(NULL, wxID_ANY, title, pos, size), expression(expr)
{
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append( menuHelp, "&Help" );
    SetMenuBar( menuBar );
    CreateStatusBar();
    SetStatusText( "Welcome to simple calculator!" );

    Bind(wxEVT_COMMAND_MENU_SELECTED, &GraphFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &GraphFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_SIZE, &GraphFrame::OnSize, this);
    Bind(wxEVT_PAINT, &GraphFrame::OnPaint, this, wxID_ANY);
    Bind(wxEVT_COMMAND_TEXT_UPDATED, &GraphFrame::OnScaleEnter, this, ID_TEXT_SCALE);

    DrawPanel = new wxPanel(this, ID_GRAPH_PANEL, wxPoint(0, 0), {GetSize().GetWidth() - 10, GetSize().GetHeight() - 210}, wxTAB_TRAVERSAL | wxFULL_REPAINT_ON_RESIZE);
    ControlPanel = new wxPanel(this, ID_CONTROL_PANEL, {0, GetSize().GetHeight() - 200}, {GetSize().GetWidth(), 200}, wxTAB_TRAVERSAL);
    LabelScale = new wxStaticText(ControlPanel, wxID_ANY, "Enter scale:", position.scaleLabel);
    TextScale = new wxTextCtrl(ControlPanel, ID_TEXT_SCALE, wxEmptyString, position.scaleField, wxDefaultSize, wxTE_PROCESS_ENTER);
    TextScale->SetValue("1");
}


void GraphFrame::OnExit(wxCommandEvent& event)
{
    Close( true );
}


void GraphFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox(about_str, "About calculator", wxOK | wxICON_INFORMATION);
}


void GraphFrame::OnSize(wxSizeEvent& event)
{
    DrawPanel->SetSize(event.GetSize().GetWidth() - 10, event.GetSize().GetHeight() - 210);
    ControlPanel->SetSize(0, event.GetSize().GetHeight() - 200, event.GetSize().GetWidth(), 200);
    event.Skip();
}

void GraphFrame::OnPaint(wxPaintEvent& event) {
    DrawFunction();
}


void GraphFrame::DrawFunction() {
    double x = 0;
    auto tokens = parseExpr(expression);
    auto rpn = parseTokens(tokens, &x);
    calcRPN(rpn);
    wxPaintDC dc(DrawPanel);
    dc.Clear();
    wxSize panelSize = DrawPanel->GetSize();
    dc.SetPen(wxPen("#619e1b"));
    dc.DrawRectangle(0, 0, panelSize.GetWidth(), panelSize.GetHeight());

    dc.SetPen(wxPen("#000000"));
    int border = 15;
    dc.DrawLine(border, panelSize.GetHeight() / 2, panelSize.GetWidth() - border, panelSize.GetHeight() / 2);
    dc.DrawLine(panelSize.GetWidth() / 2, border + 10, panelSize.GetWidth() / 2, panelSize.GetHeight() - border);
    dc.DrawText("x", panelSize.GetWidth() - border + 3, panelSize.GetHeight() / 2 - 10);
    dc.DrawText("y", panelSize.GetWidth() / 2 - 2, border - 10);

    int x_min = -DrawPanel->GetSize().GetWidth() / 2;
    int x_max = DrawPanel->GetSize().GetWidth() / 2;
    int x_0 = x_max;
    int y_0 = DrawPanel->GetSize().GetHeight() / 2;
    double y = 0;

    x = (x_min + 30) / scale;
    while (x < (x_max - 30) / scale) {
        y = calcRPN(rpn);
        std::cout << "x = " << x << ", y = " << y << std::endl;
        dc.DrawPoint({(int)(x*scale) + x_0, -(int)(y*scale)  + y_0});
        x += 1 / scale;
    }
}


void GraphFrame::OnScaleEnter(wxCommandEvent& event) {
    try {
        scale = std::stof(std::string(TextScale->GetValue().c_str()));
        DrawFunction();
    }
    catch(...) {}
}







