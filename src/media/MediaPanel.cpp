#include "media/MediaPanel.h"
#include "media/ImageContext.h"
#include "chitr/ChitrLogger.h"
#include <vector>
#include <wx/filename.h>
#include <wx/dir.h>
#include <wx/wx.h>
#include <wx/file.h>

std::vector<CFile *> MediaPanel::GetFilesInDirectory(const wxString &dirPath)
{
    std::vector<CFile *> fileList;
    wxDir directory(dirPath);
    if (directory.IsOpened()){

        wxString filename;
        bool hasFiles = directory.GetFirst(&filename, wxEmptyString, wxDIR_FILES);
        while (hasFiles)
        {
            CFile *file = new CFile(dirPath, filename, wxPATH_NATIVE);
            if(context->supportedFormats.count(file->getExt()) > 0) {
                fileList.push_back(file);
            }
            hasFiles = directory.GetNext(&filename);
        }
    }
    return fileList;
}

void MediaPanel::setBaseContext(std::shared_ptr<MediaContext> newContext) {
        context = newContext;
}

const std::vector<wxString> MediaPanel::getStatusBarData() const {
    const std::vector<wxString> temp = context->getMetaData();
    LOG_INFO("Get Status Bar Data : %s, %s, %s", temp[0], temp[1], temp[2]);
    return context->getMetaData(); 
}

wxPanel *MediaPanel::getRootPanel() const { return rootPanel; }