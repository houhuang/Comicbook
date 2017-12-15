//
//  SettingLayer.cpp
//  ComicBook
//
//  Created by houhuang on 17/12/14.
//
//

#include "SettingLayer.hpp"
#include "STVisibleRect.h"

enum{
    st_button_clearData = 10,
    st_button_continueRead,
};

SettingLayer* SettingLayer::create(Vec2 lPos)
{
    SettingLayer* layer = new SettingLayer();
    if (layer && layer->init(lPos))
    {
        layer->autorelease();
        return layer;
    }
    
    CC_SAFE_DELETE(layer);
    return nullptr;
}

SettingLayer::~SettingLayer()
{
    _eventDispatcher->removeEventListenersForTarget(this);
}

SettingLayer::SettingLayer()
{
    _contentSprite = nullptr;
    _isClickedContent = false;
    _firstPos = Vec2(0, 0);
}

bool SettingLayer::init(Vec2 lPos)
{
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 0)))  return false;
    
    this->runAction(FadeTo::create(0.15f, 100));
    
    initUI(lPos);
    addListener();
    return true;
}

void SettingLayer::initUI(Vec2 lPos)
{
    Sprite* lSprite = Sprite::create("setting.png");
    lSprite->setAnchorPoint(Vec2(0.127, 1));
    lSprite->setPosition(lPos + Vec2(-2, -20));
    addChild(lSprite);
    _contentSprite = lSprite;
    
    float scale = V::isIpad()? 0.3:0.5;
    
    lSprite->setScale(0.0);
    lSprite->runAction(ScaleTo::create(0.15, this->getContentSize().width*scale/lSprite->getContentSize().width));
    
    MenuItemImage* clearData = MenuItemImage::create("setting_btn.png", "setting_btn.png", CC_CALLBACK_1(SettingLayer::onButton, this));
    clearData->setPosition(Vec2(lSprite->getContentSize().width/2, 240));
    clearData->setTag(st_button_clearData);
    
    Label* clearData_label = Label::createWithTTF("清理数据", "fonts/d2.ttf", 90);
    clearData_label->setPosition(Vec2(clearData->getContentSize()/2));
    clearData_label->setColor(Color3B(76, 76, 76));
    clearData->addChild(clearData_label);
    
    MenuItemImage* continueReadMenuitem = MenuItemImage::create("setting_btn.png", "setting_btn.png", CC_CALLBACK_1(SettingLayer::onButton, this));
    continueReadMenuitem->setPosition(Vec2(lSprite->getContentSize().width/2, 91));
    continueReadMenuitem->setTag(st_button_continueRead);
    
    Label* continueReadMenuitem_label = Label::createWithTTF("继续阅读", "fonts/d2.ttf", 90);
    continueReadMenuitem_label->setPosition(Vec2(continueReadMenuitem->getContentSize()/2));
    continueReadMenuitem_label->setColor(Color3B(76, 76, 76));
    continueReadMenuitem->addChild(continueReadMenuitem_label);
    
    Menu* lMenu = Menu::create(clearData, continueReadMenuitem, NULL);
    lMenu->setPosition(Vec2::ZERO);
    lSprite->addChild(lMenu);
    
    
    
}

void SettingLayer::onButton(Ref* ref)
{
    MenuItemImage* lMenuItem = (MenuItemImage*)ref;
    switch (lMenuItem->getTag())
    {
        case st_button_clearData:
        {
            _eventDispatcher->dispatchCustomEvent(st_remove_settingLayer);
            _eventDispatcher->dispatchCustomEvent(st_showDialog_clearDataDialog);
        }
            break;
            
        case st_button_continueRead:
        {
            _eventDispatcher->dispatchCustomEvent(st_remove_settingLayer);
            _eventDispatcher->dispatchCustomEvent(st_showDialog_continueRead);
            
        }
            break;
            
        default:
            break;
    }
}

void SettingLayer::addListener()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch* touch, Event* event)->bool{
        _firstPos = touch->getLocation();
        _isClickedContent = false;
        
        Vec2 lPos = _contentSprite->getParent()->convertToNodeSpace(touch->getLocation());
        if (_contentSprite->boundingBox().containsPoint(lPos))
        {
            _isClickedContent = true;
        }

        return true;
    };
    
    listener->onTouchMoved = [this](Touch* touch, Event* event){
        
    };
    
    listener->onTouchEnded = [this](Touch* touch, Event* event){
        
        float distance = _firstPos.distance(touch->getLocation());
        
        if (distance < 10 && !_isClickedContent)
        {
            _eventDispatcher->dispatchCustomEvent(st_remove_settingLayer);
        }
    };
    
    listener->onTouchCancelled = [this](Touch* touch, Event* event){
        float distance = _firstPos.distance(touch->getLocation());
        
        if (distance < 10 && !_isClickedContent)
        {
            _eventDispatcher->dispatchCustomEvent(st_remove_settingLayer);
        }
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}
















