#include "media/VideoContext.h"
#include "chitr/ChitrLogger.h"
#include "chitr/CFile.h"
#include <exception>
#include <optional>
#include <algorithm>

VideoContext::VideoContext() {

    LOG_INFO("Creating Video Context");
    currentIndex = 0;
    supportedFormats.insert({ _T("mp3"), _T("mp4")});
    volume = 100;
    isPlaying = false;
}

VideoContext::~VideoContext() {

    LOG_INFO("Cleaning Video Context");
    for(int i = 0 ; i < getListSize() ; i++) {
        delete mediaList[i];
        mediaList[i] = nullptr;
    }
    mediaList.clear();
}

void VideoContext::addVideo(CFile *videoFile) {

    if (videoFile->doExist()) {
        addMedia(videoFile);
        LOG_INFO("Video File Added Successfully");
    } else {
        LOG_INFO("Video Does Not Exist, Not Added");
    }
}

void VideoContext::setIsPlaying(const bool &playStatus) {
    if (isPlaying && !playStatus){
        isPlaying = false;
        LOG_INFO("Playback Stopped");
    } else if(!isPlaying && playStatus) {
        isPlaying = true;
        LOG_INFO("playback Started");
    }
}

void VideoContext::setTotalPlaybackTime(const long long &duration) {

    totalPlaybackTime.setTime(duration);
    LOG_INFO("Total Playback Time : %d miliseconds [%s]", duration, static_cast<std::string>(totalPlaybackTime));
}

std::optional<wxString> VideoContext::getVideo() const { return getMediaByIndex(currentIndex); }
const std::optional<CFile *> VideoContext::getVideoFile() const { return getMediaFileByIndex(currentIndex); }
bool VideoContext::getIsPlaying() const { return isPlaying; }
int VideoContext::getVolume() const { return volume; }
void VideoContext::setVolume(const int &newVolume) { volume = std::clamp(newVolume, 0, 100); }
const long long VideoContext::getTotalPlaybackTimeInMiliSecond() const { return totalPlaybackTime.getMiliseconds(); }
const std::string VideoContext::getTotalPlaybackTimeString() const { return totalPlaybackTime; }