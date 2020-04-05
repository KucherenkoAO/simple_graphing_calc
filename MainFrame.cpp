#include <sstream>
#include <fstream>
#include "MainFrame.h"
#include "RPN.h"
#include "FunctionGraphFrame.h"
#include "FunctionGraphPanel.h"

#define DEBUGVAR(x) std::cout << #x << " = " << (x) << std::endl

wxSize drawSize(300, 300);

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


MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(wxID_EXIT);
    menuFile->Append(ID_MEMU_LOAD_HISTORY, "&Load history");
    menuFile->Append(ID_MEMU_EXPORT_HISTORY, "&Export history");
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append( menuFile, "&File" );
    menuBar->Append( menuHelp, "&Help" );
    SetMenuBar( menuBar );
    CreateStatusBar();
    SetStatusText( "Welcome to simple calculator!" );

    Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnLoadHistory, this, ID_MEMU_LOAD_HISTORY);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnExportHistory, this, ID_MEMU_EXPORT_HISTORY);
    Bind(wxEVT_SIZE, &MainFrame::OnSize, this);
    Bind(wxEVT_COMMAND_TEXT_UPDATED, &MainFrame::OnTextEnter, this, ID_TEXT_ENTER);
    Bind(wxEVT_TEXT_ENTER, &MainFrame::OnPressEnter, this, ID_TEXT_ENTER);
    Bind(wxEVT_BUTTON, &MainFrame::OnHistoryButton, this, ID_SAVE_HISTORY_BUTTON);
    Bind(wxEVT_BUTTON, &MainFrame::OnDrawButton, this, ID_DRAW_BUTTON);

    GlobalPanel = new wxPanel(this, ID_GLOBAL_PANEL, wxDefaultPosition, wxSize(300, 300), wxTAB_TRAVERSAL);
    DrawPanel = new FunctionGraphPanel("", this, ID_GLOBAL_PANEL, wxPoint(300, 0), drawSize, wxTAB_TRAVERSAL | wxFULL_REPAINT_ON_RESIZE);
    DrawButton = new wxButton(GlobalPanel, ID_DRAW_BUTTON, "Draw function graph", position.graphButton);

    LabelEnter = new wxStaticText(GlobalPanel, wxID_ANY, "Enter expression:", position.enterLabel);
    TextEnter = new wxTextCtrl(GlobalPanel, ID_TEXT_ENTER, wxEmptyString, position.enterField, wxDefaultSize, wxTE_PROCESS_ENTER);

    LabelAnswer = new wxStaticText(GlobalPanel, wxID_ANY, "Answer:", position.answerLabel);
    TextAnswer = new wxTextCtrl(GlobalPanel, ID_TEXT_ANSWER, wxEmptyString, position.answerField, wxDefaultSize, wxTE_READONLY);

    HistoryButton = new wxButton(GlobalPanel, ID_SAVE_HISTORY_BUTTON, "Save in history", position.historyButton);
    TextHistory = new wxTextCtrl(GlobalPanel, ID_TEXT_ANSWER, wxEmptyString, position.historyField, wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE);
}


void MainFrame::OnExit(wxCommandEvent& event)
{
    Close( true );
}


void MainFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox(about_str, "About calculator", wxOK | wxICON_INFORMATION );
}

void MainFrame::OnLoadHistory(wxCommandEvent& event) {
    ifstream fin_history("history.txt");
    if (!fin_history.is_open()) {
        wxMessageBox("Can't open file \"history.txt\"", "File error", wxOK | wxICON_INFORMATION );
        return;
    }

    TextHistory->SetValue("");
    for (std::string str; std::getline(fin_history, str); )
        TextHistory->AppendText(str + '\n');
}


void MainFrame::OnExportHistory(wxCommandEvent& event) {
    ofstream fout_history("history.txt");
    if (!fout_history.is_open()) {
        wxMessageBox("Can't open file \"history.txt\"", "File error", wxOK | wxICON_INFORMATION );
        return;
    }
    fout_history << TextHistory->GetValue();
    TextHistory->SetValue("");
}


void MainFrame::OnSize(wxSizeEvent& event)
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

    event.Skip();
}


void MainFrame::OnTextEnter(wxCommandEvent& event)
{
    bool is_const_expression = false;
    std::string expr(TextEnter->GetValue().c_str());
    DrawPanel->SetFunction("");
    try {
        if (expr.empty()) {
            TextAnswer->SetValue("");
        }
        else {
//            DrawPanel->SetFunction(expr);
            std::ostringstream os;
            os << calcExpr(expr);
            TextAnswer->SetValue(os.str());
            is_const_expression = true;
        }
    }
    catch (exception & e) {
        std::cout << e.what() << std::endl;
        TextAnswer->SetValue("");
    }

    if (!is_const_expression) {
        try {
            DrawPanel->SetFunction(expr);
        }
        catch (...) {}
    }
}

void MainFrame::OnHistoryButton(wxCommandEvent& event) {
    wxString new_line;
    DEBUGVAR(DrawPanel->function_is_valid);
    DEBUGVAR(TextAnswer->IsEmpty());
    DEBUGVAR(TextAnswer->GetValue());
    if (DrawPanel->function_is_valid)
            new_line = "y = " + TextEnter->GetValue();
    else if (!TextAnswer->IsEmpty())
            new_line = TextEnter->GetValue() + " = " + TextAnswer->GetValue();
    else
        return;

    TextHistory->AppendText("  " + new_line + '\n');
}


void MainFrame::OnDrawButton(wxCommandEvent& event) {
    FunctionGraphFrame *frame = new FunctionGraphFrame(std::string(TextEnter->GetValue().c_str()), "Function graph", wxPoint(250, 250), wxSize(700, 700) );
    frame->Show( true );
}


void MainFrame::OnPressEnter(wxCommandEvent& event) {
    OnHistoryButton(event);
    TextEnter->SetValue("");
}


