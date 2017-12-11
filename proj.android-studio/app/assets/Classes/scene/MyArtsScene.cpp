//
//  MyArtsScene.cpp
//  BabyPlay
//
//  Created by luoxp on 3/18/15.
//
//

#include "MyArtsScene.h"
#include "UICpp.h"
#include "MyArtSprite.hpp"
//#include "MyArtDataManager.hpp"
#include "ChapterManager.hpp"
#include "STSystemFunction.h"
#include "DownLoadPage.h"
#include "UIColorManager.hpp"
#include "SwallowListenerLayer.hpp"
#include "RuntimePermissionManager.h"
#include "STInstagramFunction.h"
#include <sys/stat.h>
#include "ThreadManager.hpp"
#include "SSCInternalLibManager.h"
#include "AnalyticsManager.hpp"

#define BEGIN_POSITION  (V::isIpad()? 0.2277 : 0.2027)
#define END_POSTION     (V::isIpad()? 0.7277 : 0.7067)

enum{
    st_tableCell_leftChild_tag = 11,
    st_tableCell_rightChild_tag,
    
    st_myartDialog_dupilicate_tag,
    st_myartDialog_continue_tag,
    st_myartDialog_newOne_tag,
    st_myartDialog_share_tag,
    st_myartDialog_post_tag,
    st_myartDialog_save_tag,
    st_myartDialog_delete_tag,
    st_myartDialog_cancel_tag,
};

MyArtsScene::~MyArtsScene(){
    NotificationCenter::getInstance()->removeObserver(this, st_deleteMyArtSprite);

    NotificationCenter::getInstance()->removeObserver(this, st_myPost_failed);
    NotificationCenter::getInstance()->removeObserver(this, st_myPost_success);
}

MyArtsScene::MyArtsScene()
{
    _isShowAll = true;
    _offset = 0;
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    _swallowLayer = NULL;
    
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(MyArtsScene::waitPostSuccessful), st_myPost_success, NULL);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(MyArtsScene::waitPostFaild), st_myPost_failed, NULL);
    
    _allLayer = nullptr;
    _publishedLayer = nullptr;

    _switch = false;
    _isTouchEnable = true;
    
    _btnBottomLine = nullptr;
    _btnIsTowardRightMoving = false;
    _btnIsTowardLeftMoving = false;
    _topMenu = nullptr;
    
}

