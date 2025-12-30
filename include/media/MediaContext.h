#pragma once
#include <wx/wx.h>
#include <wx/filename.h>
#include <vector>
#include <unordered_set>

class MediaContext {
    
protected:
    virtual bool      setCurrentIndex(int newIndex) = 0;

    std::vector<wxFileName *>       mediaList;
    int                             currentIndex;

public:
    virtual int       getListSize() = 0;
    virtual int       getCurrentIndex() = 0;
    virtual bool      next() = 0;
    virtual bool      previous() = 0;
    std::unordered_set<wxString>    supportedFormats;
};