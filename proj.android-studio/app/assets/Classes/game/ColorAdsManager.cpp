//
//  ColorAdsManager.cpp
//  ColorBook
//
//  Created by luoxiaopeng on 4/24/15.
//
//

#include "ColorAdsManager.h"
#include "AdsLoadingManager.h"
#include "LockManager.h"
#include "AnalyticsManager.hpp"

static ColorAdsManager *_sharedColorAds = nullptr;

ColorAdsManager* ColorAdsManager::getInstance()
{
    if (! _sharedColorAds)
    {
        _sharedColorAds = new ColorAdsManager();
    }
    
    return _sharedColorAds;
}

void ColorAdsManager::destroyInstance()
{
    CC_SAFE_DELETE(_sharedColorAds);
}

ColorAdsManager::ColorAdsManager()
{
    _showed = false;
    _shared = false;
    _isEnableShowInterstitialAds = true;
}

void ColorAdsManager::share()
{
    _shared = true;
}

void ColorAdsManager::changeScene()
{
    bool isShowRate = true;
    if (_shared)
    {
        xAdLoading->show(ADS_TYPE::kTypeInterstitialAds, CC_CALLBACK_0(ColorAdsManager::loadFinish, this));
    }else
    {
        xAdLoading->show(ADS_TYPE::kTypeInterstitialAds, CC_CALLBACK_0(ColorAdsManager::loadFinish, this));
    }
    
//    if (!_showed && _shared) {
//        //展示广告
//        _showed = true;
//        xAdLoading->show(ADS_TYPE::kTypeCrosspromoAds, CC_CALLBACK_0(ColorAdsManager::loadFinish, this));
//        isShowRate = false;
//    }
    
    if (isShowRate)
    {
//        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("chapter_rateUs");
        LockManager::getInstance()->_isShowRateUs = true;
    }
    
    _shared = false;
}

void ColorAdsManager::loadFinish()
{
    setShowed(true);
}
