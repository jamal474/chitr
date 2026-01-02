#include "component/RoundedText.h"
#include <wx/wx.h>

RoundedText::RoundedText(wxWindow* parent, wxWindowID id, const wxString& text, 
                         const wxColour& bgColor, const wxColour& fgColor, 
                         double radius)
    : wxPanel(parent, id), backColour(bgColor), cornerRadius(radius) {

    Bind(wxEVT_PAINT, &RoundedText::OnPaint, this);

    textLabel = new wxStaticText(this, wxID_ANY, text);
    textLabel->SetForegroundColour(fgColor);
    textLabel->SetBackgroundColour(bgColor);

    sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(textLabel, 1, wxALIGN_CENTER | wxALL, 12);
    SetSizer(sizer);
}

void RoundedText::SetLabel(const wxString& label) {
    textLabel->SetLabel(label);
    textLabel->Refresh();
}

wxString RoundedText::GetLabel() const {
    return textLabel->GetLabel();
}

bool RoundedText::SetFont(const wxFont& font) {

    bool res = textLabel->SetFont(font);
    InvalidateBestSize();
    Layout(); 
    Refresh();
    return res;
}

void RoundedText::SetRoundedColor(const wxColour& color) {
    backColour = color;
    textLabel->SetBackgroundColour(color);
    Refresh();
}

void RoundedText::SetTextColor(const wxColour& color) {
    textLabel->SetForegroundColour(color);
    Refresh();
}

void RoundedText::OnPaint(wxPaintEvent& event) {
    wxPaintDC dc(this);
    std::unique_ptr<wxGraphicsContext> gc(wxGraphicsContext::Create(dc));

    if (gc) {
        gc->SetBrush(wxBrush(backColour));
        gc->SetPen(*wxTRANSPARENT_PEN);

        wxSize size = GetClientSize();
        gc->DrawRoundedRectangle(0, 0, size.GetWidth(), size.GetHeight(), cornerRadius);
    }
}