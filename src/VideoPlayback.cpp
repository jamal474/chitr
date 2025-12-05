#include "VideoPlayback.h"
#include <exception>
#include <iostream>
#include <wx/wx.h>

VideoPlayback::VideoPlayback() : currentIndex(0) {}

void VideoPlayback::addVideo(wxString videoPath)
{
    videos.push_back(videoPath);
}

int VideoPlayback::getNumberOfVideos()
{
    return videos.size();
}

void VideoPlayback::setCurrentIndex(int newIndex)
{
    try{
        if(newIndex >= getNumberOfVideos())
        {
            throw std::invalid_argument("Video index is out of bound");
        }
        currentIndex = newIndex;
    }
    catch(std::invalid_argument &error)
    {
        std::cout<<"Error : "<<error.what()<<std::endl;
    }
}

int VideoPlayback::getCurrentIndex()
{
    return currentIndex;
}

wxString VideoPlayback::getVideoByIndex(int index)
{
    try{
        if(index >= videos.size())
        {
            throw std::invalid_argument("Image index is out of bound");
        }
        return videos[index];
    }
    catch(std::invalid_argument &error)
    {
        std::cout<<"Error : "<<error.what()<<std::endl;
        return wxString("");
    }
}
