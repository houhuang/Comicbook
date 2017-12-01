//
//  InspirationScene.cpp
//  BabyPlay
//
//  Created by luoxp on 3/18/15.
//
//

#include "InspirationScene.h"
#include "UICpp.h"
#include "MyArtSprite.hpp"
#include "MyArtDataManager.hpp"
#include "ChapterManager.hpp"
#include "STSystemFunction.h"
#include "DownLoadPage.h"
#include "ThreadManager.hpp"
#include "STSystemFunction.h"
#include "UIColorManager.hpp"
#include "SSCInternalLibManager.h"
#include "AnalyticsManager.hpp"
#include "RateUsLayer.hpp"

#define BEGIN_POSITION  (V::isIpad()? 0.2227 : 0.2027)
#define END_POSTION     (V::isIpad()? 0.7277 : 0.7067)

enum
{
    st_MyArtworks_deleteBtn = 15,
    st_Cell_children_tag,
    
    st_inspiration_recent,
    st_inspiration_mylike,
    
    st_top_menu,
};

InspirationScene::~InspirationScene(){

    DownLoadPage::getInstance()->m_bIsFinishDownloadImage = false;
    DownLoadPage::getInstance()->_isRequestingAsk = false;

    _eventDispatcher->removeEventListenersForTarget(this);
}

InspirationScene::InspirationScene()
{
    Director::getInstance()->getTextureCache()->removeUnusedTextures();

    m_iImageNumber = 0;
    
    m_pTableView = NULL;
    m_bIsEdit = true;
    m_bIsNetwork = false;
    m_bIsShowLoading = true;
    _topMenu = nullptr;
    
    _ShowMode = st_inspiration_recent;
    
    _likeLayer = nullptr;
    _recentLayer = nullptr;
    _switch = false;
    _isTouchEnable = true;
    _updateLayer = nullptr;
    
    _btnBottomLine = nullptr;
    _btnIsTowardRightMoving = false;
    _btnIsTowardLeftMoving = false;
}

