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
}

bool CoverSprite::init(const string folder, string url, Size size)
{
    if (!Node::init()) return false;
    
    _folder = folder;
    _url = url;
    
    this->setContentSize(size);
    
    
    auto threadLoadedEvent = EventListenerCustom::create(st_multiThread_loadImage_finish, CC_CALLBACK_1(CoverSprite::LoadMapSprite, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(threadLoadedEvent, this);
    
    auto downLoadedEvent = EventListenerCustom::create(st_download_cover_finish, CC_CALLBACK_1(CoverSprite::responseDownloadSuc, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(downLoadedEvent, this);
    
    return true;
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
        float scale1 = this->getContentSize().width/lSprite->getContentSize().width;
        float scale2 = this->getContentSize().height/lSprite->getContentSize().height;
        float scale = scale1 > scale2 ? scale1 :scale2;
        
        lSprite->setScale(scale);
        lSprite->setPosition(this->getContentSize()/2);
        this->addChild(lSprite);
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
    string psth = FileUtils::getInstance()->getWritablePath() + _folder + "/cover.png";
    return psth;
}


