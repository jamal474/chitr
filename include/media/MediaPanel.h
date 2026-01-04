#pragma once
#include <wx/wx.h>
#include <wx/event.h>
#include <functional>
#include <memory>
#include "chitr/Resource.h"
#include "media/MediaContext.h"
#include "chitr/CFile.h"

class MainFrame;

#define ID_OFFSET_ALPHA   3000  
#define ID_OFFSET_NUM     4000 
#define ID_ARROW_UP       5001
#define ID_ARROW_DOWN     5002
#define ID_ARROW_LEFT     5003
#define ID_ARROW_RIGHT    5004
#define ID_SPACE          6000
#define ID_ENTER          6001

class MediaPanel {

protected :
    virtual std::vector<CFile *> GetFilesInDirectory(const wxString& dirPath) = 0;
    virtual void init() = 0;
    virtual void setSizers() = 0;
    virtual void setBindings() = 0;
    virtual void setCursors() = 0;
    virtual void setToolTips() = 0;

    MainFrame                   *mainFrame;
    wxPanel                     *rootPanel;
    wxPanel                     *visualPanel;
    wxPanel                     *controlPanel;
    wxBoxSizer                  *rootSizer;
    wxBoxSizer                  *visualSizer;
    wxBoxSizer                  *controlSizer;
    std::shared_ptr<Resource>   assets;

public :
    virtual wxPanel *getRootPanel() const = 0;
    virtual const std::vector<wxString> getStatusBarData() const = 0;

};