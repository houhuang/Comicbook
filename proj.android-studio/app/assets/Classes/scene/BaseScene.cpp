//
//  BaseScene.cpp
//  ColorGarden
//
//  Created by houhuang on 17/4/10.
//
//

#include "BaseScene.hpp"
#include "STSystemFunction.h"
#include "MyArtDataManager.hpp"
#include "DownLoadPage.h"
#include "NewDialog.hpp"
#include "SceneManager.h"

BaseScene::BaseScene()
{
    _firstClickBack = false;
    _time = 0.0;
}

BaseScene::~BaseScene()
{
    _eventDispatcher->removeEventListenersForTarget(this);
}

bool BaseScene::init()
{
    if (!Scene::init()) return false;
    
    _isForcedUpdate = false;
    
    auto forcedUpdateEvent = EventListenerCustom::create("forcedUpdate", CC_CALLBACK_1(BaseScene::responseForcedUpdate, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(forcedUpdateEvent, this);
    
    auto xxx = EventListenerCustom::create("setIsResponseBackToTrue", CC_CALLBACK_1(BaseScene::responseUpdateBackTrue, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(xxx, this);
    
    this->addBackListener();
    this->schedule(schedule_selector(BaseScene::timingClearTexture));
    return true;
}

void BaseScene::responseUpdateBackTrue(EventCustom* event)
{
    this->runAction(Sequence::create(DelayTime::create(0.01f), CallFunc::create([](){
        
        xScene->setIsResponseBack(true);
        
    }), NULL));
}

void BaseScene::setTouchEnable(bool isEnable)
{
    if (isEnable)
    {
        this->addBackListener();
    }else
    {
        _eventDispatcher->removeEventListenersForTarget(this);
    }
}

void BaseScene::addBackListener()
{
    auto listener = EventListenerKeyboard::create();
    listener->onKeyReleased = [this](EventKeyboard::KeyCode code, Event* event)
    {
        if(code == EventKeyboard::KeyCode::KEY_BACK)
        {
            if (_firstClickBack)
            {
//                _eventDispatcher->removeEventListenersForTarget(this);
                this->setTouchEnable(false);
                NewDialog* dialog = NewDialog::create("quit", "cancel", "ok");
                dialog->addButtonListener(CC_CALLBACK_1(BaseScene::responseBack, this));
                this->addChild(dialog, 101);
            }
            
            _firstClickBack = true;
            this->scheduleOnce(schedule_selector(BaseScene::updateClickBackState), 1.0f);
        }
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void BaseScene::updateClickBackState(float dt)
{
    _firstClickBack = false;
}

void BaseScene::resetFristClick()
{
    _firstClickBack = false;
}

void BaseScene::responseBack(const std::string& name)
{
    if(name == "ok"){
            
        xMyArtData->saveMyArtData();
        DownLoadPage::getInstance()->saveClickLikeInfo();
        Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        exit(0);
#endif
    }
    if (name == "cancel")
    {

    }
}

void BaseScene::responseForcedUpdate(EventCustom* event)
{
    _isForcedUpdate = true;
    xDialog->show("forcedUpdate", "", "ok", CC_CALLBACK_1(BaseScene::onDialog, this), 24);
    
    NewDialog* dialog = NewDialog::create("forcedUpdate", "", "ok");
    dialog->addButtonListener(CC_CALLBACK_1(BaseScene::onDialog, this));
    this->addChild(dialog, 101);
}

void BaseScene::onDialog(const std::string& name)
{
    //go to store
    STSystemFunction sf;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) // iOS
    sf.openAppStoreById("1040802159");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) // Android
    sf.openGooglePlayStore();
#endif
    
    Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void BaseScene::timingClearTexture(float detalTime)
{
    _time += detalTime;
    if (_time > 15.0)
    {
        _time = 0.0;
        Director::getInstance()->getTextureCache()->removeUnusedTextures();
    }
}