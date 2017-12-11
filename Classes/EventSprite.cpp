//
//  EventSprite.cpp
//  ComicBook
//
//  Created by houhuang on 17/12/8.
//
//

#include "EventSprite.hpp"

EventSprite* EventSprite::create(const string& name)
{
    EventSprite* lSprite = new EventSprite();
    if (lSprite && lSprite->init(name))
    {
        lSprite->autorelease();
        return lSprite;
    }
    
    CC_SAFE_DELETE(lSprite);
    return nullptr;
}

bool EventSprite::init(const string& name)
{
    if (!Sprite::init()) return false;
    
    Sprite* lSprite = Sprite::create(name);
    
    this->setContentSize(lSprite->getContentSize());
    lSprite->setPosition(this->getContentSize()/2);
    this->addChild(lSprite);
    
    return true;
}


void EventSprite::addListener(SEL_CallFuncN callBack, Node* target)
{
    _target = target;
    _callBack = callBack;
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = [this](Touch* touch, Event* event)->bool{
    
        _firstPos = touch->getLocation();
        Vec2 lPos = this->getParent()->convertToNodeSpace(touch->getLocation());
        if (this->getBoundingBox().containsPoint(lPos))
        {
            return true;
        }
        
        return false;
    };
    
    listener->onTouchMoved = [this](Touch* touch, Event* event){
        
    };
    
    listener->onTouchEnded = [this](Touch* touch, Event* event){
        float distance = _firstPos.distance(touch->getLocation());
        if (distance < 10)
        {
            (_target->*_callBack)(this);
        }
    };
    
    listener->onTouchCancelled = [this](Touch* touch, Event* event){
        float distance = _firstPos.distance(touch->getLocation());
        if (distance < 10)
        {
            (_target->*_callBack)(this);
        }
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}




