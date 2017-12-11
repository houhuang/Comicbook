//
//  MyArtSprite.cpp
//  ALABS0010
//
//  Created by houhuang on 16/3/31.
//
//

#include "MyArtSprite.hpp"
#include "ThreadManager.hpp"
#include "UITools.h"
#include "UICpp.h"
#include "MyArtDataManager.hpp"
#include "ChapterManager.hpp"
#include "DownLoadPage.h"
#include "UIColorManager.hpp"

#define st_MyArtSprite_length   500
#define st_MyArtSprite_offset   7

MyArtSprite* MyArtSprite::create(const string& imageName, const Color3B& bgColor, const int serveId, bool isShowLike)
{
    MyArtSprite* lSprite = new MyArtSprite();
    if (lSprite && lSprite->init(imageName, bgColor, serveId, isShowLike))
    {
        lSprite->autorelease();
        return lSprite;
    }
    
    CC_SAFE_DELETE(lSprite);
    return NULL;
}

MyArtSprite::MyArtSprite()
{
    m_sNeedLoadImageT = "";
    m_sNeedLoadImageP = "";
    m_sNeedLoadImage = "";
    m_pTexture2D = NULL;
    m_pLikeLabel = NULL;
    m_iServeId = 0;
    m_sPreName = "";
    _ImageScale = 1.0f;
    
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(MyArtSprite::waitGetlikeSuc), st_FavoriteSprite_getLikeNumber_suc, NULL);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(MyArtSprite::loadGraphSprite), st_multiThread_loadImage_finish, NULL);
}

MyArtSprite::~MyArtSprite()
{
    NotificationCenter::getInstance()->removeObserver(this, st_FavoriteSprite_getLikeNumber_suc);
    NotificationCenter::getInstance()->removeObserver(this, st_multiThread_loadImage_finish);
    removeTouchListener();
//    DownLoadPage::getInstance()->m_vNeedGetLikeImage.clear();
}

bool MyArtSprite::init(const string& imageName, const Color3B& bgColor, const int serveId, bool isShowLike)
{
    if (!Node::init()) return false;
    
    m_iServeId = serveId;
    
    m_sNeedLoadImage = imageName;
    m_cBgColor = bgColor;
//    this->setContentSize(Size(st_MyArtSprite_length, st_MyArtSprite_length));
    this->setAnchorPoint(Vec2(0.5, 0.5));
    this->setContentSize(Size(300, 300));

    
    
    auto deleteBtn = Button::create("cocos/my_arts/delete.png");
    deleteBtn->setPosition(Vec2(this->getContentSize().width * 0.1, -this->getContentSize().height * 0.1));
    deleteBtn->setPressedActionEnabled(true);
    deleteBtn->addTouchEventListener(CC_CALLBACK_2(MyArtSprite::onButton, this));
    addChild(deleteBtn);
    deleteBtn->setVisible(false);
    
    
    if (serveId != 0 && isShowLike)
    {
        Sprite* heart = Sprite::create("other/heart_btn.png");
        heart->setAnchorPoint(Vec2(1, 0));
        heart->setPosition(Vec2(this->getContentSize().width, 0));

        this->addChild(heart);
        
        Sprite* heart_bg = Sprite::create("other/like_bg@2x.png");
        heart_bg->setAnchorPoint(Vec2(1, 0));
        heart_bg->setPosition(Vec2(this->getContentSize().width - 6.8, 6.8));
        this->addChild(heart_bg, 11);
        
        Sprite* likeBg = Sprite::create("other/like_heart_white@2x.png");
        likeBg->setAnchorPoint(Vec2(0, 0.5));
        likeBg->setPosition(Vec2(6.8, heart_bg->getContentSize().height/2));
        heart_bg->addChild(likeBg, 11);
        
        Label* label = Label::createWithTTF("", FONT_NAME, 46);
        label->setColor(Color3B(52, 52, 52));
        label->setScale(0.5f);
        label->setPosition(Vec2(heart_bg->getContentSize().width - ( heart_bg->getContentSize().width - 6.8 - likeBg->getContentSize().width)/2 - 2, heart_bg->getContentSize().height/2));
        heart_bg->addChild(label);
        m_pLikeLabel = label;
        
        Sprite* like = Sprite::create("other/like_heart@2x.png");
        like->setPosition(likeBg->getContentSize()/2);
        likeBg->addChild(like, 11);
        
        DownLoadPage::getInstance()->updateNeedGetLikeImage(serveId);

    }
    
    Sprite* lBg = Sprite::create("other/content_mask.png");
    lBg->setPosition(this->getContentSize()/2);
    lBg->setScale(this->getContentSize().width/355);
    this->addChild(lBg, 10);
    
    LayerColor* layer = LayerColor::create(Color4B(231, 231, 231, 255), 300, 300);
    this->addChild(layer);
    
    Sprite* loading = Sprite::create("other/content_loading_icon@3x.png");
    loading->setPosition(layer->getContentSize()/2);
    loading->setScale(this->getContentSize().width*0.369/loading->getContentSize().width);
    layer->addChild(loading);
    
    return true;
}