bool InspirationScene::init(){
    
    if(BaseScene::init())
    {

        this->setName("InspirationScene");
        auto _root = CSLoader::createNode((V::isIpad()? "InspirationScene_ipad.csb" : "InspirationScene.csb"));
        _root->setContentSize(this->getContentSize());
        Helper::doLayout(_root);
        this->addChild(_root);
        _root->visit();
        
        ui::Button* myArt = static_cast<Button*>(Helper::seekNodeByName(_root, "btn_myArt"));
        myArt->addClickEventListener(CC_CALLBACK_1(InspirationScene::onButton, this));
        _myart = myArt;
        
        
        ui::Button* gallery = static_cast<Button*>(Helper::seekNodeByName(_root,"btn_gallery"));
        gallery->addClickEventListener(CC_CALLBACK_1(InspirationScene::onButton, this));
        _gallery = gallery;
        
        
        Director::getInstance()->getTextureCache()->addImage("cocos/my_arts/paper.png");
        
//        loadTableView();
    
        _Recent = static_cast<Button*>(Helper::seekNodeByName(_root, "btn_recent"));
        _Recent->addClickEventListener(CC_CALLBACK_1(InspirationScene::onButton, this));
        
        _MyLike = static_cast<Button*>(Helper::seekNodeByName(_root, "btn_like"));
        _MyLike->addClickEventListener(CC_CALLBACK_1(InspirationScene::onButton, this));
        
        _Recent_text = static_cast<Text*>(Helper::seekNodeByName(_root, "btn_recent_text"));
        _MyLike_text = static_cast<Text*>(Helper::seekNodeByName(_root, "btn_like_text"));
        _MyLike_text->setColor(Color3B(119, 119, 119));
        _Recent_text->setColor(Color3B(43, 43, 43));
        
        float fontSize, scale;
        if (V::isHighVersion())
        {
            auto titleLayout = static_cast<Text*>(Helper::seekNodeByName(_root, "Text_1"));
            titleLayout->setFontSize(titleLayout->getFontSize()*0.8);
            titleLayout->setScale(titleLayout->getScale()/0.8);
            
            fontSize = _Recent_text->getFontSize() * 0.8;
            scale = _Recent_text->getScale()/0.8;
            
            _Recent_text->setFontSize(fontSize);
            _Recent_text->setScale(scale);
            _MyLike_text->setFontSize(fontSize);
            _MyLike_text->setScale(scale);
        }
        
        
        _tableBg = static_cast<Layout*>(Helper::seekNodeByName(_root, "Panel_5"));
        
        xMyArtData->m_bIsChapterEnterMyart = false;
        
        xAnalytics->sendScreenEvent("InspirationScene");
        
        xAds->removeAds(kTypeBannerAds);
        
        Sprite* btnLine = Sprite::create((V::isIpad()? "other/tabbat_line@3x.png":"other/ins_line.png"));
        btnLine->setAnchorPoint(Vec2(0, 0));
        btnLine->setPosition(Vec2(this->getContentSize().width*BEGIN_POSITION, this->getContentSize().height - TOP_HEIGHT_HEIGHT + 3));
        btnLine->setScale(V::isIpad()? 0.333 : 0.567);
        this->addChild(btnLine, 11);
        _btnBottomLine = btnLine;
        
        MenuItemImage* topMenu = MenuItemImage::create("other/topbtn.png", "other/topbtn.png", CC_CALLBACK_1(InspirationScene::onButton, this));
        topMenu->setScaleX(this->getContentSize().width/topMenu->getContentSize().width);
        topMenu->setScaleY(TOP_HEIGHT/topMenu->getContentSize().height);
        topMenu->setName("topMenu");
        topMenu->setAnchorPoint(Vec2(0.5, 1));
        topMenu->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height));
        
        Menu* lMenu = Menu::create(topMenu, NULL);
        lMenu->setPosition(Vec2::ZERO);
        this->addChild(lMenu, 99);
        _topMenu = lMenu;
        
        auto createUpdateLayer = EventListenerCustom::create("createUpdateLayer", CC_CALLBACK_1(InspirationScene::createSwallowLayer, this));
        _eventDispatcher->addEventListenerWithSceneGraphPriority(createUpdateLayer, this);
        
        auto removeUpdateLayer = EventListenerCustom::create("removeUpdateLayer", CC_CALLBACK_1(InspirationScene::removeSwallowLayer, this));
        _eventDispatcher->addEventListenerWithSceneGraphPriority(removeUpdateLayer, this);
        
        return true;
    }

    return false;
}

void InspirationScene::onButton(Ref* pSender)
{
    if (_ShowMode == st_inspiration_recent)
    {
        int index = _lookCount - 2;
        if (index < 0) index = 0;
        stringstream ss;
        ss<<index;
//        ATMInstance->sendEvent("scroll_recent", "scroll_depth", ss.str());
    }
    
    string name = ((Node*)pSender)->getName();
    
    if (name == "btn_gallery") {
        
        xScene->forward(SceneManager::Tag::CHAPTER, SceneManager::SceneAnimation::NONE);
//        if (xScene->getLastSceneTag() == SceneManager::Tag::SHARE) {
//            auto graph = xChapter->getCurrentChapter().getCurrentGraph();
//            auto path = FileUtils::getInstance()->getWritablePath() + graph.getPreviewName().c_str();
//            if (!FileUtils::getInstance()->isFileExist(path)) {
//                xScene->back(SceneManager::Tag::CHAPTER);
//                return;
//            }
//        }
//        
//        xScene->back();
    }else if (name == "btn_myArt")
    {
        xScene->forward(SceneManager::Tag::MY_ARTS, SceneManager::SceneAnimation::NONE);
    }else if (name == "btn_recent")
    {
        if (_ShowMode != st_inspiration_recent)
        {
            _MyLike_text->setColor(Color3B(119, 119, 119));
            _Recent_text->setColor(Color3B(43, 43, 43));
            
            this->moveFromClickButton(true);
            this->btnLineTowardLeftMove();
            
            _ShowMode = st_inspiration_recent;
        }
        
        
    }else if (name == "btn_like")
    {
        
        if (_ShowMode == st_inspiration_recent)
        {
            _MyLike_text->setColor(Color3B(43, 43, 43));
            _Recent_text->setColor(Color3B(119, 119, 119));
            
            this->moveFromClickButton(false);
            this->btnLineTowardRightMove();
            
            _ShowMode = st_inspiration_mylike;
        }
     
    }else if (name == "topMenu")
    {
        if (_recentLayer)
        {
            _recentLayer->tableScrollToTop();
            _likeLayer->tableScrollToTop();
            _topMenu->setEnabled(false);
            this->runAction(Sequence::create(DelayTime::create(0.3f), CallFunc::create([this](){
                _topMenu->setEnabled(true);
            }), NULL));
        }
    }
    
}

