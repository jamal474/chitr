#pragma once
#include <optional>
#include <wx/wx.h>
#include "chitr/CFile.h"
#include "media/MediaContext.h"
#include "chitr/CTime.h"

class VideoContext : public MediaContext {

private:
    bool            isPlaying;
    int             volume;
    CTime           totalPlaybackTime;

public:
    VideoContext();
    ~VideoContext();

    std::optional<wxString> getVideo() const;
    const std::optional<CFile *> getVideoFile() const;
    const long long       getTotalPlaybackTimeInMiliSecond() const;
    const std::string     getTotalPlaybackTimeString() const;
    void    addVideo(CFile *);
    bool    getIsPlaying() const;
    void    setIsPlaying(const bool&);
    int     getVolume() const;
    void    setVolume(const int&);
    void    setTotalPlaybackTime(const long long &);
};