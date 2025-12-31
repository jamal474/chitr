#include "media/VideoPanel.h"
#include "chitr/ChitrLogger.h"
#include "media/VideoContext.h"
#include "chitr/Resource.h"
#include <memory>
#include <vector>
#include <optional>
#include <wx/filename.h>
#include <wx/dir.h>
#include <wx/wx.h>
#include <wx/mediactrl.h>
#include <wx/notebook.h>
#include <wx/event.h>
#include <wx/mediactrl.h>
#include <wx/file.h>
#include <wx/filedlg.h>
#include <wx/filefn.h>

VideoPanel::VideoPanel(wxFrame *mFrame, wxNotebook *notebook, std::shared_ptr<Resource> resourceAsset) {
    
    try {
        rootPanel   = new wxPanel(notebook);
        assets      = (resourceAsset);
        context     = new VideoContext();
        mainFrame   = mFrame;
        init();
        setSizers();
    } catch(const std::exception &exc) {
        LOG_ERROR("Error occured while initializing Video Panel | %s", exc.what());
    }

    try {
        setBindings();
    } catch(const std::exception &exc) {
        LOG_ERROR("Error registering Callbacks for Video Panel | %s", exc.what());
    }

    setCursors();
    setToolTips();
    volumeSlider->SetValue(100);
    controlPanel->SetFocus();
}

void VideoPanel::init() {
    
    visualPanel         = new wxPanel(rootPanel, wxID_ANY);
    controlPanel        = new wxPanel(rootPanel, wxID_ANY);
    mediaPlayer         = new wxMediaCtrl(visualPanel, wxID_ANY);

    rootSizer           = new wxBoxSizer(wxVERTICAL);
    visualSizer         = new wxBoxSizer(wxHORIZONTAL);
    controlSizer        = new wxBoxSizer(wxHORIZONTAL);
    
    uploadButton        = new wxButton(controlPanel, wxID_ANY, "Upload Video/Audio", wxDefaultPosition, wxSize(-1, -1), wxBU_EXACTFIT | wxBU_NOTEXT | wxBORDER_NONE);
    playPauseButton     = new wxButton(controlPanel, wxID_ANY, "Play", wxDefaultPosition, wxSize(-1, -1), wxBU_EXACTFIT | wxBU_NOTEXT | wxBORDER_NONE); 
    previousButton      = new wxButton(controlPanel, wxID_ANY, "Previous Video", wxDefaultPosition, wxSize(-1, -1), wxBU_EXACTFIT | wxBU_NOTEXT | wxBORDER_NONE);
    stopButton          = new wxButton(controlPanel, wxID_ANY, "Stop", wxDefaultPosition, wxSize(-1, -1), wxBU_EXACTFIT | wxBU_NOTEXT | wxBORDER_NONE);
    nextButton          = new wxButton(controlPanel, wxID_ANY, "Next Video", wxDefaultPosition, wxSize(-1, -1), wxBU_EXACTFIT | wxBU_NOTEXT | wxBORDER_NONE);
    volumeSlider        = new wxGauge(controlPanel, wxID_ANY, 100, wxDefaultPosition, wxSize(70,2), wxGA_HORIZONTAL | wxGA_SMOOTH);
    volumeButton        = new wxButton(controlPanel, wxID_ANY, "Volume Video", wxDefaultPosition, wxSize(-1, -1), wxBU_EXACTFIT | wxBU_NOTEXT | wxBORDER_NONE);

    LOG_INFO("VideoPanel Initialization completed");
}

