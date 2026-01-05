#pragma once
#include <wx/wx.h>
#include "chitr/CFile.h"
#include <vector>
#include <optional>
#include <unordered_set>

class MediaContext {

protected:
    MediaContext() : currentIndex(-1) {}
    virtual ~MediaContext() = default; 
    
    bool                         setCurrentIndex(const int &newIndex);
    std::optional<wxString>      getMediaByIndex(const int &index) const;
    const std::optional<CFile *> getMediaFileByIndex(const int &index) const ;
    void                         addMedia(CFile *);

    std::vector<CFile *>         mediaList;
    int                          currentIndex;

public:
    bool                         reset(const wxString &filePath = wxEmptyString);
    int                          getListSize() const;
    int                          getCurrentIndex() const;
    bool                         next();
    bool                         previous();
    const std::vector<wxString>  getMetaData() const;
    std::unordered_set<wxString> supportedFormats;
    
};