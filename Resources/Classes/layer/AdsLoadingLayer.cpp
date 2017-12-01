//
//  AdsLoadingLayer.cpp
//  SpotDifferences
//
//  Created by luoxiaopeng on 6/12/15.
//
//

#include "AdsLoadingLayer.h"
#include "UICpp.h"

AdsLoadingLayer* AdsLoadingLayer::create(bool isRewards)
{
    AdsLoadingLayer *pRet = new(std::nothrow) AdsLoadingLayer();
    if (pRet && pRet->init(isRewards))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool AdsLoadingLayer::init(bool isRewards)
{
    if (!Layer::init()) {
        return false;
    }
    
    _isRewards = isRewards;
    
    bool rcode = false;
    
    do {
        loadStudio();
        
        //touch event listen
        auto touchEventListener = EventListenerTouchOneByOne::create();
        touchEventListener->setSwallowTouches(true);
        touchEventListener->onTouchBegan = [](Touch* touch, Event* event) {
            
            return true;
        };
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEventListener, this);
        rcode = true;
    } while (0);
    
    return rcode;
}

void AdsLoadingLayer::loadStudio()
{
    auto bg = Sprite::create("other/loading.png");
    bg->setPosition(getContentSize()/2);
    addChild(bg);
    
    auto scaleX = getContentSize().width / bg->getContentSize().width;
    auto scaleY = getContentSize().height / bg->getContentSize().height;
    bg->setScale(scaleX > scaleY ? scaleX : scaleY);
}

void AdsLoadingLayer::onEnter(){
    Layer::onEnter();
    AdsManager::getInstance()->removeAds(kTypeBannerAds);
}

void AdsLoadingLayer::onExit(){
    Layer::onExit();
    
    if (!_isRewards) {
        
        auto scene = Director::getInstance()->getRunningScene();
        string name = scene->getName();
        if (scene && name != "ChapterScene" && name != "InspirationScene" && name != "MyArtsScene") {
            AdsManager::getInstance()->showAds(kTypeBannerAds);
        }
        
    }
}