void VideoPanel::setSizers() {

    visualPanel->SetBackgroundColour(*wxBLACK);
    controlPanel->SetBackgroundColour(assets->getPrimaryColour());

    volumeButton->SetBitmap(assets->getVolumeIcon());
    uploadButton->SetBitmap(assets->getUploadIcon());
    playPauseButton->SetBitmap(assets->getPlayIcon());
    previousButton->SetBitmap(assets->getPreviousIcon());
    stopButton->SetBitmap(assets->getStopIcon());
    nextButton->SetBitmap(assets->getNextIcon());

    visualSizer->AddStretchSpacer(1);
    visualSizer->Add(mediaPlayer, 4, wxEXPAND | wxALL, 5);
    visualSizer->AddStretchSpacer(1);

    controlSizer->Add(uploadButton, 0, wxCENTER | wxALL, 5);
    controlSizer->AddStretchSpacer(1);
    controlSizer->Add(playPauseButton, 0, wxCENTER | wxALL, 5);
    controlSizer->AddSpacer(6);
    controlSizer->Add(previousButton, 0, wxCENTER | wxALL, 5);
    controlSizer->Add(stopButton, 0, wxCENTER | wxALL, 5);
    controlSizer->Add(nextButton, 0, wxCENTER | wxALL, 5);
    controlSizer->AddStretchSpacer(1);
    controlSizer->Add(volumeButton, 0, wxEXPAND | wxALL, 5);
    controlSizer->Add(volumeSlider, 0, wxEXPAND | wxALL, 5);
    
    controlPanel->SetSizer(controlSizer);
    controlSizer->SetSizeHints(mainFrame);

    visualPanel->SetSizer(visualSizer);
    visualSizer->SetSizeHints(mainFrame);

    
    rootSizer->Add(visualPanel, 2, wxEXPAND | wxALL, 0);
    rootSizer->Add(controlPanel, 0, wxEXPAND | wxALL, 0);
    rootPanel->SetSizer(rootSizer);
    rootSizer->SetSizeHints(mainFrame);

    
    rootSizer->Layout();

    LOG_INFO("VideoPanel Sizer Configuration Completed");
}

void VideoPanel::setBindings() {  

    uploadButton->Bind(wxEVT_BUTTON, &VideoPanel::uploadHandler, this);
    playPauseButton->Bind(wxEVT_BUTTON, &VideoPanel::playPauseHandler, this);
    stopButton->Bind(wxEVT_BUTTON, &VideoPanel::stopHandler, this);
    nextButton->Bind(wxEVT_BUTTON, &VideoPanel::nextHandler, this);
    previousButton->Bind(wxEVT_BUTTON, &VideoPanel::previousHandler, this);
    mediaPlayer->Bind(wxEVT_MEDIA_LOADED, &VideoPanel::playPauseHandler, this);
    volumeSlider->Bind(wxEVT_LEFT_DOWN, &VideoPanel::volumeHandler, this);
    volumeSlider->Bind(wxEVT_MOTION, &VideoPanel::volumeHandler, this);
    volumeButton->Bind(wxEVT_BUTTON, &VideoPanel::muteHandler, this);
}

void VideoPanel::setCursors() {

    uploadButton->SetCursor(wxCursor(wxCURSOR_HAND));
    playPauseButton->SetCursor(wxCursor(wxCURSOR_HAND));
    stopButton->SetCursor(wxCursor(wxCURSOR_HAND));
    nextButton->SetCursor(wxCursor(wxCURSOR_HAND));
    previousButton->SetCursor(wxCursor(wxCURSOR_HAND));
    volumeButton->SetCursor(wxCursor(wxCURSOR_HAND));
    volumeSlider->SetCursor(wxCursor(wxCURSOR_HAND));
}

void VideoPanel::setToolTips() {

    uploadButton->SetToolTip("Upload");
    playPauseButton->SetToolTip("Play");
    stopButton->SetToolTip("Stop");
    nextButton->SetToolTip("Next");
    previousButton->SetToolTip("Previous");
    volumeButton->SetToolTip("Mute");
    volumeSlider->SetToolTip(std::to_string(context->getVolume()) + "%");
}

void VideoPanel::uploadHandler(wxCommandEvent &event) {

    LOG_INFO("Video Upload Handler Invoked");
    wxFileDialog openFileDialog(rootPanel, "Open Video/Audio file", "", "",
                                "Media files (*.mp4;*.avi;*.mp3)|*.mp4;*.avi;*.mp3",
                                wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_OK) {

        wxString videoFilePath = openFileDialog.GetPath();
        wxFileName directory(videoFilePath);
        wxString directoryPath = directory.GetPath();

        std::vector<wxFileName *> files = GetFilesInDirectory(directoryPath);

        LOG_INFO("Found %d [Image/Video] Files in %s", files.size(), directoryPath);

        for (wxFileName *file : files) {
            context->addVideo(file);
        }
        context->reset(videoFilePath);

        std::optional<wxString> currentVideoFilePath = context->getVideoByIndex(context->getCurrentIndex());
        if(currentVideoFilePath.has_value()) {
            updateMediaPlayer(currentVideoFilePath.value());
        } else {
            LOG_ERROR("Failed to get Current Video from Context for Directory %s", directoryPath);
        }
        
    }
}

