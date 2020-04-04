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
    MyFrame *frame = new MyFrame("Simple calculator", wxPoint(250, 250), wxSize(750, 700) );
    frame->Show( true );
    return true;
}


wxIMPLEMENT_APP(MyApp); // содержит main-функцию
