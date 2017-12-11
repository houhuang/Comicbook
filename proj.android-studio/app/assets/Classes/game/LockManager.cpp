//
//  LockManager.cpp
//  ColorBook
//
//  Created by luoxp on 4/24/15.
//
//

#include "LockManager.h"
#include "UITools.h"
#include "RewardAdLoadingLayer.h"
#include "STSystemFunction.h"
#include "IAPManager.h"
#include "GraphScene.h"
#include "GameScene.h"
#include "UICpp.h"
#include "GameScene.h"
#include "BannerManager.hpp"

#define TWODAYS         (172800)

static LockManager *_sharedLock = nullptr;
bool LockManager::_isShowRateUs = false;

LockManager* LockManager::getInstance()
{
    if (! _sharedLock)
    {
        _sharedLock = new LockManager();
    }
    
    return _sharedLock;
}

void LockManager::destroyInstance()
{
    CC_SAFE_DELETE(_sharedLock);
}

LockManager::LockManager()
{
    auto event_reward = EventListenerCustom::create(EVENT_REWARD, CC_CALLBACK_1(LockManager::rewardAdSuccessful, this));
    _eventDispatcher->addEventListenerWithFixedPriority(event_reward, 1);
}

LockManager::~LockManager()
{
    _scheduler->unschedule(schedule_selector(LockManager::showBanner), this);
    _scheduler->unscheduleUpdate(this);
    _eventDispatcher->removeEventListenersForTarget(this);
}

void LockManager::unlock(const string & identifier, Type type)
{
    _identifier = identifier;
    _type = type;
    
    auto rewardAd = RewardAdLoadingLayer::create();
    Director::getInstance()->getRunningScene()->addChild(rewardAd, 100);
}

void LockManager::rewardAdSuccessful(EventCustom* event)
{
    setUnLockTime();
    _scheduler->scheduleUpdate(this, 0, false);
}

void LockManager::update(float dt)
{
    _scheduler->unscheduleUpdate(this);
    
    auto scene = Director::getInstance()->getRunningScene();
    if (scene->getName() == "GraphScene") {
        ((GraphScene*)scene)->reloadData();
    }
    else if (scene->getName() == "GameScene")
    {
        //TBD
        ((GameScene*)scene)->changePalletteName();
    }
    
    _scheduler->schedule(schedule_selector(LockManager::showBanner), this, 4.f, false);
}

void LockManager::showBanner(float dt)
{
    _scheduler->unschedule(schedule_selector(LockManager::showBanner), this);
    
    auto scene = Director::getInstance()->getRunningScene();
    string name = scene->getName();
    if (scene && name != "ChapterScene" && name != "InspirationScene" && name != "MyArtsScene") {
//        xAds->showAds(kTypeBannerAds);
        xBanner->show();
    }
}

bool LockManager::isLocked(const string& identifier, Type type)
{
    if (xIAP->isUnlockAll())
        return false;
    else
    {
        if (type == Type::PALLETTE)
        {
            return offsetTime_pallette(getUnLockTime(identifier, type));
        }else
        {
            return offsetTime(getUnLockTime(identifier, type));
        }
    }
    
}

void LockManager::setUnLockTime()
{
    if (_type == Type::GRAPH) {
        xUser->setStringForKey(string("graph_" + _identifier).c_str(), getCurrentTime());
    }
    else
    {
        xUser->setStringForKey(string("pallette_" + _identifier).c_str(), getCurrentTime());
    }
    
    xUser->flush();
}

string LockManager::getUnLockTime(const string & identifier, Type type)
{
    if (type == Type::GRAPH) {
        return xUser->getStringForKey(string("graph_" + identifier).c_str(), "0000-00-00 00-00-00");
    }
    else
    {
        return xUser->getStringForKey(string("pallette_" + identifier).c_str(), "0000-00-00 00-00-00");
    }
}

bool LockManager::offsetTime(string time)
{
    
    struct tm now;
    struct tm late;
    string now_time = getCurrentTime();
    now.tm_year = stod(now_time.substr(0,4));
    now.tm_mon = stod(now_time.substr(5,2));
    now.tm_mday = stod(now_time.substr(8,2));
    now.tm_hour = stod(now_time.substr(11,2));
    now.tm_min = stod(now_time.substr(14,2));
    now.tm_sec = stod(now_time.substr(17,2));
    
    late.tm_year = stod(time.substr(0,4));
    late.tm_mon = stod(time.substr(5,2));
    late.tm_mday = stod(time.substr(8,2));
    late.tm_hour = stod(time.substr(11,2));
    late.tm_min = stod(time.substr(14,2));
    late.tm_sec = stod(time.substr(17,2));
    //返回真为超过时间  因该加锁
    if (now.tm_year - late.tm_year > 10) {
        return true;
    }
//    else if(now.tm_mon > late.tm_mon)
//    {
//        return true;
//    }
//    else if(now.tm_mday - late.tm_mday > 2)
//    {
//        return true;
//    }
//    else
//    {
//        if ((now.tm_mday * 86400 + now.tm_hour * 3600 + now.tm_min * 60 + now.tm_sec) - (late.tm_mday * 86400 + late.tm_hour * 3600 + late.tm_min * 60 + late.tm_sec) > TWODAYS) {
//            return true;
//        }
//    }
    return false;
}

bool LockManager::offsetTime_pallette(string time)
{
    struct tm now;
    struct tm late;
    string now_time = getCurrentTime();
    now.tm_year = stod(now_time.substr(0,4));
    now.tm_mon = stod(now_time.substr(5,2));
    now.tm_mday = stod(now_time.substr(8,2));
    now.tm_hour = stod(now_time.substr(11,2));
    now.tm_min = stod(now_time.substr(14,2));
    now.tm_sec = stod(now_time.substr(17,2));
    
    late.tm_year = stod(time.substr(0,4));
    late.tm_mon = stod(time.substr(5,2));
    late.tm_mday = stod(time.substr(8,2));
    late.tm_hour = stod(time.substr(11,2));
    late.tm_min = stod(time.substr(14,2));
    late.tm_sec = stod(time.substr(17,2));
    //返回真为超过时间  因该加锁
    if (now.tm_year > late.tm_year) {
        return true;
    }
    else if(now.tm_mon - late.tm_mon >= 1)
    {
        return true;
    }
    else if(now.tm_mday - late.tm_mday >= 30)
    {
        return true;
    }
//    else
//    {
//        if ((now.tm_mday * 86400 + now.tm_hour * 3600 + now.tm_min * 60 + now.tm_sec) - (late.tm_mday * 86400 + late.tm_hour * 3600 + late.tm_min * 60 + late.tm_sec) > TWODAYS) {
//            return true;
//        }
//    }
    return false;
}