void VideoPanel::updateMediaPlayer(wxString videoFilePath) {

    context->setIsPlaying(false);
    mediaPlayer->Load(videoFilePath);
    LOG_INFO("Time in Milisecond : %d",mediaPlayer->Length());
    LOG_INFO("Updated Media Player with %s", videoFilePath);
}

void VideoPanel::playPauseHandler(wxCommandEvent &event) {
    LOG_INFO("Time in Milisecond : %d",(long long)mediaPlayer->Length());
    if (mediaPlayer->Tell() == 0 || !context->getIsPlaying()) {
        mediaPlayer->Play();
        context->setIsPlaying(true);
        playPauseButton->SetBitmap(assets->getPauseIcon());
    } else {
        mediaPlayer->Pause();
        context->setIsPlaying(false);
        playPauseButton->SetBitmap(assets->getPlayIcon());
    }
}

void VideoPanel::stopHandler(wxCommandEvent &event) {

    context->setIsPlaying(false);
    mediaPlayer->Stop();

    playPauseButton->SetBitmap(assets->getPlayIcon());
    LOG_INFO("Playback Stopped");
}

void VideoPanel::nextHandler(wxCommandEvent &event) {

    LOG_INFO("Next Video Handler Invoked");
    if(context->next()) {
        std::optional<wxString> currentVideoFilePath = context->getVideoByIndex(context->getCurrentIndex());
        if(currentVideoFilePath.has_value()) {
            updateMediaPlayer(currentVideoFilePath.value());
        } else {
            LOG_ERROR("Failed to get Current Video from Context");
        }
    }
}

void VideoPanel::previousHandler(wxCommandEvent &event) {

    LOG_INFO("Previous Video Handler Invoked");
    if(context->previous()) {
        std::optional<wxString> currentVideoFilePath = context->getVideoByIndex(context->getCurrentIndex());
        if(currentVideoFilePath.has_value()) {
            updateMediaPlayer(currentVideoFilePath.value());
        } else {
            LOG_ERROR("Failed to get Current Video from Context");
        }
    }
}

void VideoPanel::volumeHandler(wxMouseEvent& event) {

    if (event.LeftIsDown()) {
        int width = volumeSlider->GetSize().GetWidth();
        int x = event.GetX();
        int value = (x * volumeSlider->GetRange()) / width;
        
        if (value < 0) value = 0;
        if (value > volumeSlider->GetRange()) value = volumeSlider->GetRange();
        
        if (value == 0) {
            wxCommandEvent newEvent(wxEVT_BUTTON, event.GetId());
            muteHandler(newEvent);
        }
        
        context->setVolume(value);
        volumeSlider->SetValue(context->getVolume());
        mediaPlayer->SetVolume(double(context->getVolume())/100.0);  
    }
}

std::vector<wxFileName *> VideoPanel::GetFilesInDirectory(const wxString &dirPath)
{
    std::vector<wxFileName *> fileList;
    wxDir directory(dirPath);
    if (directory.IsOpened()){

        wxString filename;
        bool hasFiles = directory.GetFirst(&filename, wxEmptyString, wxDIR_FILES);
        while (hasFiles)
        {
            wxFileName *file = new wxFileName(dirPath, filename, wxPATH_NATIVE);
            if(context->supportedFormats.count(file->GetExt()) > 0){
                fileList.push_back(file);
            }
            hasFiles = directory.GetNext(&filename);
        }
    }
    return fileList;
}

wxPanel *VideoPanel::getRootPanel()
{
    return rootPanel;
}

void VideoPanel::muteHandler(wxCommandEvent &event) {

    bool isSliderVisible = volumeSlider->IsShown();
    if (isSliderVisible) {
        volumeButton->SetBitmap(assets->getMuteIcon());
        volumeSlider->Show(false);
        mediaPlayer->SetVolume(0.0);  
    } else {
        volumeButton->SetBitmap(assets->getVolumeIcon());
        volumeSlider->Show(true);
        mediaPlayer->SetVolume(double(context->getVolume())/100.0);  
    }
    LOG_INFO("Mute Handler Invoked");
    
} 
