#include "media/ImagePanel.h"
#include "media/ImageContext.h"
#include "chitr/ChitrLogger.h"
#include "chitr/CFile.h"
#include "chitr/MainFrame.h"
#include <memory>
#include <vector>
#include <chrono>
#include <future>
#include <wx/filename.h>
#include <wx/dir.h>
#include <wx/wx.h>
#include <wx/notebook.h>
#include <wx/event.h>
#include <wx/file.h>
#include <thread>

ImagePanel::ImagePanel(MainFrame *mFrame, wxNotebook *notebook, std::shared_ptr<Resource> resourceAsset) {
    
    try {
        rootPanel   = new wxPanel(notebook);
        assets      = (resourceAsset);

        std::shared_ptr<ImageContext> sharedCtx = std::make_shared<ImageContext>();
        setBaseContext(sharedCtx);
        context = sharedCtx.get();

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

ImagePanel::~ImagePanel() {
    
    if (slideShowTimer) {
        slideShowTimer->Stop(); 
        slideShowTimer->Unbind(wxEVT_TIMER, &ImagePanel::slideshowHandler, this);
        
        delete slideShowTimer;
        slideShowTimer = nullptr;
    }

    if (context) {
        delete context;
        context = nullptr;
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
    slideShowTimer      = new wxTimer();
    
    LOG_INFO("ImagePanel Initialization completed");
}

void ImagePanel::setSizers() {

    controlPanel->SetBackgroundColour(assets->getPrimaryColour());
    visualPanel->SetBackgroundColour(assets->getPrimaryColour());

    #ifdef _WIN32
        wxColour bgColour = assets->getPrimaryColour();
        uploadButton->SetBackgroundColour(bgColour);
        nextButton->SetBackgroundColour(bgColour);
        previousButton->SetBackgroundColour(bgColour);
        slideShowButton->SetBackgroundColour(bgColour);
    #endif

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
    // controlSizer->SetSizeHints(mainFrame);

    visualPanel->SetSizer(visualSizer);
    // visualSizer->SetSizeHints(mainFrame);

    rootSizer->Add(visualPanel, 2, wxEXPAND | wxALL, 0);
    rootSizer->Add(controlPanel, 0, wxEXPAND | wxALL, 0);
    rootPanel->SetSizer(rootSizer);
    // rootSizer->SetSizeHints(mainFrame);

    rootPanel->Layout();

    LOG_INFO("ImagePanel Sizer Configuration Completed");
}

void ImagePanel::setBindings() {
    
    rootPanel->Bind(wxEVT_DESTROY, &ImagePanel::OnWindowDestroy, this);
    uploadButton->Bind(wxEVT_BUTTON, &ImagePanel::uploadHandler, this);
    nextButton->Bind(wxEVT_BUTTON, &ImagePanel::nextHandler, this);
    previousButton->Bind(wxEVT_BUTTON, &ImagePanel::previousHandler, this);
    slideShowTimer->Bind(wxEVT_TIMER, &ImagePanel::slideshowHandler, this);
    slideShowButton->Bind(wxEVT_BUTTON, &ImagePanel::slideshowOpenClose, this);
    visualPanel->Bind(wxEVT_SIZE, &ImagePanel::visualPanelResizeHandler, this);

    std::vector<wxAcceleratorEntry> acceleratorEntry = getAcceleratorEntries();
    wxAcceleratorTable acceleratorTable(acceleratorEntry.size(), acceleratorEntry.data());
    rootPanel->SetAcceleratorTable(acceleratorTable);
    rootPanel->Bind(wxEVT_MENU, &ImagePanel::alphaPressHandler, this, ID_OFFSET_ALPHA, ID_OFFSET_ALPHA + 25);
    rootPanel->Bind(wxEVT_MENU, &ImagePanel::numPressHandler, this, ID_OFFSET_NUM, ID_OFFSET_NUM + 9);
    rootPanel->Bind(wxEVT_MENU, &ImagePanel::keyPressHandler, this, ID_ARROW_UP);
    rootPanel->Bind(wxEVT_MENU, &ImagePanel::keyPressHandler, this, ID_ARROW_DOWN);
    rootPanel->Bind(wxEVT_MENU, &ImagePanel::keyPressHandler, this, ID_ARROW_LEFT);
    rootPanel->Bind(wxEVT_MENU, &ImagePanel::keyPressHandler, this, ID_ARROW_RIGHT);
    rootPanel->Bind(wxEVT_MENU, &ImagePanel::keyPressHandler, this, ID_SPACE);
    rootPanel->Bind(wxEVT_MENU, &ImagePanel::keyPressHandler, this, ID_ENTER);
}

std::vector<wxAcceleratorEntry> ImagePanel::getAcceleratorEntries() {

    std::vector<wxAcceleratorEntry> entries;
    entries.push_back(wxAcceleratorEntry(wxACCEL_NORMAL, 'O', ID_OFFSET_ALPHA + int('O' - 'A')));
    entries.push_back(wxAcceleratorEntry(wxACCEL_NORMAL, WXK_UP,   ID_ARROW_UP));
    entries.push_back(wxAcceleratorEntry(wxACCEL_NORMAL, WXK_DOWN, ID_ARROW_DOWN));
    entries.push_back(wxAcceleratorEntry(wxACCEL_NORMAL, WXK_LEFT, ID_ARROW_LEFT));
    entries.push_back(wxAcceleratorEntry(wxACCEL_NORMAL, WXK_RIGHT, ID_ARROW_RIGHT));
    entries.push_back(wxAcceleratorEntry(wxACCEL_NORMAL, WXK_SPACE, ID_SPACE));

    return entries;
}

void ImagePanel::setCursors() {

    uploadButton->SetCursor(wxCursor(wxCURSOR_HAND));
    nextButton->SetCursor(wxCursor(wxCURSOR_HAND));
    previousButton->SetCursor(wxCursor(wxCURSOR_HAND));
    slideShowButton->SetCursor(wxCursor(wxCURSOR_HAND));
}

void ImagePanel::setToolTips() {

    uploadButton->SetToolTip("Upload");
    nextButton->SetToolTip("Next");
    previousButton->SetToolTip("Previous");
    slideShowButton->SetToolTip("Slideshow");
}

void ImagePanel::uploadHandler(wxCommandEvent &event) {

    LOG_INFO("Image Upload Handler Invoked");
    wxFileDialog openFileDialog(rootPanel, "Upload Image", "", "",
                                "All Image Files|*.jpg;*.jpeg;*.JPG;*.JPEG;*.png;*.PNG;*.gif;*.GIF;*.tiff;*.TIFF;*.tif;*.TIF;*.pcx;*.PCX;*.pnm;*.PNM;*.xpm;*.XPM|"
                                "JPEG files (*.jpg;*.jpeg;*.JPG;*.JPEG)|*.jpg;*.jpeg;*.JPG;*.JPEG|"
                                "PNG files (*.png;*.PNG)|*.png;*.PNG|"
                                "GIF Files (*.gif;*.GIF)|*.gif;*.GIF|"
                                "TIFF files (*.tiff;*.tif;*.TIFF;*.TIF)|*.tiff;*.TIFF;*.tif;*.TIF|"
                                "PCX files (*.pcx;*.PCX)|*.pcx;*.PCX|"
                                "PNM files (*.pnm;*.PNM)|*.pnm;*.PNM|"
                                "XPM files (*.xpm;*.XPM)|*.xpm;*.XPM",
                                wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_OK) {
        wxString imageFilePath = openFileDialog.GetPath();
        wxFileName directory(imageFilePath);
        wxString directoryPath = directory.GetPath();
        wxString selectedFileName = directory.GetFullName();

        CFile *selectedFile = new CFile(imageFilePath);
        context->addImage(selectedFile);
        context->reset();

        std::optional<wxString> currentImageFilePath = context->getImage();
        if(currentImageFilePath.has_value()) {
            updateImageViewer(currentImageFilePath.value());
        } else {
            LOG_ERROR("Failed to get Current Image from Context for Directory %s", directoryPath);
        } 
        
        std::thread([this, directoryPath, selectedFileName]() {
            
            std::vector<CFile *> files = this->GetFilesInDirectory(directoryPath, selectedFileName);
            wxTheApp->CallAfter([this, files, directoryPath]() {
                
                LOG_INFO("Found %d [Image/Video] Files in %s", files.size(), directoryPath);
                for (CFile *file : files){
                    context->addImage(file);
                }
            });

        }).detach();
    }
}

void ImagePanel::visualPanelResizeHandler(wxSizeEvent& event) {
    renderImage(); 
    event.Skip();
}
void ImagePanel::updateImageViewer(wxString imageFilePath) {

   visualPanel->Freeze(); 

    if (!currentRawImage.LoadFile(imageFilePath, wxBITMAP_TYPE_ANY)) {
        LOG_ERROR("Failed to load image file: %s", imageFilePath);
        visualPanel->Thaw();
        return;
    }
    renderImage();
    visualPanel->Thaw();

    mainFrame->setStatusBarText(getStatusBarData());
    LOG_INFO("Updated Media Player with %s", imageFilePath);
}

void ImagePanel::renderImage() {
    if (!currentRawImage.IsOk()) return;

    int panelW, panelH;
    visualPanel->GetClientSize(&panelW, &panelH);

    if (panelW <= 0 || panelH <= 0) return;

    int imgW = currentRawImage.GetWidth();
    int imgH = currentRawImage.GetHeight();

    double scaleX = (double)panelW / imgW;
    double scaleY = (double)panelH / imgH;
    double scale = std::min(scaleX, scaleY);

    int newW = (int)(imgW * scale);
    int newH = (int)(imgH * scale);

    if (newW > 0 && newH > 0) {
        wxImage scaledImg = currentRawImage.Scale(newW, newH, wxIMAGE_QUALITY_HIGH );
        imageViewer->SetBitmap(wxBitmap(scaledImg));
    }
    visualPanel->Layout(); 
}

void ImagePanel::slideshowOpenClose(wxCommandEvent &event) {

    bool slideShowOpen = context->getSlideShowFlag();
    if (!slideShowOpen) {
        LOG_INFO("Slideshow Start");
        context->setSlideShowFlag(true);
        slideShowTimer->Start(2000);
        slideShowButton->SetBitmap(assets->getCloseIcon());
    } else {
        LOG_INFO("Slideshow End");
        context->setSlideShowFlag(false);
        slideShowTimer->Stop();
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
    std::optional<wxString> currentImageFilePath = context->getImage();
    if(currentImageFilePath.has_value()) {
        updateImageViewer(currentImageFilePath.value());
    } else {
        LOG_ERROR("Failed to get Current Image from Context");
    }
}

void ImagePanel::nextHandler(wxCommandEvent &event) {

    LOG_INFO("Next Video Handler Invoked");
    if(context->next()) {
        std::optional<wxString> currentImageFilePath = context->getImage();
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
        std::optional<wxString> currentImageFilePath = context->getImage();
        if(currentImageFilePath.has_value()) {
            updateImageViewer(currentImageFilePath.value());
        } else {
            LOG_ERROR("Failed to get Current Image from Context");
        }
    }
}

void ImagePanel::alphaPressHandler(wxCommandEvent& event) {

    char alphabetPressed = 'A' + (event.GetId() - ID_OFFSET_ALPHA);
    switch (alphabetPressed) {
        case 'O': {
            dispatchEvent(&ImagePanel::uploadHandler);
            break;
        }
        default: {
            LOG_INFO("Alphabet Key Event Raised : %c [Not Handled]", alphabetPressed);
            break;
        }
    }
}

void ImagePanel::numPressHandler(wxCommandEvent& event) {

    int numberPressed = event.GetId() - ID_OFFSET_NUM;
    LOG_INFO("Number Key Event Raised : %d", numberPressed);
}

void ImagePanel::keyPressHandler(wxCommandEvent& event) {
    
    int arrowId = event.GetId();
    switch (arrowId) {
        case ID_ARROW_UP: {
            //zoom
            break;
        }
        case ID_ARROW_DOWN: {
            //zoom
            break;
        }
        case ID_ARROW_LEFT: {
            dispatchEvent(&ImagePanel::nextHandler);
            break;
        }
        case ID_ARROW_RIGHT: {
            dispatchEvent(&ImagePanel::previousHandler);
            break;
        }
        case ID_SPACE: {
            dispatchEvent(&ImagePanel::slideshowHandler);
            break;
        }
    }
}

void ImagePanel::OnWindowDestroy(wxWindowDestroyEvent& event) {

    if (event.GetEventObject() == rootPanel) {
        LOG_INFO("Image RootPanel is being destroyed - cleaning up UI references");

        if (slideShowTimer) {
            slideShowTimer->Stop();
        }

        rootPanel->Unbind(wxEVT_DESTROY, &ImagePanel::OnWindowDestroy, this);
    }
    event.Skip();
}