void InspirationScene::createSwallowLayer(EventCustom* event)
{
    SwallowListenerLayer* layer = SwallowListenerLayer::create(Color4B(0, 0, 0, 0));
    this->addChild(layer,100);
    layer->setTouchEnable(true);
    _updateLayer = layer;
}

void InspirationScene::removeSwallowLayer(EventCustom* event)
{
    if (_updateLayer)
    {
        _updateLayer->removeFromParentAndCleanup(true);
        _updateLayer = nullptr;
    }
}

void InspirationScene::addListener()
{

    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = [this](Touch* touch, Event* event)->bool{

        _firstPos = touch->getLocation();
        Vec2 lPos = this->convertToNodeSpace(touch->getLocation());
        _detalX = 0.0f;
        _moveDirectionIsH = false;
        _switch = false;
        
        if (!_isTouchEnable)
        {
            return false;
        }
        
        if (lPos.y > this->getContentSize().height - TOP_HEIGHT_HEIGHT || lPos.y < BOTTOM_HEIGHT)
        {
            return false;
        }
        
        if (this->getBoundingBox().containsPoint(lPos))
        {
            _isTouchEnable = false;
            return true;
        }
        
        return false;
    };
    
    listener->onTouchMoved = [this](Touch* touch, Event* event){
        
        if (!_switch)
        {
            _switch = true;
            Vec2 detal = touch->getDelta();
            if (abs(detal.x) > abs(detal.y))
            {
                _moveDirectionIsH = true;
                _recentLayer->shieldEvent(false);
                _likeLayer->shieldEvent(false);
                
            }else
            {
                _moveDirectionIsH = false;
                _recentLayer->shieldEvent(true);
                _likeLayer->shieldEvent(true);
            }
        }
        
        if (_moveDirectionIsH)
        {
            float offsetX = _recentLayer->getPosition().x + touch->getDelta().x;
            if (offsetX >= -0.5*this->getContentSize().width && offsetX <= 0.5*this->getContentSize().width)
            {
                _recentLayer->setPosition(_recentLayer->getPosition() + Vec2(touch->getDelta().x, 0));
                _likeLayer->setPosition(_likeLayer->getPosition() + Vec2(touch->getDelta().x, 0));
                _detalX = touch->getDelta().x;
            }
        }
        
        if (touch->getDelta().x < 0 && _recentLayer->getPosition().x < 0)
        {
            this->btnLineTowardRightMove();
        }
        
        if (touch->getDelta().x > 0 && _recentLayer->getPosition().x > 0)
        {
            this->btnLineTowardLeftMove();
        }
        
    };
    
    listener->onTouchEnded = [this](Touch* touch, Event* event){
        
        this->move();
    };
    
    listener->onTouchCancelled = [this](Touch* touch, Event* event){
        this->move();
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void InspirationScene::btnLineTowardRightMove()
{
    if (_btnIsTowardLeftMoving)
    {
        _btnIsTowardLeftMoving = false;
        _btnBottomLine->stopAllActions();
    }
    
    if (!_btnIsTowardRightMoving)
    {
        _MyLike_text->setColor(Color3B(43, 43, 43));
        _Recent_text->setColor(Color3B(119, 119, 119));
        _ShowMode = st_inspiration_mylike;
        
        _btnIsTowardRightMoving = true;
        _btnBottomLine->runAction(Sequence::create( EaseSineInOut::create(MoveTo::create(0.3f, Vec2(this->getContentSize().width * END_POSTION, this->getContentSize().height - TOP_HEIGHT_HEIGHT + 3))), CallFunc::create([this](){
            _btnIsTowardRightMoving = false;
        }), NULL));
    }
}
void InspirationScene::btnLineTowardLeftMove()
{
    if (_btnIsTowardRightMoving)
    {
        _btnIsTowardRightMoving = false;
        _btnBottomLine->stopAllActions();
    }
    
    if (!_btnIsTowardLeftMoving)
    {
        _MyLike_text->setColor(Color3B(119, 119, 119));
        _Recent_text->setColor(Color3B(43, 43, 43));
        _ShowMode = st_inspiration_recent;
        
        _btnIsTowardLeftMoving = true;
        _btnBottomLine->runAction(Sequence::create( EaseSineInOut::create(MoveTo::create(0.3f, Vec2(this->getContentSize().width*BEGIN_POSITION, this->getContentSize().height - TOP_HEIGHT_HEIGHT + 3))), CallFunc::create([this](){
            _btnIsTowardLeftMoving = false;
        }), NULL));
    }
}

void InspirationScene::moveFromClickButton(bool isClickLeft)
{
    if (isClickLeft)
    {
        if (_ShowMode == st_inspiration_mylike)
        {
            _recentLayer->shieldEvent(false);
            _likeLayer->shieldEvent(false);
            _isTouchEnable = false;
            
            _recentLayer->runAction(EaseSineOut::create(MoveTo::create(0.5, Vec2(0.5*this->getContentSize().width, _likeLayer->getPosition().y))));
            _likeLayer->runAction(Sequence::create(EaseSineOut::create(MoveTo::create(0.5, Vec2(1.5*this->getContentSize().width, _likeLayer->getPosition().y))), CallFunc::create([this](){
                this->moveEnded();
            }), NULL));
        }
    }else
    {
        if (_ShowMode == st_inspiration_recent)
        {
            _recentLayer->shieldEvent(false);
            _likeLayer->shieldEvent(false);
            _isTouchEnable = false;
            
            _recentLayer->runAction(EaseSineOut::create(MoveTo::create(0.5, Vec2(-0.5*this->getContentSize().width, _recentLayer->getPosition().y))));
            _likeLayer->runAction(Sequence::create(EaseSineOut::create(MoveTo::create(0.5, Vec2(0.5*this->getContentSize().width, _likeLayer->getPosition().y))), CallFunc::create([this](){
                this->moveEnded();
            }), NULL));
        }
    }
}

void InspirationScene::move()
{
    float layerX = _recentLayer->getPosition().x;
    if (abs(_detalX) < ACCELERATION_X)
    {
        if (abs(_detalX) > ACCELERATION_X/3 && _detalX < 0 && layerX < this->getContentSize().width/5)
        {
            this->btnLineTowardRightMove();
            
            float time = (this->getContentSize().width - layerX)/this->getContentSize().width * 0.5f;
            _recentLayer->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(-0.5*this->getContentSize().width, _recentLayer->getPosition().y))));
            _likeLayer->runAction(Sequence::create(EaseSineOut::create(MoveTo::create(time, Vec2(0.5*this->getContentSize().width, _likeLayer->getPosition().y))), CallFunc::create([this](){
                this->moveEnded();
            }), NULL));
        }else if (abs(_detalX) > ACCELERATION_X/3 && _detalX > 0 && layerX > -this->getContentSize().width/5*4)
        {
            this->btnLineTowardLeftMove();
            
            float time = (this->getContentSize().width + layerX)/this->getContentSize().width * 0.5f;
            _recentLayer->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(0.5*this->getContentSize().width, _recentLayer->getPosition().y))));
            _likeLayer->runAction(Sequence::create(EaseSineOut::create(MoveTo::create(time, Vec2(1.5*this->getContentSize().width, _likeLayer->getPosition().y))), CallFunc::create([this](){
                this->moveEnded();
            }), NULL));
        }else
        {
            if (layerX < 0)
            {
                float time = (this->getContentSize().width*0.5 + layerX)/this->getContentSize().width * 0.5 * 1.0f;
                _recentLayer->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(-0.5*this->getContentSize().width, _recentLayer->getPosition().y))));
                _likeLayer->runAction(Sequence::create(EaseSineOut::create(MoveTo::create(time, Vec2(0.5*this->getContentSize().width, _likeLayer->getPosition().y))), CallFunc::create([this](){
                    this->moveEnded();
                }), NULL));
                
                
            }else if (layerX > 0)
            {
                float time = (this->getContentSize().width*0.5 - layerX)/this->getContentSize().width* 0.5 * 1.0f;
                _recentLayer->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(0.5*this->getContentSize().width, _recentLayer->getPosition().y))));
                _likeLayer->runAction(Sequence::create(EaseSineOut::create(MoveTo::create(time, Vec2(1.5*this->getContentSize().width, _likeLayer->getPosition().y))), CallFunc::create([this](){
                    this->moveEnded();
                }), NULL));
            }
        }
        
    }else
    {
        if (_detalX < 0)
        {
            if (layerX > 0)
            {
                this->btnLineTowardRightMove();
                
                float time = (this->getContentSize().width - layerX)/this->getContentSize().width * 0.5f;
                _recentLayer->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(-0.5*this->getContentSize().width, _recentLayer->getPosition().y))));
                _likeLayer->runAction(Sequence::create(EaseSineOut::create(MoveTo::create(time, Vec2(0.5*this->getContentSize().width, _likeLayer->getPosition().y))), CallFunc::create([this](){
                    this->moveEnded();
                }), NULL));
            }else
            {
                float time = (this->getContentSize().width*0.5 + layerX)/this->getContentSize().width * 0.5f;
                _recentLayer->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(-0.5*this->getContentSize().width, _recentLayer->getPosition().y))));
                _likeLayer->runAction(Sequence::create(EaseSineOut::create(MoveTo::create(time, Vec2(0.5*this->getContentSize().width, _likeLayer->getPosition().y))), CallFunc::create([this](){
                    this->moveEnded();
                }), NULL));
            }
        }else
        {
            if (layerX > 0)
            {

                float time = (this->getContentSize().width*0.5 - layerX)/this->getContentSize().width*0.5 * 0.5f;
                _recentLayer->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(0.5*this->getContentSize().width, _recentLayer->getPosition().y))));
                _likeLayer->runAction(Sequence::create(EaseSineOut::create(MoveTo::create(time, Vec2(1.5*this->getContentSize().width, _likeLayer->getPosition().y))), CallFunc::create([this](){
                    this->moveEnded();
                }), NULL));
            }else
            {
                this->btnLineTowardLeftMove();
                
                float time = (this->getContentSize().width + layerX)/this->getContentSize().width * 0.5f;
                _recentLayer->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(0.5*this->getContentSize().width, _recentLayer->getPosition().y))));
                _likeLayer->runAction(Sequence::create(EaseSineOut::create(MoveTo::create(time, Vec2(1.5*this->getContentSize().width, _likeLayer->getPosition().y))), CallFunc::create([this](){
                    this->moveEnded();
                }), NULL));
            }
        }
        
    }
    
    //prevent bug
    if (layerX < this->getContentSize().width*-0.5 || _likeLayer->getPosition().x < this->getContentSize().width*0.5)
    {
        _recentLayer->setPositionX(this->getContentSize().width*-0.5);
        _likeLayer->setPositionX(this->getContentSize().width*0.5);
    }
    
    if (layerX > this->getContentSize().width*0.5 || _likeLayer->getPosition().x > 1.5*this->getContentSize().width)
    {
        _recentLayer->setPositionX(this->getContentSize().width*0.5);
        _likeLayer->setPositionX(this->getContentSize().width*1.5);
    }
    
}

