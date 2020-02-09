// Simple calculator
// For compilers that support precompilation, includes "wx/wx.h".

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "RPN.h"

using std::cout;
using std::cin;
using std::endl;

static const char * about_str =
"This is a simple graphic calculator \n\
It can several things: \n\
- Base operations +, -, *, / \n\
- Support brackets \n\
- sin(x), cos(x)";

class MyApp: public wxApp
{
public:
    virtual bool OnInit();
};


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


wxIMPLEMENT_APP(MyApp); // содержит main-функцию


bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame("Simple calculator", wxPoint(50, 50), wxSize(450, 250) );
    frame->Show( true );
    return true;
}


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
    Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::OnTextEnter, this, ID_TEXT_ENTER);


    GlobalPanel = new wxPanel(this, ID_GLOBAL_PANEL, wxPoint(128, 128), wxDefaultSize, wxTAB_TRAVERSAL, "ID_GLOBAL_PANEL");
    LabelEnter = new wxStaticText(GlobalPanel, wxID_ANY, _("Enter expression:"), wxPoint(25,30), wxSize(150,40), 0, "");
    TextEnter = new wxTextCtrl(GlobalPanel, ID_TEXT_ENTER, wxEmptyString, wxPoint(150,20), wxSize(250,40), 0, wxDefaultValidator, "ID_TEXT_ENTER");
    LabelAnswer = new wxStaticText(GlobalPanel, wxID_ANY, _("Answer:"), wxPoint(25,70), wxSize(150,40), 0, "");
    TextAnswer = new wxTextCtrl(GlobalPanel, ID_TEXT_ANSWER, wxEmptyString, wxPoint(150,60), wxSize(250,40), 0, wxDefaultValidator, "ID_TEXT_ANSWER");
}


void MyFrame::OnExit(wxCommandEvent& event)
{
    Close( true );
}

void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox( about_str, "About calculator", wxOK | wxICON_INFORMATION );
}


void MyFrame::OnTextEnter(wxCommandEvent& event)
{
//    cout << "Entered text: " << TextEnter->GetValue() << endl;

    try {
        std::string expr(TextEnter->GetValue().c_str());
        if (!expr.empty()) {
            double answer = calcExpr(expr);
            TextAnswer->SetValue(std::to_string(answer));
        }
        else
            TextAnswer->SetValue("");
    }
    catch (exception & e) {
        cout << e.what() << endl;
        TextAnswer->SetValue("");
    }
}
