//
//  RateUsLayer.cpp
//  ColorGarden
//
//  Created by houhuang on 17/9/18.
//
//

#include "RateUsLayer.hpp"
#include "SSCInternalLibManager.h"
#include "STSystemFunction.h"
#include "SwallowListenerLayer.hpp"
#include "InspirationScene.h"
#include "AnalyticsManager.hpp"
#include "STVisibleRect.h"

enum
{
    st_dialog_left = 1,
    st_dialog_right
};

RateUsLayer* RateUsLayer::create()
{
    RateUsLayer* layer = new RateUsLayer();
    if (layer && layer->init())
    {
        layer->autorelease();
        return layer;
    }
    
    CC_SAFE_DELETE(layer);
    return nullptr;
}

RateUsLayer::~RateUsLayer()
{
    _eventDispatcher->removeEventListenersForTarget(this);
}

bool RateUsLayer::init()
{
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 0))) return false;
    
//    this->initUILayer();
    this->runAction(Sequence::create(EaseSineOut::create(FadeTo::create(0.2f, 210)), CallFunc::create([this](){
        this->initUILayer();
        this->addListener();
    }), NULL));
    
//    _dialogBG->runAction(Sequence::create( DelayTime::create(0.25f), EaseSineOut::create(FadeIn::create(0.1f)), NULL) );
    
    return true;
}

void RateUsLayer::initUILayer()
{
    float width = this->getContentSize().width * ( V::isIpad()? 0.354 : 0.67);
    
    Sprite* bg = Sprite::create("other/popup_rate us@3x.png");
    bg->setPosition(this->getContentSize()/2);
    bg->setScale(width/680);
    this->addChild(bg);
    bg->setOpacity(0);
    _dialogBG = bg;
    
    _dialogBG->runAction(Sequence::create( DelayTime::create(0.15f), EaseSineOut::create(FadeIn::create(0.1f)), NULL) );
    
    MenuItemImage* up = MenuItemImage::create("other/popup_option@3x.png", "other/popup_option@3x.png", CC_CALLBACK_1(RateUsLayer::onButton, this));
    up->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height*0.23));
    up->setTag(st_dialog_left);
    
    Label* up_label = Label::createWithTTF("Rate Now", "res/Muli-Regular.ttf", 38 * 2);
    up_label->setPosition(Vec2(up->getContentSize()/2));
    up->addChild(up_label, 101);
    up_label->setColor(Color3B(255, 255, 255));
    up_label->setScale(0.5f);
    
    up->setOpacity(0);
    up->runAction(Sequence::create( DelayTime::create(0.15f), EaseSineOut::create(FadeIn::create(0.1f)), NULL) );
    
    MenuItemImage* bottom = MenuItemImage::create("other/left12.png", "other/left12.png", CC_CALLBACK_1(RateUsLayer::onButton, this));
    bottom->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height*0.13));
    bottom->setTag(st_dialog_right);
    
    Label* bottom_label = Label::createWithTTF("Maybe Later", "res/Muli-Regular.ttf", 38*2);
    bottom_label->setPosition(Vec2(bottom->getContentSize()/2));
    bottom->addChild(bottom_label, 101);
    bottom_label->setColor(Color3B(96, 96, 96));
    bottom_label->setScale(0.5);
    
    bottom_label->setOpacity(0);
    bottom_label->runAction(Sequence::create( DelayTime::create(0.15f), EaseSineOut::create(FadeIn::create(0.1f)), NULL) );
    
    Menu* lMenu = Menu::create(up, bottom, NULL);
    lMenu->setPosition(Vec2::ZERO);
    bg->addChild(lMenu);
}

void RateUsLayer::addListener()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch* touch, Event* event)->bool{
        _firstPos = touch->getLocationInView();
        return true;
    };
    
    listener->onTouchMoved = [this](Touch* touch, Event* event){
    
    };
    
    listener->onTouchEnded = [this](Touch* touch, Event* event){
        Vec2 lPos = touch->getLocationInView();
        float distance = lPos.distance(_firstPos);
        lPos = Director::getInstance()->convertToGL(lPos);
        
        if (!_dialogBG->getBoundingBox().containsPoint(lPos) && distance < 50)
        {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            STSystemFunction sf;
            
            string appVerison = sf.getPlatformVersion();
            string xxxName = "isCloseRateUs" + appVerison;
            UserDefault::getInstance()->setBoolForKey(xxxName.c_str(), true);
            UserDefault::getInstance()->flush();
#endif
            this->setButtonEnable();
        }
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void RateUsLayer::setButtonEnable()
{
//    InspirationScene* lScene = static_cast<InspirationScene*>(this->getParent());
//    if (lScene)
//    {
//        lScene->setButtonEnable(true);
//    }
//

    ATMInstance->sendEvent("rate_us", "rate_us_selection", "close");
    this->removeFromParentAndCleanup(true);
}

void RateUsLayer::onButton(Ref* ref)
{
    MenuItemImage* menu = static_cast<MenuItemImage*>(ref);
    
    switch (menu->getTag())
    {
        case st_dialog_right:
        {
            this->setButtonEnable();
            ATMInstance->sendEvent("rate_us", "rate_us_selection", "maybe later");
        }
            break;
            
        case st_dialog_left:
        {
            SSCInternalLibManager::getInstance()->rateUs();
            this->setButtonEnable();
            ATMInstance->sendEvent("rate_us", "rate_us_selection", "rate");
        }
            
            break;
            
        default:
            break;
    }
}



