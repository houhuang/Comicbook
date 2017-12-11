//
//  PickupMode.cpp
//  ColorGarden
//
//  Created by houhuang on 17/10/20.
//
//

#include "PickupMode.hpp"
#include "GameScene.h"

PickupMode::PickupMode(GameScene* layer):BaseMode(layer)
{

}

PickupMode::~PickupMode()
{
    stop();
}

void PickupMode::onTouchesBegan(const vector<Touch*>& touches, Event *unused_event)
{

}

void PickupMode::onTouchesMoved(const vector<Touch*>& touches, Event *unused_event)
{
    if (_pickupSpr)
    {
        Vec2 lPos = touches[0]->getLocation();
        Vec2 realPos = _zoomLayer->convertToNodeSpace(lPos);
        _pickupSpr->setPosition(realPos);
        _pickupSpr->setScale(1/_zoomLayer->getScale());
        _endMoveColor = Color3B(_gameScene->getPixelColor(lPos));
        
        _gameScene->updateColorBoard(_endMoveColor);
    }
}

void PickupMode::onTouchesEnded(const vector<Touch*>& touches, Event *unused_event)
{
    _gameScene->switchMode(Mode::NONE);
    _gameScene->setFingerCount(0);
}

void PickupMode::onTouchesCancelled(const vector<Touch*>& touches, Event *unused_event)
{
    _gameScene->switchMode(Mode::NONE);
    _gameScene->setFingerCount(0);
}

void PickupMode::start()
{
    
    _pickupSpr = (Sprite*)_zoomLayer->getChildByTag(88);
    if (_pickupSpr)
    {
        _pickupSpr->setColor(Color3B::WHITE);
        _pickupSpr->setScale(0);
        _pickupSpr->setOpacity(0);
        _pickupSpr->setVisible(true);
        _pickupSpr->runAction(Spawn::create( FadeIn::create(0.35f), EaseSineOut::create(ScaleTo::create(0.35, 1/_zoomLayer->getScale())  ), NULL));
        
        auto x = _gameScene->_paints[0].pos;
        Vec2 realPos = _zoomLayer->convertToNodeSpace(x);
        _pickupSpr->setPosition(realPos);
//        _pickupSpr->setScale(1/_zoomLayer->getScale());
        Color4B color = _gameScene->getPixelColor(_gameScene->getWaitBeginVec());

        _gameScene->updateColorBoard(Color3B(color));
    }
    
    _gameScene->_paints.clear();
    
}

void PickupMode::stop()
{
    if (_pickupSpr)
    {
//        _pickupSpr->setVisible(false);
        _pickupSpr->runAction(Spawn::create( FadeOut::create(0.35f), EaseSineOut::create(ScaleTo::create(0.35, 0) ), NULL));
    }
}