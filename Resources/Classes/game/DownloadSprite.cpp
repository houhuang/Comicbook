//
//  DownloadSprite.cpp
//  ColorGarden
//
//  Created by houhuang on 17/2/16.
//
//

#include "DownloadSprite.hpp"
#include "ThreadManager.hpp"
#include "NewDownloadManager.hpp"
#include "LockManager.h"
#include "STVisibleRect.h"
#include "AppConfigs.h"

#define newVec  Vec2(26, _thisSize.height - (V::isIpad() ? 27: 25))

DownloadSprite* DownloadSprite::create(Graph& graph)
{
    DownloadSprite* lSprite = new DownloadSprite();
    if (lSprite && lSprite->init(graph))
    {
        lSprite->autorelease();
        return lSprite;
    }
    
    CC_SAFE_DELETE(lSprite);
    return nullptr;
}

DownloadSprite* DownloadSprite::create(std::string &url, std::string &name, std::string& redownload)
{
    DownloadSprite* lSprite = new DownloadSprite();
    if (lSprite && lSprite->init(url, name, redownload))
    {
        lSprite->autorelease();
        return lSprite;
    }
    CC_SAFE_DELETE(lSprite);
    return nullptr;
}

DownloadSprite::~DownloadSprite()
{
    NotificationCenter::getInstance()->removeObserver(this, st_multiThread_loadImage_finish);
}

