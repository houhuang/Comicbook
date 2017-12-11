//
//  ZoomMode.cpp
//  ColorBook
//
//  Created by luoxiaopeng on 4/24/15.
//
//

#include "ZoomMode.h"
#include "GameScene.h"

ZoomMode::ZoomMode(GameScene * layer) : BaseMode(layer)
{
}

ZoomMode::~ZoomMode()
{
    stop();
}

void ZoomMode::onTouchesBegan(const vector<Touch*>& touches, Event *unused_event)
{
    BaseMode::onTouchesBegan(touches, unused_event);
    
//    if(_canChange && _gameScene->getFingerCount() > 2)
//    {
//        _gameScene->switchMode(Mode::MOVE);
//    }
}

void ZoomMode::onTouchesMoved(const vector<Touch*>& touches, Event *unused_event)
{
    if(touches.size() < 2) return;
    
    
    {
        // 计算两点之间得距离
        auto currDistance = touches[0]->getLocation().distance(touches[1]->getLocation());//point1.distance(point2);
        // 计算两触摸点上一时刻之间得距离
        auto prevDistance = touches[0]->getPreviousLocation().distance(touches[1]->getPreviousLocation());
        
        //        if(currDistance<prevDistance)
        //        {
        //            currDistance=(prevDistance-currDistance)*0.6+currDistance;
        //        }
        
        float scale = _zoomLayer->getScale() * (currDistance / prevDistance);
        // 控制缩放倍率在1～4倍之间，最小倍率不能太小，不让背景将不能填充满整个屏幕。
        scale = MIN(8, MAX(0.5, scale));
        
        Vec2 mPoint1=_zoomLayer->convertToNodeSpace(touches[0]->getLocation());
        Vec2 mPoint2=_zoomLayer->convertToNodeSpace(touches[1]->getLocation());
        
        Vec2 centerPos=(mPoint1+mPoint2)/2;
        Vec2 offsetPos=Vec2(_zoomLayer->getContentSize().width/2, _zoomLayer->getContentSize().height/2)-centerPos;
        offsetPos*=_zoomLayer->getScale();
        Vec2 lAnchor=Vec2(centerPos.x/this->getContentSize().width, centerPos.y/this->getContentSize().height);
        
        
        _zoomLayer->setPosition(_zoomLayer->getPosition()-offsetPos);
        _zoomLayer->setAnchorPoint(lAnchor);
        
        
        _zoomLayer->setScale(scale);
        offsetPos=Vec2(_zoomLayer->getContentSize().width/2, _zoomLayer->getContentSize().height/2)-centerPos;
        offsetPos*=_zoomLayer->getScale();
        _zoomLayer->setPosition(_zoomLayer->getPosition()+offsetPos);
        _zoomLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
        
        
        
        auto prevCenter=(touches[0]->getPreviousLocation()+touches[1]->getPreviousLocation())*0.5f;
        auto currCneter=(touches[0]->getLocation()+touches[1]->getLocation())*0.5f;
        
        auto offset=currCneter-prevCenter;
        auto newPos=_zoomLayer->getPosition()+offset;
        
        if (ATMInstance->_paintMode == 1)
        {
            _zoomLayer->setPosition(newPos);
        }
        
        checkBoundingBox();
    }
}

void ZoomMode::start()
{
    //0.5秒内如果没有收到第3根手指, 就不再检查到move 模式
    Director::getInstance()->getScheduler()->schedule(schedule_selector(ZoomMode::timeout), this, 0, 0, 0.25, false);
    _canChange = true;
    _gameScene->_paints.clear();
}

void ZoomMode::stop()
{
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(ZoomMode::timeout), this);
}

void ZoomMode::timeout(float dt)
{
    _canChange = false;
}

bool ZoomMode::checkIsMoveMode(const vector<Touch*>& touches)
{
    //如果2只手指的方向几乎相同, 就判定为move mode
    Vec2 offset0 = touches[0]->getLocation() - touches[0]->getStartLocation();
    Vec2 offset1 = touches[1]->getLocation() - touches[1]->getStartLocation();
    
    if(offset0.distance(offset1) < 55)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void ZoomMode::resetPictureAnchorPoint(const std::vector<Touch *> &touches)
{
    if(touches.size() < 2) return;
    
    auto touch0 = touches[0]->getLocation();
    auto touch1 = touches[1]->getLocation();
    auto center = convertToWorldSpace((touch0 + touch1) / 2);
    center = _zoomLayer->convertToNodeSpace(center);
    
    //重新设置锚点
    Vec2 newAnchor;
    newAnchor.x = center.x / _zoomLayer->getContentSize().width;
    newAnchor.y = center.y / _zoomLayer->getContentSize().height;
    
    //动了锚点后, 重新设置坐标
    Vec2 offset;
    Vec2 oldAnchor = _zoomLayer->getAnchorPoint();
    offset.x = (newAnchor.x - oldAnchor.x) * _zoomLayer->getContentSize().width * _zoomLayer->getScale();
    offset.y = (newAnchor.y - oldAnchor.y) * _zoomLayer->getContentSize().height * _zoomLayer->getScale();
    
    _zoomLayer->setAnchorPoint(newAnchor);
    _zoomLayer->setPosition(_zoomLayer->getPosition() + offset);
}

bool ZoomMode::isOutOfRange(const vector<Touch*>& touches)
{
    for (auto touch : touches) {
        Vec2 current = _gameScene->convertToWorldSpace(touch->getLocation());
        Vec2 start = _gameScene->convertToWorldSpace(touch->getStartLocation());
        
        Vec2 distance = current - start;
        
        if (std::abs(distance.x) + std::abs(distance.y) > 40)
        {
            return true;
        }
    }
    
    return false;
}

void ZoomMode::setMaxScale(float scale)
{
    _maxScale = (1.f / scale) * 3.f;
}