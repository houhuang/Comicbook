//
//  GraphSpriteLayer.cpp
//  ColorGarden
//
//  Created by houhuang on 17/11/7.
//
//

#include "GraphSpriteLayer.hpp"
#include "STVisibleRect.h"
#include "ChapterManager.hpp"
#include "SceneManager.h"
#include "DownLoadPage.h"
#include "NewDialog.hpp"
#include "MyArtDataManager.hpp"
#include "GraphScene.h"
#include "AnalyticsManager.hpp"

enum
{
    st_button_publish = 10,
    st_button_new,
    st_button_continue,
    st_button_delete,
    st_button_share,
    st_button_cancel
};

GraphSpriteLayer* GraphSpriteLayer::create(const string& preName, string spriteName)
{
    GraphSpriteLayer* layer = new GraphSpriteLayer();
    if (layer && layer->init(preName, spriteName))
    {
        layer->autorelease();
        return layer;
    }
    
    CC_SAFE_DELETE(layer);
    return nullptr;
}

GraphSpriteLayer::~GraphSpriteLayer()
{
    SceneManager::_isEnableClick = true;
    _contentSprite = nullptr;
    _eventDispatcher->removeEventListenersForTarget(this);
    
    NotificationCenter::getInstance()->removeObserver(this, st_myPost_failed);
    NotificationCenter::getInstance()->removeObserver(this, st_myPost_success);
}

GraphSpriteLayer::GraphSpriteLayer()
{
    _contentSprite = nullptr;
    _isExcuteAnimation = false;
    _isClickContent = false;
    
    _deleteSprite = nullptr;
    _deleteLabel = nullptr;
    _postingLayer = nullptr;
    _showImage = nullptr;

}

void GraphSpriteLayer::onExit()
{
    LayerColor::onExit();
}

bool GraphSpriteLayer::init(const string& preName, string spriteName)
{
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 210))) return false;
    
    _name = preName;
    _SpriteName = spriteName;
    
    V::isIpad() ? this->createContentFromiPad() : this->createContentFromiPhone();

    this->addListener();
    
    this->scaleContent(0.15, true);
    
    SceneManager::_isEnableClick = false;
    
    return true;
}

