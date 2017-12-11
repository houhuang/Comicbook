//
//  WaitMode.cpp
//  ColorBook
//
//  Created by luoxiaopeng on 4/24/15.
//
//

#include "WaitMode.h"
#include "GameScene.h"


WaitMode::WaitMode(GameScene * layer) : BaseMode(layer)
{
    
}

WaitMode::~WaitMode()
{
    stop();
}

void WaitMode::onTouchesBegan(const vector<Touch*>& touches, Event *unused_event)
{
    BaseMode::onTouchesBegan(touches, unused_event);
    
    //收到2根手指, 等待结束后, 进入zoom模式
    if(_gameScene->getFingerCount() == 2)
    {
        log("enter to zoom");
        //立刻到zoom, 只在zoom, 里面判断3只手指
        _gameScene->switchMode(Mode::ZOOM);
    }
    //收到2根手指, 立刻进入move模式
    else if (_gameScene->getFingerCount() > 2)
    {
        _gameScene->switchMode(Mode::ZOOM);
    }
    
}

void WaitMode::onTouchesMoved(const vector<Touch*>& touches, Event *unused_event)
{
    //收集paints
    GameScene::Paint paint;
    paint.pos = _gameScene->convertToWorldSpace(touches[0]->getLocation());
    paint.previousPos = _gameScene->convertToWorldSpace(touches[0]->getPreviousLocation());
    //_gameScene->_paints.push_back(paint);
    
    //移动距离超过200, 结束waiting
    if (isOutOfRange(touches)) {
        _gameScene->setEnableReturnColor(false);
        timeout(0);
        
    }
}

void WaitMode::start()
{
    Director::getInstance()->getScheduler()->schedule(schedule_selector(WaitMode::gotoPickup), this, 0, 0, 0.3, false);
}

void WaitMode::stop()
{
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(WaitMode::timeout), this);
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(WaitMode::gotoPickup), this);
}

void WaitMode::gotoPickup(float dt)
{
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(WaitMode::gotoPickup), this);
    _gameScene->switchMode(BaseMode::Mode::PICKUP);
}

void WaitMode::timeout(float dt)
{
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(WaitMode::timeout), this);
    
    
    if (ATMInstance->_paintMode == 0)
    {
        _gameScene->switchMode(Mode::MOVE);
    }else
    {
        _gameScene->paintPrevious();
        _gameScene->switchMode(Mode::PAINT);
    }
   
}

bool WaitMode::isOutOfRange(const vector<Touch*>& touches)
{
    for (auto touch : touches) {
        Vec2 current = _gameScene->convertToWorldSpace(touch->getLocation());
        Vec2 start = _gameScene->convertToWorldSpace(touch->getStartLocation());
        
        Vec2 distance = current - start;
        
        if (std::abs(distance.x) + std::abs(distance.y) > 10)
        {
            return true;
        }
    }
    
    return false;
}