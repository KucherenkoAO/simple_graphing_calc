#include <iostream>
#include <exception>
#include "RPN.h"
#include "FunctionGraphPanel.h"

#define DEBUGVAR(x) std::cout << #x << " = " << (x) << std::endl

FunctionGraphPanel::FunctionGraphPanel(std::string expr, wxWindow * parent, wxWindowID id, const wxPoint & pos,
                       const wxSize & size, long style, const wxString & name)
                        : wxPanel(parent, id, pos, size, style, name), func(std::move((expr)))
{
    Bind(wxEVT_PAINT, &FunctionGraphPanel::OnPaint, this, wxID_ANY);
    Bind(wxEVT_MOUSEWHEEL, &FunctionGraphPanel::OnMouseWheel, this, wxID_ANY);
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


void FunctionGraphPanel::OnPaint(wxPaintEvent& event) {
    wxPaintDC dc(this);
    dc.Clear();
    wxSize panelSize = this->GetSize();
    dc.SetPen(wxPen("#619e1b"));
    dc.DrawRectangle(0, 0, panelSize.GetWidth(), panelSize.GetHeight());

    double x = 1;
    std::queue<std::shared_ptr<Node>> rpn;
    try {
        auto tokens = parseExpr(func);
        rpn = parseTokens(tokens, &x);
        calcRPN(rpn);
    }
    catch (...) {
        return;
    }

    dc.SetPen(wxPen("#000000"));
    int axis_border = 20;
    dc.DrawLine(axis_border, panelSize.GetHeight() / 2, panelSize.GetWidth() - axis_border, panelSize.GetHeight() / 2);
    dc.DrawLine(panelSize.GetWidth() / 2, axis_border + 10, panelSize.GetWidth() / 2, panelSize.GetHeight() - axis_border);
    dc.DrawText("x", panelSize.GetWidth() - axis_border + 3, panelSize.GetHeight() / 2 - 10);
    dc.DrawText("y", panelSize.GetWidth() / 2 - 2, axis_border - 10);

    int x_max = panelSize.GetWidth() / 2;
    int x_min = -x_max;
    int x_0 = x_max;

    int y_max = panelSize.GetHeight() / 2;
    int y_min = -y_max;
    int y_0 = y_max;

    int graph_border = std::min(panelSize.GetWidth(), panelSize.GetHeight()) / 20;

    x = (x_min + graph_border) / scale;
    double y = 0;
    double prev_x, prev_y = 0;
    bool is_first_point = true;
    while (x < (x_max - graph_border) / scale) {
        y = calcRPN(rpn);
        if (y > (y_min + graph_border) / scale && y < (y_max - graph_border) / scale) {
            if (is_first_point) {
                is_first_point = false;
            }
            else {
                dc.DrawLine({(int)(x*scale) + x_0, -(int)(y*scale)  + y_0}, {(int)(prev_x*scale) + x_0, -(int)(prev_y*scale)  + y_0});
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
        SetScale(GetScale() * 1.5);
    else
        SetScale(GetScale() / 1.5);
}

