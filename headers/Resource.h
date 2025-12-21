#pragma once
#include <wx/wx.h>

/**
 * Resource class contains all the assets like images,
 * icons, primary and secondary colours that are used in the
 * application, they are defined one time in the RootFrame class's
 * member
 * All members are public because the resources will be called from
 * non friend classes
 */
class Resource
{
    public :

    wxColour        headerBackgroundColour;
    wxColour        bodyBackgroundColour;
    wxColour        footerBackgroundColour;

    wxBitmap        uploadIcon;
    wxBitmap        nextIcon;
    wxBitmap        previousIcon;
    wxBitmap        slideshowIcon;
    wxBitmap        closeIcon;
    wxBitmap        playIcon;
    wxBitmap        stopIcon;
    wxBitmap        pauseIcon;
    wxBitmap        volumeIcon;
    wxBitmap        imageIcon;
    wxBitmap        musicIcon;


    Resource();

    private :
    wxBitmap GetIconAsBitmap(const char* utf8IconCode, int size, const wxColour& color);
};