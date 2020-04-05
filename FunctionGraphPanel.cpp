#include <iostream>
#include <exception>
#include "RPN.h"
#include "FunctionGraphPanel.h"
#include "MainFrame.h"

#define DEBUGVAR(x) std::cout << #x << " = " << (x) << std::endl

FunctionGraphPanel::FunctionGraphPanel(std::string expr, wxWindow * parent, wxWindowID id, const wxPoint & pos,
                       const wxSize & size, long style, const wxString & name)
                        : wxPanel(parent, id, pos, size, style, name), func(std::move((expr)))
{
    Bind(wxEVT_PAINT, &FunctionGraphPanel::OnPaint, this, wxID_ANY);
    Bind(wxEVT_MOUSEWHEEL, &FunctionGraphPanel::OnMouseWheel, this, wxID_ANY);
    Bind(wxEVT_LEFT_DOWN, &FunctionGraphPanel::OnMouseLeftDown, this, wxID_ANY);
    Bind(wxEVT_LEFT_UP, &FunctionGraphPanel::OnMouseLeftUp, this, wxID_ANY);
    Bind(wxEVT_MOTION, &FunctionGraphPanel::OnMouseMotion, this, wxID_ANY);
}


void FunctionGraphPanel::SetScale(double sc) {
    if (sc != 0) {
        scale = sc;
        Refresh();
    }
}


void FunctionGraphPanel::SetFunction(std::string str) {
    func = std::move(str);
    Refresh();
}


void FunctionGraphPanel::SetCoord() {
    x_0 = centerScreenPoint.x - this->GetSize().GetWidth() / scale / 2;
    x_min = x_0;
    x_max = x_min + this->GetSize().GetWidth() / scale;

    y_0 = centerScreenPoint.y + this->GetSize().GetHeight() / scale / 2;
    y_max = y_0;
    y_min = y_max - this->GetSize().GetHeight() / scale;
}


int FunctionGraphPanel::TransformX(double x) const {
    return (int)((x - x_0)*scale);
}

int FunctionGraphPanel::TransformY(double y) const {
    return (int)((y_0 - y)*scale);
}

wxPoint FunctionGraphPanel::TransformXY(double x, double y) const {
    return {(int)((x - x_0)*scale), (int)((y_0 - y)*scale)};
}


void FunctionGraphPanel::OnPaint(wxPaintEvent& event) {
    wxPaintDC dc(this);
    dc.Clear();
    wxSize panelSize = this->GetSize();
    dc.SetPen(wxPen("#619e1b"));
    dc.DrawRectangle(0, 0, panelSize.GetWidth(), panelSize.GetHeight());

    double x = 1;
    std::queue<std::shared_ptr<Node>> rpn;
    try {
        if (func == "") {
            function_is_valid = false;
            return;
        }
        auto tokens = parseExpr(func);
        rpn = parseTokens(tokens, &x);
        calcRPN(rpn);
        function_is_valid = true;
    }
    catch (...) {
        function_is_valid = false;
        return;
    }

    SetCoord();
    dc.SetPen(wxPen("#000000"));
    double height = 8 / scale;

    dc.DrawLine(TransformXY(0, y_min), TransformXY(0, y_max));
    for(int x = x_min; x < x_max; ++x) {
        dc.DrawLine(TransformXY(x, height / 2), TransformXY(x, -height / 2));
        if (x != 0)
            dc.DrawText(std::to_string(x), TransformXY(x - 4 / scale, -height / 2));
    }

    dc.DrawLine(TransformXY(x_min, 0), TransformXY(x_max, 0));
    for(int y = y_min; y < y_max; ++y) {
        dc.DrawLine(TransformXY(height / 2, y), TransformXY(-height / 2, y));
        if (y != 0)
            dc.DrawText(std::to_string(y), TransformXY(8 / scale, y + 10 / scale));
    }

    dc.DrawText("0", TransformXY(-15 / scale, -3 / scale));


    x = x_min;
    double y = 0;
    double prev_x = 0, prev_y = 0;
    prev_x = prev_y;
    prev_y = prev_x;
    bool is_first_point = true;
    while (x < x_max) {
        y = calcRPN(rpn);
        if (y > y_min && y < y_max) {
            if (is_first_point) {
                is_first_point = false;
            }
            else {
                dc.DrawLine(TransformXY(x, y), TransformXY(prev_x, prev_y));
//                dc.DrawPoint(TransformXY(x, y));
            }
            prev_x = x;
            prev_y = y;
        }
        else
            is_first_point = true;
        x += 1 / scale;
    }
}


void FunctionGraphPanel::OnMouseWheel(wxMouseEvent& event) {
   if (event.GetWheelRotation() > 0)
        SetScale(GetScale() * 1.2);
    else
        SetScale(GetScale() / 1.2);
}


void FunctionGraphPanel::OnMouseLeftDown(wxMouseEvent& event) {
    CaptureMouse();
    startMotionPostition = wxRealPoint(event.GetX(), event.GetY());
    oldCenterScreenPoint = centerScreenPoint;
}


void FunctionGraphPanel::OnMouseLeftUp(wxMouseEvent& event) {
    ReleaseMouse();
}


void FunctionGraphPanel::OnMouseMotion(wxMouseEvent& event) {
    if (event.Dragging()) {
        wxRealPoint delta = startMotionPostition - wxRealPoint(event.GetX(), event.GetY());
        delta.y = -delta.y;
        delta.x /= scale;
        delta.y /= scale;
        centerScreenPoint = oldCenterScreenPoint + delta;
        Refresh();
    }
}
