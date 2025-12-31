#pragma once
#include <wx/wx.h>

class Resource
{
private :

    wxBitmap GetIconAsBitmap(const char* utf8IconCode, int size, const wxColour& color);

    wxColour        primaryColour;
    wxColour        secondaryColour;
    wxBitmap        uploadIcon;
    wxBitmap        nextIcon;
    wxBitmap        previousIcon;
    wxBitmap        slideshowIcon;
    wxBitmap        closeIcon;
    wxBitmap        playIcon;
    wxBitmap        stopIcon;
    wxBitmap        pauseIcon;
    wxBitmap        volumeIcon;
    wxBitmap        muteIcon;
    wxBitmap        imageIcon;
    wxBitmap        musicIcon;

public:
    Resource();

    wxColour getPrimaryColour();
    wxColour getSecondaryColour();
    wxBitmap getUploadIcon();
    wxBitmap getNextIcon();
    wxBitmap getPreviousIcon();
    wxBitmap getSlideshowIcon();
    wxBitmap getCloseIcon();
    wxBitmap getPlayIcon();
    wxBitmap getStopIcon();
    wxBitmap getPauseIcon();
    wxBitmap getVolumeIcon();
    wxBitmap getMuteIcon();
    wxBitmap getImageIcon();
    wxBitmap getMusicIcon();

};