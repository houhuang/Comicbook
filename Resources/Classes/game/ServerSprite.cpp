//
//  ServerSprite.cpp
//  IMG1001
//
//  Created by houhuang on 15/11/17.
//
//

#include "ServerSprite.hpp"
//#include "../helper/ScreenAdapterHelper.h"
#include "DownLoadPage.h"
//#include "../modules/STSystemFunction.h"
//#include "../helper/DeviceHelper.h"
#include "ThreadManager.hpp"
#include "STSystemFunction.h"
#include "UITools.h"
#include "UIColorManager.hpp"
#include "UICpp.h"
#include "SSCInternalLibManager.h"

ServerSprite::~ServerSprite()
{
    NotificationCenter::getInstance()->removeObserver(this, st_inspiration_downloadSuc);
    NotificationCenter::getInstance()->removeObserver(this, st_multiThread_loadImage_finish);
    NotificationCenter::getInstance()->removeObserver(this, st_FavoriteSprite_getLikeNumber_suc);
    
    m_pLikeLabel = NULL;
    m_pTextureSprite = NULL;
    m_pTargetTexture = NULL;
    m_pMenuItem = NULL;
}

ServerSprite::ServerSprite()
{
    m_sImageUrl = "";
    m_bIsclickLike = false;
    m_pLikeLabel = NULL;
    m_pTextureSprite = NULL;
    m_pTargetTexture = NULL;
    m_pMenuItem = NULL;
    m_bIsDownloadFinish = false;
    m_bIsHide = true;
    _isClick = false;
    _isMylikeSprite = false;
    _likeSprite = nullptr;
    
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(ServerSprite::responseGetLikeSuc), st_FavoriteSprite_getLikeNumber_suc, NULL);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(ServerSprite::waitDownLoadSuc), st_inspiration_downloadSuc, NULL);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(ServerSprite::LoadMapSprite), st_multiThread_loadImage_finish, NULL);
}

ServerSprite* ServerSprite::create(const string& imageUrl, int id, int likeNumber)
{
    ServerSprite* lNode = new ServerSprite();
    if (lNode && lNode->init(imageUrl, id, likeNumber))
    {
        lNode->autorelease();
        return lNode;
    }
    
    CC_SAFE_DELETE(lNode);
    return NULL;
}

ServerSprite* ServerSprite::create(const int id)
{
    ServerSprite* lNode = new ServerSprite();
    if (lNode && lNode->init(id))
    {
        lNode->autorelease();
        return lNode;
    }
    
    CC_SAFE_DELETE(lNode);
    return NULL;}

