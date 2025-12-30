#include "media/ImagePanel.h"
#include "media/ImageContext.h"
#include "chitr/ChitrLogger.h"
#include <memory>
#include <vector>
#include <chrono>
#include <wx/filename.h>
#include <wx/dir.h>
#include <wx/wx.h>
#include <wx/notebook.h>
#include <wx/event.h>
#include <wx/file.h>

ImagePanel::ImagePanel(wxFrame *mFrame, wxNotebook *notebook, std::shared_ptr<Resource> resourceAsset) {
    
    try {
        rootPanel   = new wxPanel(notebook);
        assets      = (resourceAsset);
        context     = new ImageContext();
        mainFrame   = mFrame;
        init();
        setSizers();
    } catch(const std::exception &exc) {
        LOG_ERROR("Error occured while initializing Image Panel | %s", exc.what());
    }

    try {
        setBindings();
    } catch(const std::exception &exc) {
    LOG_ERROR("Error registering Callbacks for Image Panel | %s", exc.what());
    }
}

void ImagePanel::init() {
    
    visualPanel         = new wxPanel(rootPanel, wxID_ANY);
    controlPanel        = new wxPanel(rootPanel, wxID_ANY);
    imageViewer         = new wxStaticBitmap(visualPanel, wxID_ANY, assets->getImageIcon());

    rootSizer           = new wxBoxSizer(wxVERTICAL);
    visualSizer         = new wxBoxSizer(wxHORIZONTAL);
    controlSizer        = new wxBoxSizer(wxHORIZONTAL);
    
    uploadButton        = new wxButton(controlPanel, wxID_ANY, "Upload Image", wxDefaultPosition, wxSize(-1, -1), wxBU_EXACTFIT | wxBU_NOTEXT | wxBORDER_NONE);
    nextButton          = new wxButton(controlPanel, wxID_ANY, "Next Image", wxDefaultPosition, wxSize(-1, -1), wxBU_EXACTFIT | wxBU_NOTEXT | wxBORDER_NONE);
    previousButton      = new wxButton(controlPanel, wxID_ANY, "Prev Image", wxDefaultPosition, wxSize(-1, -1), wxBU_EXACTFIT | wxBU_NOTEXT | wxBORDER_NONE);
    slideShowButton     = new wxButton(controlPanel, wxID_ANY, "Slideshow", wxDefaultPosition, wxSize(-1, -1), wxBU_EXACTFIT | wxBU_NOTEXT | wxBORDER_NONE);

    slideShowFlag       = false;
    //slideshowflag, slideshowtimer init left

    LOG_INFO("ImagePanel Initialization completed");
}

void ImagePanel::setSizers() {

    controlPanel->SetBackgroundColour(assets->getPrimaryColour());
    visualPanel->SetBackgroundColour(assets->getPrimaryColour());

    uploadButton->SetBitmap(assets->getUploadIcon());
    nextButton->SetBitmap(assets->getNextIcon());
    previousButton->SetBitmap(assets->getPreviousIcon());
    slideShowButton->SetBitmap(assets->getSlideshowIcon());

    visualSizer->AddStretchSpacer(1);
    visualSizer->Add(imageViewer, 1, wxEXPAND | wxALL, 5);
    visualSizer->AddStretchSpacer(1);

    controlSizer->AddStretchSpacer(1);
    controlSizer->Add(previousButton, 0, wxALL | wxCENTER, 5);
    controlSizer->Add(uploadButton, 0, wxALL | wxCENTER, 5);
    controlSizer->Add(nextButton, 0, wxALL | wxCENTER, 5);
    controlSizer->Add(slideShowButton, 0, wxALL | wxCENTER, 5);
    controlSizer->AddStretchSpacer(1);

    controlPanel->SetSizer(controlSizer);
    controlSizer->SetSizeHints(mainFrame);
    visualPanel->SetSizer(visualSizer);
    visualSizer->SetSizeHints(mainFrame);

    rootSizer->Add(visualPanel, 2, wxEXPAND | wxALL, 0);
    rootSizer->Add(controlPanel, 0, wxEXPAND | wxALL, 0);
    rootPanel->SetSizer(rootSizer);
    rootSizer->SetSizeHints(mainFrame);

    rootPanel->Layout();

    LOG_INFO("ImagePanel Sizer Configuration Completed");
}

void ImagePanel::setBindings() {
    
    uploadButton->Bind(wxEVT_BUTTON, &ImagePanel::uploadHandler, this);
    nextButton->Bind(wxEVT_BUTTON, &ImagePanel::nextHandler, this);
    previousButton->Bind(wxEVT_BUTTON, &ImagePanel::previousHandler, this);
    slideShowTimer.Bind(wxEVT_TIMER, &ImagePanel::slideshowHandler, this);
    slideShowButton->Bind(wxEVT_BUTTON, &ImagePanel::slideshowOpenClose, this);
}

