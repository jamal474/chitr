#pragma once
#include <wx/wx.h>
#include <wx/notebook.h>
#include <wx/event.h>
#include <functional>
#include <memory>
#include "chitr/Resource.h"
#include "media/ImageContext.h"
#include "media/MediaPanel.h"
#include "media/ImagePanel.h"

class ImagePanel: public MediaPanel {

protected:
    virtual std::vector<wxFileName *> GetFilesInDirectory(const wxString& dirPath) override;
    virtual void init() override;
    virtual void setSizers() override;
    virtual void setBindings() override;
    virtual void setCursors() override;
    virtual void setToolTips() override;

private:
    void OnWindowDestroy(wxWindowDestroyEvent& event);
    std::vector<wxAcceleratorEntry> getAcceleratorEntries();
    void uploadHandler(wxCommandEvent &event);
    void updateImageViewer(wxString imageFilePath);
    void slideshowOpenClose(wxCommandEvent &event);
    void slideshowHandler(wxTimerEvent &event);
    void nextHandler(wxCommandEvent &event);
    void previousHandler(wxCommandEvent &event);
    void alphaPressHandler(wxCommandEvent& event);
    void numPressHandler(wxCommandEvent& event);
    void keyPressHandler(wxCommandEvent& event);

    wxStaticBitmap      *imageViewer;
    wxButton            *uploadButton;
    wxButton            *nextButton;
    wxButton            *previousButton;
    wxButton            *slideShowButton;
    wxTimer             *slideShowTimer;
    ImageContext        *context;

public:
    ImagePanel(wxFrame *, wxNotebook *, std::shared_ptr<Resource>);
    ~ImagePanel();
    wxPanel *getRootPanel() override;
};