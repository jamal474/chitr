#include "chitr/RootFrame.h"
#include "chitr/ImagePlayback.h"
#include "chitr/VideoPlayback.h"
#include "chitr/Resource.h"
#include "chitr/ChitrLogger.h"
#include "materialDesign/font.h" 
#include <wx/wx.h>
#include <wx/filename.h>
#include <wx/dir.h>
#include <wx/notebook.h>
#include <wx/event.h>
#include <wx/mediactrl.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <wx/stdpaths.h>
#include <wx/file.h>
#include <wx/fontenum.h>

RootFrame::RootFrame(const wxString &title) : wxFrame(NULL, wxID_ANY, title),
                                              slideShowFlag { false }
{
    // set application icon displayed on left of title bar
    LoadEmbeddedFont();
    
    // 2. Initialize assets explicitly AFTER font is loaded
    assets = std::make_unique<Resource>();
    // this->SetIcon(wxICON(photo));

    /**
     * make notebook for tab traversal
     * initialize image page widgets : initImagePage()
     * initialize video page widgets : initVideoPage()
     * set all the event handler bindings : SetBindings()
     */
    notebook        = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_TOP);
    imgPlayBack     = new ImagePlayback();
    vidPlayBack     = new VideoPlayback();
    imagePagePanel  = new wxPanel(notebook);
    videoPagePanel  = new wxPanel(notebook);
    mainSizer       = new wxBoxSizer(wxVERTICAL);

    // notebook->SetForegroundColour(*wxBLACK);
    // notebook->SetBackgroundColour(assets.headerBackgroundColour);
    notebook->AddPage(imagePagePanel, "Image Player");
    notebook->AddPage(videoPagePanel, "Music Player");

    initImagePage();
    initVideoPage();

    mainSizer->Add(notebook, 1, wxEXPAND);
    SetBindings();
    SetSizerAndFit(mainSizer);


    wxFrame::CreateStatusBar(4,wxSTB_DEFAULT_STYLE,wxID_ANY,"Status");
    wxFrame::SetStatusText("IMG_0012.png", 0);
    wxFrame::SetStatusText("25 MB", 1);
    wxFrame::SetStatusText("12-12-2002", 2);

    // wxToolBar *toolBar = wxFrame::CreateToolBar(wxTB_BOTTOM, -1, "Folders");
    // wxButton *fileButton = new wxButton(this, wxID_ANY, "Files", wxDefaultPosition, wxSize(-1, -1), wxBU_EXACTFIT | wxBU_NOTEXT | wxBORDER_NONE);
    // // wxToolBarToolBase *fileToolElement = toolBar->CreateTool(fileButton, "Files Help");
    // toolBar->AddControl(fileButton);
    // toolBar->Realize();

    SetClientSize(800, 700);
}