void MyArtSprite::waitGetlikeSuc(Ref* ref)
{
    if (!m_pLikeLabel)
        return;
    
    for (int i = 0; i < DownLoadPage::getInstance()->m_vFavoriteInfo.size(); ++i)
    {
        FavoriteInfo lInfo = DownLoadPage::getInstance()->m_vFavoriteInfo.at(i);
        if (lInfo.imageID == m_iServeId)
        {
            stringstream ss;
            ss<<"";
            ss<<lInfo.likeNumber;
            m_pLikeLabel->setString(ss.str());
        }
    }
}

void MyArtSprite::onButton(Ref* pSender, Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED) return;
    
    NewDialog* dialog = NewDialog::create("delete_myart", "cancel", "delete");
    dialog->addButtonListener(CC_CALLBACK_1(MyArtSprite::onDialog, this));
    this->addChild(dialog, 101);
}

void MyArtSprite::onDialog(const string& name)
{
    if(name == "delete")
    {
        log("%s",m_sPreName.c_str());
        xMyArtData->deleteImageFromVec(m_sPreName);
        
        auto & graph = xChapter->getGraph(m_sNeedLoadImage);
        graph.deleteHistory(m_sPreName);
        
        NotificationCenter::getInstance()->postNotification(st_deleteMyArtSprite);
    }
    else if (name == "no")
    {
        
    }
}

void MyArtSprite::display()
{
    Texture2D* lTexture2DT = Director::getInstance()->getTextureCache()->getTextureForKey(m_sNeedLoadImageT);

    if (lTexture2DT)
    {
        m_pTexture2D = lTexture2DT;
        this->imageLoadedCallback(NULL);
    }else
    {
        xThread->addNeedloadImage(m_sNeedLoadImageT);
    }
}

void MyArtSprite::imageLoadedCallback(Ref* ref)
{
    Texture2D* lTexture2DT = Director::getInstance()->getTextureCache()->getTextureForKey(m_sNeedLoadImageT);

    if (lTexture2DT)
    {
        Sprite* lSprite2 = Sprite::createWithTexture(lTexture2DT);
        
        lSprite2->setPosition(this->getContentSize()/2);
        lSprite2->setScale(this->getContentSize().width/lSprite2->getContentSize().width);
        this->addChild(lSprite2);
        
        Sprite* lBg = Sprite::create("other/content_mask.png");
        lBg->setPosition(this->getContentSize()/2);
        lBg->setScale(this->getContentSize().width/355);
        this->addChild(lBg);
        
    }
}

void MyArtSprite::loadGraphSprite(Ref* ref)
{
    if (m_pTexture2D == NULL)
    {
        Texture2D* lTexture2D = Director::getInstance()->getTextureCache()->getTextureForKey(m_sNeedLoadImageT);
        if (lTexture2D != NULL)
        {
            this->imageLoadedCallback(NULL);
        }
    }
}

void MyArtSprite::removeTexture()
{
    Director::getInstance()->getTextureCache()->removeTextureForKey(m_sNeedLoadImageT);
}

string MyArtSprite::getSpriteName()
{
    return m_sNeedLoadImage;
}


void MyArtSprite::addTouchListener(Ref* target, SEL_CallFuncN callBack,bool isSwallow /* =true */)
{
    _selector = callBack;
    _target = target;
    
    this->removeTouchListener();
    
    auto dispatcher = Director::Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(MyArtSprite::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(MyArtSprite::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(MyArtSprite::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(MyArtSprite::onTouchCancelled, this);
    listener->setSwallowTouches(isSwallow);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void MyArtSprite::removeTouchListener()
{
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListenersForTarget(this);
}


bool MyArtSprite::onTouchBegan(Touch *touch, Event *unused_event)
{
    Vec2 lPos = touch->getLocationInView();
    firstPos = lPos;
    lPos = Director::getInstance()->convertToGL(lPos);
    lPos = this->getParent()->convertToNodeSpace(lPos);
    
    Vec2 viewPos = touch->getLocation();
    if (viewPos.y < BOTTOM_HEIGHT || viewPos.y > V::getRealDesignSize().height - TOP_HEIGHT_HEIGHT)
    {
        return false;
    }
    
    if (this->getBoundingBox().containsPoint(lPos))
    {
        _isClick = true;
        return true;
    }
    
    return false;
}

void MyArtSprite::onTouchMoved(Touch *touch, Event *unused_event)
{

}

void MyArtSprite::onTouchEnded(Touch *touch, Event *unused_event)
{
    Vec2 lPos = touch->getLocationInView();
    
    float distance = sqrt(pow(lPos.x - firstPos.x, 2) + pow(lPos.y - firstPos.y, 2));
    if (distance < 10)
    {
        if (_target && _selector)
        {
            (_target->*_selector)(this);
        }
    }
    
}

void MyArtSprite::onTouchCancelled(Touch *touch, Event *unused_event)
{
    
}

void MyArtSprite::setLoadImageName(const string& name)
{
    m_sNeedLoadImageT = name;
}

void MyArtSprite::setPreName(const string& name)
{
    
    m_sPreName = name;
}

string MyArtSprite::getPreName()
{
    return m_sPreName;
}

Color3B MyArtSprite::getBGColor()
{
    return m_cBgColor;
}

void MyArtSprite::setImageScale(float scale)
{
    _ImageScale = scale;
}