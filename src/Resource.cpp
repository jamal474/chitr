#include <wx/wx.h>
#include "Resource.h"
#include "../assets/upload"
#include "../assets/next"
#include "../assets/previous"
#include "../assets/slideshow"
#include "../assets/close"
#include "../assets/play"
#include "../assets/pause"
#include "../assets/stop"
#include "../assets/volume"
#include "../assets/music"
#include "../assets/image"

Resource::Resource()
{
    headerBackgroundColour = wxColour(131, 151, 136);
    bodyBackgroundColour = wxColour(54, 65, 62);
    footerBackgroundColour = wxColour(238,224,203);

    uploadIcon = wxBITMAP(upload).ConvertToImage();
    uploadIcon = uploadIcon.Scale(40, 40, wxIMAGE_QUALITY_HIGH);

    nextIcon = wxBITMAP(next).ConvertToImage();
    nextIcon = nextIcon.Scale(40, 40, wxIMAGE_QUALITY_HIGH);

    previousIcon = wxBITMAP(previous).ConvertToImage();
    previousIcon = previousIcon.Scale(40, 40, wxIMAGE_QUALITY_HIGH);

    slideshowIcon = wxBITMAP(slideshow).ConvertToImage();
    slideshowIcon = slideshowIcon.Scale(40, 40, wxIMAGE_QUALITY_HIGH);

    closeIcon = wxBITMAP(close).ConvertToImage();
    closeIcon = closeIcon.Scale(40, 40, wxIMAGE_QUALITY_HIGH);

    playIcon = wxBITMAP(play).ConvertToImage();
    playIcon = playIcon.Scale(40, 40, wxIMAGE_QUALITY_HIGH);

    stopIcon = wxBITMAP(stop).ConvertToImage();
    stopIcon = stopIcon.Scale(40, 40, wxIMAGE_QUALITY_HIGH);

    pauseIcon = wxBITMAP(pause).ConvertToImage();
    pauseIcon = pauseIcon.Scale(40, 40, wxIMAGE_QUALITY_HIGH);

    volumeIcon = wxBITMAP(volume).ConvertToImage();
    volumeIcon = volumeIcon.Scale(40, 40, wxIMAGE_QUALITY_HIGH);

    imageIcon = wxBITMAP(image).ConvertToImage();
    imageIcon =imageIcon.Scale(80, 80, wxIMAGE_QUALITY_HIGH);

    musicIcon = wxBITMAP(music).ConvertToImage();
    musicIcon = musicIcon.Scale(40, 40, wxIMAGE_QUALITY_HIGH);


}