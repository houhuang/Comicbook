//
//  ShwoCartoonInfoLayer.cpp
//  ComicBook
//
//  Created by houhuang on 17/12/4.
//
//

#include "ShwoCartoonInfoLayer.hpp"
#include "STVisibleRect.h"

ShowCartoonInfoLayer* ShowCartoonInfoLayer::create(CartoonInfo& info)
{
    ShowCartoonInfoLayer* layer = new ShowCartoonInfoLayer();
    if (layer && layer->init(info))
    {
        layer->autorelease();
        return layer;
    }
    
    CC_SAFE_DELETE(layer);
    return nullptr;
}

ShowCartoonInfoLayer::~ShowCartoonInfoLayer()
{
    _eventDispatcher->removeEventListenersForTarget(this);
}

ShowCartoonInfoLayer::ShowCartoonInfoLayer()
{
    _bgSprite = nullptr;
    _firstVec = Vec2(0, 0);
}

bool ShowCartoonInfoLayer::init(CartoonInfo& info)
{
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 200))) return false;
    
    _cartoonInfo = info;
    
    initUI();
    this->addListener();
    return true;
}

void ShowCartoonInfoLayer::initUI()
{
    Sprite* lSprite = Sprite::create("menu_bg.png");
    float oriScale = V::isIpad()? (this->getContentSize().width*0.8):(this->getContentSize().width*0.9);
    lSprite->setScale(oriScale/1018.);
    lSprite->setPosition(this->getContentSize()/2);
    this->addChild(lSprite);
    _bgSprite = lSprite;
    
    string path = FileUtils::getInstance()->getWritablePath() + "data/" +_cartoonInfo.folder + "_cover.png";
    Sprite* cover = Sprite::create(path);
    cover->setAnchorPoint(Vec2(0.5, 1));
    lSprite->addChild(cover);
    cover->setScale(1018./3/cover->getContentSize().width);
    cover->setPosition(Vec2(lSprite->getContentSize().width/4, lSprite->getContentSize().height - 85 - 56));
    
    
    string text = "名字：" + _cartoonInfo.name + "\n"
                    +"区域：" + _cartoonInfo.area + "\n"
                    +"类型：" + xCartoon->getCurrentCategory().name + "\n"
                    + "是否完结：是" + "\n"
                    + "页数：" + to_string(111);
    
    Label* label = Label::createWithTTF(text, "fonts/d2.ttf", 50);
    label->setAlignment(cocos2d::TextHAlignment::LEFT);
    label->setAnchorPoint(Vec2(0, 0.5));
    label->setPosition(Vec2(lSprite->getContentSize().width/2 - 20, lSprite->getContentSize().height * 0.75));
    label->setLineHeight(100);
    label->setColor(Color3B(76, 76, 76));
    lSprite->addChild(label);
    
    string desStr = "   " + _cartoonInfo.des;
    Label* des = Label::createWithTTF(desStr, "fonts/d2.ttf", 35);
    des->setAnchorPoint(Vec2(0.5, 1));
    des->setAlignment(cocos2d::TextHAlignment::LEFT);
    des->setDimensions(950, 500);
    des->setPosition(Vec2(lSprite->getContentSize().width/2, lSprite->getContentSize().height*0.6));
    des->setColor(Color3B(76, 76, 76));
    lSprite->addChild(des);
}


void ShowCartoonInfoLayer::addListener()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [&](Touch* touch, Event* event)->bool{
    
        _isClickThis = false;
        _firstVec = touch->getLocation();
        Vec2 lPos = _bgSprite->getParent()->convertToNodeSpace(touch->getLocation());
        if (_bgSprite->getBoundingBox().containsPoint(lPos))
        {
            _isClickThis = true;
        }
        return true;
    };
    
    listener->onTouchMoved = [&](Touch* touch, Event* event){
        
    };
    
    listener->onTouchEnded = [&](Touch* touch, Event* event){
        float distance = _firstVec.distance(touch->getLocation());
        if (distance < 10 && !_isClickThis)
        {
            Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("remove_cartoonLayer");
        }
    };
    
    listener->onTouchCancelled = [&](Touch* touch, Event* event){
        float distance = _firstVec.distance(touch->getLocation());
        if (distance < 10 && !_isClickThis)
        {
            Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("remove_cartoonLayer");
        }
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

