//
//  BaseMode.cpp
//  ColorBook
//
//  Created by luoxiaopeng on 4/24/15.
//
//

#include "BaseMode.h"
#include "UndoManager.h"
#include "GameScene.h"
#include "STVisibleRect.h"

BaseMode::BaseMode(GameScene * layer)
{
    _gameScene = layer;
    _zoomLayer = layer->getZoomLayer();
    _original = _zoomLayer->getBoundingBox();
    
    _gameScene->setFingerCount(0);
}

BaseMode::~BaseMode()
{
    
}

void BaseMode::onTouchesBegan(const vector<Touch*>& touches, Event *unused_event)
{
    _gameScene->setFingerCount(_gameScene->getFingerCount() + touches.size());
}

void BaseMode::onTouchesMoved(const vector<Touch*>& touches, Event *unused_event)
{
    
}

void BaseMode::onTouchesEnded(const vector<Touch*>& touches, Event *unused_event)
{
    int fingerCount = _gameScene->getFingerCount() - touches.size();

    if(fingerCount <= 0)
    {
        fingerCount = 0;
        if(!_gameScene->_paints.empty())
            xUndo->painted();
        _gameScene->paintPrevious();
        _gameScene->switchMode(Mode::NONE);
    }
    
    _gameScene->setFingerCount(fingerCount);
}

void BaseMode::onTouchesCancelled(const vector<Touch*>& touches, Event *unused_event)
{
    int fingerCount = _gameScene->getFingerCount() - touches.size();
    
    if(fingerCount <= 0)
    {
        fingerCount = 0;
        if(!_gameScene->_paints.empty())
            xUndo->painted();
        _gameScene->paintPrevious();
        _gameScene->switchMode(Mode::NONE);
    }
    
    _gameScene->setFingerCount(fingerCount);
}

void BaseMode::start()
{
    
}

void BaseMode::stop()
{
    
}

void BaseMode::checkBoundingBox()
{
    Rect current = _zoomLayer->getBoundingBox();
//    log("%f, %f",current.origin.x, current.origin.y);
    Vec2 offset = Vec2::ZERO;

    float offset1 = 270;
    if (V::isIpad())
    {
        if (current.getMinX() > _original.getMinX() -38 - offset1*((_zoomLayer->getScale()-1)/7) + _zoomLayer->getParent()->getContentSize().width/2)
        {
            offset.x =  _original.getMinX()- 38 - current.getMinX()- offset1*((_zoomLayer->getScale()-1)/7) + _zoomLayer->getParent()->getContentSize().width/2;
        }
        else if (current.getMaxX() < 38 + _original.getMaxX()+offset1*((_zoomLayer->getScale()-1)/7) - _zoomLayer->getParent()->getContentSize().width/2) {
            offset.x =  38 + _original.getMaxX() - current.getMaxX()+offset1*((_zoomLayer->getScale()-1)/7) - _zoomLayer->getParent()->getContentSize().width/2;
        }
        
        if (current.getMinY() > -38 + _original.getMinY() + _zoomLayer->getParent()->getContentSize().width/2) {
            offset.y = -38 + _original.getMinY() - current.getMinY() + _zoomLayer->getParent()->getContentSize().width/2;
        }
        else if (current.getMaxY() <38 + _original.getMaxY() - _zoomLayer->getParent()->getContentSize().width/2) {
            offset.y = 38+ _original.getMaxY() - current.getMaxY() - _zoomLayer->getParent()->getContentSize().width/2;
        }
    }else
    {
        if (current.getMinX() > _original.getMinX() + _zoomLayer->getParent()->getContentSize().width/2)
        {
            offset.x =  _original.getMinX() - current.getMinX() + _zoomLayer->getParent()->getContentSize().width/2;
        }
        else if (current.getMaxX() < _original.getMaxX() - _zoomLayer->getParent()->getContentSize().width/2) {
            offset.x =  _original.getMaxX() - current.getMaxX() - _zoomLayer->getParent()->getContentSize().width/2;
        }
        
        if (current.getMinY() > _original.getMinY() - 540*((_zoomLayer->getScale()-1)/7) + _zoomLayer->getParent()->getContentSize().width/2) {
            offset.y =  _original.getMinY() - current.getMinY() - 540*((_zoomLayer->getScale()-1)/7) + _zoomLayer->getParent()->getContentSize().width/2;
        }
        else if (current.getMaxY() < _original.getMaxY()+540*((_zoomLayer->getScale()-1)/7) - _zoomLayer->getParent()->getContentSize().width/2) {
            offset.y =  _original.getMaxY() - current.getMaxY()+ 540*((_zoomLayer->getScale()-1)/7) - _zoomLayer->getParent()->getContentSize().width/2;
        }
    }
    
    _zoomLayer->setPosition(_zoomLayer->getPosition() + offset);
    
    
//    log("%f   %f   %f", _zoomLayer->getPositionY(), current.size.height/2, _zoomLayer->getParent()->getContentSize().height/2);
//    if (_zoomLayer->getPositionX() > _zoomLayer->getParent()->getContentSize().width/2 + current.size.width/2)
//    {
//        _zoomLayer->setPositionX(_zoomLayer->getParent()->getContentSize().width/2 + current.size.width/2);
//    }else if (_zoomLayer->getPositionX() < _zoomLayer->getParent()->getContentSize().width/2 - current.size.width/2)
//    {
//        _zoomLayer->setPositionX(_zoomLayer->getParent()->getContentSize().width/2 - current.size.width/2);
//    }
//    
//    if (_zoomLayer->getPositionY() > 270 + _zoomLayer->getContentSize().height/2 + current.size.height/2)
//    {
//        _zoomLayer->setPositionY(270 + _zoomLayer->getContentSize().height/2 + current.size.height/2);
//    }
//    else if (_zoomLayer->getPositionY() < 270 + _zoomLayer->getContentSize().height/2 - current.size.height/2)
//    {
//        _zoomLayer->setPositionY(270 + _zoomLayer->getContentSize().height/2 - current.size.height/2);
//    }

    
}