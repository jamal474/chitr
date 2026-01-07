#pragma once
#include <memory>
#include <wx/wx.h>
#include <wx/notebook.h>
#include <wx/event.h>
#include <wx/mediactrl.h>
#include "media/MediaPanel.h"
#include "media/VideoContext.h"
#include "chitr/Resource.h"
#include "component/RoundedText.h"
#include "chitr/CFile.h"

#define PLAYBACK_RANGE 10000
#define VOLUME_RANGE 100

#ifdef _WIN32
#define UPLOAD_STRING "Media files (*.aac;*.mp4;*.mov;...)|" \
                      "*.aac;*.AAC;*.aiff;*.AIFF;*.au;*.AU;*.flac;*.FLAC;*.m4a;*.M4A" \
                      "*.mp3;*.MP3;*.wma;*.WMA;*.wav;*.WAV;*.mp4;*.MP4;*.m4v;*.M4V;" \
                      "*.mov;*.MOV;*.mpg;*.MPG;*.avi;*.AVI;*.mkv;*.MKV;*.wmv;*.WMV"
#else
#define UPLOAD_STRING "Media files (*.aac;*.mp4;*.wav;...)|" \
                      "*.aac;*.AAC;*.aiff;*.AIFF;*.au;*.AU;*.caf;*.CAF;" \
                      "*.flac;*.FLAC;*.m4a;*.M4A;*.m4v;*.M4V;*.mov;*.MOV;" \
                      "*.mp3;*.MP3;*.mp4;*.MP4;*.mpg;*.MPG;*.ogg;*.OGG;" \
                      "*.opus;*.OPUS;*.wav;*.WAV"
#endif

class VideoPanel: public MediaPanel {

protected:
    virtual void init() override;
    virtual void setSizers() override;
    virtual void setBindings() override;
    virtual void setCursors() override;
    virtual void setToolTips() override;
    virtual void OnWindowDestroy(wxWindowDestroyEvent& event) override;
    virtual std::vector<wxAcceleratorEntry> getAcceleratorEntries() override;

private:
    void uploadHandler(wxCommandEvent &event);
    void updateMediaPlayer(wxString videoFilePath);
    void mediaLoadedHandler(wxCommandEvent &event);
    void mediaEndedHandler(wxCommandEvent &event);
    void playPauseHandler(wxCommandEvent &event);
    void nextHandler(wxCommandEvent &event);
    void previousHandler(wxCommandEvent &event);
    void volumeHandler(wxMouseEvent& event);
    void seekHandler(wxMouseEvent& event);
    void seekToValue(int value);
    void muteHandler(wxCommandEvent &event);
    void playbackLengthHandler(wxTimerEvent &event);
    void alphaPressHandler(wxCommandEvent& event);
    void numPressHandler(wxCommandEvent& event);
    void keyPressHandler(wxCommandEvent& event);

    wxMediaCtrl     *mediaPlayer;
    wxGauge         *volumeSlider;
    wxGauge         *playbackSlider;
    wxButton        *uploadButton;
    wxButton        *nextButton;
    wxButton        *previousButton;
    wxButton        *playPauseButton;
    wxButton        *volumeButton;
    wxBoxSizer      *controlPanelButtonRowSizer;
    wxBoxSizer      *controlPanelPBSliderSizer;
    wxTimer         *playbackTimer;
    RoundedText     *playbackTimeText;
    VideoContext    *context;

public:
    VideoPanel(MainFrame *, wxNotebook *, std::shared_ptr<Resource>);
    ~VideoPanel();
};