void GraphSpriteLayer::createContentFromiPhone()
{
    Sprite* lSprite = Sprite::create("other/popup@3x.png");
    lSprite->setPosition(this->getContentSize()/2);
    lSprite->setScale(this->getContentSize().width*0.9/1016.);
    this->addChild(lSprite);
    _contentSprite = lSprite;
    
    Sprite* preSprite = Sprite::create(_name);
    preSprite->setScale(1017.5/preSprite->getContentSize().width);
    preSprite->setPosition(Vec2(lSprite->getContentSize().width/2, lSprite->getContentSize().height/2 + 120));
    lSprite->addChild(preSprite);
    _showImage = preSprite;
    
    MenuItemImage* publish = MenuItemImage::create("other/myart_continue.png", "other/myart_continue.png", CC_CALLBACK_1(GraphSpriteLayer::onButton, this));
    publish->setPosition(Vec2(lSprite->getContentSize().width/2, lSprite->getContentSize().height*0.075));
    publish->setTag(st_button_publish);
    
    MenuItemImage* newBtn = MenuItemImage::create("other/myart_continue.png", "other/myart_continue.png", CC_CALLBACK_1(GraphSpriteLayer::onButton, this));
    newBtn->setPosition(Vec2(lSprite->getContentSize().width/2, lSprite->getContentSize().height*0.148));
    newBtn->setTag(st_button_new);
    
    MenuItemImage* continueBtn = MenuItemImage::create("other/myart_continue.png", "other/myart_continue.png", CC_CALLBACK_1(GraphSpriteLayer::onButton, this));
    continueBtn->setPosition(Vec2(lSprite->getContentSize().width/2, lSprite->getContentSize().height*0.216));
    continueBtn->setTag(st_button_continue);
    
    
    MenuItemImage* cancel = MenuItemImage::create("other/myart_cancel.png", "other/myart_cancel.png", CC_CALLBACK_1(GraphSpriteLayer::onButton, this));
    cancel->setAnchorPoint(Vec2(0, 1));
    cancel->setPosition(Vec2(70, lSprite->getContentSize().height - 62));
    cancel->setTag(st_button_cancel);
    
    MenuItemImage* deleteBtn = MenuItemImage::create("other/myart_cancel.png", "other/myart_cancel.png", CC_CALLBACK_1(GraphSpriteLayer::onButton, this));
    deleteBtn->setAnchorPoint(Vec2(1, 1));
    deleteBtn->setPosition(Vec2(lSprite->getContentSize().width - 200, lSprite->getContentSize().height - 62));
    deleteBtn->setTag(st_button_delete);
    
    MenuItemImage* share = MenuItemImage::create("other/myart_cancel.png", "other/myart_cancel.png", CC_CALLBACK_1(GraphSpriteLayer::onButton, this));
    share->setAnchorPoint(Vec2(1, 1));
    share->setPosition(Vec2(lSprite->getContentSize().width - 70, lSprite->getContentSize().height - 62));
    share->setTag(st_button_share);
    
    
    Menu* lMenu = Menu::create(publish, newBtn, continueBtn, cancel, share, deleteBtn, NULL);
    lMenu->setPosition(Vec2::ZERO);
    lSprite->addChild(lMenu);
    
    Sprite* deleteSprite = Sprite::create("other/popip_icon_delete@3x.png");
    deleteSprite->setPosition(Vec2(deleteBtn->getContentSize()/2) - Vec2(0, 5));
    deleteSprite->setScale(8./8);
    deleteBtn->addChild(deleteSprite);
    _deleteSprite = deleteSprite;
    
    
    float fontSize, scale;
    if (V::isHighVersion())
    {
        fontSize = 90 * 0.4;
        scale = 0.5/0.4;
    }else
    {
        fontSize = 90;
        scale = 0.5;
    }
    Label* deleteLabel = Label::createWithTTF("Delete?", "res/Muli-Regular.ttf", fontSize);
    deleteLabel->setScale(scale);
    deleteLabel->setColor(Color3B(66, 180, 103));
    deleteLabel->setPosition(Vec2(deleteBtn->getContentSize()/2) - Vec2(30, 5));
    deleteBtn->addChild(deleteLabel);
    deleteLabel->setVisible(false);
    _deleteLabel = deleteLabel;
}

