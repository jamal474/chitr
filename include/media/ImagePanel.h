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
    virtual std::vector<wxString> GetFilesInDirectory(const wxString& dirPath) override;
    virtual void init() override;
    virtual void setSizers() override;
    virtual void setBindings() override;

private:
    void uploadHandler(wxCommandEvent &event);
    void updateImageViewer(wxString imageFilePath);
    void slideshowOpenClose(wxCommandEvent &event);
    void slideshowHandler(wxTimerEvent &event);
    void nextHandler(wxCommandEvent &event);
    void previousHandler(wxCommandEvent &event);

    wxStaticBitmap      *imageViewer;
    wxButton            *uploadButton;
    wxButton            *nextButton;
    wxButton            *previousButton;
    wxButton            *slideShowButton;
    bool                slideShowFlag;
    wxTimer             slideShowTimer;
    ImageContext        *context;

public:
    ImagePanel(wxFrame *, wxNotebook *, std::shared_ptr<Resource>);
    wxPanel *getRootPanel() override;
};