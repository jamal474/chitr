#include "media/VideoPanel.h"
#include "chitr/ChitrLogger.h"
#include "media/VideoContext.h"
#include "chitr/Resource.h"
#include "component/RoundedText.h"
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
#include <wx/accel.h>

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

VideoPanel::~VideoPanel() {
    
    if (playbackTimer) {
        playbackTimer->Stop(); 
        playbackTimer->Unbind(wxEVT_TIMER, &VideoPanel::playbackLengthHandler, this);
        
        delete playbackTimer;
        playbackTimer = nullptr;
    }

    if (context) {
        delete context;
        context = nullptr;
    }
}

void VideoPanel::init() {
    
    visualPanel         = new wxPanel(rootPanel, wxID_ANY);
    controlPanel        = new wxPanel(rootPanel, wxID_ANY);
    mediaPlayer         = new wxMediaCtrl(visualPanel, wxID_ANY);

    rootSizer                   = new wxBoxSizer(wxVERTICAL);
    visualSizer                 = new wxBoxSizer(wxHORIZONTAL);
    controlSizer                = new wxBoxSizer(wxVERTICAL);
    controlPanelButtonRowSizer  = new wxBoxSizer(wxHORIZONTAL);
    controlPanelPBSliderSizer   = new wxBoxSizer(wxHORIZONTAL);

    uploadButton        = new wxButton(controlPanel, wxID_ANY, "Upload Video/Audio", wxDefaultPosition, wxSize(-1, -1), wxBU_EXACTFIT | wxBU_NOTEXT | wxBORDER_NONE);
    playPauseButton     = new wxButton(controlPanel, wxID_ANY, "Play", wxDefaultPosition, wxSize(-1, -1), wxBU_EXACTFIT | wxBU_NOTEXT | wxBORDER_NONE); 
    previousButton      = new wxButton(controlPanel, wxID_ANY, "Previous Video", wxDefaultPosition, wxSize(-1, -1), wxBU_EXACTFIT | wxBU_NOTEXT | wxBORDER_NONE);
    nextButton          = new wxButton(controlPanel, wxID_ANY, "Next Video", wxDefaultPosition, wxSize(-1, -1), wxBU_EXACTFIT | wxBU_NOTEXT | wxBORDER_NONE);
    volumeSlider        = new wxGauge(controlPanel, wxID_ANY, 100, wxDefaultPosition, wxSize(70,2), wxGA_HORIZONTAL | wxGA_SMOOTH);
    volumeButton        = new wxButton(controlPanel, wxID_ANY, "Volume Video", wxDefaultPosition, wxSize(-1, -1), wxBU_EXACTFIT | wxBU_NOTEXT | wxBORDER_NONE);
    playbackSlider      = new wxGauge(controlPanel, wxID_ANY, 10000, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL | wxGA_SMOOTH);
    playbackTimer       = new wxTimer();
    playbackTimeText    = new RoundedText(controlPanel, wxID_ANY, "00:00 / 00:00", assets->getSecondaryColour(), *wxWHITE, 20.0);

    LOG_INFO("VideoPanel Initialization completed");
}

