#pragma once
#include <memory>
#include <vector>
#include <wx/wx.h>
#include <wx/notebook.h>
#include <wx/statusbr.h>
#include "chitr/Resource.h"
// (removed stray `#include "vector"` — replaced by <vector> above)

class MediaPanel;

class MainFrame : public wxFrame {

private:

    void LoadEmbeddedFont();
    void LoadImageHandlers();
    void LoadAppIcon();
    void notebookChangeHandler(wxBookCtrlEvent& event);

    wxNotebook                   *notebook;
    wxBoxSizer                   *mainSizer;
    wxStatusBar                  *statusBar;
    std::shared_ptr<Resource>    assets;
    std::vector<MediaPanel *>    pages;

public:
    MainFrame(const wxString &title); 
    void setStatusBarText(const std::vector<wxString> &statusBarData) ;
};

