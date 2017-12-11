//
//  GraphSprite.cpp
//  ALABS0010
//
//  Created by houhuang on 16/3/29.
//
//

#include "GraphSprite.hpp"
#include "ThreadManager.hpp"
#include "LockManager.h"
#include "AppConfigs.h"

#define st_graphSprite_length   400
#define BG_SIZE                (440)

GraphSprite* GraphSprite::create(Graph & graph)
{
    GraphSprite* lSprite = new GraphSprite();
    if (lSprite && lSprite->init(graph))
    {
        lSprite->autorelease();
        return lSprite;
    }
    
    CC_SAFE_DELETE(lSprite);
    return NULL;
}

GraphSprite::GraphSprite()
{
    m_pRotateSprite = NULL;
    m_bIsLock = false;
    
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GraphSprite::loadGraphSprite), st_multiThread_loadImage_finish, NULL);

}

GraphSprite::~GraphSprite()
{
    NotificationCenter::getInstance()->removeObserver(this, st_multiThread_loadImage_finish);
    Director::getInstance()->getTextureCache()->removeTexture(m_saveTexture);
    Director::getInstance()->getTextureCache()->removeTexture(m_originalTexture);
}

bool GraphSprite::init(Graph & graph)
{
    if (!Node::init()) return false;
    
    m_gGraphInfo = graph;
    
    m_thumbs = graph.getThumbsName();
    auto path =  FileUtils::getInstance()->getWritablePath();
    if (FileUtils::getInstance()->isFileExist(path + graph.getHistoryName()))
    {
        m_save = graph.getHistoryName();
    }

    m_cBGColor = graph.bgColor;
    
    setContentSize(Size(st_graphSprite_length, st_graphSprite_length));
    setAnchorPoint(Vec2(0.5, 0.5));
    
    LayerColor * bg = LayerColor::create(Color4B::WHITE, BG_SIZE, BG_SIZE);
    bg->setPosition(Vec2((getContentSize() - bg->getContentSize())/2));
    addChild(bg);
    
    if (strcmp(graph.name.substr(0, 3).c_str(), "map") != 0)
    {
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
            Sprite* lSprite = Sprite::create("other/new.png");
            lSprite->setPosition(Vec2(st_graphSprite_length - 27, st_graphSprite_length + 25));
            addChild(lSprite, 10);
        }
    }else
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
            Sprite* lSprite = Sprite::create("other/new.png");
            lSprite->setPosition(Vec2(st_graphSprite_length - 27, st_graphSprite_length + 25));
            addChild(lSprite, 10);
        }
    }
    
    
    Sprite* lRotateSprite = Sprite::create("cocos/other/load_rotate_icon.png");
    lRotateSprite->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    addChild(lRotateSprite, 10);
    lRotateSprite->runAction(RepeatForever::create(RotateBy::create(1.0f, 250)));
    m_pRotateSprite = lRotateSprite;
    lRotateSprite->setScale(0.5f);
    
    return true;
}

void GraphSprite::display()
{
    Sprite* lSprite = Sprite::create("other/Gallery_5.png");
    lSprite->setPosition(Vec2(this->getContentSize()/2));
    lSprite->setScale(440/lSprite->getContentSize().width);
    this->addChild(lSprite);
    
    if (!m_save.empty())
    {
        m_saveTexture = Director::getInstance()->getTextureCache()->getTextureForKey(m_save);
    }
    else
    {
        m_originalTexture = Director::getInstance()->getTextureCache()->getTextureForKey(m_gGraphInfo.getPngName());
    }
    
    
    if (m_saveTexture || m_originalTexture)
    {
        imageLoadedCallback();
    }else
    {
        if (!m_save.empty())
        {
            xThread->m_vNeedLoadImage.push_back(m_save);
        }else
        {
            xThread->m_vNeedLoadImage.push_back(m_gGraphInfo.getPngName());
        }
        
        xThread->startLoadImage();
    }
}

void GraphSprite::imageLoadedCallback()
{
    Sprite* lSprite;
    
    if (!m_save.empty())
    {
        lSprite = Sprite::createWithTexture(m_saveTexture);
    }else
    {
        lSprite = Sprite::createWithTexture(m_originalTexture);
    }
    
    
    if (lSprite)
    {
        auto color = LayerColor::create(Color4B(m_gGraphInfo.bgColor), 435, 435);
        color->ignoreAnchorPointForPosition(false);
        color->setAnchorPoint(Vec2(0.5, 0.5));
        color->setPosition(Vec2(this->getContentSize()/2) );
        addChild(color);
        
        lSprite->setPosition(Vec2(this->getContentSize()/2));
        addChild(lSprite);
        
        lSprite->setScale(quickAdaptScale(lSprite, this));
        lSprite->setScale(lSprite->getScale() * 1.06);
        
        if (m_pRotateSprite)
        {
            m_pRotateSprite->removeFromParentAndCleanup(true);
            m_pRotateSprite = NULL;
        }
        
        log("%s",m_gGraphInfo.name.c_str());
        if (m_gGraphInfo.isLock && xLock->isLocked(m_gGraphInfo.name, LockManager::Type::GRAPH))
        {
            m_bIsLock = true;
            Sprite* lock = Sprite::create("cocos/other/graph_lock.png");
            lock->setPosition(Vec2(lSprite->getContentSize().width * 0.95, lSprite->getContentSize().height * 0.05));
            lock->setScale(1/lSprite->getScale());
            lSprite->addChild(lock);
        }
    }
}

void GraphSprite::loadGraphSprite(Ref* ref)
{
    m_originalTexture = Director::getInstance()->getTextureCache()->getTextureForKey(m_gGraphInfo.getPngName());
    m_saveTexture = Director::getInstance()->getTextureCache()->getTextureForKey(m_save);
    
    if ((m_save.empty() && m_originalTexture) ||
        (!m_save.empty() && m_saveTexture)) {
        imageLoadedCallback();
    }
}

bool GraphSprite::getCurrentGraphIslock()
{
    return m_bIsLock;
}
