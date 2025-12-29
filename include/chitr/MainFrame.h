#pragma once
#include <wx/wx.h>
#include <wx/notebook.h>
#include "Resource.h"

class MainFrame : public wxFrame {

private:

    void LoadEmbeddedFont();
    void LoadImageHandlers();
    void LoadAppIcon();

    wxNotebook                   *notebook;
    wxBoxSizer                   *mainSizer;
    std::shared_ptr<Resource>    assets;

public:
    MainFrame(const wxString &title); 

};

