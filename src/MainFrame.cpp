#include "chitr/MainFrame.h"
#include "media/ImagePanel.h"
#include "media/VideoPanel.h"
#include "chitr/Resource.h"
#include "chitr/ChitrLogger.h"
#include <wx/stdpaths.h>
#include <wx/file.h>
#include <wx/fontenum.h>
#include <wx/filename.h>
#include <wx/wx.h>
#include <wx/icon.h>

MainFrame::MainFrame(const wxString &title) : wxFrame(NULL, wxID_ANY, title)
{
   
    LoadEmbeddedFont();
    LoadImageHandlers();
    LoadAppIcon();

    assets = std::make_shared<Resource>();
    


    notebook        = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_TOP);

    
    MediaPanel      *imagePanel = new ImagePanel(this, notebook, assets);
    MediaPanel      *videoPanel = new VideoPanel(this, notebook, assets);
    
    mainSizer       = new wxBoxSizer(wxVERTICAL);

    // notebook->SetForegroundColour(*wxBLACK);
    
    notebook->AddPage(imagePanel->getRootPanel(), "Image Player");
    notebook->AddPage(videoPanel->getRootPanel(), "Video Player");

    mainSizer->Add(notebook, 1, wxEXPAND);
    SetSizerAndFit(mainSizer);


    // wxFrame::CreateStatusBar(4,wxSTB_DEFAULT_STYLE,wxID_ANY,"Status");
    // wxFrame::SetStatusText("IMG_0012.png", 0);
    // wxFrame::SetStatusText("25 MB", 1);
    // wxFrame::SetStatusText("12-12-2002", 2);

    // wxToolBar *toolBar = wxFrame::CreateToolBar(wxTB_BOTTOM, -1, "Folders");
    // wxButton *fileButton = new wxButton(this, wxID_ANY, "Files", wxDefaultPosition, wxSize(-1, -1), wxBU_EXACTFIT | wxBU_NOTEXT | wxBORDER_NONE);
    // // wxToolBarToolBase *fileToolElement = toolBar->CreateTool(fileButton, "Files Help");
    // toolBar->AddControl(fileButton);
    // toolBar->Realize();

    SetClientSize(800, 700);
}

void MainFrame::LoadEmbeddedFont() {
    wxString exeDir = wxFileName(wxStandardPaths::Get().GetExecutablePath()).GetPath();
    wxString fontPath = exeDir + "/Fonts/material_v1.ttf";

    if (!wxFileName::FileExists(fontPath)) {
        LOG_ERROR("Font file missing at: %s", fontPath);
        return;
    }

    bool iconFontLoaded = wxFont::AddPrivateFont(fontPath);
    if( iconFontLoaded )
    {
        LOG_INFO("Icon Font loaded Successfully");
    }
    else{
        LOG_ERROR("Icon Font Could Not Be loaded");
    }
}

void MainFrame::LoadImageHandlers() {

    wxImage::AddHandler(new wxPNGHandler);
    wxImage::AddHandler(new wxJPEGHandler);
    wxImage::AddHandler(new wxGIFHandler);
    wxImage::AddHandler(new wxPCXHandler);
    wxImage::AddHandler(new wxPNMHandler);
    wxImage::AddHandler(new wxTIFFHandler);
    wxImage::AddHandler(new wxXPMHandler);

    LOG_INFO("Image Handlers Loaded Successfully");
}

void MainFrame::LoadAppIcon() {
    wxString exePath = wxStandardPaths::Get().GetExecutablePath();
    wxString exeDir = wxFileName(exePath).GetPath();

    wxString iconPath = exeDir + "/Assets/MainIcon.png";

    if (wxFileName::FileExists(iconPath)) {
        wxIcon appIcon;
        appIcon.LoadFile(iconPath, wxBITMAP_TYPE_PNG);
        
        this->SetIcon(appIcon);
        LOG_INFO("App icon loaded from: %s", iconPath);
    } else {
        LOG_ERROR("Could not find icon at: %s", iconPath);
    }
}