void VideoPanel::setSizers() {

    visualPanel->SetBackgroundColour(*wxBLACK);
    controlPanel->SetBackgroundColour(assets->getPrimaryColour());

    volumeButton->SetBitmap(assets->getVolumeIcon());
    uploadButton->SetBitmap(assets->getUploadIcon());
    playPauseButton->SetBitmap(assets->getPlayIcon());
    previousButton->SetBitmap(assets->getPreviousIcon());
    nextButton->SetBitmap(assets->getNextIcon());

    visualSizer->AddStretchSpacer(1);
    visualSizer->Add(mediaPlayer, 4, wxEXPAND | wxALL, 5);
    visualSizer->AddStretchSpacer(1);

    controlPanelButtonRowSizer->AddStretchSpacer(1);
    controlPanelButtonRowSizer->Add(playPauseButton, 0, wxCENTER | wxALL, 5);
    controlPanelButtonRowSizer->Add(playbackTimeText, 0, wxCENTER | wxALL, 5);
    controlPanelButtonRowSizer->AddStretchSpacer(15);
    controlPanelButtonRowSizer->Add(previousButton, 0, wxCENTER | wxALL, 5);
    controlPanelButtonRowSizer->Add(uploadButton, 0, wxCENTER | wxALL, 5);
    controlPanelButtonRowSizer->Add(nextButton, 0, wxCENTER | wxALL, 5);
    controlPanelButtonRowSizer->AddStretchSpacer(15);
    controlPanelButtonRowSizer->Add(volumeButton, 0, wxEXPAND | wxALL, 5);
    controlPanelButtonRowSizer->Add(volumeSlider, 0, wxEXPAND | wxALL, 5);
    controlPanelButtonRowSizer->AddStretchSpacer(1);
    
    controlPanelPBSliderSizer->AddStretchSpacer(1); 
    controlPanelPBSliderSizer->Add(playbackSlider, 38, wxEXPAND | wxALL, 0);
    controlPanelPBSliderSizer->AddStretchSpacer(1);

    controlSizer->Add(controlPanelPBSliderSizer, 0, wxEXPAND | wxTOP, 10); 
    controlSizer->Add(controlPanelButtonRowSizer, 0, wxEXPAND | wxBOTTOM, 5);

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

    rootPanel->Bind(wxEVT_DESTROY, &VideoPanel::OnWindowDestroy, this);
    uploadButton->Bind(wxEVT_BUTTON, &VideoPanel::uploadHandler, this);
    playPauseButton->Bind(wxEVT_BUTTON, &VideoPanel::playPauseHandler, this);
    nextButton->Bind(wxEVT_BUTTON, &VideoPanel::nextHandler, this);
    previousButton->Bind(wxEVT_BUTTON, &VideoPanel::previousHandler, this);
    mediaPlayer->Bind(wxEVT_MEDIA_LOADED, &VideoPanel::mediaLoadedHandler, this);
    mediaPlayer->Bind(wxEVT_MEDIA_STOP, &VideoPanel::mediaEndedHandler, this);
    volumeSlider->Bind(wxEVT_LEFT_DOWN, &VideoPanel::volumeHandler, this);
    volumeSlider->Bind(wxEVT_MOTION, &VideoPanel::volumeHandler, this);
    volumeButton->Bind(wxEVT_BUTTON, &VideoPanel::muteHandler, this);
    playbackSlider->Bind(wxEVT_LEFT_DOWN, &VideoPanel::seekHandler, this);
    playbackSlider->Bind(wxEVT_MOTION, &VideoPanel::seekHandler, this);
    playbackTimer->Bind(wxEVT_TIMER, &VideoPanel::playbackLengthHandler, this);

    std::vector<wxAcceleratorEntry> acceleratorEntry = getAcceleratorEntries();
    wxAcceleratorTable acceleratorTable(acceleratorEntry.size(), acceleratorEntry.data());
    rootPanel->SetAcceleratorTable(acceleratorTable);
    rootPanel->Bind(wxEVT_MENU, &VideoPanel::alphaPressHandler, this, ID_OFFSET_ALPHA, ID_OFFSET_ALPHA + 25);
    rootPanel->Bind(wxEVT_MENU, &VideoPanel::numPressHandler, this, ID_OFFSET_NUM, ID_OFFSET_NUM + 9);
    rootPanel->Bind(wxEVT_MENU, &VideoPanel::keyPressHandler, this, ID_ARROW_UP);
    rootPanel->Bind(wxEVT_MENU, &VideoPanel::keyPressHandler, this, ID_ARROW_DOWN);
    rootPanel->Bind(wxEVT_MENU, &VideoPanel::keyPressHandler, this, ID_ARROW_LEFT);
    rootPanel->Bind(wxEVT_MENU, &VideoPanel::keyPressHandler, this, ID_ARROW_RIGHT);
    rootPanel->Bind(wxEVT_MENU, &VideoPanel::keyPressHandler, this, ID_SPACE);
    rootPanel->Bind(wxEVT_MENU, &VideoPanel::keyPressHandler, this, ID_ENTER);
}

std::vector<wxAcceleratorEntry> VideoPanel::getAcceleratorEntries() {

    std::vector<wxAcceleratorEntry> entries;
    entries.push_back(wxAcceleratorEntry(wxACCEL_NORMAL, 'M', ID_OFFSET_ALPHA + int('M' - 'A')));
    entries.push_back(wxAcceleratorEntry(wxACCEL_NORMAL, 'O', ID_OFFSET_ALPHA + int('O' - 'A')));

    for (int i = 0; i <= 9; ++i) {
        int key = '0' + i; 
        int cmdId = ID_OFFSET_NUM + i;
        entries.push_back(wxAcceleratorEntry(wxACCEL_NORMAL, key, cmdId));
    }
    entries.push_back(wxAcceleratorEntry(wxACCEL_NORMAL, WXK_UP,   ID_ARROW_UP));
    entries.push_back(wxAcceleratorEntry(wxACCEL_NORMAL, WXK_DOWN, ID_ARROW_DOWN));
    entries.push_back(wxAcceleratorEntry(wxACCEL_NORMAL, WXK_LEFT, ID_ARROW_LEFT));
    entries.push_back(wxAcceleratorEntry(wxACCEL_NORMAL, WXK_RIGHT, ID_ARROW_RIGHT));
    entries.push_back(wxAcceleratorEntry(wxACCEL_NORMAL, WXK_RETURN, ID_ENTER));
    entries.push_back(wxAcceleratorEntry(wxACCEL_NORMAL, WXK_SPACE, ID_SPACE));

    return entries;
}

void VideoPanel::setCursors() {

    uploadButton->SetCursor(wxCursor(wxCURSOR_HAND));
    playPauseButton->SetCursor(wxCursor(wxCURSOR_HAND));
    nextButton->SetCursor(wxCursor(wxCURSOR_HAND));
    previousButton->SetCursor(wxCursor(wxCURSOR_HAND));
    volumeButton->SetCursor(wxCursor(wxCURSOR_HAND));
    volumeSlider->SetCursor(wxCursor(wxCURSOR_HAND));
    playbackSlider->SetCursor(wxCursor(wxCURSOR_HAND));
}