bool MyArtsScene::init(){
    
    if(BaseScene::init())
    {
        
        
        this->setName("MyArtsScene");
        auto _root = CSLoader::createNode((V::isIpad()? "MyArtsScene_ipad.csb" : "MyArtsScene.csb"));
        _root->setContentSize(this->getContentSize());
        Helper::doLayout(_root);
        this->addChild(_root,1);
        _root->visit();
       
        ui::Button* gallery = static_cast<Button*>(Helper::seekNodeByName(_root, "btn_gallery"));
        gallery->addClickEventListener(CC_CALLBACK_1(MyArtsScene::onButton, this));
        
        ui::Button* insp = static_cast<Button*>(Helper::seekNodeByName(_root,"btn_inspiration"));
        insp->addClickEventListener(CC_CALLBACK_1(MyArtsScene::onButton, this));
        
        Director::getInstance()->getTextureCache()->addImage("cocos/my_arts/paper.png");

        
        _All = static_cast<Button*>(Helper::seekNodeByName(_root, "btn_all"));
        _All->addClickEventListener(CC_CALLBACK_1(MyArtsScene::onButton, this));
        
        _Pub = static_cast<Button*>(Helper::seekNodeByName(_root, "btn_pub"));
        _Pub->addClickEventListener(CC_CALLBACK_1(MyArtsScene::onButton, this));
        
        _All_btn_text = static_cast<Text*>(Helper::seekNodeByName(_root, "btn_all_text"));
        _Pub_btn_text = static_cast<Text*>(Helper::seekNodeByName(_root, "btn_pub_text"));
        _All_btn_text->setColor(Color3B(43, 43, 43));
        _Pub_btn_text->setColor(Color3B(119, 119, 119));

        float fontSize, scale;
        if (V::isHighVersion())
        {
            auto titleLayout = static_cast<Text*>(Helper::seekNodeByName(_root, "Text_1"));
            titleLayout->setFontSize(titleLayout->getFontSize()*0.8);
            titleLayout->setScale(titleLayout->getScale()/0.8);
            
            fontSize = _All_btn_text->getFontSize() * 0.8;
            scale = _All_btn_text->getScale()/0.8;
            
            _All_btn_text->setFontSize(fontSize);
            _All_btn_text->setScale(scale);
            _Pub_btn_text->setFontSize(fontSize);
            _Pub_btn_text->setScale(scale);
        }
        
        xAnalytics->sendScreenEvent("MyArtsScene");
        xAds->removeAds(kTypeBannerAds);
        
        _tableBg = static_cast<Layout*>(Helper::seekNodeByName(_root, "Panel_6"));
        
        Sprite* btnLine = Sprite::create((V::isIpad()? "other/tabbat_line@3x.png":"other/ins_line.png"));
        btnLine->setAnchorPoint(Vec2(0, 0));
        btnLine->setPosition(Vec2(this->getContentSize().width*BEGIN_POSITION, this->getContentSize().height - TOP_HEIGHT_HEIGHT + 3));
        btnLine->setScale(V::isIpad()? 0.333 : 0.567);
        this->addChild(btnLine, 11);
        _btnBottomLine = btnLine;
        
        MenuItemImage* topMenu = MenuItemImage::create("other/topbtn.png", "other/topbtn.png", CC_CALLBACK_1(MyArtsScene::onButton, this));
        topMenu->setScaleX(this->getContentSize().width/topMenu->getContentSize().width);
        topMenu->setScaleY(TOP_HEIGHT/topMenu->getContentSize().height);
        topMenu->setName("topMenu");
        topMenu->setAnchorPoint(Vec2(0.5, 1));
        topMenu->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height));
        
        Menu* lMenu = Menu::create(topMenu, NULL);
        lMenu->setPosition(Vec2::ZERO);
        this->addChild(lMenu, 99);
        _topMenu = lMenu;
        
        return true;
        
    }

    return false;
}

void MyArtsScene::responseUpdloadImage(EventCustom* event)
{
    if (!SSCInternalLibManager::getInstance()->checkNetworkAvailable())
    {
        NewDialog* dialog = NewDialog::create("network_err", "", "ok");
        dialog->addButtonListener(CC_CALLBACK_1(MyArtsScene::onDialog, this));
        this->addChild(dialog, 101);
        
        return;
    }
    
    if (_swallowLayer)
    {
        _swallowLayer->removeFromParentAndCleanup(true);
    }
    
    PostingLayer* lSwallowLayer = PostingLayer::create(Color4B(0, 0, 0, 200));
    this->addChild(lSwallowLayer, 100);
    lSwallowLayer->setTouchEnable(true);
    m_pPostingLayer = lSwallowLayer;
    
    runAction(Sequence::create(DelayTime::create(0.1), CallFunc::create([&](){
        
        int pre_name_index = (int)DownLoadPage::m_sCurrentPostImageName.find_last_of("_");
        string post_name = DownLoadPage::m_sCurrentPostImageName.substr(0, pre_name_index+1) + "save.png";
        DownLoadPage::getInstance()->postImage(post_name);
//        DownLoadPage::getInstance()->postImageFromHttp(post_name);
        
    }), nullptr));
    
//    map<string, string> map;
//    map.insert(pair<string, string>("image_id", name));
//    map.insert(pair<string, string>("category_name", xChapter->getCurrentChapter().name));
//    map.insert(pair<string, string>("published_location", "myartwork_all"));
//    ATMInstance->sendEvent("image_publish", map);
    
}

