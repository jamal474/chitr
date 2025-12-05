#pragma once
#include <vector>
#include <string>
#include <wx/wx.h>

/**
 * ImagePlayback class depicts the class that handles
 * the images uploaded. It stores the file address
 * of the images, the index of the current image being displayed
 */
class ImagePlayback
{
    std::vector<wxString>   images;
    int                     currentIndex;

    public:
    
    ImagePlayback();
    void      addImage(wxString imagePath);
    wxString  getImageByIndex(int index);
    int       getNumberOfImages();
    void      setCurrentIndex(int newIndex);
    int       getCurrentIndex();

};