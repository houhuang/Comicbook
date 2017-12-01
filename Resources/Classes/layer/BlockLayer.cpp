//
//  BlockLayer.cpp
//  ColorGarden
//
//  Created by luoxiaopeng on 15/8/4.
//
//

#include "BlockLayer.h"

bool BlockLayer::init()
{
    if(!LayerColor::initWithColor(Color4B(0, 0, 0, 0))) return false;
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](cocos2d::Touch *pTouch, cocos2d::Event *pEvent)->bool{
        return true;
    };
    listener->onTouchMoved = [](cocos2d::Touch *pTouch, cocos2d::Event *pEvent)->void{};
    listener->onTouchEnded = [](cocos2d::Touch *pTouch, cocos2d::Event *pEvent)->void{};
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void BlockLayer::setLoadingPosition(const Vec2 &pos)
{
    auto rotateSprite = Sprite::create("other/loading@2x.png");
    rotateSprite->setPosition(pos);
    rotateSprite->setScale(0.85f);
    addChild(rotateSprite);
    
    Label* loading = Label::createWithTTF("loading", "res/Muli-Regular.ttf", 44);
    loading->setPosition(pos - Vec2(0, 50));
    this->addChild(loading);
    loading->setColor(Color3B(113, 113, 113));
    loading->setScale(0.5f);
    
    RotateBy* lRotateBy = RotateBy::create(0.5f, 360.0f);
    RepeatForever* lRepeatForever=RepeatForever::create(lRotateBy);
    rotateSprite->runAction(lRepeatForever);
}