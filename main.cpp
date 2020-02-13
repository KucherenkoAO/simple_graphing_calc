// Simple calculator
// For compilers that support precompilation, includes "wx/wx.h".

#include "MyFrame.h"
#include <iostream>

class MyApp: public wxApp
{
public:
    virtual bool OnInit();
};

bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame("Simple calculator", wxPoint(50, 50), wxSize(450, 250) );
    frame->Show( true );
    return true;
}


wxIMPLEMENT_APP(MyApp); // содержит main-функцию
