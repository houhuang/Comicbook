//
//  ReadScene.cpp
//  ComicBook
//
//  Created by houhuang on 17/12/6.
//
//

#include "ReadScene.hpp"
#include "STVisibleRect.h"
#include "CartoonManager.hpp"

#define THIS_SIZE   this->getContentSize()
#define MOVE_TIME   0.3f

enum
{
    st_button_right = 10,
    st_button_left
};

ReadScene::ReadScene()
{
    _leftLayer = nullptr;
    _centerLayer = nullptr;
    _rightLayer = nullptr;
    
    _isMoving = false;
}

bool ReadScene::init()
{
    if (!Scene::init()) return false;
    
    LayerColor* layer = LayerColor::create(Color4B(245, 245, 245, 255));
    this->addChild(layer);
    
    initUILayer();
    addListener();
    return true;
}

void ReadScene::initUILayer()
{
    LayerColor* leftLayer = LayerColor::create(Color4B(100, 0, 0, 100), THIS_SIZE.width, THIS_SIZE.height);
    leftLayer->ignoreAnchorPointForPosition(false);
    leftLayer->setAnchorPoint(Vec2(0.5, 0.5));
    leftLayer->setPosition(Vec2(-0.5*THIS_SIZE.width, THIS_SIZE.height/2));
    this->addChild(leftLayer);
    _leftLayer = leftLayer;
    
    LayerColor* centerLayer = LayerColor::create(Color4B(0, 100, 0, 100), THIS_SIZE.width, THIS_SIZE.height);
    centerLayer->ignoreAnchorPointForPosition(false);
    centerLayer->setAnchorPoint(Vec2(0.5, 0.5));
    centerLayer->setPosition(Vec2(0.5*THIS_SIZE.width, THIS_SIZE.height/2));
    this->addChild(centerLayer);
    _centerLayer = centerLayer;
    
    LayerColor* rightLayer = LayerColor::create(Color4B(0, 0, 100, 100), THIS_SIZE.width, THIS_SIZE.height);
    rightLayer->ignoreAnchorPointForPosition(false);
    rightLayer->setAnchorPoint(Vec2(0.5, 0.5));
    rightLayer->setPosition(Vec2(1.5*THIS_SIZE.width, THIS_SIZE.height/2));
    this->addChild(rightLayer);
    _rightLayer = rightLayer;
    
    MenuItemImage* lLeftMenu = MenuItemImage::create("read_btn.png", "read_btn.png", CC_CALLBACK_1(ReadScene::onButton, this));
    lLeftMenu->setAnchorPoint(Vec2(0, 0.5));
    V::setPosition(lLeftMenu, 0, 300, false, false, true, false);
    lLeftMenu->setTag(st_button_left);
    
    MenuItemImage* lRightMenu = MenuItemImage::create("read_btn.png", "read_btn.png", CC_CALLBACK_1(ReadScene::onButton, this));
    lRightMenu->setAnchorPoint(Vec2(1, 0.5));
    V::setPosition(lRightMenu, 640, 300, false, false, false, true);
    lRightMenu->setTag(st_button_right);
    
    Menu* lMenu = Menu::create(lLeftMenu, lRightMenu, nullptr);
    lMenu->setPosition(Vec2::ZERO);
    this->addChild(lMenu);
}


void ReadScene::onButton(Ref* ref)
{
    MenuItemImage* lMenuItem = (MenuItemImage*)ref;
    switch (lMenuItem->getTag())
    {
        case st_button_left:
        {
            if (!_isMoving)
            {
                _isMoving = true;
                resetCenterLayer();
                towardRightMove();
            }
        }
            break;
            
        case st_button_right:
        {
            if (!_isMoving)
            {
                _isMoving = true;
                resetCenterLayer();
                towardLeftMove();
            }
            
        }
            break;
            
        default:
            break;
    }
}

void ReadScene::towardLeftMove()
{
    _leftLayer->setPosition(Vec2(1.5*THIS_SIZE.width, THIS_SIZE.height/2));
    _centerLayer->runAction(EaseSineOut::create(MoveTo::create(MOVE_TIME, Vec2(-0.5*THIS_SIZE.width, THIS_SIZE.height/2))));
    _rightLayer->runAction(Sequence::create(EaseSineOut::create(MoveTo::create(MOVE_TIME, Vec2(0.5*THIS_SIZE.width, THIS_SIZE.height/2))), CallFunc::create([this](){
        this->resetLayerPointer(true);
        _isMoving = false;
    }), NULL));
}