void MyArtsScene::onEnterTransitionDidFinish()
{
    Scene::onEnterTransitionDidFinish();
//    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    xThread->removeAllNeedloadImage();
    DownLoadPage::getInstance()->m_vNeedGetLikeImage.clear();
    
    this->runAction(Sequence::create(DelayTime::create(0.2f), CallFunc::create([](){
        Director::getInstance()->getTextureCache()->removeUnusedTextures();
    }), NULL));
    
    MyartAllLayer* allLayer = MyartAllLayer::create(Size(this->getContentSize().width, this->getContentSize().height - TOP_HEIGHT_HEIGHT - BOTTOM_HEIGHT));
    allLayer->ignoreAnchorPointForPosition(false);
    allLayer->setAnchorPoint(Vec2(0.5, 0));
    allLayer->setPosition(Vec2(this->getContentSize().width/2, BOTTOM_HEIGHT));
    _tableBg->addChild(allLayer, 1);
    _allLayer = allLayer;
    
    MyartPublishedLayer* pubLayer = MyartPublishedLayer::create(Size(this->getContentSize().width, this->getContentSize().height - TOP_HEIGHT_HEIGHT - BOTTOM_HEIGHT));
    pubLayer->ignoreAnchorPointForPosition(false);
    pubLayer->setAnchorPoint(Vec2(0.5, 0));
    pubLayer->setPosition(Vec2(this->getContentSize().width*1.5, BOTTOM_HEIGHT));
    _tableBg->addChild(pubLayer, 1);
    _publishedLayer = pubLayer;
    
    this->addListener();
    
    auto responseUploadEvent = EventListenerCustom::create("myart_upload", CC_CALLBACK_1(MyArtsScene::responseUpdloadImage, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(responseUploadEvent, this);
}

void MyArtsScene::addListener()
{
    float top,bottom;
    if (V::isIpad())
    {
        top = TOP_LINE + 50;
        bottom = BOTTOM_LINE;
    }else
    {
        top = 140;
        bottom = 98;
    }
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = [this, top, bottom](Touch* touch, Event* event)->bool{
        
        _firstPos = touch->getLocation();
        Vec2 lPos = this->convertToNodeSpace(touch->getLocation());
        _detalX = 0.0f;
        _moveDirectionIsH = false;
        _switch = false;
        
        if (!_isTouchEnable)
        {
            return false;
        }
        
        if (lPos.y > this->getContentSize().height - top || lPos.y < bottom)
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
                _allLayer->shieldEvent(false);
                _publishedLayer->shieldEvent(false);
                
            }else
            {
                _moveDirectionIsH = false;
                _allLayer->shieldEvent(true);
                _publishedLayer->shieldEvent(true);
            }
        }
        
        if (_moveDirectionIsH)
        {
            float offsetX = _allLayer->getPosition().x + touch->getDelta().x;
            if (offsetX >= -0.5*this->getContentSize().width && offsetX <= 0.5*this->getContentSize().width)
            {
                _allLayer->setPosition(_allLayer->getPosition() + Vec2(touch->getDelta().x, 0));
                _publishedLayer->setPosition(_publishedLayer->getPosition() + Vec2(touch->getDelta().x, 0));
                _detalX = touch->getDelta().x;
            }
        }
        
        if (touch->getDelta().x < 0 && _allLayer ->getPosition().x < 0)
        {
            this->btnLineTowardRightMove();
        }
        
        if (touch->getDelta().x > 0 && _allLayer->getPosition().x > 0)
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


void MyArtsScene::btnLineTowardRightMove()
{
    if (_btnIsTowardLeftMoving)
    {
        _btnIsTowardLeftMoving = false;
        _btnBottomLine->stopAllActions();
    }
    
    if (!_btnIsTowardRightMoving)
    {
        _All_btn_text->setColor(Color3B(119, 119, 119));
        _Pub_btn_text->setColor(Color3B(43, 43, 43));
        _isShowAll = false;
        
        _btnIsTowardRightMoving = true;
        _btnBottomLine->runAction(Sequence::create( EaseSineInOut::create(MoveTo::create(0.3f, Vec2(this->getContentSize().width*END_POSTION, this->getContentSize().height - TOP_HEIGHT_HEIGHT + 3))), CallFunc::create([this](){
            _btnIsTowardRightMoving = false;
        }), NULL));
    }
}
void MyArtsScene::btnLineTowardLeftMove()
{
    if (_btnIsTowardRightMoving)
    {
        _btnIsTowardRightMoving = false;
        _btnBottomLine->stopAllActions();
    }
    
    if (!_btnIsTowardLeftMoving)
    {
        _All_btn_text->setColor(Color3B(43, 43, 43));
        _Pub_btn_text->setColor(Color3B(119, 119, 119));
        _isShowAll = true;
        
        _btnIsTowardLeftMoving = true;
        _btnBottomLine->runAction(Sequence::create( EaseSineInOut::create(MoveTo::create(0.3f, Vec2(this->getContentSize().width * BEGIN_POSITION, this->getContentSize().height - TOP_HEIGHT_HEIGHT + 3))), CallFunc::create([this](){
            _btnIsTowardLeftMoving = false;
        }), NULL));
    }
}

void MyArtsScene::moveFromClickButton(bool isClickLeft)
{
    if (isClickLeft)
    {
        if (!_isShowAll)
        {
            _allLayer->shieldEvent(false);
            _publishedLayer->shieldEvent(false);
            _isTouchEnable = false;
            
            _allLayer->runAction(EaseSineOut::create(MoveTo::create(0.5, Vec2(0.5*this->getContentSize().width, _allLayer->getPosition().y))));
            _publishedLayer->runAction(Sequence::create(EaseSineOut::create(MoveTo::create(0.5, Vec2(1.5*this->getContentSize().width, _publishedLayer->getPosition().y))), CallFunc::create([this](){
                this->moveEnded();
            }), NULL));
        }
    }else
    {
        if (_isShowAll)
        {
            _allLayer->shieldEvent(false);
            _publishedLayer->shieldEvent(false);
            _isTouchEnable = false;
            
            _allLayer->runAction(EaseSineOut::create(MoveTo::create(0.5, Vec2(-0.5*this->getContentSize().width, _allLayer->getPosition().y))));
            _publishedLayer->runAction(Sequence::create(EaseSineOut::create(MoveTo::create(0.5, Vec2(0.5*this->getContentSize().width, _publishedLayer->getPosition().y))), CallFunc::create([this](){
                this->moveEnded();
            }), NULL));
        }
    }
}

void MyArtsScene::move()
{
    float layerX = _allLayer->getPosition().x;
    if (abs(_detalX) < ACCELERATION_X)
    {
        if (abs(_detalX) > ACCELERATION_X/3 && _detalX < 0 && layerX < this->getContentSize().width/5)
        {
            this->btnLineTowardRightMove();
            
            float time = (this->getContentSize().width - layerX)/this->getContentSize().width * 0.5f;
            _allLayer->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(-0.5*this->getContentSize().width, _allLayer->getPosition().y))));
            _publishedLayer->runAction(Sequence::create(EaseSineOut::create(MoveTo::create(time, Vec2(0.5*this->getContentSize().width, _publishedLayer->getPosition().y))), CallFunc::create([this](){
                this->moveEnded();
            }), NULL));
        }else if (abs(_detalX) > ACCELERATION_X/3 && _detalX > 0 && layerX > -this->getContentSize().width/5*4)
        {
            this->btnLineTowardLeftMove();
            
            float time = (this->getContentSize().width + layerX)/this->getContentSize().width * 0.5f;
            _allLayer->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(0.5*this->getContentSize().width, _allLayer->getPosition().y))));
            _publishedLayer->runAction(Sequence::create(EaseSineOut::create(MoveTo::create(time, Vec2(1.5*this->getContentSize().width, _publishedLayer->getPosition().y))), CallFunc::create([this](){
                this->moveEnded();
            }), NULL));
        }else
        {
            if (layerX < 0)
            {
                float time = (this->getContentSize().width*0.5 + layerX)/this->getContentSize().width * 0.5 * 1.0f;
                _allLayer->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(-0.5*this->getContentSize().width, _allLayer->getPosition().y))));
                _publishedLayer->runAction(Sequence::create(EaseSineOut::create(MoveTo::create(time, Vec2(0.5*this->getContentSize().width, _publishedLayer->getPosition().y))), CallFunc::create([this](){
                    this->moveEnded();
                }), NULL));
                
                
            }else if (layerX > 0)
            {
                float time = (this->getContentSize().width*0.5 - layerX)/this->getContentSize().width* 0.5 * 1.0f;
                _allLayer->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(0.5*this->getContentSize().width, _allLayer->getPosition().y))));
                _publishedLayer->runAction(Sequence::create(EaseSineOut::create(MoveTo::create(time, Vec2(1.5*this->getContentSize().width, _publishedLayer->getPosition().y))), CallFunc::create([this](){
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
                _allLayer->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(-0.5*this->getContentSize().width, _allLayer->getPosition().y))));
                _publishedLayer->runAction(Sequence::create(EaseSineOut::create(MoveTo::create(time, Vec2(0.5*this->getContentSize().width, _publishedLayer->getPosition().y))), CallFunc::create([this](){
                    this->moveEnded();
                }), NULL));
            }else
            {
                float time = (this->getContentSize().width*0.5 + layerX)/this->getContentSize().width * 0.5f;
                _allLayer->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(-0.5*this->getContentSize().width, _allLayer->getPosition().y))));
                _publishedLayer->runAction(Sequence::create(EaseSineOut::create(MoveTo::create(time, Vec2(0.5*this->getContentSize().width, _publishedLayer->getPosition().y))), CallFunc::create([this](){
                    this->moveEnded();
                }), NULL));
            }
        }else
        {
            if (layerX > 0)
            {
                float time = (this->getContentSize().width*0.5 - layerX)/this->getContentSize().width*0.5 * 0.5f;
                _allLayer->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(0.5*this->getContentSize().width, _allLayer->getPosition().y))));
                _publishedLayer->runAction(Sequence::create(EaseSineOut::create(MoveTo::create(time, Vec2(1.5*this->getContentSize().width, _publishedLayer->getPosition().y))), CallFunc::create([this](){
                    this->moveEnded();
                }), NULL));
            }else
            {
                this->btnLineTowardLeftMove();
                
                float time = (this->getContentSize().width + layerX)/this->getContentSize().width * 0.5f;
                _allLayer->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(0.5*this->getContentSize().width, _allLayer->getPosition().y))));
                _publishedLayer->runAction(Sequence::create(EaseSineOut::create(MoveTo::create(time, Vec2(1.5*this->getContentSize().width, _publishedLayer->getPosition().y))), CallFunc::create([this](){
                    this->moveEnded();
                }), NULL));
            }
        }
        
    }
    
    //prevent bug
    if (layerX < this->getContentSize().width*-0.5 || _publishedLayer->getPosition().x < this->getContentSize().width*0.5)
    {
        _allLayer->setPositionX(this->getContentSize().width*-0.5);
        _publishedLayer->setPositionX(this->getContentSize().width*0.5);
    }
    
    if (layerX > this->getContentSize().width*0.5 || _publishedLayer->getPosition().x > 1.5*this->getContentSize().width)
    {
        _allLayer->setPositionX(this->getContentSize().width*0.5);
        _publishedLayer->setPositionX(this->getContentSize().width*1.5);
    }
}