void VideoPanel::setToolTips() {

    uploadButton->SetToolTip("Upload");
    playPauseButton->SetToolTip("Play");
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
    LOG_INFO("Updated Media Player with %s", videoFilePath);
}

void VideoPanel::mediaLoadedHandler(wxCommandEvent &event) {

    context->setTotalPlaybackTime((long long)mediaPlayer->Length());
    wxCommandEvent newEvent(wxEVT_BUTTON, event.GetId());
    playPauseHandler(newEvent);
    playbackTimer->Start(30);
}

void VideoPanel::mediaEndedHandler(wxCommandEvent &event) {

    wxCommandEvent newEvent(wxEVT_BUTTON, event.GetId());
    playPauseHandler(newEvent);
    playbackTimer->Stop();
}

void VideoPanel::playPauseHandler(wxCommandEvent &event) {

    if (!context->getIsPlaying()) {
        if (mediaPlayer->Play()) {
            LOG_INFO("Playing Media");
        } else {
            
        } 
        context->setIsPlaying(true);
        playbackTimer->Start(30);
        playPauseButton->SetBitmap(assets->getPauseIcon());
    } else {
        mediaPlayer->Pause();
        context->setIsPlaying(false);
        playbackTimer->Stop();
        playPauseButton->SetBitmap(assets->getPlayIcon());
    }
}

void VideoPanel::playbackLengthHandler(wxTimerEvent &event) {

    if (!context || !playbackTimer || !mediaPlayer || !playbackSlider || !playbackTimeText) {
        return;
    }

    long long totalMiliseconds = context->getTotalPlaybackTimeInMiliSecond();
    if (totalMiliseconds <= 0) return; 

    long long timeCompletedMiliseconds = mediaPlayer->Tell();
    std::string totalTime = context->getTotalPlaybackTimeString(); 
    int percentDone = (timeCompletedMiliseconds * 10000 / totalMiliseconds);
    std::string timeCompleted = CTime::timeString(timeCompletedMiliseconds) + " / " + totalTime;
     
    if (playbackSlider) playbackSlider->SetValue(percentDone);
    if (playbackTimeText) playbackTimeText->SetLabel(timeCompleted);
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

void VideoPanel::seekHandler(wxMouseEvent& event) {
    if (event.LeftIsDown()) {
        int width = playbackSlider->GetSize().GetWidth();
        int x = event.GetX();
        int value = (x * playbackSlider->GetRange()) / width;
        
        if (value < 0) value = 0;
        if (value > playbackSlider->GetRange()) value = playbackSlider->GetRange();
        
        long long timeToSeek = (context->getTotalPlaybackTimeInMiliSecond() * value)/10000;

        playbackSlider->SetValue(value);
        mediaPlayer->Seek(timeToSeek);
        
        if(!context->getIsPlaying()) {
            wxCommandEvent newEvent(wxEVT_BUTTON, event.GetId());
            playPauseHandler(newEvent);
        }
        LOG_INFO("Seek Handler Called for Time : %s", CTime::timeString(timeToSeek));
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

void VideoPanel::alphaPressHandler(wxCommandEvent& event) {

    char alphabetPressed = 'A' + (event.GetId() - ID_OFFSET_ALPHA);
    switch (alphabetPressed) {
        case 'M': {
            wxCommandEvent newEvent(wxEVT_BUTTON, event.GetId());
            muteHandler(newEvent);
            LOG_INFO("Alphabet Key Event Raised : M [Mute]");
            break;
        }
        default: {
            LOG_INFO("Alphabet Key Event Raised : %c [Not Handled]", alphabetPressed);
            break;
        }
    }
}

void VideoPanel::numPressHandler(wxCommandEvent& event) {

    int numberPressed = event.GetId() - ID_OFFSET_NUM;
    LOG_INFO("Number Key Event Raised : %d", numberPressed);
}

void VideoPanel::keyPressHandler(wxCommandEvent& event) {
    
    int arrowId = event.GetId();
    switch (arrowId) {
        case ID_ARROW_UP: {

            break;
        }
        case ID_ARROW_DOWN: {

            break;
        }
        case ID_ARROW_LEFT: {

            break;
        }
        case ID_ARROW_RIGHT: {

            break;
        }
        case ID_SPACE:
        case ID_ENTER: {
            wxCommandEvent newEvent(wxEVT_BUTTON, event.GetId());
            playPauseHandler(newEvent);
            break;
        }
    }
}

void VideoPanel::OnWindowDestroy(wxWindowDestroyEvent& event) {

    if (event.GetEventObject() == rootPanel) {
        LOG_INFO("RootPanel is being destroyed - cleaning up UI references");

        if (playbackTimer) {
            playbackTimer->Stop();
        }

        playbackSlider = nullptr;
        volumeSlider = nullptr;
        playbackTimeText = nullptr;
        mediaPlayer = nullptr;
        
        rootPanel->Unbind(wxEVT_DESTROY, &VideoPanel::OnWindowDestroy, this);
    }
    event.Skip();
}




