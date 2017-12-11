//
//  PostingLayer.cpp
//  ColorGarden
//
//  Created by houhuang on 17/4/5.
//
//

#include "PostingLayer.hpp"
#include "DownLoadPage.h"

PostingLayer* PostingLayer::create(const Color4B& color)
{
    PostingLayer* layer = new PostingLayer();
    if (layer && layer->init(color))
    {
        layer->autorelease();
        return layer;
    }
    
    CC_SAFE_DELETE(layer);
    return nullptr;
}

PostingLayer::PostingLayer():_time(0.0f)
{
    _label = nullptr;
}

bool PostingLayer::init(const Color4B& color)
{
    if (!SwallowListenerLayer::init(Color4B(0, 0, 0, 210))) return false;
    
    Label* label = Label::createWithTTF("Posting...", "res/Muli-Regular.ttf", 40 * 2);
    label->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2 - 50));
    this->addChild(label);
    label->setScale(0.5f);
    _label = label;
    
    auto rotateSprite = Sprite::create("other/loading@2x.png");
    rotateSprite->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    rotateSprite->setScale(0.7f);
    addChild(rotateSprite);
    
//    rotateSprite->runAction(RepeatForever::create(RotateBy::create(0.5f, 360)));
    
    
    this->schedule(schedule_selector(PostingLayer::update));
    return true;
}

void PostingLayer::update(float dt)
{
    _time += dt;
    
    if (_time > 20)
    {
        NotificationCenter::getInstance()->postNotification(st_myPost_failed);
    }
}

void PostingLayer::setPushSuccessed()
{
    if (_label)
    {
        _label->setString("Published!");
    }
    
    this->runAction(Sequence::create(DelayTime::create(1.0f), FadeOut::create(0.3f), CallFunc::create([this](){
        this->removeFromParentAndCleanup(true);
    }), NULL));
}