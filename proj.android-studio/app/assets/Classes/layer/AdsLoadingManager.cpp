//
//  AdsLoadingManager.cpp
//
//  Created by luoxiaopeng on 15/6/10.
//
//

#include "AdsLoadingManager.h"
#include "AnalyticsManager.hpp"

static AdsLoadingManager *_sharedAdLoadingBase = nullptr;

AdsLoadingManager* AdsLoadingManager::getInstance()
{
    if (! _sharedAdLoadingBase)
    {
        _sharedAdLoadingBase = new AdsLoadingManager();
    }
    
    return _sharedAdLoadingBase;
}

AdsLoadingManager::AdsLoadingManager()
{
    _ads = AdsManager::getInstance();
}

AdsLoadingManager::~AdsLoadingManager()
{
    CC_SAFE_RELEASE(_loadingLayer);
    _scheduler->unscheduleAllForTarget(this);
}

void AdsLoadingManager::init(Layer * loadingLayer)
{
    _loadingLayer = loadingLayer;
    CC_SAFE_RETAIN(_loadingLayer);
    
    _scheduler->scheduleUpdate(this, 0, false);
}

void AdsLoadingManager::show(ADS_TYPE type)
{
    __CCLOGWITHFUNCTION("%s %d", __FILE__, _adType);
    
    _adType = type;
    _state = State::WaitForShow;
    
    //记录runningScene, 当runningScene 改变的时候, 再show
    _runningScene = Director::getInstance()->getRunningScene();
    
    _ads->onAdsLoadFailed = CC_CALLBACK_2(AdsLoadingManager::onAdsLoadFailed, this);
    _ads->onAdsExpanded = CC_CALLBACK_1(AdsLoadingManager::onAdsExpanded, this);
    _ads->onAdsLoaded = CC_CALLBACK_1(AdsLoadingManager::onAdsLoaded, this);
    _ads->onAdsCollapsed = CC_CALLBACK_1(AdsLoadingManager::onAdsCollapsed, this);
}

void AdsLoadingManager::show(ADS_TYPE type, const ccAdsLoadingCallback &callback)
{
    show(type);
    _callback = callback;
}

void AdsLoadingManager::update(float dt)
{
    switch (_state) {
        case State::WaitForShow:
        {
            if (_runningScene != Director::getInstance()->getRunningScene()) {
                Director::getInstance()->getRunningScene()->addChild(_loadingLayer, INT_MAX);
                
                _isRequestingLoadAd = false;
                _adShowing = false;
                _bLoadingCanRemove = false;
                _adLoadDone = false;
                _loadingAcc = 0;
                
                bool ret = AdsManager::getInstance()->showAds(_adType);
                if(!ret)
                {
                    _isRequestingLoadAd = true;
                }
                _state = State::Timing;
            }
        }
            break;
        case State::Timing:
        {
            _loadingAcc += dt;
            
            if (((_loadingAcc >= _loadingMinTime) && _adLoadDone) ||
                _loadingAcc >=_loadingMaxTime)
            {
                adLoadDone();
            }
        }
            break;
        default:
            break;
    }
}

//继承方法 //////////////////////////////////////////////////////////////////////////////////////////
void AdsLoadingManager::onAdsLoadFailed(std::string error, ADS_TYPE adType)
{
    if(_adType == adType)
        _adLoadDone = true;
}

void AdsLoadingManager::onAdsExpanded(ADS_TYPE adType)
{
    if (adType == kTypeInterstitialAds)
    {
        ATMInstance->sendInterstitialEvent("ads_interstital");
    }

    if(_adType == adType)
    {
        Director::getInstance()->stopAnimation();
        Director::getInstance()->pause();
        _adLoadDone = true;
        _adShowing  = true;
    }
}

void AdsLoadingManager::onAdsLoaded(ADS_TYPE adType)
{
    if(_adType == adType)
    {
        if (_isRequestingLoadAd)
            AdsManager::getInstance()->showAds(_adType);
    }
}

void AdsLoadingManager::onAdsCollapsed(ADS_TYPE adType)
{
    if(_adType == adType)
    {
        Director::getInstance()->startAnimation();
        Director::getInstance()->resume();
        if(_bLoadingCanRemove)
        {
            taskDone();
            return;
        }
        _adShowing = false;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////

void AdsLoadingManager::adLoadDone()
{
    //广告已关闭或加载失败
    if(!_adShowing)
    {
        taskDone();
    }
    //广告正在显示
    else
    {
        _bLoadingCanRemove = true;
    }
}

void AdsLoadingManager::taskDone()
{
    _state = State::None;
    
    _loadingLayer->removeFromParent();
    
    _ads->onAdsLoadFailed = nullptr;
    _ads->onAdsExpanded = nullptr;
    _ads->onAdsLoaded = nullptr;
    _ads->onAdsCollapsed = nullptr;
    
    if(_callback)
        _callback();
    
    _callback = nullptr;
}