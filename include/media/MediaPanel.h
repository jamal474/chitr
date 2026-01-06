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

private :
    std::shared_ptr<MediaContext> context;

protected :
    std::vector<CFile *> GetFilesInDirectory(const wxString& dirPath, const wxString &);
    void setBaseContext(std::shared_ptr<MediaContext> newContext);

    template <typename T, typename EventType>
    void dispatchEvent(void (T::*handler)(EventType&), int id = wxID_ANY) {
        EventType event;
        event.SetId(id);
        (static_cast<T*>(this)->*handler)(event);
    }

    virtual void OnWindowDestroy(wxWindowDestroyEvent& event) = 0;
    virtual std::vector<wxAcceleratorEntry> getAcceleratorEntries() = 0;
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
    wxPanel *getRootPanel() const;
    const std::vector<wxString> getStatusBarData() const;

};