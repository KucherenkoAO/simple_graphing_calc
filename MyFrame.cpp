#include "MyFrame.h"
#include "RPN.h"
#include <sstream>

static const char * about_str =
"This is a simple graphic calculator. \n\
Its features: \n\
- Support for basic functions +, -, *, / \n\
- Support unary + and - \n\
- support brackets \n\
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


    GlobalPanel = new wxPanel(this, ID_GLOBAL_PANEL, wxPoint(128, 128), wxDefaultSize, wxTAB_TRAVERSAL, "");
    LabelEnter = new wxStaticText(GlobalPanel, wxID_ANY, _("Enter expression:"), enterLabel.position, enterLabel.size, 0, "");
    TextEnter = new wxTextCtrl(GlobalPanel, ID_TEXT_ENTER, wxEmptyString, enterField.position, enterField.size, 0, wxDefaultValidator, "");
    LabelAnswer = new wxStaticText(GlobalPanel, wxID_ANY, _("Answer:"), answerLabel.position, answerLabel.size, 0, "");
    TextAnswer = new wxTextCtrl(GlobalPanel, ID_TEXT_ANSWER, wxEmptyString, answerField.position, answerField.size, wxTE_READONLY, wxDefaultValidator, "");
}


void MyFrame::OnExit(wxCommandEvent& event)
{
    Close( true );
}


void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox( about_str, "About calculator", wxOK | wxICON_INFORMATION );
}

void MyFrame::OnSize(wxSizeEvent& event)
{
    int width = std::max(60, std::max(static_cast<int>(TextEnter->GetValue().size())*9, event.GetSize().GetWidth() - TextEnter->GetPosition().x - 50));
    wxSize size(width, enterField.size.GetHeight());
    TextEnter->SetSize(size);
    TextAnswer->SetSize(size);
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
