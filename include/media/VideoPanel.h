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

class VideoPanel: public MediaPanel {

protected:
    virtual std::vector<CFile *> GetFilesInDirectory(const wxString& dirPath) override;
    virtual void init() override;
    virtual void setSizers() override;
    virtual void setBindings() override;
    virtual void setCursors() override;
    virtual void setToolTips() override;

private:
    void OnWindowDestroy(wxWindowDestroyEvent& event);
    std::vector<wxAcceleratorEntry> getAcceleratorEntries();
    void uploadHandler(wxCommandEvent &event);
    void updateMediaPlayer(wxString videoFilePath);
    void mediaLoadedHandler(wxCommandEvent &event);
    void mediaEndedHandler(wxCommandEvent &event);
    void playPauseHandler(wxCommandEvent &event);
    void nextHandler(wxCommandEvent &event);
    void previousHandler(wxCommandEvent &event);
    void volumeHandler(wxMouseEvent& event);
    void seekHandler(wxMouseEvent& event);
    void muteHandler(wxCommandEvent &event);
    void playbackLengthHandler(wxTimerEvent &event);
    void alphaPressHandler(wxCommandEvent& event);
    void numPressHandler(wxCommandEvent& event);
    void keyPressHandler(wxCommandEvent& event);

    template <typename EventType>
    void dispatchEvent(void (VideoPanel::*handler)(EventType&), int id = wxID_ANY);

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
    wxPanel *getRootPanel() const override;
    const std::vector<wxString> getStatusBarData() const override;
    
};