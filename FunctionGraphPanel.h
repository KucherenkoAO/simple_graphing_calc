#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class FunctionGraphPanel : public wxPanel {
public:
    FunctionGraphPanel(std::string expr,
                wxWindow * parent,
                wxWindowID id = wxID_ANY,
                const wxPoint & pos = wxDefaultPosition,
                const wxSize & size = wxDefaultSize,
                long style = wxTAB_TRAVERSAL,
                const wxString & name = wxPanelNameStr);
    void SetScale(double sc);
    double GetScale() const {return scale; }
    void SetFunction(std::string str);
private:
    std::string func;
    double scale = 1;
    void OnPaint(wxPaintEvent& event);
    void OnMouseWheel(wxMouseEvent& event);
};
