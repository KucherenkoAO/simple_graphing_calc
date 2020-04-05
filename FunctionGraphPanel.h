#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class FunctionGraphPanel : public wxPanel {
public:
    FunctionGraphPanel( std::string expr,
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
    double scale = 40;
    void OnPaint(wxPaintEvent& event);
    void OnMouseWheel(wxMouseEvent& event);
    void OnMouseLeftDown(wxMouseEvent& event);
    void OnMouseLeftUp(wxMouseEvent& event);
    void OnMouseMotion(wxMouseEvent& event);

    wxRealPoint startMotionPostition;
    wxRealPoint oldCenterScreenPoint;
    wxRealPoint centerScreenPoint {0, 0};
    void SetCoord();
    int TransformX(double x) const;
    int TransformY(double y) const;
    wxPoint TransformXY(double x, double y) const;

    double x_0;
    double x_min;
    double x_max;

    double y_0;
    double y_max;
    double y_min;
};

