#include "ImagePlayback.h"
#include <exception>
#include <iostream>
#include <wx/wx.h>

ImagePlayback::ImagePlayback() : currentIndex(0) {}

void ImagePlayback::addImage(wxString imagePath)
{
    images.push_back(imagePath);
}

int ImagePlayback::getNumberOfImages()
{
    return images.size();
}

void ImagePlayback::setCurrentIndex(int newIndex)
{
    try{
        if(newIndex >= getNumberOfImages())
        {
            throw std::invalid_argument("Image index is out of bound");
        }
        currentIndex = newIndex;
    }
    catch(std::invalid_argument &error)
    {
        std::cout<<"Error : "<<error.what()<<std::endl;
    }
}

int ImagePlayback::getCurrentIndex()
{
    return currentIndex;
}

wxString ImagePlayback::getImageByIndex(int index)
{
    try{
        if(index >= images.size())
        {
            throw std::invalid_argument("Image index is out of bound");
        }
        return images[index];
    }
    catch(std::invalid_argument &error)
    {
        std::cout<<"Error : "<<error.what()<<std::endl;
        return wxString("");
    }
}
