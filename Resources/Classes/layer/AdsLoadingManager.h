//
//  AdsLoadingManager.h
//
//  Created by luoxiaopeng on 15/6/10.
//
//

#pragma once

#include "cocos2d.h"
#include "AdsManager.h"

#define xAdLoading              (AdsLoadingManager::getInstance())

using namespace cocos2d;

typedef std::function<void()> ccAdsLoadingCallback;

class AdsLoadingManager: public Node, public AdsManagerDelegate
{
public:
    static AdsLoadingManager* getInstance();
    ~AdsLoadingManager();
    void init(Layer * loadingLayer);
    void show(ADS_TYPE type);
    void show(ADS_TYPE type, const ccAdsLoadingCallback &callback);
    
public:
    void update(float dt) override;
    
private:
    AdsLoadingManager();
    
    virtual void onAdsLoaded(ADS_TYPE adType) override;
    virtual void onAdsLoadFailed(std::string error, ADS_TYPE adType) override;
    virtual void onAdsExpanded(ADS_TYPE adType) override;
    virtual void onAdsCollapsed(ADS_TYPE adType) override;
    
    //广告按时加载出来
    virtual void adLoadDone();
    virtual void taskDone();
    
private:
    enum class State
    {
        None,
        WaitForShow,
        Timing,
    };
    
    //Loading界面最多停留的时间(秒)
    float _loadingMinTime = 0;
    float _loadingMaxTime = 5;
    float _loadingAcc;
    
    //广告是否在显示
    bool _adShowing = false;
    bool _isRequestingLoadAd = false;
    bool _bLoadingCanRemove = false;
    bool _adLoadDone = false;               //广告加载完成与否(失败成功回调都算完成)
    
    AdsManager *    _ads;
    ADS_TYPE        _adType = (ADS_TYPE)0;
    Scene *         _runningScene;
    Layer *         _loadingLayer = nullptr;
    State           _state = State::None;
    ccAdsLoadingCallback _callback = nullptr;
};

