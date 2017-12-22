//
//  BaseScene.cpp
//  ComicBook
//
//  Created by houhuang on 17/12/22.
//
//

#include "BaseScene.hpp"
#include "DownloadManager.h"
#include "NewDialog.h"

BaseScene::~BaseScene()
{
    _eventDispatcher->removeEventListenersForTarget(this);
}

BaseScene::BaseScene()
{

}

bool BaseScene::init()
{
    if (!Scene::init()) return false;
    
    auto getResurceFaildEvent = EventListenerCustom::create(st_download_rootCsv_faild, CC_CALLBACK_1(BaseScene::responseGetResourceFaild, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(getResurceFaildEvent, this);
    
    return true;
}

void BaseScene::responseGetResourceFaild(EventCustom* event)
{
    
}




