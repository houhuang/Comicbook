//
//  ContentLayer.cpp
//  ComicBook
//
//  Created by houhuang on 17/12/6.
//
//

#include "ContentLayer.h"
#include "ThreadManager.h"
#include "DownloadManager.h"

ContentLayer* ContentLayer::create(Picture& picture)
{
    ContentLayer* layer = new ContentLayer();
    if (layer && layer->init(picture))
    {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return nullptr;
}

ContentLayer::ContentLayer()
{
    _contentSprite = nullptr;
    _rotateSprite = nullptr;
    
    _alreadyLoaded = false;
    _preImagePath = "";
}

bool ContentLayer::init(Picture& picture)
{
    if (!LayerColor::initWithColor(Color4B(245, 245, 245, 255))) return false;
    
    this->setContentSize(Size(this->getContentSize().width, this->getContentSize().height-100));
    this->ignoreAnchorPointForPosition(false);
    this->setAnchorPoint(Vec2(0.5, 0.5));
    
    _picture = picture;
    
    auto threadLoadedEvent = EventListenerCustom::create(st_multiThread_loadImage_finish, CC_CALLBACK_1(ContentLayer::LoadMapSprite, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(threadLoadedEvent, this);
    
    auto downLoadedEvent = EventListenerCustom::create(st_download_picture_suc, CC_CALLBACK_1(ContentLayer::responseDownloadSuc, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(downLoadedEvent, this);
    
    Sprite* rotate = Sprite::create("load_rotate_icon.png");
    rotate->setPosition(this->getContentSize()/2);
    rotate->setVisible(false);
    this->addChild(rotate);
    rotate->runAction(RepeatForever::create(RotateBy::create(1, 360)));
    _rotateSprite = rotate;
    
    return true;
}

void ContentLayer::onEnterTransitionDidFinish()
{
    LayerColor::onEnterTransitionDidFinish();
    
    display();
}


void ContentLayer::updateContent(Picture& picture)
{
    if (_contentSprite)
    {
        _contentSprite->removeFromParent();
        _contentSprite = nullptr;
        
        Director::getInstance()->getTextureCache()->removeTextureForKey(_preImagePath);
    }
    
    _alreadyLoaded = false;
    _picture = picture;
    display();
}

void ContentLayer::display()
{
    _rotateSprite->setVisible(true);
    
    if (FileUtils::getInstance()->isFileExist(getPath()))
    {
        Texture2D* lTexture = Director::getInstance()->getTextureCache()->getTextureForKey(getPath());
        if (lTexture)
        {
            imageLoadedCallback(lTexture);
        }else
        {
            xThread->addNeedloadImage(getPath());
        }
    }else
    {
        xDownload->downloadPicture(_picture);
    }
}

void ContentLayer::imageLoadedCallback(Texture2D* texture)
{
    
    Sprite* lSprite = Sprite::createWithTexture(texture);
    if (lSprite)
    {
        float scaleX = this->getContentSize().width/lSprite->getContentSize().width;
        float scaleY = this->getContentSize().height/lSprite->getContentSize().height;
        float scale;
        if (xCartoon->getReadMode() == CartoonManager::Mode::VERTICAL)
        {
            scale =this->getContentSize().height/lSprite->getContentSize().height;
        }else if (xCartoon->getReadMode() == CartoonManager::Mode::HORIZONTAL)
        {
            scale = this->getContentSize().width/lSprite->getContentSize().width;
        }else if (xCartoon->getReadMode() == CartoonManager::Mode::AUTOMODE)
        {
            scale = scaleX < scaleY ? scaleX : scaleY;
        }
        
        lSprite->setScale(scale);
        lSprite->setPosition(this->getContentSize()/2);
        this->addChild(lSprite);
        
        _contentSprite = lSprite;
        _alreadyLoaded = true;
        _preImagePath = getPath();
        _rotateSprite->setVisible(false);
    }
}

void ContentLayer::LoadMapSprite(EventCustom* event)
{
    if (_alreadyLoaded)
    {
        return;
    }
    
    Texture2D* lTexture = Director::getInstance()->getTextureCache()->getTextureForKey(getPath());
    if (lTexture)
    {
        imageLoadedCallback(lTexture);
    }
}

void ContentLayer::responseDownloadSuc(EventCustom* event)
{
    if (_alreadyLoaded)
    {
        return;
    }
    
    if (FileUtils::getInstance()->isFileExist(getPath()))
    {
        xThread->addNeedloadImage(getPath());
    }
}

string ContentLayer::getPath()
{
    string path = "picture/" + xCartoon->getCurrentFolder() + "/" + _picture.chaID + "_" + _picture.index + ".jpg";
    return path;
}








