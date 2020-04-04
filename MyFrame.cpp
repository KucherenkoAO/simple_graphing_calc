#include "MyFrame.h"
#include "RPN.h"
#include <sstream>

#define DEBUGVAR(x) std::cout << #x << " = " << (x) << std::endl


wxSize drawSize(200, 200);

static const char * about_str =
"This is a simple graphic calculator. \n\
Its features: \n\
- Support for basic functions +, -, *, / \n\
- Support unary + and - \n\
- Support brackets \n\
- Factorial calculation \n\
- Exponentiation \n\
- Support trigonometric functions \n\
- Support constants \n\
- Ability to write whitespace in an expression";


MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(wxID_EXIT);
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append( menuFile, "&File" );
    menuBar->Append( menuHelp, "&Help" );
    SetMenuBar( menuBar );
    CreateStatusBar();
    SetStatusText( "Welcome to simple calculator!" );

    Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_SIZE, &MyFrame::OnSize, this);
    Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::OnTextEnter, this, ID_TEXT_ENTER);
    Bind(wxEVT_TEXT_ENTER, &MyFrame::OnPressEnter, this, ID_TEXT_ENTER);
    Bind(wxEVT_BUTTON, &MyFrame::OnHistoryButton, this, ID_SAVE_HISTORY_BUTTON);
    Bind(wxEVT_PAINT, &MyFrame::OnPaint, this, wxID_ANY);

    GlobalPanel = new wxPanel(this, ID_GLOBAL_PANEL, wxDefaultPosition, wxSize(300, 300), wxTAB_TRAVERSAL);
    DrawPanel = new wxPanel(this, ID_GLOBAL_PANEL, wxPoint(300, 0), drawSize, wxTAB_TRAVERSAL | wxFULL_REPAINT_ON_RESIZE);
    LabelEnter = new wxStaticText(GlobalPanel, wxID_ANY, "Enter expression:", position.enterLabel);
    TextEnter = new wxTextCtrl(GlobalPanel, ID_TEXT_ENTER, wxEmptyString, position.enterField, wxDefaultSize, wxTE_PROCESS_ENTER);
    LabelAnswer = new wxStaticText(GlobalPanel, wxID_ANY, "Answer:", position.answerLabel);
    TextAnswer = new wxTextCtrl(GlobalPanel, ID_TEXT_ANSWER, wxEmptyString, position.answerField, wxDefaultSize, wxTE_READONLY);
    HistoryButton = new wxButton(GlobalPanel, ID_SAVE_HISTORY_BUTTON, "Save in history", position.historyButton);
    TextHistory = new wxTextCtrl(GlobalPanel, ID_TEXT_ANSWER, wxEmptyString, position.historyField, wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE);

}


void MyFrame::OnExit(wxCommandEvent& event)
{
    Close( true );
}


void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox(about_str, "About calculator", wxOK | wxICON_INFORMATION );
}


void MyFrame::OnSize(wxSizeEvent& event)
{
    DrawPanel->SetSize(event.GetSize().GetWidth() - drawSize.GetWidth() - 20, 0, drawSize.GetWidth(), drawSize.GetHeight());
    GlobalPanel->SetSize(event.GetSize().GetWidth() - drawSize.GetWidth() - 50, event.GetSize().GetHeight() - 70);

    int right_border = 0;
    for (const auto obj_ptr : {TextEnter, TextAnswer}) {
        right_border = std::max(right_border, obj_ptr->GetPosition().x + obj_ptr->GetSizeFromTextSize(obj_ptr->GetTextExtent("99999")).GetWidth());
    }
    right_border = std::max(right_border, GlobalPanel->GetSize().GetWidth() - 5);

    for (const auto obj_ptr : {TextEnter, TextAnswer, TextHistory}) {
        obj_ptr->SetSize(right_border - obj_ptr->GetPosition().x, obj_ptr->GetSize().GetHeight());
    }

    int bottom_border = std::max(TextHistory->GetPosition().y + 40, GlobalPanel->GetSize().GetHeight() - 5);
    TextHistory->SetSize(TextHistory->GetSize().GetWidth(), bottom_border - TextHistory->GetPosition().y);
    DEBUGVAR(bottom_border - TextHistory->GetPosition().y);

    event.Skip();
}


void MyFrame::OnTextEnter(wxCommandEvent& event)
{
    try {
        std::string expr(TextEnter->GetValue().c_str());
        if (expr.empty()) {
            TextAnswer->SetValue("");
        }
        else {
            std::ostringstream os;
            os << calcExpr(expr);
            TextAnswer->SetValue(os.str());
        }
    }
    catch (exception & e) {
        std::cout << e.what() << std::endl;
        TextAnswer->SetValue("");
    }
}

void MyFrame::OnHistoryButton(wxCommandEvent& event) {
    if (TextAnswer->IsEmpty())
        return;

    wxString new_line = TextEnter->GetValue() + " = " + TextAnswer->GetValue();
    if (TextHistory->IsEmpty())
        TextHistory->AppendText("  " + new_line);
    else
        TextHistory->AppendText("\n  " + new_line);
}

void MyFrame::OnPressEnter(wxCommandEvent& event) {
    OnHistoryButton(event);
    TextEnter->SetValue("");
}

void MyFrame::OnPaint(wxPaintEvent& event) {
    wxPaintDC dc(DrawPanel);
    wxSize panelSize = DrawPanel->GetSize();
    dc.SetPen(wxPen("#619e1b"));
    dc.DrawRectangle(0, 0, panelSize.GetWidth(), panelSize.GetHeight());

    dc.SetPen(wxPen("#000000"));
    int border = 15;
    dc.DrawLine(border, panelSize.GetHeight() / 2, panelSize.GetWidth() - border, panelSize.GetHeight() / 2);
    dc.DrawLine(panelSize.GetWidth() / 2, border + 10, panelSize.GetWidth() / 2, panelSize.GetHeight() - border);
    dc.DrawText("x", panelSize.GetWidth() - border + 3, panelSize.GetHeight() / 2 - 10);
    dc.DrawText("y", panelSize.GetWidth() / 2 - 2, border - 10);
}