void RootFrame::LoadEmbeddedFont() {
    wxString exeDir = wxFileName(wxStandardPaths::Get().GetExecutablePath()).GetPath();
    wxString fontPath = exeDir + "/Fonts/material_v1.ttf";

    if (!wxFileName::FileExists(fontPath)) {
        LOG_ERROR("Font file missing at: %s", fontPath.ToStdString());
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

void RootFrame::initImagePage()
{
    /**
     * this initializes all the GUI elements in the image section
     * 
     * imagePageSizer : parent page layout sizer for the whole page
     *      Panels : Image Panel : currentImagePanel | image uses this as parent
     *               Image Control Panel : imageControlPanel | all control buttons use this as parent
     *      Sizer : Both panels have sizers for them
     * 
     */
    imagePageSizer      = new wxBoxSizer(wxVERTICAL);
    imageControlPanel   = new wxPanel(imagePagePanel);
    currentImagePanel   = new wxPanel(imagePagePanel);
    imageControlSizer   = new wxBoxSizer(wxHORIZONTAL);
    currentImageSizer   = new wxBoxSizer(wxHORIZONTAL);
    imageView           = new wxStaticBitmap(currentImagePanel, wxID_ANY, assets->imageIcon);
    prevButton          = new wxButton(imageControlPanel, ID::PREV_IMAGE_BUTTON, "Prev Image", wxDefaultPosition, wxSize(-1, -1), wxBU_EXACTFIT | wxBU_NOTEXT | wxBORDER_NONE);
    uploadImageButton   = new wxBitmapButton(imageControlPanel, ID::UPLOAD_IMAGE_BUTTON, assets->uploadIcon, wxDefaultPosition, wxSize(-1, -1), wxBU_EXACTFIT | wxBU_NOTEXT | wxNO_BORDER); // wxBORDER_NONE
    nextButton          = new wxButton(imageControlPanel, ID::NEXT_IMAGE_BUTTON, "Next Image", wxDefaultPosition, wxSize(-1, -1), wxBU_EXACTFIT | wxBU_NOTEXT | wxBORDER_NONE);
    slideShowButton     = new wxButton(imageControlPanel, ID::SLIDESHOW_BUTTON, "Slideshow", wxDefaultPosition, wxSize(-1, -1), wxBU_EXACTFIT | wxBU_NOTEXT | wxBORDER_NONE);

    // set Primary Colour and Secondary Colour
    // Panels are coloured here
    imageControlPanel->SetBackgroundColour(assets->footerBackgroundColour);
    currentImagePanel->SetBackgroundColour(assets->footerBackgroundColour);

    // set the button icons
    // uploadImageButton->SetBitmap(assets->uploadIcon);
    nextButton->SetBitmap(assets->nextIcon);
    prevButton->SetBitmap(assets->previousIcon);
    slideShowButton->SetBitmap(assets->slideshowIcon);

    // set the layout setting for ImagePanel
    currentImageSizer->AddStretchSpacer(1);
    currentImageSizer->Add(imageView, 1, wxEXPAND | wxALL, 5);
    currentImageSizer->AddStretchSpacer(1);

    // set the layout setting for Image Control Panel
    imageControlSizer->AddStretchSpacer(1);
    imageControlSizer->Add(prevButton, 0, wxALL | wxCENTER, 5);
    imageControlSizer->Add(uploadImageButton, 0, wxALL | wxCENTER, 5);
    imageControlSizer->Add(nextButton, 0, wxALL | wxCENTER, 5);
    imageControlSizer->Add(slideShowButton, 0, wxALL | wxCENTER, 5);
    imageControlSizer->AddStretchSpacer(1);

    // fix panel with their sizers
    imageControlPanel->SetSizer(imageControlSizer);
    imageControlSizer->SetSizeHints(this);
    currentImagePanel->SetSizer(currentImageSizer);
    currentImageSizer->SetSizeHints(this);

    // add the panels to the parent page layout sizer
    imagePageSizer->Add(currentImagePanel, 2, wxEXPAND | wxALL, 0);
    imagePageSizer->Add(imageControlPanel, 0, wxEXPAND | wxALL, 0);
    imagePagePanel->SetSizer(imagePageSizer);
    imagePageSizer->SetSizeHints(this);

    //refresh the layout
    imagePagePanel->Layout();
}

void RootFrame::initVideoPage()
{
    /**
     * this initializes all the GUI elements in the video section
     * 
     * videoPageSizer : parent page layout sizer for the whole page
     *      Panels : Video Panel : currentVideoPanel | mediaCtrl uses this as parent
     *               Video Control Panel : videoControlPanel | all control buttons use this as parent
     *      Sizer : Both panels have sizers for them
     * 
     */
    videoPageSizer          = new wxBoxSizer(wxVERTICAL);
    currentVideoPanel       = new wxPanel(videoPagePanel, wxID_ANY);
    videoControlPanel       = new wxPanel(videoPagePanel, wxID_ANY);
    currentVideoSizer       = new wxBoxSizer(wxHORIZONTAL);
    videoControlSizer       = new wxBoxSizer(wxHORIZONTAL);
    videoView               = new wxMediaCtrl(currentVideoPanel, wxID_ANY);
    uploadButton            = new wxButton(videoControlPanel, wxID_ANY, "Upload Video/Audio", wxDefaultPosition, wxSize(-1, -1), wxBU_EXACTFIT | wxBU_NOTEXT | wxBORDER_NONE);
    playPauseButton         = new wxButton(videoControlPanel, wxID_ANY, "Play", wxDefaultPosition, wxSize(-1, -1), wxBU_EXACTFIT | wxBU_NOTEXT | wxBORDER_NONE); 
    previousVideoButton     = new wxButton(videoControlPanel, wxID_ANY, "Previous Video", wxDefaultPosition, wxSize(-1, -1), wxBU_EXACTFIT | wxBU_NOTEXT | wxBORDER_NONE);
    stopButton              = new wxButton(videoControlPanel, wxID_ANY, "Stop", wxDefaultPosition, wxSize(-1, -1), wxBU_EXACTFIT | wxBU_NOTEXT | wxBORDER_NONE);
    nextVideoButton         = new wxButton(videoControlPanel, wxID_ANY, "Next Video", wxDefaultPosition, wxSize(-1, -1), wxBU_EXACTFIT | wxBU_NOTEXT | wxBORDER_NONE);
    wxStaticBitmap *volIcon = new wxStaticBitmap(videoControlPanel, wxID_ANY, wxBitmap());
    volumeSlider            = new wxSlider(videoControlPanel, wxID_ANY, 100, 0, 100,wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
    isPlaying               = (false);
    volume                  = (1.0);

    // set Primary Colour and Secondary Colour
    // Panels are coloured here
    currentVideoPanel->SetBackgroundColour(assets->footerBackgroundColour);
    videoControlPanel->SetBackgroundColour(assets->footerBackgroundColour);

    // set the button icons
    volIcon->SetBitmap(assets->volumeIcon);
    uploadButton->SetBitmap(assets->uploadIcon);
    playPauseButton->SetBitmap(assets->playIcon);
    previousVideoButton->SetBitmap(assets->previousIcon);
    stopButton->SetBitmap(assets->stopIcon);
    nextVideoButton->SetBitmap(assets->nextIcon);

    // set layout setting for video/music panel
    currentVideoSizer->AddStretchSpacer(1);
    currentVideoSizer->Add(videoView, 4, wxEXPAND | wxALL, 5);
    currentVideoSizer->AddStretchSpacer(1);

    // set layout setting for video control panel
    videoControlSizer->Add(uploadButton, 0, wxCENTER | wxALL, 5);
    videoControlSizer->AddStretchSpacer(1);
    videoControlSizer->Add(playPauseButton, 0, wxCENTER | wxALL, 5);
    videoControlSizer->AddSpacer(6);
    videoControlSizer->Add(previousVideoButton, 0, wxCENTER | wxALL, 5);
    videoControlSizer->Add(stopButton, 0, wxCENTER | wxALL, 5);
    videoControlSizer->Add(nextVideoButton, 0, wxCENTER | wxALL, 5);
    videoControlSizer->AddStretchSpacer(1);
    videoControlSizer->Add(volIcon, 0, wxEXPAND | wxALL, 5);
    videoControlSizer->Add(volumeSlider, 0, wxEXPAND | wxALL, 5);

    // fix panel with their sizers
    videoControlPanel->SetSizer(videoControlSizer);
    videoControlSizer->SetSizeHints(this);
    currentVideoPanel->SetSizer(currentVideoSizer);
    currentVideoSizer->SetSizeHints(this);

    // add the panels to the parent page layout sizer
    videoPageSizer->Add(currentVideoPanel, 2, wxEXPAND | wxALL, 0);
    videoPageSizer->Add(videoControlPanel, 0, wxEXPAND | wxALL, 0);
    videoPagePanel->SetSizer(videoPageSizer);
    videoPageSizer->SetSizeHints(this);

    // refresh the layout
    videoPageSizer->Layout();
}

void RootFrame::SetBindings()
{
    /**
     * Attach the buttons and other widgets with event handlers
     * both the image player and video player have their bindings 
     * here
     */
    // Image Player Event  Bindings
    prevButton->Bind(wxEVT_BUTTON, &RootFrame::previousImageHandler, this);
    uploadImageButton->Bind(wxEVT_BUTTON, &RootFrame::imageUploadHandler, this);
    nextButton->Bind(wxEVT_BUTTON, &RootFrame::nextImageHandler, this);
    slideShowButton->Bind(wxEVT_BUTTON, &RootFrame::slideshowOpenClose, this);
    slideShowTimer.Bind(wxEVT_TIMER, &RootFrame::slideshowHandler, this);

    // Music Player Event Bindings
    uploadButton->Bind(wxEVT_BUTTON, &RootFrame::videoUploadHandler, this);
    playPauseButton->Bind(wxEVT_BUTTON, &RootFrame::playPauseHandler, this);
    previousVideoButton->Bind(wxEVT_BUTTON, &RootFrame::previousVideoHandler, this);
    stopButton->Bind(wxEVT_BUTTON, &RootFrame::stopHandler, this);
    nextVideoButton->Bind(wxEVT_BUTTON, &RootFrame::nextVideoHandler, this);
    volumeSlider->Bind(wxEVT_SLIDER, &RootFrame::volumeChangeHandler, this);
}

void RootFrame::imageUploadHandler(wxCommandEvent &evt)
{
    //prompt to select Image
    wxFileDialog openFileDialog(this, "Upload Image", "", "",
                                "(*.jpg; *.jpeg; *.png)|*.jpg;*.jpeg;*.png",
                                wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_OK)
    {
        //get the file path of the selected file
        wxString imageFilePath = openFileDialog.GetPath();
        wxFileName directory(imageFilePath);
        wxString directoryPath = directory.GetPath();

        //fetch all the files present in the current selected folder
        std::vector<wxString> files = GetFilesInDirectory(directoryPath);
        int index = 0;
        for (wxString &file : files)
        {
            //set the currentIndex for the image that was selected
            imgPlayBack->addImage(file);
            if (imageFilePath == file)
            {
                imgPlayBack->setCurrentIndex(index);
            }

            index++;
        }

        //create and load the image in currentImagePanel
        //and update the Title bar accordingly
        wxBitmap bitmap;
        if (bitmap.LoadFile(imgPlayBack->getImageByIndex(imgPlayBack->getCurrentIndex()), wxBITMAP_TYPE_ANY))
        {
            updateImageViewer(bitmap);
            this->SetTitle("Chitr | " + std::string(wxFileName(imgPlayBack->getImageByIndex(imgPlayBack->getCurrentIndex())).GetFullName().mb_str()));
        }
    }
}

void RootFrame::videoUploadHandler(wxCommandEvent &)
{
    // prompt user to select the video
    wxFileDialog openFileDialog(this, "Open Video/Audio file", "", "",
                                "Media files (*.mp4;*.avi;*.mp3)|*.mp4;*.avi;*.mp3",
                                wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_OK)
    {
        // file path of the selected video
        wxString videoFilePath = openFileDialog.GetPath();
        wxFileName directory(videoFilePath);
        wxString directoryPath = directory.GetPath();

        // get all the files in the current directory
        std::vector<wxString> files = GetFilesInDirectory(directoryPath);
        int index = 0;
        for (wxString &file : files)
        {
            std::cout << file << std::endl;
            vidPlayBack->addVideo(file);
            if (videoFilePath == file)
            {
                vidPlayBack->setCurrentIndex(index);
            }
            index++;
        }

        updateVideoViewer(vidPlayBack->getVideoByIndex(vidPlayBack->getCurrentIndex()));
    }
}

void RootFrame::updateImageViewer(wxBitmap &bitmap)
{
    //update image in the current image panel
    imageView->SetBitmap(bitmap);
    currentImagePanel->SetMinSize(wxSize(bitmap.GetWidth(), bitmap.GetHeight()));

    currentImagePanel->Layout();
    imagePagePanel->Layout();

    imageView->Refresh();
}

void RootFrame::updateVideoViewer(wxString videoFilePath)
{
    //update video in the current video panel
    videoView->Load(videoFilePath);
    this->SetTitle("Chitr | Playing " + std::string(wxFileName(videoFilePath).GetFullName().mb_str()));
    isPlaying = false;
    volume = 1.0;
    videoView->SetVolume(volume);
    volumeSlider->SetValue(50);
}

void RootFrame::previousImageHandler(wxCommandEvent &evt)
{
    /**
     * move the current image index to previous
     * update the title bar
     * update the current image panel
     */
    int tempCurrentIndex = imgPlayBack->getCurrentIndex();
    if (tempCurrentIndex != 0)
    {
        wxBitmap bitmap;
        if (bitmap.LoadFile(imgPlayBack->getImageByIndex(tempCurrentIndex - 1), wxBITMAP_TYPE_ANY))
        {
            updateImageViewer(bitmap);
            imgPlayBack->setCurrentIndex(tempCurrentIndex - 1);

            if(!slideShowFlag)
            {
                this->SetTitle("Chitr | " + std::string(wxFileName(imgPlayBack->getImageByIndex(tempCurrentIndex - 1)).GetFullName().mb_str()));
            }
        }
    }
}

void RootFrame::nextImageHandler(wxCommandEvent &evt)
{
    int tempCurrentIndex = imgPlayBack->getCurrentIndex();
    std::cout << "next called with current at : " << tempCurrentIndex << std::endl;
    if (tempCurrentIndex < imgPlayBack->getNumberOfImages() - 1)
    {
        wxBitmap bitmap;
        if (bitmap.LoadFile(imgPlayBack->getImageByIndex(tempCurrentIndex + 1), wxBITMAP_TYPE_ANY))
        {
            updateImageViewer(bitmap);
            imgPlayBack->setCurrentIndex(tempCurrentIndex + 1);
            if(!slideShowFlag)
            {
                this->SetTitle("Chitr | " + std::string(wxFileName(imgPlayBack->getImageByIndex(tempCurrentIndex + 1)).GetFullName().mb_str()));
            }
        }
    }
}

void RootFrame::slideshowOpenClose(wxCommandEvent &evt)
{
    slideShowFlag = slideShowFlag ? false : true;
    if (slideShowFlag)
    {
        slideShowTimer.Start(2000);
        slideShowButton->SetBitmap(assets->closeIcon);
        this->SetTitle("Chitr | Image Slideshow");
    }
    else
    {
        slideShowTimer.Stop();
        slideShowButton->SetBitmap(assets->slideshowIcon);
        this->SetTitle("Chitr | " + std::string(wxFileName(imgPlayBack->getImageByIndex(imgPlayBack->getCurrentIndex())).GetFullName().mb_str()));
    }
}

void RootFrame::slideshowHandler(wxTimerEvent &evt)
{
    if (imgPlayBack->getCurrentIndex() == imgPlayBack->getNumberOfImages() - 1)
    {
        imgPlayBack->setCurrentIndex(-1);
    }
    wxCommandEvent event(wxEVT_BUTTON, slideShowButton->GetId());
    nextImageHandler(event);
}

std::vector<wxString> RootFrame::GetFilesInDirectory(const wxString &dirPath)
{
    std::vector<wxString> files;
    wxDir directory(dirPath);
    if (directory.IsOpened())
    {
        wxString filename;
        bool hasFiles = directory.GetFirst(&filename, wxEmptyString, wxDIR_FILES);
        while (hasFiles)
        {
            files.push_back(dirPath + wxString("/") + filename);
            hasFiles = directory.GetNext(&filename);
        }
    }
    return files;
}

void RootFrame::playPauseHandler(wxCommandEvent &)
{
    if (videoView->Tell() == 0 || !isPlaying)
    {
        videoView->Play();
        isPlaying = true;
        playPauseButton->SetBitmap(assets->pauseIcon);
    }
    else
    {
        videoView->Pause();
        isPlaying = false;
        playPauseButton->SetBitmap(assets->playIcon);
    }
}

void RootFrame::stopHandler(wxCommandEvent &)
{
    videoView->Stop();
    isPlaying = false;
    playPauseButton->SetBitmap(assets->playIcon);
    volumeSlider->SetValue(100);
    videoView->SetVolume(1.0);
}

void RootFrame::volumeChangeHandler(wxCommandEvent &)
{
    volume = volumeSlider->GetValue() / 100.0;
    videoView->SetVolume(volume);
}

void RootFrame::previousVideoHandler(wxCommandEvent &evt)
{
    int tempCurrentIndex = vidPlayBack->getCurrentIndex();
    if (tempCurrentIndex != 0)
    {
       updateVideoViewer(vidPlayBack->getVideoByIndex(tempCurrentIndex - 1));
       vidPlayBack->setCurrentIndex(tempCurrentIndex - 1);
    }
    // else for when no prev left
}

void RootFrame::nextVideoHandler(wxCommandEvent &evt)
{
    int tempCurrentIndex = vidPlayBack->getCurrentIndex();
    if (tempCurrentIndex != vidPlayBack->getNumberOfVideos() - 1)
    {
       updateVideoViewer(vidPlayBack->getVideoByIndex(tempCurrentIndex + 1));
       vidPlayBack->setCurrentIndex(tempCurrentIndex + 1);
    }
}