void ImagePanel::uploadHandler(wxCommandEvent &event) {

    LOG_INFO("Image Upload Handler Invoked");
    wxFileDialog openFileDialog(rootPanel, "Upload Image", "", "",
                                "JPEG files (*.jpg;*.jpeg)|*.jpg;*.jpeg|"
                                "PNG files (*.png)|*.png|"
                                "GIF Files (*.gif)|*.gif|"
                                "TIFF files (*.tiff;*.tif)|*.tiff;*.tif|"
                                "PCX files (*.pcx)|*.pcx|"
                                "PNM files (*.pnm)|*.pnm|"
                                "XPM files (*.xpm)|*.xpm",
                                wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_OK) {
        wxString imageFilePath = openFileDialog.GetPath();
        wxFileName directory(imageFilePath);
        wxString directoryPath = directory.GetPath();

        std::vector<wxFileName *> files = GetFilesInDirectory(directoryPath);
    
        LOG_INFO("Found %d [Image/Video] Files in %s", files.size(), directoryPath);
        
        for (wxFileName *file : files){
            context->addImage(file);    
        }
        context->reset(imageFilePath);

        std::optional<wxString> currentImageFilePath = context->getImageByIndex(context->getCurrentIndex());
        if(currentImageFilePath.has_value()) {
            updateImageViewer(currentImageFilePath.value());
        } else {
            LOG_ERROR("Failed to get Current Image from Context for Directory %s", directoryPath);
        }        
    }
}

void ImagePanel::updateImageViewer(wxString imageFilePath) {

    wxBitmap tempBitmap;
    tempBitmap.LoadFile(imageFilePath, wxBITMAP_TYPE_ANY);
    imageViewer->SetBitmap(tempBitmap);
    visualPanel->SetMinSize(wxSize(tempBitmap.GetWidth(), tempBitmap.GetHeight()));
    visualPanel->Layout();
    rootPanel->Layout();
    imageViewer->Refresh();
    LOG_INFO("Updated Media Player with %s", imageFilePath);
}

void ImagePanel::slideshowOpenClose(wxCommandEvent &event) {

    slideShowFlag = slideShowFlag ? false : true;
    if (slideShowFlag) {
        LOG_INFO("Slideshow Start");
        slideShowTimer.Start(2000);
        slideShowButton->SetBitmap(assets->getCloseIcon());
    } else {
        LOG_INFO("Slideshow End");
        slideShowTimer.Stop();
        slideShowButton->SetBitmap(assets->getSlideshowIcon());
    }
}

void ImagePanel::slideshowHandler(wxTimerEvent &event) {

    if(!context->next()) {
        if(!context->reset()){
            LOG_ERROR("Could Not Reset Index to 0");
            return;
        }
    }
    std::optional<wxString> currentImageFilePath = context->getImageByIndex(context->getCurrentIndex());
    if(currentImageFilePath.has_value()) {
        updateImageViewer(currentImageFilePath.value());
    } else {
        LOG_ERROR("Failed to get Current Image from Context");
    }
}

void ImagePanel::nextHandler(wxCommandEvent &event) {

    LOG_INFO("Next Video Handler Invoked");
    if(context->next()) {
        std::optional<wxString> currentImageFilePath = context->getImageByIndex(context->getCurrentIndex());
        if(currentImageFilePath.has_value()) {
            updateImageViewer(currentImageFilePath.value());
        } else {
            LOG_ERROR("Failed to get Current Image from Context");
        }
    }
}

void ImagePanel::previousHandler(wxCommandEvent &event) {

    LOG_INFO("Previous Image Handler Invoked");
    if(context->previous()) {
        std::optional<wxString> currentImageFilePath = context->getImageByIndex(context->getCurrentIndex());
        if(currentImageFilePath.has_value()) {
            updateImageViewer(currentImageFilePath.value());
        } else {
            LOG_ERROR("Failed to get Current Image from Context");
        }
    }
}

std::vector<wxFileName *> ImagePanel::GetFilesInDirectory(const wxString &dirPath) {

    std::vector<wxFileName *> fileList;
    wxDir directory(dirPath);

    if (directory.IsOpened()) {

        wxString filename;
        bool hasFiles = directory.GetFirst(&filename, wxEmptyString, wxDIR_FILES);
        while (hasFiles) {

            wxFileName *file = new wxFileName(dirPath, filename, wxPATH_NATIVE);
            if(context->supportedFormats.count(file->GetExt()) > 0){
                fileList.push_back(file);
            }
            hasFiles = directory.GetNext(&filename);
        }
    }
    return fileList;
}

wxPanel *ImagePanel::getRootPanel()
{
    return rootPanel;
}