#pragma once
#include <wx/wx.h>
#include <wx/graphics.h>

class RoundedText : public wxPanel {
    
public:
    RoundedText(wxWindow* parent, wxWindowID id, const wxString& text, 
                const wxColour& bgColor, const wxColour& fgColor, 
                double radius = 10.0);

    void SetLabel(const wxString& label) override;
    wxString GetLabel() const override;

    bool SetFont(const wxFont& font) override;
    void SetRoundedColor(const wxColour& color);
    void SetTextColor(const wxColour& color);

protected:
    void OnPaint(wxPaintEvent& event);

private:
    wxStaticText    *textLabel;
    wxBoxSizer      *sizer;
    wxColour        backColour;
    double          cornerRadius;
};
