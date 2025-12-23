#include <wx/wx.h>
#include <wx/graphics.h>
#include <wx/window.h>
#include "chitr/Resource.h"
#include "chitr/ChitrLogger.h"
#include "IconsMaterialDesign.h"

Resource::Resource()
{
    headerBackgroundColour = wxColour(45, 55, 52);
    bodyBackgroundColour   = wxColour(30, 33, 32);
    footerBackgroundColour = wxColour(38, 42, 41);

    int iconSize = wxWindow::FromDIP(40, (wxWindow*)NULL); 

    uploadIcon =    GetIconAsBitmap(ICON_MD_FILE_UPLOAD, iconSize, *wxWHITE);
    nextIcon =      GetIconAsBitmap(ICON_MD_SKIP_NEXT, iconSize, *wxWHITE);
    previousIcon =  GetIconAsBitmap(ICON_MD_SKIP_PREVIOUS, iconSize, *wxWHITE);
    slideshowIcon = GetIconAsBitmap(ICON_MD_SLIDESHOW, iconSize, *wxWHITE);
    closeIcon =     GetIconAsBitmap(ICON_MD_CLOSE, iconSize, *wxWHITE);
    playIcon =      GetIconAsBitmap(ICON_MD_PLAY_ARROW, iconSize, *wxWHITE);
    stopIcon =      GetIconAsBitmap(ICON_MD_STOP, iconSize, *wxWHITE);
    pauseIcon =     GetIconAsBitmap(ICON_MD_PAUSE, iconSize, *wxWHITE);
    volumeIcon =    GetIconAsBitmap(ICON_MD_VOLUME_UP, iconSize, *wxWHITE);
    imageIcon =     GetIconAsBitmap(ICON_MD_IMAGE, iconSize, *wxWHITE);
    musicIcon =     GetIconAsBitmap(ICON_MD_LIBRARY_MUSIC, iconSize, *wxWHITE);
}

wxBitmap Resource::GetIconAsBitmap(const char* utf8IconCode, int size, const wxColour& fgColor) {
    
    LOG_INFO("Getting Icons: %s", utf8IconCode);
    wxImage img(size, size);
    img.InitAlpha();
    unsigned char* alpha = img.GetAlpha();
    memset(alpha, 0, size * size); 

    wxBitmap bmp(img);
    wxMemoryDC memDC(bmp);

    std::unique_ptr<wxGraphicsContext> gc(wxGraphicsContext::Create(memDC));

    if (gc) {
        wxString text = wxString::FromUTF8(utf8IconCode);
        wxDouble width, height, descent, externalLeading;
        double radius = size / 2.0; 
        wxFont iconFont(wxFontInfo(size * 0.6).FaceName("Material Icons"));

        if(!iconFont.IsOk())
        {
            LOG_ERROR("Failed to Create Icon Font");
        }

        gc->SetBrush(wxBrush(bodyBackgroundColour));
        gc->SetPen(*wxTRANSPARENT_PEN);
        gc->DrawRoundedRectangle(0, 0, size, size, radius);
        gc->SetFont(iconFont, fgColor);
        gc->GetTextExtent(text, &width, &height, &descent, &externalLeading);
        gc->DrawText(text, (size - width) / 2.0, (size - height) / 2.0);
    }
    else
    {
        LOG_ERROR("Could Not Create Graphics Context");
    }

    memDC.SelectObject(wxNullBitmap);
    return bmp;
}