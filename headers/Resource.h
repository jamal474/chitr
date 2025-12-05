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

    wxImage         uploadIcon;
    wxImage         nextIcon;
    wxImage         previousIcon;
    wxImage         slideshowIcon;
    wxImage         closeIcon;
    wxImage         playIcon;
    wxImage         stopIcon;
    wxImage         pauseIcon;
    wxImage         volumeIcon;
    wxImage         imageIcon;
    wxImage         musicIcon;


    Resource();
};