DownloadSprite::DownloadSprite()
{
    m_bIsLock = false;
    m_bIsCover = false;
    m_bIsLoaded = false;
    
    m_bIsThreaded = false;
    m_bIsDownloaded = false;
    
    m_pRotateSprite = nullptr;
    _bgLayer = nullptr;
    
    m_sSave  = "";
    m_sThumb = "";
    m_sUrl   = "";
    m_sName  = "";
    m_sRedownload = "";
    
    m_pThumbTexture = nullptr;
    m_pSaveTexture = nullptr;
    
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(DownloadSprite::loadGraphSprite), st_multiThread_loadImage_finish, NULL);
    auto downEvent = EventListenerCustom::create("imageDownloaded", CC_CALLBACK_1(DownloadSprite::responseImageDownloaded, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(downEvent, this);
}

bool DownloadSprite::init(Graph& graph)
{
    if (!Node::init()) return false;
    
    m_gGraphInfo = graph;
    m_sThumb = graph.getThumbsName();
    m_sUrl = graph.url;
    m_sName = graph.name;

    _thisSize = Size(400, 400);
    
    //读取History
    graph.historyIndex = xUser->getIntegerForKey(string(graph.name + "_hIndex").c_str(), 0);
    graph.historyCount = xUser->getIntegerForKey(string(graph.name + "_hCount").c_str(), 1);
    
    std::string path = FileUtils::getInstance()->getWritablePath() + graph.getHistoryName();
    if (FileUtils::getInstance()->isFileExist(path.c_str()))
    {
        m_sSave = graph.getPreviewName();
    }
    
    this->setContentSize(_thisSize);
    this->setAnchorPoint(Vec2(0.5, 0));
    
    float scale = V::isIpad() ? 0.4f : 0.35f;
    if (strcmp(graph.name.substr(0, 3).c_str(), "map") == 0)
    {
        string userData = graph.name;
        string aa = userData.substr(3, userData.length());
        int index = stoi(aa);
        
        stringstream ss;
        ss<<"";
        ss<<graph.name;
        ss<<"_white.png_spare";
        
        string oldUser = graph.name + "_white.png";
        
        bool isNewImage = FileUtils::getInstance()->isFileExist(FileUtils::getInstance()->getWritablePath() + graph.getHistoryName());
        
        if (!UserDefault::getInstance()->getBoolForKey(ss.str().c_str(), false) && !UserDefault::getInstance()->getBoolForKey(oldUser.c_str(),false) && !isNewImage && index>20)
        {
            Sprite* lSprite = Sprite::create("other/new_lg.png");
            lSprite->setPosition(newVec);
            lSprite->setScale(scale);
            addChild(lSprite, 10);
        }
        
    }else if(graph.name.find("_1024") > 0 && graph.chapter_id > 0)
    {
        log("%lu", graph.name.find("_1024"));
        string userData = graph.name;
        string aa = userData.substr(0, 4);
        int index = stoi(aa);
        
        stringstream ss;
        ss<<"";
        ss<<"map";
        ss<<index + 20;
        ss<<"_white.png";
        
        string oldUser = graph.name + "_white.png";
        
        bool isNewImage = FileUtils::getInstance()->isFileExist(FileUtils::getInstance()->getWritablePath() + graph.getHistoryName());
        
        if (!UserDefault::getInstance()->getBoolForKey(ss.str().c_str(), false) && !UserDefault::getInstance()->getBoolForKey(oldUser.c_str(),false) && !isNewImage)
        {
            Sprite* lSprite = Sprite::create("other/new_lg.png");
            lSprite->setPosition(newVec);
            lSprite->setScale(scale);
            addChild(lSprite, 10);
        }

    }else
    {
        if (!UserDefault::getInstance()->getBoolForKey(graph.name.c_str(), false) )
        {
            Sprite* lSprite = Sprite::create("other/new_lg.png");
            lSprite->setPosition(newVec);
            lSprite->setScale(scale);
            addChild(lSprite, 10);
        }
    }
    
    Sprite* lRotateSpr = Sprite::create("cocos/other/load_rotate_icon.png");
    lRotateSpr->setScale(0.5f);
    lRotateSpr->setPosition(_thisSize/2);
    this->addChild(lRotateSpr, 10);
    lRotateSpr->runAction(RepeatForever::create(RotateBy::create(1.0f, 250)));
    m_pRotateSprite = lRotateSpr;
    lRotateSpr->setVisible(false);
    
    Sprite* lBg = Sprite::create("res/category_mask.png");
    lBg->setPosition(this->getContentSize()/2);
    lBg->setScale(this->getContentSize().width/606);
    this->addChild(lBg, 8);
    
    LayerColor* layer = LayerColor::create(Color4B(231, 231, 231, 255), 606, 606);
    layer->ignoreAnchorPointForPosition(false);
    layer->setAnchorPoint(Vec2(0.5, 0.5));
    layer->setPosition(Vec2(lBg->getContentSize()/2));
    //    layer->setScale(this->getContentSize().width/lBg->getContentSize().width);
    lBg->addChild(layer, -1);
    _bgLayer = layer;
    
    Sprite* loading = Sprite::create("other/content_loading_icon@3x.png");
    loading->setPosition(layer->getContentSize()/2);
    loading->setScale(layer->getContentSize().width*0.2/loading->getContentSize().width);
    layer->addChild(loading);
    loading->setTag(44);
    
    return true;
}

bool DownloadSprite::init(std::string &url, std::string &name, std::string& redownload)
{
    if (!Node::init()) return false;
    
    m_sUrl = url;
    m_sName = name;
    m_sThumb = name;
    m_sRedownload = redownload;
    _thisSize = Size(506, 607);
    
    m_bIsCover = true;
    
    this->setContentSize(_thisSize);
    this->setAnchorPoint(Vec2(0.5, 0));
    
    Sprite* lRotateSpr = Sprite::create("cocos/other/load_rotate_icon.png");
    lRotateSpr->setScale(0.5f);
    lRotateSpr->setPosition(_thisSize/2);
    this->addChild(lRotateSpr, 10);
    lRotateSpr->runAction(RepeatForever::create(RotateBy::create(1.0f, 250)));
    m_pRotateSprite = lRotateSpr;
    lRotateSpr->setVisible(false);
    
    Sprite* lBg = Sprite::create("other/library_mask.png");
    lBg->setPosition(this->getContentSize()/2);
//    lBg->setScale(this->getContentSize().width/lBg->getContentSize().width);
    this->addChild(lBg, 10);
    
    LayerColor* layer = LayerColor::create(Color4B(231, 231, 231, 255), 506, 506);
    layer->ignoreAnchorPointForPosition(false);
    layer->setAnchorPoint(Vec2(0.5, 0.5));
    layer->setPosition(Vec2(lBg->getContentSize().width/2, 369));
//    layer->setScale(this->getContentSize().width/lBg->getContentSize().width);
    lBg->addChild(layer, -1);
    _bgLayer = layer;
    
    Sprite* loading = Sprite::create("other/content_loading_icon@3x.png");
    loading->setPosition(layer->getContentSize()/2);
    loading->setScale(this->getContentSize().width*0.369/loading->getContentSize().width);
    layer->addChild(loading);
    loading->setTag(44);
    
    
    return true;
}

void DownloadSprite::display()
{
    
    if (!m_sSave.empty())
    {
        m_pSaveTexture = Director::getInstance()->getTextureCache()->getTextureForKey(m_sSave);
    }else
    {
        m_pThumbTexture = Director::getInstance()->getTextureCache()->getTextureForKey(m_sThumb);
    }
    
    if (m_pThumbTexture || m_pSaveTexture)
    {
        this->imageLoadedCallback();
    }else
    {
        if (!m_sSave.empty())
        {
            xThread->addNeedloadImage(m_sSave);

        }else
        {
            std::string userKey = m_sThumb + "_reDownload";
            std::string downloadKey = UserDefault::getInstance()->getStringForKey(userKey.c_str(), "00000");
            
            if (FileUtils::getInstance()->isFileExist(m_sThumb))
            {
                if (downloadKey != m_gGraphInfo.reDownload && m_gGraphInfo.reDownload != "NewResource")
                {
                    //download...
                    DownloadSpriteInfo info;
                    info.name = m_sName;
                    info.url = m_sUrl;
                    info.imageType = ImageType::PNG_THUMB_512;
                    xNewDownLoad->addNeedDownloadImage(info);
                }else
                {
                    m_bIsDownloaded = true;
                    xThread->addNeedloadImage(m_sThumb);
                }
                
            }else
            {
                //download...
                DownloadSpriteInfo info;
                info.name = m_sName;
                info.url = m_sUrl;
                info.imageType = ImageType::PNG_THUMB_512;
                xNewDownLoad->addNeedDownloadImage(info);
                
            }
        }
        
    }
    
}

void DownloadSprite::display_cover()
{
    
    if ((!m_sThumb.empty()) && Director::getInstance()->getTextureCache()->getTextureForKey(m_sThumb))
    {
        m_pThumbTexture = Director::getInstance()->getTextureCache()->getTextureForKey(m_sName);
    }
    
    if (m_pThumbTexture)
    {
        imageLoadedCallback_cover();
    }else
    {
        std::string userKey = m_sThumb + "_reDownload";
        std::string downloadKey = UserDefault::getInstance()->getStringForKey(userKey.c_str(), "00000");
        
        if (FileUtils::getInstance()->isFileExist(m_sThumb))
        {
            if (downloadKey != m_sRedownload && m_sRedownload != "NewResource")
            {
                DownloadSpriteInfo info;
                info.name = m_sName;
                info.url = m_sUrl;
                info.imageType = ImageType::PNG_COVER_512;
                xNewDownLoad->addNeedDownloadImage(info);
            }else
            {
                m_bIsDownloaded = true;
                xThread->addNeedloadImage(m_sThumb);
            }
            
        }else
        {
            DownloadSpriteInfo info;
            info.name = m_sName;
            info.url = m_sUrl;
            info.imageType = ImageType::PNG_COVER_512;
            xNewDownLoad->addNeedDownloadImage(info);
        }

    }
}

void DownloadSprite::addListener(Ref* target, SEL_CallFuncN selecter, bool isSwallow)
{
    _target = target;
    _callBack = selecter;
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(isSwallow);
    listener->onTouchBegan = [this](Touch* touch, Event* event)->bool{
        _firstPos = touch->getLocationInView();
//        log("%f",_firstPos.y);
        
        Vec2 lPos = Director::getInstance()->convertToGL(_firstPos);
        lPos = this->getParent()->convertToNodeSpace(lPos);
        
        Vec2 viewPos = touch->getLocation();
        if (viewPos.y < BOTTOM_HEIGHT || viewPos.y > V::getRealDesignSize().height - TOP_HEIGHT_HEIGHT)
        {
            return false;
        }
        
        if (this->getBoundingBox().containsPoint(lPos))
        {
            return true;
        }
        
        return false;
    };
    
    listener->onTouchMoved = [this](Touch* touch, Event* event)->void{
        
    };
    
    listener->onTouchEnded = [this](Touch* touch, Event* event)->void{
        Vec2 lPos = touch->getLocationInView();
        float distance = sqrt(pow(lPos.x - _firstPos.x, 2) + pow(lPos.y - _firstPos.y, 2));
        if (distance < 15)
        {
            (_target->*_callBack)(this);
        }
    };
    
    listener->onTouchCancelled = [this](Touch* touch, Event* event)->void{
        
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void DownloadSprite::responseImageDownloaded(EventCustom* event)
{
//    std::string name = *(std::string*)event->getUserData();
    
    if (m_bIsDownloaded) return;
    
    if (m_bIsCover)
    {
        std::string key = m_sThumb + "_reDownload";
        UserDefault::getInstance()->setStringForKey(key.c_str(), m_sRedownload);
        UserDefault::getInstance()->flush();
        this->display_cover();
    }else
    {
        std::string key = m_sThumb + "_reDownload";
        UserDefault::getInstance()->setStringForKey(key.c_str(), m_gGraphInfo.reDownload);
        UserDefault::getInstance()->flush();
        this->display();
    }
}

void DownloadSprite::imageLoadedCallback()
{
 
    Sprite* lSprite;
    if (!m_sSave.empty())
    {
        lSprite = Sprite::createWithTexture(m_pSaveTexture);
    }else
    {
        lSprite = Sprite::createWithTexture(m_pThumbTexture);
    }
    
    if (lSprite)
    {
        _bgLayer->setColor(Color3B(255, 255, 255));
        _bgLayer->addChild(lSprite);
        lSprite->setScale(_bgLayer->getContentSize().width/lSprite->getContentSize().width);
        lSprite->setPosition(_bgLayer->getContentSize()/2);
        
        Sprite* lS = (Sprite*)_bgLayer->getChildByTag(44);
        if (lS)
        {
            lS->removeFromParentAndCleanup(true);
        }
        
        if (m_pRotateSprite)
        {
            m_pRotateSprite->removeFromParentAndCleanup(true);
            m_pRotateSprite = nullptr;
        }
        
        if (m_gGraphInfo.isLock && xLock->isLocked(m_gGraphInfo.name, LockManager::Type::GRAPH))
        {
            m_bIsLock = true;
            Sprite* lock = Sprite::create("cocos/other/graph_lock.png");
            lock->setPosition(Vec2(lSprite->getContentSize().width * 0.95, lSprite->getContentSize().height * 0.05));
            lock->setScale(1/lSprite->getScale());
            lSprite->addChild(lock);
        }
        
        m_bIsLoaded = true;
    }
}

void DownloadSprite::imageLoadedCallback_cover()
{
    Sprite* lSprite;
    if (!m_sSave.empty())
    {
        lSprite = Sprite::createWithTexture(m_pSaveTexture);
    }else
    {
        lSprite = Sprite::createWithTexture(m_pThumbTexture);
    }
    
    if (lSprite)
    {

        lSprite->setPosition(_bgLayer->getContentSize()/2);
        lSprite->setScale(_bgLayer->getContentSize().width/lSprite->getContentSize().width);
        
        _bgLayer->setColor(Color3B(255, 255, 255));
        _bgLayer->addChild(lSprite);
        
        Sprite* lS = (Sprite*)_bgLayer->getChildByTag(44);
        if (lS)
        {
            lS->removeFromParentAndCleanup(true);
        }

        if (m_pRotateSprite)
        {
            m_pRotateSprite->removeFromParentAndCleanup(true);
            m_pRotateSprite = nullptr;
        }
        
        m_bIsLoaded = true;
    }
}

void DownloadSprite::loadGraphSprite(Ref* ref)
{
    if (m_bIsThreaded) return;
    
    m_pSaveTexture = Director::getInstance()->getTextureCache()->getTextureForKey(m_sSave);
    m_pThumbTexture = Director::getInstance()->getTextureCache()->getTextureForKey(m_sThumb);
    
    if ((!m_sSave.empty() && m_pSaveTexture) || (!m_sThumb.empty() && m_pThumbTexture))
    {
        m_bIsThreaded = true;
        
        if (m_bIsCover)
        {
            this->imageLoadedCallback_cover();
        }else
        {
            this->imageLoadedCallback();
        }
        
    }
}

void DownloadSprite::removeTextureCache()
{
    xNewDownLoad->removeNeedDownloadImage(m_sName);
    xThread->removeNeedloadImage(m_sThumb);
    
    xThread->removeNeedloadImage(m_sSave);
    Director::getInstance()->getTextureCache()->removeTexture(m_pSaveTexture);
    Director::getInstance()->getTextureCache()->removeTexture(m_pThumbTexture);
}

bool DownloadSprite::getCurrentGraphIslock()
{
    return m_bIsLock;
}

bool DownloadSprite::isDownloaded()
{
    return m_bIsLoaded;
}

