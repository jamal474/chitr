#include "media/VideoContext.h"
#include "chitr/ChitrLogger.h"
#include <exception>
#include <optional>

VideoContext::VideoContext() {
    currentIndex = 0;
    supportedFormats.insert({ _T("mp3"), _T("mp4")});
}

int VideoContext::getListSize() {
    return mediaList.size();
}

bool VideoContext::setCurrentIndex(int newIndex) {

    try{
        if(newIndex >= getListSize() || newIndex < 0)
        {
            throw std::invalid_argument("Video index is out of bound");
        }
        LOG_INFO("Video Index set to %d", newIndex);
        currentIndex = newIndex;
        return true;
    }
    catch(std::invalid_argument &exc)
    {
        LOG_ERROR(exc.what());
        return false;
    }
}

int VideoContext::getCurrentIndex() {
    return currentIndex;
}

void VideoContext::addVideo(wxFileName *videoFile) {
    mediaList.push_back(videoFile);
    LOG_INFO("Video with Path \"%s\" Added", videoFile->GetFullPath());
}

std::optional<wxString> VideoContext::getVideoByIndex(int index) {
    try{
        if(index >= mediaList.size())
        {
            throw std::invalid_argument("Image index is out of bound");
        }
        LOG_INFO("Found Video with Index %d", index);
        return mediaList[index]->GetFullPath();
    }
    catch(std::invalid_argument &error)
    {
        LOG_ERROR(error.what());
        return std::nullopt;
    }
}

bool VideoContext::next() {
    return setCurrentIndex(currentIndex + 1);
}

bool VideoContext::previous() {
    return setCurrentIndex(currentIndex - 1);
}

bool VideoContext::reset(wxString videoFilePath) {
    for(int index = 0 ; index < getListSize(); index++){
        if(mediaList[index]->GetFullPath() == videoFilePath)
        {
            setCurrentIndex(index);
            return true;
        }
    }
    return false;
}