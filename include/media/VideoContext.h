#pragma once
#include <optional>
#include <wx/wx.h>
#include "chitr/CFile.h"
#include "media/MediaContext.h"
#include "chitr/CTime.h"

#ifdef _WIN32
#else
#define SUPPORTED_FORMATS \
    _T("aac"), _T("aiff"), _T("au"), _T("caf"), \
    _T("flac"), _T("m4a"), _T("m4v"), _T("mov"), _T("mp3"), \
    _T("mp4"), _T("mpg"), _T("ogg"), _T("opus"), _T("wav")
#endif

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