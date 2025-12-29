#pragma once
#include <wx/wx.h>
#include <wx/notebook.h>
#include <wx/event.h>
#include <wx/mediactrl.h>
#include "media/MediaPanel.h"
#include "media/VideoContext.h"
#include <memory>
#include "chitr/Resource.h"

class VideoPanel: public MediaPanel {

protected:
    virtual std::vector<wxString> GetFilesInDirectory(const wxString& dirPath) override;
    virtual void init() override;
    virtual void setSizers() override;
    virtual void setBindings() override;

private:
    void uploadHandler(wxCommandEvent &event);
    void updateMediaPlayer(wxString videoFilePath);
    void playPauseHandler(wxCommandEvent &event);
    void stopHandler(wxCommandEvent &event);
    void nextHandler(wxCommandEvent &event);
    void previousHandler(wxCommandEvent &event);
    void volumeHandler(wxCommandEvent &event);

    wxMediaCtrl     *mediaPlayer;
    wxSlider        *volumeSlider;
    wxButton        *uploadButton;
    wxButton        *stopButton;
    wxButton        *nextButton;
    wxButton        *previousButton;
    wxButton        *playPauseButton;
    wxStaticBitmap  *volumeIcon;
    bool            isPlaying;
    double          volume;
    VideoContext    *context;

public:
    VideoPanel(wxFrame *, wxNotebook *, std::shared_ptr<Resource>);
    wxPanel *getRootPanel() override;
    
};