void GraphSpriteLayer::createContentFromiPad()
{
    Sprite* lSprite = Sprite::create("other/popup_ipad.png");
    lSprite->setPosition(this->getContentSize()/2);
    lSprite->setScale(this->getContentSize().width*0.57/1318.);
    this->addChild(lSprite);
    _contentSprite = lSprite;
    
    Sprite* preSprite = Sprite::create(_name);
    preSprite->setScale(1320./preSprite->getContentSize().width);
    preSprite->setPosition(Vec2(lSprite->getContentSize().width/2, lSprite->getContentSize().height/2 + 140));
    lSprite->addChild(preSprite);
    _showImage = preSprite;
    
    MenuItemImage* publish = MenuItemImage::create("other/myart_continue_ipad.png", "other/myart_continue_ipad.png", CC_CALLBACK_1(GraphSpriteLayer::onButton, this));
    publish->setPosition(Vec2(lSprite->getContentSize().width/2, lSprite->getContentSize().height*0.063));
    publish->setTag(st_button_publish);
    
    MenuItemImage* newBtn = MenuItemImage::create("other/myart_continue_ipad.png", "other/myart_continue_ipad.png", CC_CALLBACK_1(GraphSpriteLayer::onButton, this));
    newBtn->setPosition(Vec2(lSprite->getContentSize().width/2, lSprite->getContentSize().height*0.131));
    newBtn->setTag(st_button_new);
    
    MenuItemImage* continueBtn = MenuItemImage::create("other/myart_continue_ipad.png", "other/myart_continue_ipad.png", CC_CALLBACK_1(GraphSpriteLayer::onButton, this));
    continueBtn->setPosition(Vec2(lSprite->getContentSize().width/2, lSprite->getContentSize().height*0.201));
    continueBtn->setTag(st_button_continue);
    
    
    MenuItemImage* cancel = MenuItemImage::create("other/myart_cancel_ipad.png", "other/myart_cancel_ipad.png", CC_CALLBACK_1(GraphSpriteLayer::onButton, this));
    cancel->setAnchorPoint(Vec2(0, 1));
    cancel->setPosition(Vec2(70, lSprite->getContentSize().height - 62));
    cancel->setTag(st_button_cancel);
    
    MenuItemImage* deleteBtn = MenuItemImage::create("other/myart_cancel_ipad.png", "other/myart_cancel_ipad.png", CC_CALLBACK_1(GraphSpriteLayer::onButton, this));
    deleteBtn->setAnchorPoint(Vec2(1, 1));
    deleteBtn->setPosition(Vec2(lSprite->getContentSize().width - 200, lSprite->getContentSize().height - 65));
    deleteBtn->setTag(st_button_delete);
    
    MenuItemImage* share = MenuItemImage::create("other/myart_cancel_ipad.png", "other/myart_cancel_ipad.png", CC_CALLBACK_1(GraphSpriteLayer::onButton, this));
    share->setAnchorPoint(Vec2(1, 1));
    share->setPosition(Vec2(lSprite->getContentSize().width - 60, lSprite->getContentSize().height - 62));
    share->setTag(st_button_share);
    
    
    Menu* lMenu = Menu::create(publish, newBtn, continueBtn, cancel, share, deleteBtn, NULL);
    lMenu->setPosition(Vec2::ZERO);
    lSprite->addChild(lMenu);
    
    Sprite* deleteSprite = Sprite::create("other/popip_icon_delete@3x.png");
    deleteSprite->setPosition(Vec2(deleteBtn->getContentSize()/2) - Vec2(0, 5));
    deleteSprite->setScale(8.7/8);
    deleteBtn->addChild(deleteSprite);
    _deleteSprite = deleteSprite;
    
    float fontSize, scale;
    if (V::isHighVersion())
    {
        fontSize = 100 * 0.4;
        scale = 0.5/0.4;
    }else
    {
        fontSize = 100;
        scale = 0.5;
    }
    
    Label* deleteLabel = Label::createWithTTF("Delete?", "res/Muli-Regular.ttf", fontSize);
    deleteLabel->setScale(scale);
    deleteLabel->setColor(Color3B(66, 180, 0.3));
    deleteLabel->setPosition(Vec2(deleteBtn->getContentSize()/2) - Vec2(30, 5));
    deleteBtn->addChild(deleteLabel);
    deleteLabel->setVisible(false);
    _deleteLabel = deleteLabel;
}

void GraphSpriteLayer::onButton(Ref* ref)
{
    MenuItemImage* lMenuItem = (MenuItemImage*)ref;
    switch (lMenuItem->getTag())
    {
        case st_button_new:
        {
            xChapter->getCurrentChapter().getCurrentGraph().setNew(true);
            xScene->forward(SceneManager::Tag::GAME);
            
            map<string, string> map;
            map.insert(pair<string, string>("image_id", xChapter->getCurrentChapter().getCurrentGraph().name));
            map.insert(pair<string, string>("restart_location", "library"));
            ATMInstance->sendEvent("image_restart", map);
        }
            break;
            
        case st_button_continue:
        {
            xChapter->getCurrentChapter().getCurrentGraph().setNew(false);
            xScene->forward(SceneManager::Tag::GAME);

            map<string, string> map;
            map.insert(pair<string, string>("image_id", xChapter->getCurrentChapter().getCurrentGraph().name));
            map.insert(pair<string, string>("continue_location", "library"));
            ATMInstance->sendEvent("image_continue", map);

        }
            break;
            
        case st_button_publish:
        {

            DownLoadPage::_postFailNeedNewOneName = _SpriteName;
            DownLoadPage::m_sCurrentPostImageName = _name;
            
            map<string, string> map;
            map.insert(pair<string, string>("image_id", xChapter->getCurrentChapter().getCurrentGraph().name));
            map.insert(pair<string, string>("published_location", "library"));
            ATMInstance->sendEvent("image_publish", map);
            
            Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("graph_upload", (void*)&_name);
            
//            this->removeFromParentAndCleanup(true);
            
            Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("removeGraphSpriteLayer");
            
            
        }
            break;
            
        case st_button_cancel:
        {
            this->scaleContent(0.15, false);
        }
            break;
            
        case st_button_delete:
        {
            if (!_deleteLabel->isVisible())
            {
                _deleteLabel->setVisible(true);
                _deleteSprite->setPositionX(_deleteSprite->getPositionX() - 160);
            }else
            {
                this->scaleContent(0.15, false);
                xMyArtData->deleteImageFromVec(_name);
                
                auto & graph = xChapter->getGraph(_SpriteName);
                graph.deleteHistory(_name);
                
//                Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("updateTalbe_graphscene");
                GraphScene* scene = (GraphScene*)this->getParent();
                if (scene)
                {
                    scene->updateTableView();
                }
                
                map<string, string> map;
                map.insert(pair<string, string>("image_id", xChapter->getCurrentChapter().getCurrentGraph().name));
                map.insert(pair<string, string>("delete_location", "library"));
                ATMInstance->sendEvent("image_delete", map);
                
            }
            
            
        }
            break;
            
        case st_button_share:
        {
            xChapter->enterGraph(_SpriteName);
            Graph & graph = xChapter->getCurrentChapter().getCurrentGraph();

            graph.setIndex(_name);
            xScene->forward(SceneManager::Tag::SHARE);
            
            map<string, string> map;
            map.insert(pair<string, string>("image_id", xChapter->getCurrentChapter().getCurrentGraph().name));
            map.insert(pair<string, string>("share_location", "library"));
            ATMInstance->sendEvent("image_share", map);
        }
            break;
            
        default:
            break;
    }
}

