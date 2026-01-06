#include "media/ImageContext.h"
#include "chitr/ChitrLogger.h"
#include "chitr/CFile.h"
#include <exception>
#include <optional>

ImageContext::ImageContext() {

    LOG_INFO("Creating Image Context");
    currentIndex = 0;
    slideShowFlag = false;
    supportedFormats.insert({_T("jpg"), _T("jpeg"),
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

void ImageContext::addImage(CFile *imageFile) {
    
    if (imageFile->doExist()) {
        addMedia(imageFile);
        LOG_INFO("Image File Added Successfully");
    } else {
        LOG_INFO("Image Does Not Exist, Not Added");
    }
}

std::optional<wxString> ImageContext::getImage() const { return getMediaByIndex(currentIndex); }
const std::optional<CFile *> ImageContext::getImageFile() const { return getMediaFileByIndex(currentIndex); }
bool ImageContext::getSlideShowFlag() const { return slideShowFlag; }
void ImageContext::setSlideShowFlag(const bool &flag) { slideShowFlag = flag; }

