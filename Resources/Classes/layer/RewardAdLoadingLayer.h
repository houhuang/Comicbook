//
//  RewardAdLoadingLayer.h
//
//  Created by zw on 15/6/16.
//
//

#ifndef __RewardAdLoadingLayer__
#define __RewardAdLoadingLayer__

#include <stdio.h>
#include "STVisibleRect.h"
#include "editor-support/cocostudio/CocoStudio.h"

#include "cocos2d.h"
#include "AdsManager.h"

USING_NS_CC;
using namespace cocostudio;

#define EVENT_REWARD           "event_reward"

class AdsLoadingLayer;

class RewardAdLoadingLayer : public Layer, public AdsManagerDelegate
{
protected:
    RewardAdLoadingLayer(){};
    ~RewardAdLoadingLayer();
    
    virtual void onAdsLoaded(ADS_TYPE adType);
    virtual void onAdsLoadFailed(std::string error, ADS_TYPE adType);
    
public:
    CREATE_FUNC(RewardAdLoadingLayer);
    virtual void onEnter();

    bool init();
    void closeReward(float dt);
    
    //Ad Delegate
    virtual void onAdsCollapsed(ADS_TYPE adType);
    virtual void onAdsExpanded(ADS_TYPE adType);
    virtual void onAdsRewarded(std::string, int, bool);
    
private:
    float _musicValue;
};

#endif /* defined(__HUGS0216_DoctorMania__RewardAdLoadingLayer__) */