void InspirationScene::moveEnded()
{
    _isTouchEnable = true;
    _recentLayer->shieldEvent(true);
    _likeLayer->shieldEvent(true);
    
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

void InspirationScene::onExit()
{
    Scene::onExit();
    xThread->m_bIsload = false;
    
    if (_recentLayer)
    {
        int count = _recentLayer->getlookCount();
        count = count - 2;
        if (count < 0)
        {
            count = 0;
        }
        
        ATMInstance->sendEvent("scroll_recent", "scroll_depth", std::to_string(count));
    }
    
//    DataManager::getInstance()->m_vNeedLoadImage.clear();
//    DataManager::getInstance()->m_bIsLoadFinish = false;
}


void InspirationScene::onEnterTransitionDidFinish()
{
    Scene::onEnterTransitionDidFinish();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    xThread->removeAllNeedloadImage();
    DownLoadPage::getInstance()->m_mNeedDownloadImage.clear();
    DownLoadPage::getInstance()->m_vNeedGetLikeImage.clear();
//    this->runAction(Sequence::create(DelayTime::create(0.2f), CallFunc::create([](){
//        Director::getInstance()->getTextureCache()->removeUnusedTextures();
//    }), NULL));
    
    if (!SSCInternalLibManager::getInstance()->checkNetworkAvailable())
    {
        NewDialog* dialog = NewDialog::create("network_err", "", "ok");
        dialog->addButtonListener(CC_CALLBACK_1(InspirationScene::onDialog, this));
        this->addChild(dialog, 101);
    }
    
    InspirationRecentLayer* insReLayer = InspirationRecentLayer::create(Size(this->getContentSize().width, this->getContentSize().height - TOP_HEIGHT_HEIGHT - BOTTOM_HEIGHT));
    insReLayer->ignoreAnchorPointForPosition(false);
    insReLayer->setAnchorPoint(Vec2(0.5, 0));
    insReLayer->setPosition(Vec2(this->getContentSize().width/2, BOTTOM_HEIGHT));
    _tableBg->addChild(insReLayer);
    _recentLayer = insReLayer;
    
    InspirationLikeLayer* insLiLayer = InspirationLikeLayer::create(Size(this->getContentSize().width, this->getContentSize().height - TOP_HEIGHT_HEIGHT - BOTTOM_HEIGHT - 2));
    insLiLayer->ignoreAnchorPointForPosition(false);
    insLiLayer->setAnchorPoint(Vec2(0.5, 0));
    insLiLayer->setPosition(Vec2(this->getContentSize().width*1.5, BOTTOM_HEIGHT));
    _tableBg->addChild(insLiLayer);
    _likeLayer = insLiLayer;
    
    this->addListener();
}

void InspirationScene::setButtonEnable(bool isEnable)
{
    this->runAction(Sequence::create(DelayTime::create(0.0f), CallFunc::create([this, isEnable](){
        _gallery->setTouchEnabled(isEnable);
        _myart->setTouchEnabled(isEnable);
        _MyLike->setTouchEnabled(isEnable);
        _Recent->setTouchEnabled(isEnable);
    
    }), NULL));
}

void InspirationScene::rateUsDialog(const string& name)
{

}

void InspirationScene::onDialog(const string & name)
{
    if (name == "ok") {
        xScene->forward(SceneManager::Tag::CHAPTER);
    }
}
