#include "media/VideoPanel.h"
#include "chitr/ChitrLogger.h"
#include "media/VideoContext.h"
#include "chitr/Resource.h"
#include "component/RoundedText.h"
#include "chitr/CFile.h"
#include "chitr/MainFrame.h"
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
#include <thread>

VideoPanel::VideoPanel(MainFrame *mFrame, wxNotebook *notebook, std::shared_ptr<Resource> resourceAsset) {
    
    try {
        rootPanel   = new wxPanel(notebook);
        assets      = (resourceAsset);
        
        std::shared_ptr<VideoContext> sharedCtx = std::make_shared<VideoContext>();
        setBaseContext(sharedCtx);
        context = sharedCtx.get();

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

    #ifdef _WIN32
        mediaPlayer     = new wxMediaCtrl(visualPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxMEDIABACKEND_WMP10);
    #else
        mediaPlayer     = new wxMediaCtrl(visualPanel, wxID_ANY);
    #endif

    rootSizer                   = new wxBoxSizer(wxVERTICAL);
    visualSizer                 = new wxBoxSizer(wxHORIZONTAL);
    controlSizer                = new wxBoxSizer(wxVERTICAL);
    controlPanelButtonRowSizer  = new wxBoxSizer(wxHORIZONTAL);
    controlPanelPBSliderSizer   = new wxBoxSizer(wxHORIZONTAL);

    int gaugeHeight = wxWindow::FromDIP(7, (wxWindow*)NULL); 
    int gaugeWidth = wxWindow::FromDIP(70, (wxWindow*)NULL);

    uploadButton        = new wxButton(controlPanel, wxID_ANY, "Upload Video/Audio", wxDefaultPosition, wxSize(-1, -1), wxBU_EXACTFIT | wxBU_NOTEXT | wxBORDER_NONE);
    playPauseButton     = new wxButton(controlPanel, wxID_ANY, "Play", wxDefaultPosition, wxSize(-1, -1), wxBU_EXACTFIT | wxBU_NOTEXT | wxBORDER_NONE); 
    previousButton      = new wxButton(controlPanel, wxID_ANY, "Previous Video", wxDefaultPosition, wxSize(-1, -1), wxBU_EXACTFIT | wxBU_NOTEXT | wxBORDER_NONE);
    nextButton          = new wxButton(controlPanel, wxID_ANY, "Next Video", wxDefaultPosition, wxSize(-1, -1), wxBU_EXACTFIT | wxBU_NOTEXT | wxBORDER_NONE);
    volumeSlider        = new wxGauge(controlPanel, wxID_ANY, VOLUME_RANGE, wxDefaultPosition, wxSize(gaugeWidth, gaugeHeight), wxGA_HORIZONTAL | wxGA_SMOOTH);
    volumeButton        = new wxButton(controlPanel, wxID_ANY, "Volume Video", wxDefaultPosition, wxSize(-1, -1), wxBU_EXACTFIT | wxBU_NOTEXT | wxBORDER_NONE);
    playbackSlider      = new wxGauge(controlPanel, wxID_ANY, PLAYBACK_RANGE, wxDefaultPosition, wxSize(-1,gaugeHeight), wxGA_HORIZONTAL | wxGA_SMOOTH);
    playbackTimer       = new wxTimer();
    playbackTimeText    = new RoundedText(controlPanel, wxID_ANY, "00:00 / 00:00", assets->getSecondaryColour(), *wxWHITE, 20.0);

    LOG_INFO("VideoPanel Initialization completed");
}

void VideoPanel::setSizers() {

    visualPanel->SetBackgroundColour(*wxBLACK);
    controlPanel->SetBackgroundColour(assets->getPrimaryColour());

    #ifdef _WIN32
        wxColour bgColour = assets->getPrimaryColour();
        uploadButton->SetBackgroundColour(bgColour);
        volumeButton->SetBackgroundColour(bgColour);
        nextButton->SetBackgroundColour(bgColour);
        previousButton->SetBackgroundColour(bgColour);
        playPauseButton->SetBackgroundColour(bgColour);
    #endif

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
    controlPanelButtonRowSizer->Add(volumeSlider, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    controlPanelButtonRowSizer->AddStretchSpacer(1);
    
    controlPanelPBSliderSizer->AddStretchSpacer(1); 
    controlPanelPBSliderSizer->Add(playbackSlider, 38, wxEXPAND | wxALL, 0);
    controlPanelPBSliderSizer->AddStretchSpacer(1);

    controlSizer->Add(controlPanelPBSliderSizer, 0, wxEXPAND | wxTOP, 10); 
    controlSizer->Add(controlPanelButtonRowSizer, 0, wxEXPAND | wxBOTTOM, 5);

    controlPanel->SetSizer(controlSizer);
    // controlSizer->SetSizeHints(mainFrame);

    visualPanel->SetSizer(visualSizer);
    // visualSizer->SetSizeHints(mainFrame);

    rootSizer->Add(visualPanel, 2, wxEXPAND | wxALL, 0);
    rootSizer->Add(controlPanel, 0, wxEXPAND | wxALL, 0);
    rootPanel->SetSizer(rootSizer);
    // rootSizer->SetSizeHints(mainFrame);

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
    rootPanel->Bind(wxEVT_MENU, &VideoPanel::keyPressHandler, this, ID_ALT_ARROW_LEFT);
    rootPanel->Bind(wxEVT_MENU, &VideoPanel::keyPressHandler, this, ID_ALT_ARROW_RIGHT);
}

std::vector<wxAcceleratorEntry> VideoPanel::getAcceleratorEntries() {

    std::vector<wxAcceleratorEntry> entries;
    entries.push_back(wxAcceleratorEntry(wxACCEL_NORMAL, 'M', ID_OFFSET_ALPHA + int('M' - 'A')));
    entries.push_back(wxAcceleratorEntry(wxACCEL_NORMAL, 'O', ID_OFFSET_ALPHA + int('O' - 'A')));
    entries.push_back(wxAcceleratorEntry(wxACCEL_NORMAL, 'J', ID_OFFSET_ALPHA + int('J' - 'A')));
    entries.push_back(wxAcceleratorEntry(wxACCEL_NORMAL, 'K', ID_OFFSET_ALPHA + int('K' - 'A')));
    entries.push_back(wxAcceleratorEntry(wxACCEL_NORMAL, 'L', ID_OFFSET_ALPHA + int('L' - 'A')));

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
    entries.push_back(wxAcceleratorEntry(wxACCEL_ALT, WXK_LEFT, ID_ALT_ARROW_LEFT));
    entries.push_back(wxAcceleratorEntry(wxACCEL_ALT, WXK_RIGHT, ID_ALT_ARROW_RIGHT));

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
                                UPLOAD_STRING,
                                wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_OK) {

        wxString videoFilePath = openFileDialog.GetPath();
        wxFileName directory(videoFilePath);
        wxString directoryPath = directory.GetPath();
        wxString selectedFileName = directory.GetFullName();

        CFile *selectedFile = new CFile(videoFilePath);
        context->addVideo(selectedFile);
        context->reset();

        std::optional<wxString> currentVideoFilePath = context->getVideo();
        if(currentVideoFilePath.has_value()) {
            updateMediaPlayer(currentVideoFilePath.value());
        } else {
            LOG_ERROR("Failed to get Current Video from Context for Directory %s", directoryPath);
        }

        std::thread([this, directoryPath, selectedFileName]() {
            
            std::vector<CFile *> files = this->GetFilesInDirectory(directoryPath, selectedFileName);
            wxTheApp->CallAfter([this, files, directoryPath]() {
                
                LOG_INFO("Found %d Video Files in %s", files.size(), directoryPath);
                for (CFile *file : files){
                    context->addVideo(file);
                }
            });

        }).detach();
        
    }
}

void VideoPanel::updateMediaPlayer(wxString videoFilePath) {

    context->setIsPlaying(false);
    mediaPlayer->Load(videoFilePath);
    LOG_INFO("Updated Media Player with %s", videoFilePath);
}

void VideoPanel::mediaLoadedHandler(wxCommandEvent &event) {

    LOG_INFO("Video Loaded EVT");
    context->setTotalPlaybackTime((long long)mediaPlayer->Length());
    playbackTimeText->SetupTimerLayout(context->getTotalPlaybackTimeInMiliSecond());
    dispatchEvent(&VideoPanel::playPauseHandler);
    try {
        if (mainFrame)
            mainFrame->setStatusBarText(getStatusBarData());
    } catch (const std::exception &exc) {
        LOG_ERROR("Exception in changeing statusbar : %s", exc.what());
    }
}

void VideoPanel::mediaEndedHandler(wxCommandEvent &event) {

    LOG_INFO("Video Ended EVT");
    #ifdef _WIN32
        if (mediaPlayer->Tell() < context->getTotalPlaybackTimeInMiliSecond()) {
            return;
        } else if (mediaPlayer->Tell() == 0 && context->getIsPlaying()) {
            dispatchEvent(&VideoPanel::playPauseHandler);
        }
    #else
        dispatchEvent(&VideoPanel::playPauseHandler);
    #endif
}

void VideoPanel::playPauseHandler(wxCommandEvent &event) {

    if (!context->getIsPlaying()) {
        if (mediaPlayer->Play()) {
            LOG_INFO("Playing Media");
        } else {
            LOG_INFO("Could Not Play the Media [Either file is not loaded or it does not exist]");
            std::optional<wxString> file = context->getVideo();
            if (file.has_value()) LOG_INFO("Current File is : %s", file.value());
            dispatchEvent(&VideoPanel::uploadHandler);
        } 
        context->setIsPlaying(true);
        LOG_INFO("Timer Start");
        playbackTimer->Start(30);
        playPauseButton->SetBitmap(assets->getPauseIcon());
    } else {
        mediaPlayer->Pause();
        context->setIsPlaying(false);
        LOG_INFO("Timer Stopped");
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
        std::optional<wxString> currentVideoFilePath = context->getVideo();
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
        std::optional<wxString> currentVideoFilePath = context->getVideo();
        if(currentVideoFilePath.has_value()) {
            updateMediaPlayer(currentVideoFilePath.value());
        } else {
            LOG_ERROR("Failed to get Current Video from Context");
        }
    }
}

void VideoPanel::volumeToValue(const int &volume) {

    context->setVolume(volume);

    if (volume == 0) volumeButton->SetBitmap(assets->getMuteIcon());
    if (volume != 0) volumeButton->SetBitmap(assets->getVolumeIcon());

    volumeSlider->SetValue(context->getVolume());
    mediaPlayer->SetVolume(static_cast<double>(context->getVolume())/VOLUME_RANGE); 
}

void VideoPanel::volumeHandler(wxMouseEvent& event) {

    if (event.LeftIsDown()) {
        int width = volumeSlider->GetSize().GetWidth();
        int x = event.GetX();
        int value = (x * volumeSlider->GetRange()) / width;
        
        if (value < 0) value = 0;
        if (value > volumeSlider->GetRange()) value = volumeSlider->GetRange();
        volumeToValue(value);
    }
}

void VideoPanel::seekHandler(wxMouseEvent& event) {
    if (event.LeftIsDown()) {
        int width = playbackSlider->GetSize().GetWidth();
        int x = event.GetX();
        int value = (x * playbackSlider->GetRange()) / width;
        seekToValue(value);
    }
}

void VideoPanel::seekToValue(int value) {

    if (value < 0) value = 0;
    if (value > playbackSlider->GetRange()) value = playbackSlider->GetRange();
    
    long long timeToSeek = (context->getTotalPlaybackTimeInMiliSecond() * value)/PLAYBACK_RANGE;

    playbackSlider->SetValue(value);
    mediaPlayer->Seek(timeToSeek);
    mediaPlayer->Play();
    
    if(!context->getIsPlaying()) { dispatchEvent(&VideoPanel::playPauseHandler); }
    LOG_INFO("Seek To Time : %s", CTime::timeString(timeToSeek));
}

void VideoPanel::muteHandler(wxCommandEvent &event) {

    bool isMute = context->getVolume() == 0;
    if (isMute) {
        volumeToValue(10); 
    } else {
        volumeToValue(0);  
    }
    LOG_INFO("Mute Handler Invoked");
    
}

void VideoPanel::alphaPressHandler(wxCommandEvent& event) {

    char alphabetPressed = 'A' + (event.GetId() - ID_OFFSET_ALPHA);
    switch (alphabetPressed) {
        case 'M': {
            dispatchEvent(&VideoPanel::muteHandler);
            LOG_INFO("Alphabet Key Event Raised : M [Mute]");
            break;
        }
        case 'J': {
            int valueDecrement = (PLAYBACK_RANGE * 10000)/context->getTotalPlaybackTimeInMiliSecond();
            seekToValue(playbackSlider->GetValue() - valueDecrement);
            break;
        }
        case 'K': {
            dispatchEvent(&VideoPanel::playPauseHandler);
            break;
        }
        case 'L': {
            int valueIncrement = (PLAYBACK_RANGE * 10000)/context->getTotalPlaybackTimeInMiliSecond();
            seekToValue(playbackSlider->GetValue() + valueIncrement);
            break;
        }
        case 'O': {
            dispatchEvent(&VideoPanel::uploadHandler);
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
    int value = PLAYBACK_RANGE * (static_cast<double>(numberPressed)*0.1);
    seekToValue(value);
}

void VideoPanel::keyPressHandler(wxCommandEvent& event) {
    
    int arrowId = event.GetId();
    switch (arrowId) {
        case ID_ARROW_UP: {
            int currentVolume = context->getVolume();
            if (currentVolume != 100) {
                context->setVolume(currentVolume + 1);
                dispatchEvent(&VideoPanel::muteHandler);
            }
            break;
        }
        case ID_ARROW_DOWN: {
            int currentVolume = context->getVolume();
            if (currentVolume != 0) {
                context->setVolume(currentVolume - 1);
                dispatchEvent(&VideoPanel::muteHandler);
            }
            break;
        }
        case ID_ARROW_LEFT: {
            int valueDecrement = (PLAYBACK_RANGE * 5000)/context->getTotalPlaybackTimeInMiliSecond();
            seekToValue(playbackSlider->GetValue() - valueDecrement);
            break;
        }
        case ID_ARROW_RIGHT: {
            int valueIncrement = (PLAYBACK_RANGE * 5000)/context->getTotalPlaybackTimeInMiliSecond();
            seekToValue(playbackSlider->GetValue() + valueIncrement);
            break;
        }
        case ID_SPACE: {
            dispatchEvent(&VideoPanel::playPauseHandler);
            break;
        }
        case ID_ALT_ARROW_LEFT: {
            dispatchEvent(&VideoPanel::previousHandler);
            break;
        }
        case ID_ALT_ARROW_RIGHT: {
            dispatchEvent(&VideoPanel::nextHandler);
            break;
        }
    }
}

void VideoPanel::OnWindowDestroy(wxWindowDestroyEvent& event) {

    if (event.GetEventObject() == rootPanel) {
        LOG_INFO("Video RootPanel is being destroyed - cleaning up UI references");

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





