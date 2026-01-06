#include "component/RoundedText.h"
#include "chitr/ChitrLogger.h"
#include <wx/wx.h>
#include <wx/dcbuffer.h>

RoundedText::RoundedText(wxWindow* parent, wxWindowID id, const wxString& text, 
                         const wxColour& bgColor, const wxColour& fgColor, 
                         double radius)
    : wxPanel(parent, id), labelText(text), backColour(bgColor), foreColour(fgColor), cornerRadius(radius) {

    SetBackgroundStyle(wxBG_STYLE_PAINT);
    Bind(wxEVT_PAINT, &RoundedText::OnPaint, this);

    SetupTimerLayout(0);
}

void RoundedText::SetupTimerLayout(long long totalDurationMs) {
    long totalSeconds = totalDurationMs / 1000;

    wxString templateString;
    if (totalSeconds >= 3600) {
        templateString = "00:00:00 / 00:00:00";
    } else {
        templateString = "00:00 / 00:00";
    }

    wxClientDC dc(this);
    dc.SetFont(GetFont());
    wxSize textSize = dc.GetTextExtent(templateString);

    int totalPaddingX = 30; 
    int totalPaddingY = 25; 

    backSize = wxSize(textSize.GetWidth() + totalPaddingX, textSize.GetHeight() + totalPaddingY);
    LOG_INFO("SetupTimerLayout");

    SetMinSize(backSize);
    SetMaxSize(backSize);

    if (GetParent()) GetParent()->Layout();
    Refresh();
}

void RoundedText::SetLabel(const wxString& label) {
    labelText = label;
    Refresh();
}

wxString RoundedText::GetLabel() const {
    return labelText;
}

bool RoundedText::SetFont(const wxFont& font) {
    bool res = wxPanel::SetFont(font);
    InvalidateBestSize();
    Refresh();
    return res;
}

void RoundedText::SetRoundedColor(const wxColour& color) {
    backColour = color;
    Refresh();
}

void RoundedText::SetTextColor(const wxColour& color) {
    foreColour = color;
    Refresh();
}

void RoundedText::OnPaint(wxPaintEvent& event) {
    
    wxAutoBufferedPaintDC dc(this);
    dc.SetBackground(wxBrush(GetParent()->GetBackgroundColour()));
    dc.Clear();

    std::unique_ptr<wxGraphicsContext> gc(wxGraphicsContext::Create(dc));

    if (gc) {
        gc->SetBrush(wxBrush(backColour));
        gc->SetPen(*wxTRANSPARENT_PEN);

        wxSize size = GetClientSize();
        gc->DrawRoundedRectangle(0, 0, size.GetWidth(), size.GetHeight(), cornerRadius);
        gc->SetFont(GetFont(), foreColour);

        wxDouble textWidth, textHeight;
        gc->GetTextExtent(labelText, &textWidth, &textHeight, NULL, NULL);
        wxDouble x = (size.GetWidth() - textWidth) / 2.0;
        wxDouble y = (size.GetHeight() - textHeight) / 2.0;
        
        gc->DrawText(labelText, x, y);
    }
}