void ReadScene::towardRightMove()
{
    _rightLayer->setPosition(Vec2(-0.5*THIS_SIZE.width, THIS_SIZE.height/2));
    _centerLayer->runAction(EaseSineOut::create(MoveTo::create(MOVE_TIME, Vec2(1.5*THIS_SIZE.width, THIS_SIZE.height/2))));
    _leftLayer->runAction(Sequence::create(EaseSineOut::create(MoveTo::create(MOVE_TIME, Vec2(0.5*THIS_SIZE.width, THIS_SIZE.height/2))), CallFunc::create([this](){
        this->resetLayerPointer(false);
        _isMoving = false;
    }), NULL));
}

void ReadScene::resetLayerPointer(bool towardLeftMove)
{
    if (towardLeftMove)
    {
        LayerColor* layer = _leftLayer;
        _leftLayer = _centerLayer;
        _centerLayer = _rightLayer;
        _rightLayer = layer;
    }else
    {
        LayerColor* layer = _rightLayer;
        _rightLayer = _centerLayer;
        _centerLayer = _leftLayer;
        _leftLayer = layer;
    }
}

void ReadScene::addListener()
{
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = [&](const vector<Touch*>& touches, Event* events){
        

    };
    
    listener->onTouchesMoved = [&](const vector<Touch*>& touches, Event* events){
        
        if (touches.size() == 1)
        {
            _centerLayer->setPosition(_centerLayer->getPosition() + touches[0]->getDelta());
        }else if (touches.size() == 2)
        {
            auto currDistance = touches[0]->getLocation().distance(touches[1]->getLocation());
            auto prevDistance = touches[0]->getPreviousLocation().distance(touches[1]->getPreviousLocation());
         
            float scale = _centerLayer->getScale() * (currDistance / prevDistance);
            scale = MIN(2, MAX(0.5, scale));
            
            Vec2 mPoint1=_centerLayer->convertToNodeSpace(touches[0]->getLocation());
            Vec2 mPoint2=_centerLayer->convertToNodeSpace(touches[1]->getLocation());
            
            Vec2 centerPos=(mPoint1+mPoint2)/2;
            Vec2 offsetPos=Vec2(_centerLayer->getContentSize().width/2, _centerLayer->getContentSize().height/2)-centerPos;
            offsetPos*=_centerLayer->getScale();
            
            Vec2 lAnchor=Vec2(centerPos.x/this->getContentSize().width, centerPos.y/this->getContentSize().height);
            
            _centerLayer->setPosition(_centerLayer->getPosition()-offsetPos);
            _centerLayer->setAnchorPoint(lAnchor);
            _centerLayer->setScale(scale);
            offsetPos=Vec2(_centerLayer->getContentSize().width/2, _centerLayer->getContentSize().height/2)-centerPos;
            offsetPos*=_centerLayer->getScale();
            _centerLayer->setPosition(_centerLayer->getPosition()+offsetPos);
            _centerLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
        
            
            auto prevCenter=(touches[0]->getPreviousLocation()+touches[1]->getPreviousLocation())*0.5f;
            auto currCneter=(touches[0]->getLocation()+touches[1]->getLocation())*0.5f;
            
            auto offset=currCneter-prevCenter;
            auto newPos=_centerLayer->getPosition()+offset;
            _centerLayer->setPosition(newPos);
        }
        
        if (_centerLayer->getPositionX() < (THIS_SIZE.width/2 - _centerLayer->getBoundingBox().size.width/2))
        {
            _centerLayer->setPositionX((THIS_SIZE.width/2 - _centerLayer->getBoundingBox().size.width/2));
        }
        
        if (_centerLayer->getPositionX() > (THIS_SIZE.width/2 + _centerLayer->getBoundingBox().size.width/2))
        {
            _centerLayer->setPositionX(THIS_SIZE.width/2 + _centerLayer->getBoundingBox().size.width/2);
        }
        
        if (_centerLayer->getPositionY() < (THIS_SIZE.height/2 - _centerLayer->getBoundingBox().size.height/2))
        {
            _centerLayer->setPositionY(THIS_SIZE.height/2 - _centerLayer->getBoundingBox().size.height/2);
        }
        
        if (_centerLayer->getPositionY() > (THIS_SIZE.height/2 + _centerLayer->getBoundingBox().size.height/2))
        {
            _centerLayer->setPositionY(THIS_SIZE.height/2 + _centerLayer->getBoundingBox().size.height/2);
        }
    };
    
    listener->onTouchesEnded = [&](const vector<Touch*>& touches, Event* events){
        
    };
    
    listener->onTouchesCancelled = [&](const vector<Touch*>& touches, Event* events){
        
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}


void ReadScene::resetCenterLayer()
{
    _centerLayer->setScale(1.0f);
    _centerLayer->setPosition(Vec2(0.5*THIS_SIZE.width, THIS_SIZE.height/2));
}







