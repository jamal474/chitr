#pragma once
#include <optional>
#include <wx/wx.h>
#include <wx/filename.h>
#include <media/MediaContext.h>

class VideoContext : public MediaContext {

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
};