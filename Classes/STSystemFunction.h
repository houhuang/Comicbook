//
//  STSystemFunction.h
//  MysteryGuardians
//
//  Created by Steven.Xc.Tian on 13-7-9.
//
//

#ifndef __MysteryGuardians__SystemFunction__
#define __MysteryGuardians__SystemFunction__
#include <string>
#include "cocos2d.h"

class STSystemFunction
{
public:
    STSystemFunction();
    ~STSystemFunction();
public:
    /**
     *  @brief Send Email by system default.
     *
     *  @param subject email subject.
     *  @param content email subject.
     */
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) // iOS
    virtual void sendEmail(const char* subject, const char* body , cocos2d::CCImage* image);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) // Android
    virtual void sendEmail(const char* subject, const char* body);
#endif
    /**
     *
     */
    virtual void popAlertDialog(const char* message);
    virtual void makeToast(const char* message);
    
    /**
     *
     */
    virtual void showMoreGame();
    virtual void rating(const char* appleID, const char* message);
    virtual void rating();
    virtual bool isRate();
    
    std::string getPlatformVersion();
    std::string getPlatformBundleID();
    
    //    virtual void sendImageToInstagram(std::string fileName);
    virtual void sendPictureToMore(std::string fileName);
    virtual void go2MarketDetail(std::string packagename);
    virtual void contactUs();
    virtual void sendEmailAndAssetPic(std::string subject, std::string message, std::string assetName);
    virtual void sendEmailAndFilePic(std::string subject, std::string message, std::string fileName);
    
    virtual bool checkNetworkAvailable();
    virtual bool isTabletAvailable();
    virtual float densityScale();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) // iOS
    virtual void sendEvent(std::string name, std::map<std::string, std::string> map);
    virtual void go2PrivacyPage();
    virtual void openAppStoreById(std::string appId);
    virtual float getTitleBarHeight();
    virtual bool isIpad();
    virtual bool systemVersionHigh10();
    virtual std::string getDeviceVersion();
    virtual bool isShowSystemRateUs();
    virtual void showSystemRateUs();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) // Android
    virtual void showBanner();
    virtual void showFullScreen();
    virtual void showVideoAds();
    virtual void go2PrivacyPage();
    virtual void openGooglePlayStore();
    
    virtual void refreshDCIM(std::string packagename);
    virtual void endSession();
    
    virtual void sendFireBase(std::string jsonString);
    virtual void sendEmailToUs();
    virtual void shareToFriend();
#endif
    
    
    //    virtual void go2PrivacyPage();
    //    virtual void openURL(std::string url);
    void saveLocalImage(cocos2d::Image *image);
    virtual void openURL(std::string url);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) // iOS
    std::string getPlatformTempPath();
    std::string getPlatformSavePath();
    bool isIosFiveUp();
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) // Android
    std::string getSdCardPath();
    
#endif
    
    
    
};

#endif /* defined(__MysteryGuardians__SystemFunction__) */
