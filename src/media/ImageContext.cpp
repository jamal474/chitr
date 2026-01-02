#include "media/ImageContext.h"
#include "chitr/ChitrLogger.h"
#include <exception>
#include <optional>

ImageContext::ImageContext() {

    LOG_INFO("Creating Image Context");
    currentIndex = 0;
    slideShowFlag = false;
    supportedFormats.insert({ _T("jpg"), _T("jpeg"),
                            _T("png"), _T("gif"),
                            _T("pcx"), _T("pnm"),
                            _T("tiff"), _T("tif"),
                            _T("xpm")});
}

ImageContext::~ImageContext() {

    LOG_INFO("Cleaning Image Context");
    for(int i = 0 ; i < getListSize() ; i++) {
        delete mediaList[i];
        mediaList[i] = nullptr;
    }
    mediaList.clear();
}

int ImageContext::getListSize() {
    return mediaList.size();
}

bool ImageContext::setCurrentIndex(int newIndex) {
    try{
        if(newIndex >= getListSize() || newIndex < 0)
        {
            throw std::invalid_argument("Image index is out of bound");
        }
        LOG_INFO("Image Index set to %d", newIndex);
        currentIndex = newIndex;
        return true;
    }
    catch(std::invalid_argument &error)
    {
        LOG_ERROR(error.what());
        return false;
    }
}

int ImageContext::getCurrentIndex() {
    return currentIndex;
}

void ImageContext::addImage(wxFileName *imageFile) {
    mediaList.push_back(imageFile);
    LOG_INFO("Image with Path \"%s\" Added", imageFile->GetFullPath());
}

std::optional<wxString> ImageContext::getImageByIndex(int index) {
    try {
        if(index >= getListSize()) {
            throw std::invalid_argument("Image index is out of bound");
        }
        LOG_INFO("Found Image with Index %d", index);
        return mediaList[index]->GetFullPath();
    } catch(std::invalid_argument &error) {
        LOG_ERROR(error.what());
        return std::nullopt;
    }
}

bool ImageContext::next() {
    return setCurrentIndex(currentIndex + 1);
}

bool ImageContext::previous() {
    return setCurrentIndex(currentIndex - 1);
}

bool ImageContext::reset() {
    return setCurrentIndex(0);
}

bool ImageContext::reset(wxString imageFilePath) {
    for(int index = 0 ; index < getListSize(); index++){
        if(mediaList[index]->GetFullPath() == imageFilePath)
        {
            setCurrentIndex(index);
            return true;
        }
    }
    return false;
}

bool ImageContext::getSlideShowFlag() {
    return slideShowFlag;
}

void ImageContext::setSlideShowFlag(bool flag) {
    slideShowFlag = flag;
}