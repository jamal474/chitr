#pragma once
#include <wx/wx.h>
#include "ImagePlayback.h"
#include "VideoPlayback.h"
#include <wx/mediactrl.h>
#include <wx/slider.h>
#include <wx/notebook.h>
#include <wx/bitmap.h>
#include "Resource.h"

// enum for widgets IDs
typedef enum {
    PREV_IMAGE_BUTTON = 6000,
    UPLOAD_IMAGE_BUTTON,
    NEXT_IMAGE_BUTTON,
    SLIDESHOW_BUTTON
}ID;

class RootFrame : public wxFrame
{
/**
 * RootFrame class is inherited from wxFrame, the base widget
 * with with other widgets are created as its child. It has
 * many members  :
 *  1. VideoPlayback : videos file storage class
 *  2. ImagePlayback : image file storage class
 *  3. Resource : constains all the icons, colours and images
 *  4. all the widgets pointers wxButton, wxPanels, wxBoxSizers
 *  5. All handlers for both the image viewer and video/music player
*/
    ImagePlayback              *imgPlayBack;
    VideoPlayback              *vidPlayBack;
    wxNotebook                 *notebook;

    wxStaticBitmap              *imageView;
    wxMediaCtrl                 *videoView;
    wxSlider        *volumeSlider;

    wxBitmapButton        *uploadImageButton;
    wxButton        *nextButton;
    wxButton        *prevButton;
    wxButton        *slideShowButton;
    wxButton        *uploadButton;
    wxButton        *stopButton;
    wxButton        *nextVideoButton;
    wxButton        *previousVideoButton;
    wxButton        *playPauseButton;

    bool            slideShowFlag;
    bool            isPlaying;
    double          volume;
    wxTimer         slideShowTimer;

    wxPanel                     *imageControlPanel;
    wxPanel                     *currentImagePanel;
    wxPanel                     *imagePagePanel;
    wxPanel                     *videoPagePanel;
    wxPanel                     *currentVideoPanel;
    wxPanel                     *videoControlPanel;

    wxBoxSizer                  *mainSizer;
    wxBoxSizer                  *imagePageSizer;
    wxBoxSizer                  *videoPageSizer;
    wxBoxSizer                  *imageControlSizer;
    wxBoxSizer                  *currentImageSizer;
    wxBoxSizer                  *currentVideoSizer;
    wxBoxSizer                  *videoControlSizer;
    std::unique_ptr<Resource>   assets;

    // Initialize widgets
    void initImagePage();
    void initVideoPage();

    // Set Event Handler Bindings
    void SetBindings();

    // Image event handlers
    void imageUploadHandler(wxCommandEvent &evt);
    void updateImageViewer(wxBitmap &bitmap);
    void previousImageHandler(wxCommandEvent &evt);
    void nextImageHandler(wxCommandEvent &evt);
    void slideshowOpenClose(wxCommandEvent &evt);
    void slideshowHandler(wxTimerEvent &evt);

    // Video Event Handler
    void videoUploadHandler(wxCommandEvent&);
    void updateVideoViewer(wxString);
    void playPauseHandler(wxCommandEvent&);
    void previousVideoHandler(wxCommandEvent&);
    void stopHandler(wxCommandEvent&);
    void nextVideoHandler(wxCommandEvent&);
    void volumeChangeHandler(wxCommandEvent&);
    void LoadEmbeddedFont();

    std::vector<wxString> GetFilesInDirectory(const wxString& dirPath);

    public:
    RootFrame(const wxString &title); 
};

