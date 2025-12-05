#pragma once
#include <vector>
#include <string>
#include <wx/wx.h>

/**
 * VideoPlayback class depicts the class that handles
 * the video/music uploaded. It stores the file address
 * of the video/music, the index of the current media being played
 */

class VideoPlayback
{
    std::vector<wxString>   videos;
    int                     currentIndex;

    public:

    VideoPlayback();
    void        addVideo(wxString videoPath);
    wxString    getVideoByIndex(int index);
    int         getNumberOfVideos();
    void        setCurrentIndex(int newIndex);
    int         getCurrentIndex();
};