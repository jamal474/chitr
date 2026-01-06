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
#else
#define UPLOAD_STRING "Media files (*.aac;*.aiff;*.au;*.caf;*.flac;*.m4a;*.m4v;" \
                      "*.mov;*.mp3;*.mp4;*.mpg;*.ogg;*.opus;*.wav)|*.aac;*.aiff;*.au;" \
                      "*.caf;*.flac;*.m4a;*.m4v;*.mov;*.mp3;*.mp4;*.mpg;*.ogg;*.opus;*.wav"
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