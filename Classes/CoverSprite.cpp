//
//  CoverSprite.cpp
//  ComicBook
//
//  Created by houhuang on 17/12/1.
//
//

#include "CoverSprite.hpp"
#include "ThreadManager.hpp"
#include "DownloadManager.hpp"

CoverSprite* CoverSprite::create(const string folder, string url, Size size)
{
    CoverSprite* lNode = new CoverSprite();
    if (lNode && lNode->init(folder, url, size))
    {
        lNode->autorelease();
        return lNode;
    }
    
    CC_SAFE_DELETE(lNode);
    return nullptr;
}

CoverSprite::~CoverSprite()
{
    _eventDispatcher->removeEventListenersForTarget(this);
}

CoverSprite::CoverSprite()
{
    _folder = "";
    _url = "";
    
    _firstVec = Vec2(0, 0);
    _target = nullptr;
    _callBack = nullptr;
    _loadedSuc = false;
}

bool CoverSprite::init(const string folder, string url, Size size)
{
    if (!Node::init()) return false;
    
    _folder = folder;
    _url = url;
    
    this->setContentSize(Size(346, 608));
    
    Sprite* lbg = Sprite::create("library_mask.png");
    lbg->setPosition(this->getContentSize()/2);
    this->addChild(lbg, 10);
    
    auto threadLoadedEvent = EventListenerCustom::create(st_multiThread_loadImage_finish, CC_CALLBACK_1(CoverSprite::LoadMapSprite, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(threadLoadedEvent, this);
    
    auto downLoadedEvent = EventListenerCustom::create(st_download_cover_finish, CC_CALLBACK_1(CoverSprite::responseDownloadSuc, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(downLoadedEvent, this);
    
    return true;
}

void CoverSprite::addListener(Ref* target, SEL_CallFuncO callBack, bool isSwallow)
{
    _target = target;
    _callBack = callBack;
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(isSwallow);
    listener->onTouchBegan = [&](Touch* touch, Event* event)->bool{
        Vec2 lPos = touch->getLocation();
        _firstVec = lPos;
        lPos = this->getParent()->convertToNodeSpace(lPos);
        if (this->getBoundingBox().containsPoint(lPos))
        {
            return true;
        }
        
        return false;
    };
    
    listener->onTouchMoved = [&](Touch* touch, Event* event){
        
    };
    
    listener->onTouchEnded = [&](Touch* touch, Event* event){
        float distance = _firstVec.distance(touch->getLocation());
        if (distance < 10 && _loadedSuc)
        {
            (_target->*_callBack)(this);
        }
    };
    
    listener->onTouchCancelled = [&](Touch* touch, Event* event){
        float distance = _firstVec.distance(touch->getLocation());
        if (distance < 10 && _loadedSuc)
        {
            (_target->*_callBack)(this);
        }
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void CoverSprite::diaplay()
{

    if (FileUtils::getInstance()->isFileExist(this->getFullPathFromFolder()))
    {
        Texture2D* lTexture = Director::getInstance()->getTextureCache()->getTextureForKey(this->getFullPathFromFolder());
        if (lTexture)
        {
            this->imageLoadedCallback(lTexture);
        }else
        {
            xThread->addNeedloadImage(this->getFullPathFromFolder());

        }
    }else
    {
        //download
        DownloadCoverInfo info;
        info.url = _url;
        info.folder = _folder;
        xDownload->addNeedDownloadCover(info);
    }
}

void CoverSprite::imageLoadedCallback(Texture2D* texture)
{
    Sprite* lSprite = Sprite::createWithTexture(texture);
    if (lSprite)
    {
        lSprite->setScaleX(this->getContentSize().width/lSprite->getContentSize().width);
        lSprite->setScaleY(504./lSprite->getContentSize().height);
        lSprite->setAnchorPoint(Vec2(0.5, 1));
        lSprite->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height));

        this->addChild(lSprite);
        _loadedSuc = true;
    }
}

void CoverSprite::LoadMapSprite(EventCustom* event)
{
    Texture2D* texture = Director::getInstance()->getTextureCache()->getTextureForKey(this->getFullPathFromFolder());
    if (texture)
    {
        this->imageLoadedCallback(texture);
    }
}

void CoverSprite::responseDownloadSuc(EventCustom* event)
{
    if (FileUtils::getInstance()->isFileExist(this->getFullPathFromFolder()))
    {
        xThread->addNeedloadImage(this->getFullPathFromFolder());
    }
}

string CoverSprite::getFullPathFromFolder()
{
//    string psth = FileUtils::getInstance()->getWritablePath() + "cover/" + _folder + "_cover.png";
    string psth = "cover/" + _folder + "_cover.png";
    return psth;
}


