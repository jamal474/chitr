#pragma once
#include <vector>
#include <optional>
#include <wx/wx.h>
#include "chitr/CFile.h"
#include "media/MediaContext.h"

class ImageContext : public MediaContext {

private:
    bool slideShowFlag;

public:
    ImageContext();
    ~ImageContext();
    
    std::optional<wxString> getImage() const;
    const std::optional<CFile *> getImageFile() const;
    void    addImage(CFile *);
    bool    getSlideShowFlag() const;
    void    setSlideShowFlag(const bool& );
    
};