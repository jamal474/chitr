#include <wx/string.h>
#include <wx/filename.h>
#include <wx/datetime.h>
#include "chitr/ChitrLogger.h"
#include "chitr/CFile.h"
#include <cstdio>
#include <cstdint>
#include <string>
#include <vector>
#include <cmath>

CFile::CFile(const wxString& fullPath) : file(fullPath) {
    refresh(true); 
}

CFile::CFile(const wxString& dir, const wxString& name, const wxPathFormat &format) : file(dir, name, format) {
    refresh(true);
}

bool CFile::refresh(bool forceUpdate = false) {

    if (!file.FileExists()) {
        LOG_INFO("File Does not Exist");
        formattedSize = "";
        sizeBytes = 0;
        return true;
    }
    wxDateTime currentModTime = file.GetModificationTime();
    if (!forceUpdate && lastKnownModTime.IsValid() && currentModTime == lastKnownModTime) {
        return false;
    }
    sizeBytes = file.GetSize();
    formattedSize = wxFileName::GetHumanReadableSize(sizeBytes);
    wxDateTime dt0, dt1, dt2;
    if (file.GetTimes(&dt0, &dt1, &dt2)) {
        creationDateStr = dt2.FormatISODate();
        LOG_INFO("Creation Date Found : %s", dt2.FormatISODate());
    } else {
        creationDateStr = "";
        LOG_INFO("Creation Date Failed to fetch");
    }
    lastKnownModTime = currentModTime;
    return true;
}

std::string CFile::calculateSize(uint64_t size) {   

    uint64_t multiplier = exbibytes;
    char buffer[64];

    for (int i = 0; i < DIM(sizes); i++, multiplier /= 1024) {   
        if (size < multiplier) continue;
        if (size % multiplier == 0) {
            std::sprintf(buffer, "%llu %s", size / multiplier, sizes[i]);
        } else {
            std::sprintf(buffer, "%.2f %s", (double) size / multiplier, sizes[i]);
        }
        return std::string(buffer);
    }
    return std::string("0 B");
}

wxString CFile::getName() const { return file.GetName(); }
wxString CFile::getSizeString() const { return formattedSize; }
wxString CFile::getDateString() const { return creationDateStr; }
wxString CFile::getExt() const { return file.GetExt(); }
wxString CFile::getPath() const {return file.GetFullPath(); }
bool     CFile::doExist() const {return file.FileExists(); }
const wxFileName& CFile::getFileNameObj() const { return file; }