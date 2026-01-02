#pragma once
#include <optional>
#include <wx/wx.h>
#include <wx/filename.h>
#include <media/MediaContext.h>
#include <chitr/CTime.h>

class VideoContext : public MediaContext {

private:
    bool            isPlaying;
    int             volume;
    CTime           totalPlaybackTime;

protected:
    bool setCurrentIndex(int newIndex) override;

public:
    VideoContext();
    ~VideoContext();
    
    std::optional<wxString> getVideoByIndex(int index);
    long long       getTotalPlaybackTimeInMiliSecond();
    std::string     getTotalPlaybackTimeString();
    int     getListSize() override;
    int     getCurrentIndex() override;
    bool    next() override;
    bool    previous() override;
    bool    reset(wxString);
    void    addVideo(wxFileName *);
    bool    getIsPlaying();
    void    setIsPlaying(bool);
    int     getVolume();
    void    setVolume(int);
    void    setTotalPlaybackTime(long long);
};