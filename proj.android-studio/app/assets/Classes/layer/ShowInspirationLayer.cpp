//
//  ShowInspirationLayer.cpp
//  ColorGarden
//
//  Created by houhuang on 17/5/18.
//
//

#include "ShowInspirationLayer.hpp"
#include "UITools.h"
#include "STVisibleRect.h"
#include "DownLoadPage.h"
#include "AppConfigs.h"

enum{
    st_menu_tag_back = 11,
    st_menu_tag_heart,
};

ShowInspirationLayer* ShowInspirationLayer::create(const int postID, const int likeNumber, bool isMylike)
{
    ShowInspirationLayer* layer = new ShowInspirationLayer();
    if (layer && layer->init(postID, likeNumber, isMylike))
    {
        layer->autorelease();
        return layer;
    }
    
    CC_SAFE_DELETE(layer);
    return nullptr;
}

ShowInspirationLayer::~ShowInspirationLayer()
{
    NotificationCenter::getInstance()->removeObserver(this, st_FavoriteSprite_getLikeNumber_suc);
    _contentSprite = nullptr;
}

ShowInspirationLayer::ShowInspirationLayer():_isAlreadyLike(false)
{
    _heart = nullptr;
    _label = nullptr;
    _likeNumber = 0;
    _postID = 0;
    
    _bgSprite = nullptr;
    _contentSprite = nullptr;
    
    _heartMenu = nullptr;
    _backMenu = nullptr;
    _isListenerClick = false;
    
    _firstPos = Vec2(0, 0);
    _heartMenu = nullptr;
    
    _isMylike = false;
    
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(ShowInspirationLayer::responseGetLikeSuc), st_FavoriteSprite_getLikeNumber_suc, NULL);
}

bool ShowInspirationLayer::init(const int postID, const int likeNumber, bool isMylike)
{
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 125))) return false;
    
    _likeNumber = likeNumber;
    _postID = postID;
    _isMylike = isMylike;
    
    MenuItemImage* back = MenuItemImage::create("back_3.png", "back_3.png", CC_CALLBACK_1(ShowInspirationLayer::onAction, this));
    back->setPosition(Vec2(this->getContentSize().width*0.08, this->getContentSize().height*0.95));
    back->setTag(st_menu_tag_back);
    
    Menu* lMenu = Menu::create(back, NULL);
    lMenu->setPosition(Vec2::ZERO);
    this->addChild(lMenu);
    _backMenu = lMenu;
    
    this->createContent();
    
    this->addListener();
    return true;
}

void ShowInspirationLayer::createContent()
{
    Sprite* bg = Sprite::create("other/showIns.png");
    bg->setPosition(this->getContentSize()/2);
    this->addChild(bg);
    _bgSprite = bg;
    
    std::string imageName = std::to_string(_postID) + ".png";
    Sprite* lSprite = Sprite::create(imageName);
    lSprite->setScale(quickAdaptScale(lSprite->getContentSize(), bg->getContentSize()));
    lSprite->setAnchorPoint(Vec2(0.5, 1));
    lSprite->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height));
    bg->addChild(lSprite);
    _contentSprite = lSprite;
    
    //    Sprite* boreder = Sprite::create("art_bg.png");
    //    boreder->setScale(quickAdaptScale(boreder->getContentSize(), lSprite->getContentSize()));
    //    boreder->setPosition(lSprite->getContentSize()/2);
    //    lSprite->addChild(boreder);
    
    for (int i = 0; i < DownLoadPage::getInstance()->m_vAlreadyClickLick.size(); ++i)
    {
        if (DownLoadPage::getInstance()->m_vAlreadyClickLick.at(i) == _postID)
        {
            _isAlreadyLike = true;
            break;
        }
    }
    
    Label* label = Label::createWithTTF(std::to_string(_likeNumber), "res/kalinga.ttf", 30);
    label->ignoreAnchorPointForPosition(false);
    label->setAnchorPoint(Vec2(1, 0.5));
    label->setPosition(Vec2(bg->getContentSize().width - 15, 30));
    bg->addChild(label);
    label->setColor(Color3B(76, 76, 76));
    _label = label;
    
    MenuItemImage* heart = MenuItemImage::create("other/heart1.png", "other/heart1.png", CC_CALLBACK_1(ShowInspirationLayer::onAction, this));
    heart->setPosition(Vec2(label->getPosition().x - label->getContentSize().width - 20, 33));
    heart->setScale(0.5f);
    heart->setTag(st_menu_tag_heart);
    _hearMenuItem = heart;
    
    Menu* lHeartMenu = Menu::create(heart, NULL);
    lHeartMenu->setPosition(Vec2::ZERO);
    bg->addChild(lHeartMenu);
    _heartMenu = lHeartMenu;
    
    _heart = Sprite::create("other/heart.png");
    _heart->setPosition(heart->getContentSize()/2);
    heart->addChild(_heart);
    _heart->setVisible(_isAlreadyLike);
    
    this->setContentTouchEnbale(false);
    this->runAction(Sequence::create(DelayTime::create(0.85f), CallFunc::create([this](){
        this->setContentTouchEnbale(true);
    }), NULL));
    
    this->scaleContent(0.8f, true);
    
    DownLoadPage::getInstance()->getLikeNumber(_postID);
}

