//
//  LoadingScene.cpp
//  ALABS0002NEW
//
//  Created by houhuang on 16/8/8.
//
//

#include "LoadingScene.hpp"
#include "STVisibleRect.h"
#include "SceneManager.h"
#include "NewDownloadManager.hpp"

bool LoadingScene::init()
{
    if (!BaseScene::init()) return false;
    
    Sprite* bg = Sprite::create("other/loading.png");
    bg->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    this->addChild(bg);
    float scale = this->getContentSize().height/bg->getContentSize().height > this->getContentSize().width/bg->getContentSize().width ? this->getContentSize().height/bg->getContentSize().height : this->getContentSize().width/bg->getContentSize().width;
    bg->setScale(scale);
    this->scheduleOnce(schedule_selector(LoadingScene::gotoChapterScene), 2.0f);
//
    
//    Sprite* lSprite = Sprite::create("other/popup@3x.png");
//    lSprite->setPosition(Vec2(this->getContentSize()/2) + Vec2(50, 200));
//    lSprite->setScale(this->getContentSize().width/1016.);
//    this->addChild(lSprite);
    
    return true;
}

void LoadingScene::gotoChapterScene(float dt)
{
    if (!_isForcedUpdate)
    {
        xScene->forward(SceneManager::Tag::CHAPTER);
    }
}

void LoadingScene::onEnterTransitionDidFinish()
{
    BaseScene::onEnterTransitionDidFinish();
    xNewDownLoad->downloadJsonFile();
}