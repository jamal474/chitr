#pragma once

#include <wx/wx.h>
#include <wx/datetime.h>
#include <wx/filename.h>
#include <wx/string.h>

static const char *sizes[] = { "EiB", "PiB", "TiB", "GiB", "MiB", "KiB", "B" };
static const uint64_t exbibytes = 1024ULL * 1024ULL * 1024ULL * 1024ULL * 1024ULL * 1024ULL;

#define DIM(x) (sizeof(x)/sizeof(*(x)))

class CFile {

private:
    std::string calculateSize(uint64_t size);
    wxFileName  file;
    wxDateTime  lastKnownModTime;
    wxULongLong sizeBytes;
    wxString    formattedSize;      
    wxString    creationDateStr;

public:
    CFile(const wxString& fullPath);
    CFile(const wxString& dir, const wxString& name, const wxPathFormat &format);

    bool refresh(bool forceUpdate);

    wxString getName() const;
    wxString getSizeString() const;
    wxString getDateString() const;
    wxString getExt() const;
    wxString getPath() const;
    bool     doExist() const;
    const wxFileName& getFileNameObj() const;
};
