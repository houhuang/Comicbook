//
//  STSystemFunction.cpp
//  MysteryGuardians
//
//  Created by Steven.Tian on 13-7-9.
//
//

#include "STSystemFunction.h"


STSystemFunction::STSystemFunction()
{
}

STSystemFunction::~STSystemFunction()
{
}

void STSystemFunction::sendEmail(const char* subject, const char* body , cocos2d::CCImage* image)
{

}

void STSystemFunction::popAlertDialog(const char *message)
{

}

void STSystemFunction::showMoreGame()
{

}

void STSystemFunction::go2MarketDetail(std::string packagename)
{

}

void STSystemFunction::makeToast(const char* message)
{

}

void STSystemFunction::rating(const char* appleID, const char* message)
{

}

bool STSystemFunction::isRate()
{

}

bool STSystemFunction::isIpad()
{

}

bool STSystemFunction::systemVersionHigh10()
{
    return false;
}

std::string STSystemFunction::getPlatformVersion()
{
    return "";
}

std::string STSystemFunction::getDeviceVersion()
{
    return "";
}

std::string STSystemFunction::getPlatformBundleID()
{
    return "";
}

bool STSystemFunction::isShowSystemRateUs()
{
    return false;
}

void STSystemFunction::showSystemRateUs()
{

}

float STSystemFunction::getTitleBarHeight()
{
    return 0;
}

void STSystemFunction::openAppStoreById(std::string appId)
{

}

void STSystemFunction::rating()
{
    // only for Android
}

void STSystemFunction::contactUs()
{
}

void STSystemFunction::sendEmailAndAssetPic(std::string subject, std::string message, std::string assetName)
{}

void STSystemFunction::sendEmailAndFilePic(std::string subject, std::string message, std::string fileName)
{}

void STSystemFunction::sendPictureToMore(std::string fileName)
{
    

}

std::string STSystemFunction::getPlatformTempPath()
{

}

bool STSystemFunction::checkNetworkAvailable()
{

    return false;

}

bool STSystemFunction::isTabletAvailable()
{
    return true;
}

float STSystemFunction::densityScale()
{
    return 0.0f;
}

void STSystemFunction::go2PrivacyPage()
{

}

void STSystemFunction::sendEvent(std::string name, std::map<std::string, std::string> map)
{


}

void STSystemFunction::openURL(std::string url)
{

}

void STSystemFunction::saveLocalImage(cocos2d::CCImage* m_image)
{

}

std::string STSystemFunction::getPlatformSavePath()
{

}


bool STSystemFunction::isIosFiveUp()
{
 
}