void MyArtsScene::moveEnded()
{
    _isTouchEnable = true;
    _allLayer->shieldEvent(true);
    _publishedLayer->shieldEvent(true);
    
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

void MyArtsScene::onButton(Ref* pSender)
{
    string name = ((Node*)pSender)->getName();
    
    if (name == "btn_gallery") {
        
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
        xScene->forward(SceneManager::Tag::CHAPTER, SceneManager::SceneAnimation::NONE);
    }else if (name == "btn_inspiration")
    {
        DownLoadPage::m_bIsShareByInsp = false;
        xScene->forward(SceneManager::Tag::INSPIRATION, SceneManager::SceneAnimation::NONE);
    }else if (name == "btn_all")
    {
        if (!_isShowAll)
        {
            this->moveFromClickButton(true);
            this->btnLineTowardLeftMove();
        }
        
    }else if (name == "btn_pub")
    {
        if (_isShowAll)
        {
            this->moveFromClickButton(false);
            this->btnLineTowardRightMove();
        }
    }else if (name == "topMenu")
    {
        if (_allLayer)
        {
            _allLayer->tableScrollToTop();
            _publishedLayer->tableScrollToTop();
            _topMenu->setEnabled(false);
            this->runAction(Sequence::create(DelayTime::create(0.3f), CallFunc::create([this](){
                _topMenu->setEnabled(true);
            }), NULL));
        }
    }
}


void MyArtsScene::hideDialog()
{
    if (_swallowLayer)
    {
        _swallowLayer->removeFromParentAndCleanup(true);
    }
}

void MyArtsScene::doAction(Ref* ref)
{
    MenuItemImage* lMenuItem = dynamic_cast<MenuItemImage*>(ref);

    switch (lMenuItem->getTag())
    {
        case st_myartDialog_dupilicate_tag:
        {
//            xChapter->enterGraph(m_pMyartSprite->getSpriteName());
//            Graph & graph = xChapter->getCurrentChapter().getCurrentGraph();
//            graph.setDuplicate(m_pMyartSprite->getPreName(), spriteBGColor);
//            xScene->forward(SceneManager::Tag::GAME);
//            
//            ATMInstance->sendEvent("image_duplicate", "image_id", graph.name);
        }
            break;
            

        case st_myartDialog_continue_tag:
        {
            xChapter->enterGraph(delete_NeedLoadImage);
            Graph & graph = xChapter->getCurrentChapter().getCurrentGraph();
            graph.setBGColor(spriteBGColor);
            
            map<string, string> map;
            map.insert(pair<string, string>("image_id", name));
            map.insert(pair<string, string>("continue_location", "myartwork_all"));
            ATMInstance->sendEvent("image_continue", map);

//            log("%d,%d,%d",lSprite->getBGColor().r,lSprite->getBGColor().g,lSprite->getBGColor().b);
            graph.setIndex(delete_PreName);
            graph.setNew(false);
            xScene->forward(SceneManager::Tag::GAME);
        }
            break;
            
        case st_myartDialog_newOne_tag:
        {
            this->enterNextScene();

            map<string, string> map;
            map.insert(pair<string, string>("image_id", name));
            if (_isShowAll)
            {
                map.insert(pair<string, string>("restart_location", "myartwork_all"));
            }else
            {
                map.insert(pair<string, string>("restart_location", "published"));
            }
            
            ATMInstance->sendEvent("image_restart", map);

        }
            break;
            
        case st_myartDialog_share_tag:
        {
            Image* image = new Image();
            image->initWithImageFile(FileUtils::getInstance()->getWritablePath() + delete_PreName);
            
            string lPath;
            STSystemFunction sf;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            lPath = FileUtils::getInstance()->getWritablePath() + delete_PreName;
            sf.sendPictureToMore(lPath.c_str());
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            lPath = SSCInternalLibManager::getInstance()->getSDCardPath() + "/share_alabs0006.png";
            
            image->saveToFile(lPath, false);
            
            SSCInternalLibManager::getInstance()->sendEmailAndFilePic("", "Just painted this myself. So cool and relaxing! Come join me! ", lPath.c_str());
            
            //            STInstagramFunction st;
            //            st.sendImageToInstagram(path.c_str());
#endif

            image->release();
            
            if (_swallowLayer)
            {
                _swallowLayer->removeFromParentAndCleanup(true);
            }
            
            map<string, string> map;
            map.insert(pair<string, string>("image_id", name));
            if (_isShowAll)
            {
                map.insert(pair<string, string>("share_location", "myartwork_all"));
            }else
            {
                map.insert(pair<string, string>("share_location", "published"));
            }
            
            ATMInstance->sendEvent("image_share", map);
            
        }
            break;
            
        case st_myartDialog_post_tag:
        {

            if (_swallowLayer)
            {
                _swallowLayer->removeFromParentAndCleanup(true);
            }
            
            PostingLayer* lSwallowLayer = PostingLayer::create(Color4B(0, 0, 0, 200));
            this->addChild(lSwallowLayer, 100);
            lSwallowLayer->setTouchEnable(true);
            m_pPostingLayer = lSwallowLayer;
            
            Label* label = Label::createWithTTF("POSTING...", "res/kalinga.ttf", 50);
            label->setPosition(Vec2(lSwallowLayer->getContentSize().width/2, lSwallowLayer->getContentSize().height/2));
            lSwallowLayer->addChild(label);
            
            runAction(Sequence::create(DelayTime::create(0.1), CallFunc::create([&](){

                int pre_name_index = (int)DownLoadPage::m_sCurrentPostImageName.find_last_of("_");
                string post_name = DownLoadPage::m_sCurrentPostImageName.substr(0, pre_name_index+1) + "save.png";
                DownLoadPage::getInstance()->postImage(post_name);
                
            }), nullptr));
            
            map<string, string> map;
            map.insert(pair<string, string>("image_id", name));
            map.insert(pair<string, string>("category_name", xChapter->getCurrentChapter().name));
            map.insert(pair<string, string>("published_location", "myartwork_all"));
            ATMInstance->sendEvent("image_publish", map);
            
        }
            break;
            
        case st_myartDialog_save_tag:
        {
            Image* image = new Image();
            image->initWithImageFile(FileUtils::getInstance()->getWritablePath() + delete_PreName);
            saveImage = image;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

            STSystemFunction function = STSystemFunction();
            function.saveLocalImage(saveImage);
            
            this->runAction(Sequence::create(DelayTime::create(2.0f), CallFunc::create([this](){
                    saveImage->release();
                }), NULL));

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            RuntimePermissionManager::getInstance()->onPermissionGrantedResult = [&](int requestcode,bool bgranted){
                onPermissionGrantedResult(requestcode, bgranted);
            };
            RuntimePermissionManager::getInstance()->requestRuntimePermissions(1, PERMISSION::kWriteExternalStorage);
#endif
            
            if (_swallowLayer)
            {
                _swallowLayer->removeFromParentAndCleanup(true);
            }
            
//
            
            map<string, string> map;
            map.insert(pair<string, string>("image_id", name));
            if (_isShowAll)
            {
                map.insert(pair<string, string>("save_location", "myartwork_all"));
            }else
            {
                map.insert(pair<string, string>("save_location", "published"));
            }
            
            ATMInstance->sendEvent("image_save", map);
        }
            break;
            
        case st_myartDialog_delete_tag:
        {
//            xMyArtData->deleteImageFromVec(delete_PreName);
//            this->initMyartData();
//            
//            auto & graph = xChapter->getGraph(delete_NeedLoadImage);
//            graph.deleteHistory(delete_PreName);
//            
//            this->reponseDeleteMyArt(NULL);
//            
//            if (_swallowLayer)
//            {
//                _swallowLayer->removeFromParentAndCleanup(true);
//            }
        }
            break;
            
        case st_myartDialog_cancel_tag:
        {
            if (_swallowLayer)
            {
                _swallowLayer->removeFromParentAndCleanup(true);
            }
        }
            break;
            
        default:
            break;
    }
}

void MyArtsScene::waitPostFaild(Ref* ref)
{
    
    if (m_pPostingLayer)
    {
        m_pPostingLayer->removeFromParentAndCleanup(true);
        m_pPostingLayer = NULL;
    }
    
    
    NewDialog* dialog = NewDialog::create("postFail", "later", "postfail_ok");
    dialog->addButtonListener(CC_CALLBACK_1(MyArtsScene::onDialog, this));
    this->addChild(dialog, 101);
    
}

void MyArtsScene::waitPostSuccessful(Ref* ref)
{
    if (m_pPostingLayer)
    {
        m_pPostingLayer->setPushSuccessed();
        m_pPostingLayer = NULL;
    }
    
    if (_publishedLayer)
    {
        _publishedLayer->updateTableView();
    }

//    NewDialog* dialog = NewDialog::create("post to insprision", "later", "go_ins");
//    dialog->addButtonListener(CC_CALLBACK_1(MyArtsScene::onDialog, this));
//    this->addChild(dialog, 101);
    
    quickFlurry(string("insprision_") + delete_NeedLoadImage);
}

void MyArtsScene::enterNextScene()
{
//    xChapter->enterGraph(m_pMyartSprite->getSpriteName());
//    Graph & graph = xChapter->getCurrentChapter().getCurrentGraph();
//    graph.setNew(true);
//    xScene->forward(SceneManager::Tag::GAME);
}

void MyArtsScene::onDialog(const string& name)
{
    if(name == "new"){
        this->enterNextScene();
    }
    else if (name == "cancel"){
        
    }else if (name == "later")
    {
        if (m_pPostingLayer)
        {
            m_pPostingLayer->removeFromParentAndCleanup(true);
            m_pPostingLayer = NULL;
        }
    }else if (name == "tryAgain")
    {
        PostingLayer* lSwallowLayer = PostingLayer::create(Color4B(0, 0, 0, 200));
        this->addChild(lSwallowLayer, 100);
        lSwallowLayer->setTouchEnable(true);
        m_pPostingLayer = lSwallowLayer;
    
        runAction(Sequence::create(DelayTime::create(0.1), CallFunc::create([&](){
            
            int pre_name_index = (int)DownLoadPage::m_sCurrentPostImageName.find_last_of("_");
            string post_name = DownLoadPage::m_sCurrentPostImageName.substr(0, pre_name_index+1) + "save.png";
            DownLoadPage::getInstance()->postImage(post_name);
            
        }), nullptr));
    }else if (name == "go_ins")
    {
        xScene->forward(SceneManager::Tag::INSPIRATION);
    }else if (name == "postfail_ok")
    {
        string imageName = DownLoadPage::_postFailNeedNewOneName;
        if (!xChapter->enterGraph(imageName))
        {
            Graph graph;
            graph.name = imageName.substr(0, imageName.find(".png"));
            graph.historyIndex = xUser->getIntegerForKey(string(graph.name + "_hIndex").c_str(), 0);
            graph.historyCount = xUser->getIntegerForKey(string(graph.name + "_hCount").c_str(), 1);
            graph.setNew(true);
            
            xChapter->getCurrentChapter().setCurrentGraph(graph);
            xScene->forward(SceneManager::Tag::GAME);
            
        }else{
            xChapter->enterGraph(imageName);
            Graph & graph = xChapter->getCurrentChapter().getCurrentGraph();
            graph.setNew(true);
            xScene->forward(SceneManager::Tag::GAME);
        }
        
    }
}

void MyArtsScene::onPermissionGrantedResult(int requestCode,bool bGranted) {
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
    if (requestCode == 1) {
        if (bGranted) {
            //add your code....
            log("-------->anroid runtime permisson was granted,requestcode = %d",requestCode);
            
            auto writePath = SSCInternalLibManager::getInstance()->getSDCardPath()+"/Color Book/";
            
            if(!FileUtils::sharedFileUtils()->isFileExist(writePath))
            {
                mode_t processMask = umask(0);
                int ret = mkdir(writePath.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
                umask(processMask);
                if (ret != 0 && (errno != EEXIST))
                    return;
            }
            
            struct timeval now;
            gettimeofday(&now, nullptr);
            std::string _time = StringUtils::format("%ld", (now.tv_sec * 1000 + now.tv_usec / 1000));
            std::string fullPath = writePath + _time + ".jpg";
            
            bool isSaveSuccess = saveImage->saveToFile(fullPath.c_str());
            if(isSaveSuccess)
                SSCInternalLibManager::getInstance()->makeToast("Saved to Camera Roll!");
            else
                SSCInternalLibManager::getInstance()->makeToast("Allow access to your album in \"Setting\"->\"Privacy\"->\"Photos\"");
            SSCInternalLibManager::getInstance()->refreshDCIM(fullPath);
            
            saveImage->release();
        }else{
            //add your code....
            log("-------->anroid runtime permisson was not  granted,requestcode = %d",requestCode);
        }
    }
#endif
}

