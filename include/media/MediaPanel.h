#pragma once
#include <wx/wx.h>
#include <wx/event.h>
#include <functional>
#include <memory>
#include "chitr/Resource.h"
#include "media/MediaContext.h"

class MediaPanel {

protected :
    virtual std::vector<wxFileName *> GetFilesInDirectory(const wxString& dirPath) = 0;
    virtual void init() = 0;
    virtual void setSizers() = 0;
    virtual void setBindings() = 0;

    wxFrame                     *mainFrame;
    wxPanel                     *rootPanel;
    wxPanel                     *visualPanel;
    wxPanel                     *controlPanel;
    wxBoxSizer                  *rootSizer;
    wxBoxSizer                  *visualSizer;
    wxBoxSizer                  *controlSizer;
    std::shared_ptr<Resource>   assets;

public :
    virtual wxPanel *getRootPanel() = 0;

};