void GraphSpriteLayer::addListener()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch* touch, Event* event)->bool{
        
        _isClickContent = true;
        _isClickImage = false;
        
        Vec2 lPos = touch->getLocation();
        _firstPos = lPos;
        lPos = _contentSprite->getParent()->convertToNodeSpace(lPos);
        if (!_contentSprite->boundingBox().containsPoint(lPos))
        {
            _isClickContent = false;
        }
        
        Vec2 lPos2 = _showImage->getParent()->convertToNodeSpace(touch->getLocation());
        if (_showImage->boundingBox().containsPoint(lPos2))
        {
            _isClickImage = true;
        }
        
        return true;
    };
    
    listener->onTouchMoved = [this](Touch* touch, Event* event){
    
    };
    
    listener->onTouchEnded = [this](Touch* touch, Event* event){
        float offset = _firstPos.distance(touch->getLocation());
        if (offset < 5 && !_isExcuteAnimation && !_isClickContent)
        {
            this->scaleContent(0.15, false);
        }
        
        if (offset < 5 && _isClickImage)
        {
            xChapter->getCurrentChapter().getCurrentGraph().setNew(false);
            xScene->forward(SceneManager::Tag::GAME);
        }
    };
    
    listener->onTouchCancelled = [this](Touch* touch, Event* event){
        float offset = _firstPos.distance(touch->getLocation());
        if (offset < 5 && !_isExcuteAnimation && !_isClickContent)
        {
            this->scaleContent(0.15, false);
        }
        
        if (offset < 5 && _isClickImage)
        {
            xChapter->getCurrentChapter().getCurrentGraph().setNew(false);
            xScene->forward(SceneManager::Tag::GAME);
        }
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
}

void GraphSpriteLayer::scaleContent(const float duration, bool isEnlarge)
{
    _isExcuteAnimation = true;
    
    if (isEnlarge)
    {
        float scale = _contentSprite->getScale();
        _contentSprite->setOpacity(0);
        _contentSprite->setScale(0.0f);
        _contentSprite->runAction(Spawn::create(EaseSineIn::create(FadeIn::create(duration)), EaseSineOut::create(ScaleTo::create(duration, scale)), NULL) );
        
        this->setOpacity(0);
        this->runAction(Sequence::create(EaseSineIn::create(FadeTo::create(duration, 210)), CallFunc::create([this](){
            _isExcuteAnimation = false;
        }), NULL));
        
        
    }else
    {
        _contentSprite->runAction(Spawn::create(EaseSineIn::create(FadeOut::create(duration)), EaseSineIn::create(ScaleTo::create(duration, 0.0f)), NULL) );
        
        this->runAction(Sequence::create(EaseSineIn::create(FadeOut::create(duration)), CallFunc::create([this](){
            _isExcuteAnimation = false;
//            this->removeFromParentAndCleanup(true);
            Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("removeGraphSpriteLayer");
        }), NULL));
    }
}