bool ServerSprite::init(const string& imageUrl, int id, int likeNumber)
{
    if (!Node::init()) return false;
    
    this->setAnchorPoint(Vec2(0.5, 0.5));
    
    _imageID = id;
    _likeNumber = likeNumber;
    m_sImageUrl = imageUrl;
    _isMylikeSprite = false;
    
    this->setContentSize(Size(300, 300));

    for (int i = 0; i < DownLoadPage::getInstance()->m_vAlreadyClickLick.size(); ++i)
    {
        if (DownLoadPage::getInstance()->m_vAlreadyClickLick.at(i) == id)
        {
            m_bIsclickLike = true;
            break;
        }
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

bool ServerSprite::init(const int id)
{
    if (!Node::init()) return false;
    
    this->setAnchorPoint(Vec2(0.5, 0.5));
    
    _imageID = id;
    _likeNumber = 0;
    _isMylikeSprite = true;
    m_bIsclickLike = true;

    this->setContentSize(Size(300, 300));
    
    DownLoadPage::getInstance()->updateNeedGetLikeImage(id);
    
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

void ServerSprite::responseGetLikeSuc(Ref *ref)
{
    if (!_isMylikeSprite)
        return;
    
    for (int i = 0; i < DownLoadPage::getInstance()->m_vFavoriteInfo.size(); ++i)
    {
        FavoriteInfo lInfo = DownLoadPage::getInstance()->m_vFavoriteInfo.at(i);
        if (lInfo.imageID == _imageID)
        {
            _likeNumber = lInfo.likeNumber;
            if (_likeNumber < 0)
            {
                _likeNumber = 0;
            }
            
            if (m_pLikeLabel)
            {
                m_pLikeLabel->setString(to_string(_likeNumber));
            }
            
        }
    }
}

void ServerSprite::addListener(Ref* target, SEL_CallFuncN callBack, bool isSwallow /* = false */)
{
    _target = target;
    _callBack = callBack;
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(isSwallow);
    
    listener->onTouchBegan = [this](Touch* touch, Event* event)->bool{
        _firstPos = touch->getLocationInView();
        
        Vec2 lPos = Director::getInstance()->convertToGL(_firstPos);
        lPos = this->getParent()->convertToNodeSpace(lPos);

        Vec2 viewPos = touch->getLocation();
        if (viewPos.y < BOTTOM_HEIGHT || viewPos.y > V::getRealDesignSize().height - TOP_HEIGHT_HEIGHT)
        {
            return false;
        }
        
        if(this->getBoundingBox().containsPoint(lPos))
        {
            return true;
        }
    
        return false;
    };
    
    listener->onTouchMoved = [this](Touch* touch, Event* event){
        
        
    };
    
    listener->onTouchEnded = [this](Touch* touch, Event* event){
        Vec2 lPos = touch->getLocationInView();
        float distance = sqrt(pow(lPos.x - _firstPos.x, 2) + pow(lPos.y - _firstPos.y, 2));
        if (distance < 10 && _isClick)
        {
            (_target->*_callBack)(this);
        }
        
    };
    
    listener->onTouchCancelled = [this](Touch* touch, Event* event){
        
        
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void ServerSprite::display()
{
    if (!m_bIsHide)
    {
        return;
    }
    
    m_bIsHide = false;
    
    

    if (FileUtils::getInstance()->isFileExist(this->getImageFullPath()) )
    {
        m_bIsDownloadFinish = true;
        
        Texture2D* lTexture2D = Director::getInstance()->getTextureCache()->getTextureForKey(this->getImageFullPath());
        if (lTexture2D)
        {
            this->imageLoadedCallback(lTexture2D);
        }else
        {
            xThread->addNeedloadImage(this->getImageFullPath());
        }
        
    }
    else
    {
        DownloadImageInfo imageInfo;
        imageInfo.imageID = _imageID;
        imageInfo.imageUrl = m_sImageUrl;

        DownLoadPage::getInstance()->addNeedDownloadImage(imageInfo);
    }

}

void ServerSprite::removeTextureCache()
{
    for (int i = 0; i < DownLoadPage::getInstance()->m_mNeedDownloadImage.size(); ++i)
    {
        DownloadImageInfo info = DownLoadPage::getInstance()->m_mNeedDownloadImage.at(i);
        if (info.imageID == _imageID)
        {
            std::vector<DownloadImageInfo>::iterator iter = DownLoadPage::getInstance()->m_mNeedDownloadImage.begin() + i;
            DownLoadPage::getInstance()->m_mNeedDownloadImage.erase(iter);
            
            break;
        }
    }
    
    for (int i = 0; i < xThread->m_vNeedLoadImage.size(); ++i)
    {
        string str = xThread->m_vNeedLoadImage.at(i);
        if (str == this->getImageFullPath())
        {
            deque<string>::iterator iter = xThread->m_vNeedLoadImage.begin() + i;
            xThread->m_vNeedLoadImage.erase(iter);

            break;
        }
    }
    
    
    if (m_pTextureSprite)
    {
        Director::getInstance()->getTextureCache()->removeTexture(m_pTextureSprite->getTexture());
    }
    string name = to_string(_imageID) + ".png";
    Director::getInstance()->getTextureCache()->removeTextureForKey(name);
}

void ServerSprite::hideThis()
{
    if (m_bIsHide)
    {
        return;
    }
    
    m_bIsHide = true;
    if (m_pTextureSprite)
    {
        
        m_pTextureSprite->removeFromParentAndCleanup(true);
        m_pTextureSprite = NULL;
        
        stringstream ss;
        ss<<"";
        ss<<_imageID;
        ss<<".png";
        Director::getInstance()->getTextureCache()->removeTextureForKey(ss.str());
    }
    
}

void ServerSprite::waitDownLoadSuc(Ref* ref)
{
    if (m_bIsHide)
    {
        if (m_pTextureSprite)
        {
            
            m_pTextureSprite->removeFromParentAndCleanup(true);
            m_pTextureSprite = NULL;
            
            stringstream ss;
            ss<<"";
            ss<<_imageID;
            ss<<".png";
            Director::getInstance()->getTextureCache()->removeTextureForKey(ss.str());
        }
        
        return;
    }
    
    if (FileUtils::getInstance()->isFileExist(this->getImageFullPath()) && m_bIsDownloadFinish == false)
    {
        m_bIsDownloadFinish = true;
    
        xThread->addNeedloadImage(this->getImageFullPath());
    }
}

void ServerSprite::reponseMenu(Ref* ref)
{
    if (_isMylikeSprite)
    {
        if (_likeSprite)
        {
            
            STSystemFunction sf;
            if(!sf.checkNetworkAvailable())
            {
                sf.popAlertDialog("error network");
                return;
            }
            
            DownLoadPage::getInstance()->postClickCancelLike(_imageID);
            
            //                DownLoadPage::getInstance()->m_vCancelLike.push_back(_imageID);
            for (int i = 0; i < DownLoadPage::getInstance()->m_vAlreadyClickLick.size(); ++i)
            {
                if (DownLoadPage::getInstance()->m_vAlreadyClickLick.at(i) == _imageID)
                {
                    std::vector<int>::iterator it = DownLoadPage::getInstance()->m_vAlreadyClickLick.begin() + i;
                    DownLoadPage::getInstance()->m_vAlreadyClickLick.erase(it);
                    break;
                }
            }
            
            
                this->runAction(Sequence::create(EaseSineOut::create(ScaleTo::create(0.5, 0)), CallFunc::create([](){
                    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("cancel_collection");
                    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("update_recent");
            }), NULL));
            
        }
        
    }else
    {
        if (m_bIsclickLike == false)
        {
            m_bIsclickLike = true;
            if (_likeSprite)
            {
                
                STSystemFunction sf;
                if(!sf.checkNetworkAvailable())
                {
                    sf.popAlertDialog("error network");
                    return;
                }
                
                DownLoadPage::getInstance()->postClickLike(_imageID);
                DownLoadPage::getInstance()->m_vAlreadyClickLick.push_back(_imageID);
                
                _likeSprite->setVisible(true);
                
                ++_likeNumber;
                m_pLikeLabel->setString(to_string(_likeNumber));
                //            DownLoadPage::getInstance()->getLikeNumber(_imageID);
                
            }
        }else
        {
            m_bIsclickLike = false;
            
            if (_likeSprite)
            {
                
                STSystemFunction sf;
                if(!sf.checkNetworkAvailable())
                {
                    sf.popAlertDialog("error network");
                    return;
                }
                
                DownLoadPage::getInstance()->postClickCancelLike(_imageID);
                
                for (int i = 0; i < DownLoadPage::getInstance()->m_vAlreadyClickLick.size(); ++i)
                {
                    if (DownLoadPage::getInstance()->m_vAlreadyClickLick.at(i) == _imageID)
                    {
                        std::vector<int>::iterator it = DownLoadPage::getInstance()->m_vAlreadyClickLick.begin() + i;
                        DownLoadPage::getInstance()->m_vAlreadyClickLick.erase(it);
                        break;
                    }
                }
                
                _likeSprite->setVisible(false);
                
                --_likeNumber;
                m_pLikeLabel->setString(to_string(_likeNumber));
                //            DownLoadPage::getInstance()->getLikeNumber(_imageID);
                
            }
        }
        
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("cancel_collection");
    }
}

void ServerSprite::imageLoadedCallback(Ref* pSender)
{
    Texture2D* lTexture = (Texture2D*)pSender;
    
    Sprite* lRenderSprite = Sprite::createWithTexture(lTexture);
    
    if (lRenderSprite != NULL && m_pTextureSprite == NULL)
    {
        lRenderSprite->getTexture()->setAntiAliasTexParameters();
        lRenderSprite->setPosition(Vec2(this->getContentSize()/2));

        this->addChild(lRenderSprite);
        lRenderSprite->setScale(this->getContentSize().width/lRenderSprite->getContentSize().width);
        m_pTextureSprite = lRenderSprite;
        
        MenuItemSprite* heart = MenuItemSprite::create(Sprite::create("other/heart_btn.png"), Sprite::create("other/heart_btn.png"), CC_CALLBACK_1(ServerSprite::reponseMenu, this));
        heart->setAnchorPoint(Vec2(1, 0));
        heart->setPosition(Vec2(this->getContentSize().width, 0));
        
        Menu* lMenu = Menu::create(heart, NULL);
        lMenu->setPosition(Vec2::ZERO);
        this->addChild(lMenu, 11);
        
        Sprite* heart_bg = Sprite::create("other/like_bg@2x.png");
        heart_bg->setAnchorPoint(Vec2(1, 0));
        heart_bg->setPosition(Vec2(this->getContentSize().width - 6.8, 6.8));
        this->addChild(heart_bg, 11);
        
        Sprite* likeBg = Sprite::create("other/like_heart_white@2x.png");
        likeBg->setAnchorPoint(Vec2(0, 0.5));
        likeBg->setPosition(Vec2(6.8, heart_bg->getContentSize().height/2));
        heart_bg->addChild(likeBg, 11);
        
        string num = to_string(_likeNumber);
        if (_isMylikeSprite)
        {
            if (num == "0")
            {
                num = "";
            }
        }

        Label* label = Label::createWithTTF(num, FONT_NAME, 46);
        label->setColor(Color3B(52, 52, 52));
        label->setScale(0.5f);
        label->setPosition(Vec2(heart_bg->getContentSize().width - ( heart_bg->getContentSize().width - 6.8 - likeBg->getContentSize().width)/2 - 2, heart_bg->getContentSize().height/2));
        heart_bg->addChild(label);
        m_pLikeLabel = label;
        
        Sprite* like = Sprite::create("other/like_heart@2x.png");
        like->setPosition(likeBg->getContentSize()/2);
        likeBg->addChild(like, 11);
        _likeSprite = like;
        like->setVisible(m_bIsclickLike);
        
        _isClick = true;

//        lRenderSprite->setOpacity(0);
//        lRenderSprite->runAction(EaseExponentialIn::create(FadeIn::create(3)) );
    }else
    {
//        Director::getInstance()->getTextureCache()->removeTexture(lTexture);
    }
    
//    Director::getInstance()->getTextureCache()->removeTexture(lTexture);
}

void ServerSprite::LoadMapSprite(Ref* pSender)
{
    //解决快速滑动时，连续调用display()和hideThis()导致图片缓存未释放问题
    
//    if (m_bIsHide)
//    {
//        if (m_pTextureSprite)
//        {
//            
//            m_pTextureSprite->removeFromParentAndCleanup(true);
//            m_pTextureSprite = NULL;
//            
//            stringstream ss;
//            ss<<"";
//            ss<<_imageID;
//            ss<<".png";
//            Director::getInstance()->getTextureCache()->removeTextureForKey(ss.str());
//        }
//        
//        if (m_pRotateSprite)
//        {
//            m_pRotateSprite->removeFromParentAndCleanup(true);
//            m_pRotateSprite = NULL;
//        }
//        
//        return;
//    }
//    
//    if (m_bIsHide)
//    {
//        Director::getInstance()->getTextureCache()->removeTextureForKey(this->getImageFullPath());
//        
//        return;
//    }
    
    if (m_pTargetTexture == NULL)
    {
        Texture2D* lTexture;
        lTexture = Director::getInstance()->getTextureCache()->getTextureForKey(this->getImageFullPath());
        
        if (lTexture != NULL)
        {
            this->imageLoadedCallback(lTexture);
        }
    }
}

string ServerSprite::getImageFullPath()
{
    
    string path = FileUtils::getInstance()->getWritablePath();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    path+="Inspiration/";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    path+="Inspiration/";
#endif
    
    stringstream ss;
    ss<<"";
    ss<<path;
    ss<<_imageID;
    ss<<".png";
    
    return ss.str();
}


ServerSprite* ServerSprite::create(int id, int likeNumber)
{
    ServerSprite* lNode = new ServerSprite();
    if (lNode && lNode->init(id, likeNumber))
    {
        lNode->autorelease();
        return lNode;
    }
    
    CC_SAFE_DELETE(lNode);
    return NULL;
}

bool ServerSprite::init(int id, int likeNumber)
{
    if (!Node::init()) return false;
    
    this->setAnchorPoint(Vec2(0.5, 0.5));
    
    _imageID = id;
    _likeNumber = likeNumber;
//    m_sImageUrl = imageUrl;
    
    this->setContentSize(Size(300, 300));

    
    return true;
    
}

