//
//  ShowLargeImageLayer.cpp
//  ColorGarden
//
//  Created by houhuang on 17/11/7.
//
//

#include "ShowLargeImageLayer.hpp"
#include "STVisibleRect.h"
#include "BaseScene.hpp"

ShowLargeImageLayer* ShowLargeImageLayer::create(const int imageID, float width)
{
    ShowLargeImageLayer* layer = new ShowLargeImageLayer();
    if (layer && layer->init(imageID, width))
    {
        layer->autorelease();
        return layer;
    }
    
    CC_SAFE_DELETE(layer);
    return nullptr;
}

ShowLargeImageLayer::~ShowLargeImageLayer()
{
    _eventDispatcher->removeEventListenersForTarget(this);
    _contentSprite = nullptr;
}

ShowLargeImageLayer::ShowLargeImageLayer()
{
    _contentSprite = nullptr;
    _isExcuteAnimation = false;
}

bool ShowLargeImageLayer::init(const int imageID, float width)
{
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 210))) return false;
    
    Sprite* lSprite = Sprite::create(std::to_string(imageID)+".png");
    
    _contentSprite = V::maskedSprite(lSprite);
    _contentSprite->setPosition(this->getContentSize()/2);
    _contentSprite->setScale(width/_contentSprite->getContentSize().width);
    this->addChild(_contentSprite);
    
//    auto lNode = V::drawRoundRect(lSprite, 32, 500);
//    lNode->setPosition(this->getContentSize()/2);
//    lNode->setScale(width/lNode->getContentSize().width);
//    this->addChild(lNode);
    
    this->scaleContent(0.15f, true);
    
    this->addListener();
    
    return true;
}

void ShowLargeImageLayer::addListener()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch* touch, Event* event)->bool{
        _firstPos = touch->getLocation();
        
        return true;
    };
    
    listener->onTouchMoved = [this](Touch* touch, Event* event)
    {
    
    };
    
    listener->onTouchEnded = [this](Touch* touch, Event* event){
        float distance = _firstPos.distance(touch->getLocation());
        if (distance < 5)
        {
            this->scaleContent(0.15f, false);
        }
        
    };
    
    listener->onTouchCancelled = [this](Touch* touch, Event* event){
        float distance = _firstPos.distance(touch->getLocation());
        if (distance < 5)
        {
            this->scaleContent(0.15f, false);
        }
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    auto listener2 = EventListenerKeyboard::create();
    listener2->onKeyReleased = [this](EventKeyboard::KeyCode code, Event* event)
    {
        if(code == EventKeyboard::KeyCode::KEY_BACK)
        {
            BaseScene* scene = (BaseScene*)Director::getInstance()->getRunningScene();
            if (scene)
            {
                scene->resetFristClick();
            }
            this->removeFromParentAndCleanup(true);
        }
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener2, this);
}


void ShowLargeImageLayer::scaleContent(const float duration, bool isEnlarge)
{
    _isExcuteAnimation = true;
    
    if (isEnlarge)
    {
        float scale = _contentSprite->getScale();
        _contentSprite->setOpacity(0);
        _contentSprite->setScale(0.0f);
        _contentSprite->runAction(Spawn::create(EaseSineIn::create(FadeIn::create(duration)), EaseSineOut::create(ScaleTo::create(duration, scale)), NULL) );
        
        this->setOpacity(0);
        this->runAction(Sequence::create(EaseSineIn::create(FadeTo::create(duration, 210)), CallFunc::create([this](){
            _isExcuteAnimation = false;
        }), NULL));
        
        
    }else
    {
        _contentSprite->runAction(Spawn::create(EaseSineIn::create(FadeOut::create(duration)), EaseSineIn::create(ScaleTo::create(duration, 0.0f)), NULL) );
        
        this->runAction(Sequence::create(EaseSineIn::create(FadeOut::create(duration)), CallFunc::create([this](){
            _isExcuteAnimation = false;
            this->removeFromParentAndCleanup(true);
        }), NULL));
    }
}







