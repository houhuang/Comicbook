

#include "RewardAdLoadingLayer.h"
#include "UICpp.h"
#include "Dialog.h"
#include "AdsLoadingLayer.h"
#include "STSystemFunction.h"
#include "BannerManager.hpp"

RewardAdLoadingLayer::~RewardAdLoadingLayer()
{
    AdsManager::getInstance()->delegate = nullptr;
    unschedule(schedule_selector(RewardAdLoadingLayer::closeReward));
}

bool RewardAdLoadingLayer::init()
{
    if (Layer::init())
    {
        return true;
    }
    return false;
}

void RewardAdLoadingLayer::onEnter()
{
    Layer::onEnter();
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [](Touch* touch, Event* event){
        return true;
    };
    listener->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    AdsManager::getInstance()->delegate = this;
    bool rtn = AdsManager::getInstance()->showAds(kTypeRewardedAds);
    log("reward showAds %d", rtn);
    
    if (!rtn) {
        xDialog->show("try_later", "try_later", nullptr);
        AdsManager::getInstance()->delegate = nullptr;
        removeFromParent();
        //弹出失败对话框
    }else{
        
    }
}

void RewardAdLoadingLayer::onAdsLoaded(ADS_TYPE adType)
{
    if(adType == ADS_TYPE::kTypeRewardedAds)
    {
        log("reward loaded.");
    }
    
};
void RewardAdLoadingLayer::onAdsLoadFailed(std::string error, ADS_TYPE adType)
{
    if(adType == ADS_TYPE::kTypeRewardedAds)
    {
        log("reward load failed : %s", error.c_str());
    }
};


#pragma mark - Ad Delegate
void RewardAdLoadingLayer::onAdsCollapsed(ADS_TYPE adType)
{
}

void RewardAdLoadingLayer::onAdsExpanded(ADS_TYPE adType) {
    if(adType == ADS_TYPE::kTypeRewardedAds)
    {
        auto loading = AdsLoadingLayer::create(true);
        addChild(loading);
        scheduleOnce(schedule_selector(RewardAdLoadingLayer::closeReward), 45.f);
    }
}

//这个方法内不要操作任何cocos2d-x的东西
void RewardAdLoadingLayer::onAdsRewarded(std::string str, int num, bool isSkiped)
{
    AdsManager::getInstance()->delegate = nullptr;
    
    if (isSkiped) {
        auto scene = Director::getInstance()->getRunningScene();
        string name = scene->getName();
        if (scene && name != "ChapterScene" && name != "InspirationScene" && name != "MyArtsScene") {
//            xAds->showAds(kTypeBannerAds);
            xBanner->show();
        }
    }else{
        _eventDispatcher->dispatchCustomEvent(EVENT_REWARD);
    }
    
    scheduleOnce(schedule_selector(RewardAdLoadingLayer::closeReward), 0.f);
}

void RewardAdLoadingLayer::closeReward(float dt)
{
    removeFromParent();
}


