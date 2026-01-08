#pragma once
#include <wx/wx.h>
#include <wx/event.h>
#include <functional>
#include <memory>
#include "chitr/Resource.h"
#include "media/MediaContext.h"
#include "chitr/CFile.h"

class MainFrame;

enum {
    ID_OFFSET_ALPHA = wxID_HIGHEST,
    ID_OFFSET_NUM = wxID_HIGHEST + 1000,
    ID_ARROW_UP = wxID_HIGHEST + 2000,
    ID_ARROW_DOWN,
    ID_ARROW_LEFT,
    ID_ARROW_RIGHT,
    ID_ALT_ARROW_LEFT,
    ID_ALT_ARROW_RIGHT,
    ID_SPACE = wxID_HIGHEST + 3000,
    ID_ENTER
};

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