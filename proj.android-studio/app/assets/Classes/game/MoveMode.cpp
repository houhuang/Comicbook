//
//  MoveMode.cpp
//  ColorBook
//
//  Created by luoxiaopeng on 4/24/15.
//
//

#include "MoveMode.h"
#include "GameScene.h"

MoveMode::MoveMode(GameScene * layer) : BaseMode(layer)
{
}

void MoveMode::onTouchesMoved(const vector<Touch*>& touches, Event *unused_event)
{
    //singleFinger
    if (ATMInstance->_paintMode == 0)
    {
        Vec2 offset = touches[0]->getLocation() - touches[0]->getPreviousLocation();
        
        _zoomLayer->setPosition(_zoomLayer->getPosition() + offset);
        
        
        checkBoundingBox();
    }

}

void MoveMode::start()
{
    _gameScene->_paints.clear();
}