#pragma once
#include <optional>
#include <wx/wx.h>
#include <wx/filename.h>
#include <media/MediaContext.h>

class VideoContext : public MediaContext {

private:
    bool            isPlaying;
    int             volume;

protected:
    bool setCurrentIndex(int newIndex) override;

public:
    VideoContext();
    std::optional<wxString> getVideoByIndex(int index);
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
};