#pragma once
#include <optional>
#include <wx/wx.h>
#include <wx/filename.h>
#include <media/MediaContext.h>

class ImageContext : public MediaContext {

protected:
    bool setCurrentIndex(int newIndex) override;

public:
    ImageContext();
    std::optional<wxString> getImageByIndex(int index);
    int     getListSize() override;
    int     getCurrentIndex() override;
    bool    next() override;
    bool    previous() override;
    bool    reset();
    bool    reset(wxString);
    void    addImage(wxFileName *);
    
};