void ShowInspirationLayer::destoryContent()
{
    this->setContentTouchEnbale(false);
    this->scaleContent(0.8f, false);
 
    if (!_isAlreadyLike && _isMylike)
    {
        for (int i = 0; i < DownLoadPage::getInstance()->m_vAlreadyClickLick.size(); ++i)
        {
            if (DownLoadPage::getInstance()->m_vAlreadyClickLick.at(i) == _postID)
            {
                std::vector<int>::iterator it = DownLoadPage::getInstance()->m_vAlreadyClickLick.begin() + i;
                DownLoadPage::getInstance()->m_vAlreadyClickLick.erase(it);
                break;
            }
        }
        
        DownLoadPage::getInstance()->postClickCancelLike(_postID);
        
        this->runAction(Sequence::create(DelayTime::create(0.8f), CallFunc::create([this](){
            this->removeFromParentAndCleanup(true);
            
        }), NULL));
    }else
    {
        this->runAction(Sequence::create(DelayTime::create(0.8f), CallFunc::create([this](){
            this->removeFromParentAndCleanup(true);
        }), NULL));
    }
    
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("cancel_collection");
    
}

void ShowInspirationLayer::scaleContent(const float duration, bool isEnlarge)
{
    if (isEnlarge)
    {
        _bgSprite->setOpacity(0);
        _bgSprite->setScale(0.0f);
        _bgSprite->runAction(Spawn::create(EaseExponentialInOut::create(FadeIn::create(duration)), EaseExponentialInOut::create(ScaleTo::create(duration, 1.0f)), NULL) );
        
        _contentSprite->setOpacity(0);
        _contentSprite->runAction(EaseExponentialInOut::create(FadeIn::create(duration)));
        
        this->setOpacity(0);
        this->runAction(EaseExponentialInOut::create(FadeTo::create(duration, 135)));
    }else
    {
        _bgSprite->runAction(Spawn::create(EaseExponentialInOut::create(FadeOut::create(duration)), EaseExponentialInOut::create(ScaleTo::create(duration, 0.0f)), NULL) );
        
        _contentSprite->runAction(EaseExponentialInOut::create(FadeOut::create(duration)));
        
        this->runAction(EaseExponentialInOut::create(FadeOut::create(duration)));
    }
}

void ShowInspirationLayer::setContentTouchEnbale(bool isEnble)
{
    _backMenu->setEnabled(isEnble);
    _heartMenu->setEnabled(isEnble);
    _isListenerClick = isEnble;
}

void ShowInspirationLayer::responseGetLikeSuc(Ref* ref)
{
    vector<FavoriteInfo> vFavorite = DownLoadPage::getInstance()->m_vFavoriteInfo;
    for (int i = 0; i < vFavorite.size(); ++i)
    {
        if (vFavorite.at(i).imageID == _postID)
        {
            _likeNumber = vFavorite.at(i).likeNumber;
            _label->setString(to_string(_likeNumber));
            
            NotificationCenter::getInstance()->removeObserver(this, st_FavoriteSprite_getLikeNumber_suc);
            return;
        }
    }
}

void ShowInspirationLayer::onAction(Ref* ref)
{
    MenuItemImage* lMenuItem = dynamic_cast<MenuItemImage*>(ref);
    switch (lMenuItem->getTag())
    {
        case st_menu_tag_back:
        {
            this->destoryContent();
        }
            break;
            
        case st_menu_tag_heart:
        {
            if (_isMylike)
            {
                if (_isAlreadyLike)
                {
                    _isAlreadyLike = false;
                    _heart->setVisible(false);
                    
                    --_likeNumber;
                    _label->setString(std::to_string(_likeNumber));
                    _hearMenuItem->setPosition(Vec2(_label->getPosition().x - _label->getContentSize().width - 20, 33));
                }else
                {
                    _isAlreadyLike = true;
                    _heart->setVisible(true);
                    
                    ++_likeNumber;
                    _label->setString(std::to_string(_likeNumber));
                    _hearMenuItem->setPosition(Vec2(_label->getPosition().x - _label->getContentSize().width - 20, 33));
                }
            }else
            {
                if (!_isAlreadyLike)
                {
                    _isAlreadyLike = true;
                    _heart->setVisible(_isAlreadyLike);
                    _label->setString(std::to_string(++_likeNumber));
                    
                    _hearMenuItem->setPosition(Vec2(_label->getPosition().x - _label->getContentSize().width - 20, 33));
                    
                    DownLoadPage::getInstance()->postClickLike(_postID);
                    DownLoadPage::getInstance()->m_vAlreadyClickLick.push_back(_postID);
                }else
                {
                    _isAlreadyLike = false;
                    _heart->setVisible(_isAlreadyLike);
                    _label->setString(std::to_string(--_likeNumber));
                    
                    _hearMenuItem->setPosition(Vec2(_label->getPosition().x - _label->getContentSize().width - 20, 33));
                    DownLoadPage::getInstance()->postClickCancelLike(_postID);
                    
                    for (int i = 0; i < DownLoadPage::getInstance()->m_vAlreadyClickLick.size(); ++i)
                    {
                        if (DownLoadPage::getInstance()->m_vAlreadyClickLick.at(i) == _postID)
                        {
                            std::vector<int>::iterator it = DownLoadPage::getInstance()->m_vAlreadyClickLick.begin() + i;
                            DownLoadPage::getInstance()->m_vAlreadyClickLick.erase(it);
                            break;
                        }
                    }
                }
            }
            
        }
            break;
            
        default:
            break;
    }
}

void ShowInspirationLayer::addListener()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = [this](Touch* touch, Event* event)->bool{
        
        _firstPos = touch->getLocationInView();
        
        return true;
    };
    
    listener->onTouchMoved = [this](Touch* touch, Event* event){
        
    };
    
    listener->onTouchEnded = [this](Touch* touch, Event* event){
        
        Vec2 lPos = touch->getLocationInView();
        float distance = sqrt(pow(lPos.x - _firstPos.x, 2) + sqrt(pow(lPos.y - _firstPos.y, 2)));
        if (distance < 10 && _isListenerClick)
        {
            this->destoryContent();
        }

    };
    
    listener->onTouchCancelled = [this](Touch* touch, Event* event){
        
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}




