#include "media/MediaContext.h"
#include "chitr/ChitrLogger.h"
#include "chitr/CFile.h"
#include <exception>
#include <optional>
#include <algorithm>

bool MediaContext::setCurrentIndex(const int &newIndex) {

    try{
        if(newIndex >= getListSize() || newIndex < 0) {
            throw std::invalid_argument("Media index is out of bound");
        }
        LOG_INFO("Media Index set to %d", newIndex);
        currentIndex = newIndex;
        return true;
    } catch(std::invalid_argument &exc) {
        LOG_ERROR(exc.what());
        return false;
    }
}

std::optional<wxString> MediaContext::getMediaByIndex(const int &index) const {
    try{
        if(index >= mediaList.size()) {
            throw std::invalid_argument("Media index is out of bound");
        }
        LOG_INFO("Found Media with Index %d", index);
        return mediaList[index]->getPath();
    } catch(std::invalid_argument &error)
    {
        LOG_ERROR(error.what());
        return std::nullopt;
    }
}

const std::optional<CFile *> MediaContext::getMediaFileByIndex(const int &index) const {
    try{
        if(index >= mediaList.size())
        {
            throw std::invalid_argument("Media File index is out of bound");
        }
        LOG_INFO("Found Media File with Index %d", index);
        return mediaList[index];
    } catch(std::invalid_argument &error) {
        LOG_ERROR(error.what());
        return std::nullopt;
    }
}

const std::vector<wxString> MediaContext::getMetaData() const {

    const std::optional<CFile *> file = getMediaFileByIndex(currentIndex);

    if (file.has_value()) {
        std::vector<wxString> metaData = {
            file.value()->getName(),
            file.value()->getSizeString(),
            file.value()->getDateString()
        };
        return metaData;
    }
    std::vector<wxString> noMetaData = {_(""),_(""),_("")};
    return noMetaData;
}

void MediaContext::addMedia(CFile *file) {
    mediaList.push_back(file);
}

bool MediaContext::reset(const wxString &filePath) {

    if (filePath.IsEmpty()) {
        if (getListSize() > 0) {
            setCurrentIndex(0);
            return true;
        }
        return false;
    }
    for(int index = 0 ; index < getListSize(); index++){
        if(mediaList[index]->getPath() == filePath) {
            setCurrentIndex(index);
            return true;
        }
    }
    return false;
}

int  MediaContext::getListSize() const { return mediaList.size(); }
int  MediaContext::getCurrentIndex() const { return currentIndex; }
bool MediaContext::next() { return setCurrentIndex(currentIndex + 1); }
bool MediaContext::previous() { return setCurrentIndex(currentIndex - 1); }
