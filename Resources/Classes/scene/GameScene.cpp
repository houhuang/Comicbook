//
//  GameScene.cpp
//  BabyPlay
//
//  Created by luoxp on 3/18/15.
//
//

#include "GameScene.h"
#include "UICpp.h"
#include "AppConfigs.h"
#include "UndoManager.h"
#include "PalletteManager.hpp"
#include "MakeMask.h"
#include "BaseMode.h"
#include "NoneMode.h"
#include "PaintMode.h"
#include "MoveMode.h"
#include "ZoomMode.h"
#include "WaitMode.h"
#include "PickupMode.hpp"
#include "ChapterManager.hpp"
#include "BlockLayer.h"
#include "ProgressManager.h"
#include "LockManager.h"
#include "MyArtDataManager.hpp"

#include "STSystemFunction.h"
#include "DownloadManager.hpp"
#include "UIColorManager.hpp"
#include "STVisibleRect.h"
#include "NewDownloadManager.hpp"
#include "AnalyticsManager.hpp"
#include "BannerManager.hpp"
#include "ColorLoopScrollLayer.hpp"
#include "ColorAdsManager.h"

GameScene::GameScene():_graph(xChapter->getCurrentChapter().getCurrentGraph()){
    NotificationCenter::getInstance()->addObserver(NULL, callfuncO_selector(GameScene::waitDownload), st_download_finish, this);
    
    _time = 0.0;
    _fillNode = NULL;
    _blockLayer = nullptr;
    _pickupSprite = nullptr;
    _pickupImage = nullptr;
}

GameScene::~GameScene(){
    _scheduler->unscheduleAllForTarget(this);
    NotificationCenter::getInstance()->removeObserver(this, st_download_finish);
}

void GameScene::waitDownload(Ref* ref)
{
    _graph = xChapter->getCurrentChapter().getCurrentGraph();
}

bool GameScene::init(){
    
    if(BaseScene::init())
    {
        setName("GameScene");

        _root = CSLoader::createNode((V::isIpad()? "GameScene_ipad.csb" : "GameScene.csb"));
        _root->setContentSize(this->getContentSize());
        Helper::doLayout(_root);
        this->addChild(_root);
        _root->visit();
        
        _back = static_cast<Button*>(Helper::seekNodeByName(_root, "back_touch"));
        _back->addClickEventListener(CC_CALLBACK_1(GameScene::onButton, this));
        
        _done = static_cast<Button*>(Helper::seekNodeByName(_root, "done_touch"));
        _done->addClickEventListener(CC_CALLBACK_1(GameScene::onButton, this));

        auto undo = static_cast<Button*>(Helper::seekNodeByName(_root, "undo"));
        undo->addClickEventListener(CC_CALLBACK_1(GameScene::onButton, this));
        
        auto up = (Layout*)Helper::seekNodeByName(_root, "Panel_4");
        up->setLocalZOrder(6);
        
        auto bottom = (Layout*)Helper::seekNodeByName(_root, "downL");
        bottom->setLocalZOrder(6);
        
        auto event_move_pages = EventListenerCustom::create("event_move_pages", CC_CALLBACK_1(GameScene::onPageMove, this));
        _eventDispatcher->addEventListenerWithSceneGraphPriority(event_move_pages, this);
        
        auto event_color = EventListenerCustom::create("event_color", CC_CALLBACK_1(GameScene::onChooseColor, this));
        _eventDispatcher->addEventListenerWithSceneGraphPriority(event_color, this);

        _blockLayer = BlockLayer::create();
        addChild(_blockLayer, 10);
        
        auto topBar = static_cast<Layout*>(Helper::seekNodeByName(_root, "Panel_4"));
        auto undo_bg = static_cast<Layout*>(Helper::seekNodeByName(_root, "mode_bg"));
        
        MenuItemSprite* modeBtn = MenuItemSprite::create(Sprite::create("other/icon_tap@2x.png"), Sprite::create("other/icon_tap@2x.png"), CC_CALLBACK_1(GameScene::onModeButton, this));
        modeBtn->setScale(V::isIpad()? 0.65 : 0.85);
        modeBtn->setPosition(undo_bg->getPosition());
        _modeMenuItem = modeBtn;
        
        Menu* lMenu = Menu::create(modeBtn, NULL);
        lMenu->setPosition(Vec2::ZERO);
        topBar->addChild(lMenu);
        
        
        createPallette();
        
        xAnalytics->sendScreenEvent("GameScene");
        ATMInstance->_bannerEventParas = "play_screen";
        xBanner->show();
     
        ATMInstance->_paintMode = 0;

        if (xColorAds->getIsEnableShowInterstitialAds())
        {
            xColorAds->changeScene();
        }
        xColorAds->setIsEnableShowInterstitialAds(true);
        
        return true;
    }

    return false;
}


void GameScene::onModeButton(Ref* ref)
{
    if (ATMInstance->_paintMode == 0)
    {
        ATMInstance->_paintMode = 1;
        
        map<string, string> map;
        map.insert(pair<string, string>("N/A", "N/A"));
        ATMInstance->sendEvent("paint_swipe", map);
        
        _modeMenuItem->setNormalImage(Sprite::create("other/icon_swipe@2x.png"));
    }else
    {
        ATMInstance->_paintMode = 0;
        _modeMenuItem->setNormalImage(Sprite::create("other/icon_tap@2x.png"));
        
        map<string, string> map;
        map.insert(pair<string, string>("N/A", "N/A"));
        ATMInstance->sendEvent("paint_tap", map);
    }
}

void GameScene::responseImageDownloaded(EventCustom* event)
{
    if (FileUtils::getInstance()->isFileExist(_graph.getPngName()) && FileUtils::getInstance()->isFileExist(_graph.getMaskName()))
    {
        if (_hasEnterGame == false) {
            _hasEnterGame = true;
            
            string key = _graph.getPngName() + "_reDownload";
            UserDefault::getInstance()->setStringForKey(key.c_str(), _graph.reDownload);
            UserDefault::getInstance()->flush();
            
            xProgress->start(CC_CALLBACK_1(GameScene::onEnterGame, this));
        }
    }
}

void GameScene::onEnterTransitionDidFinish()
{
    Scene::onEnterTransitionDidFinish();
    NewDownloadManager::m_bIsdownloadFail = false;

    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    
    this->createZoomLayer();
    this->runAction(Sequence::create(DelayTime::create(0.0f), CallFunc::create([this](){
        
        string key = _graph.getPngName() + "_reDownload";
        string downloadStr = UserDefault::getInstance()->getStringForKey(key.c_str(), "00000");
        if (((!FileUtils::getInstance()->isFileExist(_graph.getPngName())) || (_graph.reDownload != downloadStr) ) && (_graph.chapter_id > 0) & (_graph.url!= ""))
        {
            DownloadSpriteInfo info;
            info.name = _graph.name;
            info.url = _graph.url;
            info.imageType = ImageType::PNG_NOMAL_1024;
            xNewDownLoad->addNeedDownloadImage(info);
            
            DownloadSpriteInfo info2;
            info2.name = _graph.name;
            info2.url = _graph.url;
            info2.imageType = ImageType::PNG_MASK_1024;
            xNewDownLoad->addNeedDownloadImage(info2);
            
            auto downloadImageEvent = EventListenerCustom::create("imageDownloaded", CC_CALLBACK_1(GameScene::responseImageDownloaded, this));
            _eventDispatcher->addEventListenerWithSceneGraphPriority(downloadImageEvent, this);
        }else
        {
            
            xProgress->start(CC_CALLBACK_1(GameScene::onEnterGame, this));
            
        }
    }), NULL));
    
    this->scheduleUpdate();
}

void GameScene::addBackListener()
{
    auto listener = EventListenerKeyboard::create();
    listener->onKeyReleased = [this](EventKeyboard::KeyCode code, Event* event)
    {
        if(code == EventKeyboard::KeyCode::KEY_BACK)
        {
            this->makeButtonNotEnable();
            _blockTouch = true;
            _forward = false;
            xProgress->start(CC_CALLBACK_1(GameScene::onExitGame, this));
        }
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void GameScene::update(float dt)
{
    _time += dt;
    
    if (_blockLayer && _time > 10 && NewDownloadManager::m_bIsdownloadFail == false)
    {
        NewDownloadManager::m_bIsdownloadFail = true;
        _forward = false;
        xProgress->start(CC_CALLBACK_1(GameScene::onExitGame, this));
    }
}

void GameScene::makeButtonNotEnable()
{
//    _del->setEnabled(false);
//    _undo->setEnabled(false);
//    _back->setEnabled(false);
//    _done->setEnabled(false);
}

void GameScene::onButton(Ref *pSender)
{
    if (_blockTouch) return;
    
    string name = ((Node*)pSender)->getName();
 
    if (name == "done_touch") {
        this->makeButtonNotEnable();
        _blockTouch = true;
        xProgress->start(CC_CALLBACK_1(GameScene::onExitGame, this));
    }
    else if(name == "back_touch")
    {
        this->makeButtonNotEnable();
        _blockTouch = true;
        _forward = false;
        xProgress->start(CC_CALLBACK_1(GameScene::onExitGame, this));
    }
    else if(name == "undo")
    {
        _blockTouch = true;
 
        if (!xUndo->undo()) {
//            xDialog->show("undo", "ok", nullptr);
        }
        else
        {
            quickFlurry("btn_undo");
            ATMInstance->sendEvent("paint_undo");
        }
 
        runAction(Sequence::create(DelayTime::create(0.2), CallFunc::create([&](){
            _blockTouch = false;
        }), nullptr));
    }
    else if(name == "delete_touch")
    {
        NewDialog* dialog = NewDialog::create("reset", "cancel", "reset");
        dialog->addButtonListener(CC_CALLBACK_1(GameScene::onDialog, this));
        this->addChild(dialog, 101);
    }
}

void GameScene::onAfterVisit(EventCustom* event)
{
    //修正坐标
    _eventDispatcher->removeCustomEventListeners("director_after_visit");
    auto offset = (_done->getPositionX() - _back->getPositionX()) / 3;
    _del->setPositionX(_back->getPositionX() + offset);
    _del->setVisible(true);
    _undo->setPositionX(_back->getPositionX() + 2 * offset);
    _undo->setVisible(true);
}

float GameScene::onEnterGame(int index)
{
    float interval = 0;
    
    switch (index) {
        case 0:
        {
            SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
            Director::getInstance()->getTextureCache()->removeUnusedTextures();
        }
            break;
        case 1: //检查并生成mask
        {
//            MakeMask mask;
//            mask.make(_graph.getPngName());
        }
            break;
        case 2:
        {
            createScribble();
        }
            break;
        case 3:
        {
            _noneMode = new NoneMode(this);
            _waitMode = new WaitMode(this);
            _paintMode = new PaintMode(this);
            _moveMode = new MoveMode(this);
            _zoomMode = new ZoomMode(this);
            _pickupMode = new PickupMode(this);
            switchMode(BaseMode::Mode::NONE);
            _zoomMode->setMaxScale(_fillNode->getScale());
            
            auto listener = EventListenerTouchAllAtOnce::create();
            listener->onTouchesBegan = CC_CALLBACK_2(GameScene::onTouchesBegan, this);
            listener->onTouchesMoved = CC_CALLBACK_2(GameScene::onTouchesMoved, this);
            listener->onTouchesEnded = CC_CALLBACK_2(GameScene::onTouchesEnded, this);
            listener->onTouchesCancelled = CC_CALLBACK_2(GameScene::onTouchesCancelled, this);
            _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
        }
            break;
        case 4:
        {
            if (_blockLayer)
            {
                _blockLayer->removeFromParent();
                _blockLayer = nullptr;
            }
        }
            break;
        case 5:
        {
            xProgress->stop();
        }
            break;
        default:
            break;
    }
    
    return interval;
}

float GameScene::onExitGame(int index)
{
    float interval = 0;
    
    switch (index) {
        case 0:
        {
            _eventDispatcher->removeEventListenersForTarget(this);
            
            auto rotateSprite = Sprite::create("other/loading@2x.png");
            rotateSprite->setScale(0.85f);
            rotateSprite->setPosition(_rotatePos);
            addChild(rotateSprite);
            
            auto action = RotateBy::create(1.0f, 360.0f);
            rotateSprite->runAction(RepeatForever::create(action));

            
            //截屏保存
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            utils::captureScreen(CC_CALLBACK_2(GameScene::onAfterCaptured, this), "screen.png");
            xProgress->pause();
#else
            
            
#endif
            
            
        }
            break;
        case 1:
        {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            //释放不需要的资源
            CC_SAFE_RETAIN(_fillNode);
            _root->removeFromParent();
#endif
        }
            break;
        case 2:
        {
            //清除内存
            SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
            Director::getInstance()->getTextureCache()->removeUnusedTextures();
            GUIReader::getInstance()->destroyInstance();
            FileUtils::getInstance()->purgeCachedEntries();
        }
            break;
        case 3:
        {
            if (NewDownloadManager::m_bIsdownloadFail) {
                xProgress->jump(6);
                break;
            }
            
            
            Sprite* lSprite = Sprite::create(_graph.getPngName());
            lSprite->setScale(THUMB_WIDTH/_fillNode->getContentSize().width);
            lSprite->setPosition(Vec2(THUMB_WIDTH/2, THUMB_WIDTH/2));
            
//            _graph = xChapter->getCurrentChapter().getCurrentGraph();
            
            //保存 preview, 用于 GraphScene 预览
            auto pos = _fillNode->getPosition();
            
            _fillNode->setPosition(Vec2::ZERO);
            auto canvans = _fillNode->getCanvas()->newImage();
            canvans->autorelease();
            auto texture = new Texture2D();
            texture->initWithImage(canvans);
            texture->autorelease();
            auto image = Sprite::createWithTexture(texture);
            image->setScale(THUMB_WIDTH/_fillNode->getContentSize().width);
            image->setPosition(Vec2(THUMB_WIDTH/2, THUMB_WIDTH/2));
            
            auto renderTexture = RenderTexture::create(THUMB_WIDTH, THUMB_WIDTH, Texture2D::PixelFormat::RGBA8888);
            renderTexture->begin();
            image->visit();
            lSprite->visit();
            renderTexture->end();
            Director::getInstance()->getRenderer()->render();
            
            auto previewImage = renderTexture->newImage();
            previewImage->autorelease();
            eraseLineFrame(previewImage->getData());
            previewImage->saveToFile(FileUtils::getInstance()->getWritablePath() + _graph.getPreviewName(), false);
            
            _fillNode->setPosition(pos);
        }
            break;
        case 4:
        {
            //保存 save
            auto count = xUndo->getRecordCount();
            _undoBackupCount = count;
            xGraph->destorySaveImage();
            
//            if (_forward)
            {
//                Sprite* lSprite = Sprite::create(_graph.getPngName());
                
                auto pos = _fillNode->getPosition();
                _fillNode->setPosition(Vec2::ZERO);
                
                auto scale = _fillNode->getScale();
                _fillNode->setScale(1);
                
                auto renderTexture = RenderTexture::create(_fillNode->getContentSize().width, _fillNode->getContentSize().height, Texture2D::PixelFormat::RGBA8888);
                renderTexture->beginWithClear(1.0, 1.0, 1.0, 1.0);
                _fillNode->visit();
//                lSprite->visit();
                renderTexture->end();
                Director::getInstance()->getRenderer()->render();
                
                _fillNode->setScale(scale);
                
                auto saveImage = renderTexture->newImage();
                xGraph->setSaveImage(saveImage);
                
                _fillNode->setPosition(pos);
 
            }
            
            if (_forward)
            {
                MyArtData lMyart;
                lMyart.name = _graph.getPngName();
                lMyart.previewName = _graph.getPreviewName();
                
                xMyArtData->addImageToVec(lMyart);
            }else
            {
                if (count > 0)
                {
                    MyArtData lMyart;
                    lMyart.name = _graph.getPngName();
                    lMyart.previewName = _graph.getPreviewName();
                    
                    xMyArtData->addImageToVec(lMyart);
                    
                }
            }

        }
            break;
        case 5:
        {
            
            string path = FileUtils::getInstance()->getWritablePath() + _graph.getHistoryName();
            if (_forward) {
                xGraph->getSaveImage()->saveToFile(path);
                Director::getInstance()->getRenderer()->render();
            }else
            {
                if (_undoBackupCount > 0) {
                    xGraph->getSaveImage()->saveToFile(path);
                    Director::getInstance()->getRenderer()->render();
                }else
                {
                    if (_graph.isNew)
                    {
                        int index = --_graph.historyIndex;
                        xUser->setIntegerForKey(string(_graph.name + "_hIndex").c_str(), index);
                        xUser->flush();
                    }
                    
                }
            }

        }
            break;
        case 6:
        {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            CC_SAFE_DELETE(_fillNode);
#endif
        }
            break;
        case 7:
        {
            //清除内存
            SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
            Director::getInstance()->getTextureCache()->removeUnusedTextures();
            GUIReader::getInstance()->destroyInstance();
            FileUtils::getInstance()->purgeCachedEntries();
        }
            break;
        case 8:
        {
            xProgress->stop();
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
//            _root->setClippingEnabled(true);
#endif
            if (_forward) {
                xScene->forward(SceneManager::Tag::SHARE, SceneManager::SceneAnimation::INLEFT);
            }
            else
            {
                SceneManager::Tag tag = SceneManager::getInstance()->getLastSceneTag();
                if (tag == SceneManager::Tag::MY_ARTS)
                {
                    xScene->back(SceneManager::SceneAnimation::NONE);
                }else
                {
                    xScene->back(SceneManager::SceneAnimation::OUTLEFT);
                }
                
            }
        }
            break;
        default:
            break;
    }
    
    return interval;
}

void GameScene::applicationDidEnterBackground()
{
    if (!_fillNode) return;
    
    Sprite* lSprite = Sprite::create(_graph.getPngName());
    lSprite->setScale(THUMB_WIDTH/_fillNode->getContentSize().width);
    lSprite->setPosition(Vec2(THUMB_WIDTH/2, THUMB_WIDTH/2));
    
    //保存 preview, 用于 GraphScene 预览
    auto pos = _fillNode->getPosition();
    
    _fillNode->setPosition(Vec2::ZERO);
    auto canvans = _fillNode->getCanvas()->newImage();
    canvans->autorelease();
    auto texture = new Texture2D();
    texture->initWithImage(canvans);
    texture->autorelease();
    auto image = Sprite::createWithTexture(texture);
    image->setScale(THUMB_WIDTH/_fillNode->getContentSize().width);
    image->setPosition(Vec2(THUMB_WIDTH/2, THUMB_WIDTH/2));
    
    auto renderTexture = RenderTexture::create(THUMB_WIDTH, THUMB_WIDTH, Texture2D::PixelFormat::RGBA8888);
    renderTexture->beginWithClear(1.0, 1.0, 1.0, 1.0);
    image->visit();
    lSprite->visit();
    renderTexture->end();
    Director::getInstance()->getRenderer()->render();
    
    auto previewImage = renderTexture->newImage();
    previewImage->autorelease();
    eraseLineFrame(previewImage->getData());
    previewImage->saveToFile(FileUtils::getInstance()->getWritablePath() + _graph.getPreviewName(), false);
    
    _fillNode->setPosition(pos);
    
    _fillNode->getCanvas()->saveToFile(_graph.getHistoryName());
    Director::getInstance()->getRenderer()->render();
}

void GameScene::createZoomLayer()
{

    auto up = (Layout*)Helper::seekNodeByName(_root, "Panel_4");
    up->setContentSize(Size(getContentSize().width, up->getContentSize().height));
    auto bottom = (Layout*)Helper::seekNodeByName(_root, "downL");
    bottom->setContentSize(Size(getContentSize().width, bottom->getContentSize().height-2));
    bottom->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    bottom->setBackGroundColor(cocos2d::Color3B(255, 255, 255));
    
    up->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    up->setBackGroundColor(cocos2d::Color3B(255, 255, 255));
    
    auto height = up->getPosition().y- TOP_HEIGHT - bottom->getPosition().y - bottom->getContentSize().height;
    _zoomLayer = LayerColor::create(Color4B(255,255,255,255), getContentSize().width, height);
    _zoomLayer->ignoreAnchorPointForPosition(false);
    _zoomLayer->setPosition(Vec2(_zoomLayer->getContentSize() / 2) + Vec2(0, bottom->getPosition().y + bottom->getContentSize().height + 1));
    _root->addChild(_zoomLayer, 4);
    
    _rotatePos = _zoomLayer->getPosition();
    _blockLayer->setLoadingPosition(_rotatePos);
    
    Sprite* lSprite = Sprite::create("other/picker_small@2x.png");
    lSprite->setVisible(false);
    lSprite->setTag(88);
    _zoomLayer->addChild(lSprite, 100);
    _pickupSprite = lSprite;
    
    Sprite* lBg = Sprite::create("other/picker_large@2x.png");
    lBg->setPosition(Vec2(lSprite->getContentSize()/2));
    lSprite->addChild(lBg, -1);
    
}

void GameScene::createScribble()
{
    string str = "123_mask.png";
    string str2 = "123.png";
//    _fillNode = ColoringNode::create(str, str2);
    _fillNode = ColoringNode::create(_graph.getMaskName(), _graph.getPngName());
    _fillNode->setDefaultColor(Color4F(Color3B(255, 255, 255)));
    _fillNode->setColoringMode(ColoringNode::ColoringMode::eFlat);
    
    if (!_graph.isNew || !_graph.historyName.empty()) {
        auto path = FileUtils::getInstance()->getWritablePath();
        
        string historyName;
        if (_graph.historyName.empty()) {
            historyName = _graph.getHistoryName();
        } else {
            historyName = _graph.historyName;
        }
                
        bool isExsit = FileUtils::getInstance()->isFileExist(path + historyName);
        if (isExsit)
        {
            Sprite *history = Sprite::create(historyName);
            history->setPosition(_fillNode->getContentSize()/2);
            
            _fillNode->getCanvas()->begin();
            history->visit();
            _fillNode->getCanvas()->end();
            quickFlurry("img_paint_" + _graph.name);
        }
    }

    _fillNode->setScale(quickAdaptScale(_fillNode, _zoomLayer));
    Size sizeOffset = _zoomLayer->getContentSize() - _fillNode->getBoundingBox().size;
    _fillNode->setPosition(sizeOffset/2);
    
    _zoomLayer->addChild(_fillNode);
    _zoomLayer->setColor(Color3B(255, 255, 255));
    
    xUndo->init(_fillNode, this);
    drawLineFrame();
}

//这个函数会卡住1秒钟
void GameScene::drawLineFrame()
{
    _fillNode->setStartColor(Color4B::BLACK);
    _fillNode->drawLineFrame();
    _fillNode->setStartColor(Color4B(xPallette->getCurrentColor()));
    _currentChooseColor = Color3B(Color4B(xPallette->getCurrentColor()));
}

void GameScene::onTouchesBegan(const std::vector<Touch *> &touches, cocos2d::Event *unused_event){
    _currentMode->onTouchesBegan(touches, unused_event);
}

void GameScene::onTouchesMoved(const std::vector<Touch *> &touches, cocos2d::Event *unused_event){
    _currentMode->onTouchesMoved(touches, unused_event);
}

void GameScene::onTouchesEnded(const std::vector<Touch *> &touches, cocos2d::Event *unused_event){
    _currentMode->onTouchesEnded(touches, unused_event);
}

void GameScene::onTouchesCancelled(const std::vector<Touch *> &touches, cocos2d::Event *unused_event){

    _currentMode->onTouchesCancelled(touches, unused_event);
}

void GameScene::switchMode(BaseMode::Mode mode)
{
    _isEnableReturnColor = true;
    if (_currentMode)
    {
        _currentMode->stop();
    }
    
    if (_pickupImage)
    {
        _pickupImage->release();
        _pickupImage = nullptr;
    }
    
    switch (mode) {
        case BaseMode::Mode::NONE:
        {
            _currentMode = _noneMode;
        }
            break;
        case BaseMode::Mode::PAINT:
        {
            _currentMode = _paintMode;
        }
            break;
        case BaseMode::Mode::WAIT:
        {
            _currentMode = _waitMode;
        }
            break;
        case BaseMode::Mode::ZOOM:
        {
            _currentMode = _zoomMode;
        }
            break;
        case BaseMode::Mode::MOVE:
        {
            _currentMode = _moveMode;
        }
            break;
        case BaseMode::Mode::PICKUP:
        {
            _currentMode = _pickupMode;
            _pickupImage = _fillNode->getCanvas()->newImage();
            string path = FileUtils::getInstance()->getWritablePath()+"sss.png";
            _pickupImage->saveToFile(path);
        }
            break;
        default:
            break;
    }
    
    _currentMode->start();
}

void GameScene::paintPrevious()
{
    if (ATMInstance->_paintMode == 0)
    {
        if (_preColorRecord == _currentChooseColor)
        {
            _fillNode->setStartColor(Color4B(255, 255, 255, 255));
        }
    }else
    {
        if (_preColorRecord == _currentChooseColor && _isEnableReturnColor)
        {
             _fillNode->setStartColor(Color4B(255, 255, 255, 255));
        }
    }
    
    for (int i = 0; i < _paints.size(); i ++) {
        Paint paint = _paints.at(i);
        if(!_fillNode->insideLineframe(paint.pos))
        {
//            xUndo->record(_graph.bgColor);
//            changeBackgroundColor();

        }
        else if(_fillNode->willColoring(paint.pos))
        {
            xUndo->record(paint.pos);
            _fillNode->coloring(paint.pos);
            
            _nexPaletteName = _prePaletteName;
            _prePaletteName = xPallette->getData().at(xPallette->getPalletteIndex()).name;
            
            if (_nexPaletteName != _prePaletteName)
            {
                ATMInstance->sendEvent("palette_choose", "palette_name", _prePaletteName);
            }
        }
    }
    
    _paints.clear();
    
    _fillNode->setStartColor(Color4B(_currentChooseColor));
}

void GameScene::paint(Vec2 & current, Vec2 & previous)
{
    if(!_fillNode->insideLineframe(current))
    {
//        xUndo->record(_graph.bgColor);
//        changeBackgroundColor();
    }
    else if(_fillNode->willColoring(current))
    {
        xUndo->record(current);
        _fillNode->coloring(current);
        
        _nexPaletteName = _prePaletteName;
        _prePaletteName = xPallette->getData().at(xPallette->getPalletteIndex()).name;
        
        if (_nexPaletteName != _prePaletteName)
        {
            ATMInstance->sendEvent("palette_choose", "palette_name", _prePaletteName);
        }
    }
}

void GameScene::changeBackgroundColor()
{
    _graph.setBGColor(xPallette->getCurrentColor());
    _fillNode->coloringNodeSpace(Vec2::ZERO + Vec2(1, 1));
    _fillNode->coloringNodeSpace(Vec2::ZERO + Vec2(1023, 1));
    _zoomLayer->setColor(xPallette->getCurrentColor());
}

void GameScene::undoBackgroundColor(const Color3B & color)
{
    _graph.setBGColor(color);
    
    _fillNode->setStartColor(Color4B(color));
    _fillNode->coloringNodeSpace(Vec2::ZERO + Vec2(1, 1));
    _fillNode->coloringNodeSpace(Vec2::ZERO + Vec2(1023, 1));
    
    _zoomLayer->setColor(color);
}

void GameScene::stopPaint()
{
    _fillNode->resetPatternColor();
    _paints.clear();
}

Layer * GameScene::getZoomLayer()
{
    return _zoomLayer;
}

void GameScene::onDialog(const string& name)
{
    if(name == "reset"){
        xUndo->reset();
        _graph.setBGColor(Color3B(255, 255, 255));
        _zoomLayer->setColor(Color3B(255, 255, 255));
        _fillNode->reset();
        drawLineFrame();
        
        _isReset = true;
        
        quickFlurry("btn_reset");
    }
    else if (name == "watch_ad"){
        xLock->unlock(xDialog->getUserString(), LockManager::Type::PALLETTE);
        quickFlurry("watchAds_" + xDialog->getUserString());
    }
}


void GameScene::createPallette()
{ 
    //替换 pallette
    auto pallettes = static_cast<Layout*>(Helper::seekNodeByName(_root, "pallette"));
    pallettes->setZOrder(6);
    
    ColorLoopScrollLayer* colorLayer = ColorLoopScrollLayer::create();
    colorLayer->setPosition(Vec2(pallettes->getContentSize().width/2, pallettes->getContentSize().height));
    pallettes->addChild(colorLayer);
    _palletteLayer = colorLayer;
}

void GameScene::updateColorBoard(Color3B color)
{
    vector<PalletteInfo> vPalletteDatas = xPallette->getData();
    
    for (int i = 0; i < vPalletteDatas.size(); ++i)
    {
        PalletteInfo info = vPalletteDatas.at(i);
        for (int j = 0; j < info.vRGBInfo.size(); ++j)
        {
            Color3B sColor = info.vRGBInfo.at(j);
            if (sColor.r == color.r && sColor.g == color.g && sColor.b == color.b)
            {
                
                auto color = xPallette->getColor(i, j);
                _fillNode->setStartColor(Color4B(color));
                _currentChooseColor = color;
                
                _palletteLayer->updateColorLayer(i, color);
                break;
            }
        }
    }
    
}

void GameScene::onPageMove(EventCustom* event)
{
    float percent = _pallettes->getPercent();
    _palletteName->setOpacity(255 - 255 * percent);
}

void GameScene::onChooseColor(EventCustom* event)
{
    ColorIndex * ci = (ColorIndex*)event->getUserData();
    
    //检查是否解锁
    auto & data = xPallette->getData();
    if (data.at(ci->page).isLock &&
        xLock->isLocked(data.at(ci->page).name, LockManager::Type::PALLETTE))
    {
        NewDialog* dialog = NewDialog::create("watch_ad_pallette", "cancel", "watch_ad");
        dialog->addButtonListener(CC_CALLBACK_1(GameScene::onDialog, this));
        this->addChild(dialog, 101);
        xDialog->setUserString(xPallette->getData().at(ci->page).name);
    }
    else
    {
        int index = ci->index;
//        string name = xPallette->getData().at(ci->page).name;
//        if (index == 12 && (name == "Skin Tones" || name == "Blacks&Whites" || name == "Reds" || name == "Greens" || name == "Oranges" || name == "Purples" || name == "Blues" || name == "Dreams" || name == "Sunset" || name == "Sky" || name == "Wildlife" || name == "Paradise" || name == "Cyans"))
//        {
//            index = 17;
//        }
//        
//        if (index == 12 && name == "Yellows")
//        {
//            index = 16;
//        }
        
        
        auto color = xPallette->getColor(ci->page, index);
        _fillNode->setStartColor(Color4B(color));
        _currentChooseColor = color;
    }
    quickFlurry("pallete_" + xPallette->getData().at(ci->page).name);
    
    
}

void GameScene::onAfterCaptured(bool succeed, const string & filename)
{
    if (succeed) {
        auto screen = Sprite::create(filename);
        screen->setPosition(getContentSize()/2);
        screen->setScale(V::getRealDesignScale());
        addChild(screen);
        
        //add animation
//        auto rotateSprite = Sprite::create("other/load_rotate_icon.png");
//        rotateSprite->setPosition(_rotatePos);
//        addChild(rotateSprite);
//        
//        auto action = RotateBy::create(1.0f, 360.0f);
//        rotateSprite->runAction(RepeatForever::create(action));
        
        xProgress->resume();
    }
}

void GameScene::eraseLineFrame(unsigned char * imageData)
{
    return;
    for (int y = 0; y < THUMB_WIDTH; y++) {
        
        for (int x = 0; x < THUMB_WIDTH; x++) {
            
            unsigned int *data = (unsigned int *)(imageData);
            data += (y * THUMB_WIDTH + x);
            
            Color4B color;
            color.r = *data & 0xFF;
            color.g = (*data >> 8) & 0xFF;
            color.b = (*data >> 16) & 0xFF;
            color.a = (*data >> 24) & 0xFF;

            //去掉 line frame
            if (color.r < 100 && color.a != 0 &&
                color.r == color.g && color.g == color.b) {
                *data = (color.r | (color.g << 8) | (color.b << 16) | (0 << 24));
            }
        }
    }
}

Color4B GameScene::getPixelColor(Vec2 lpos)
{
    
    if (!_pickupImage)
    {
        _pickupImage = _fillNode->getCanvas()->newImage();
    }
    
    Vec2 lPos = _fillNode->convertToNodeSpace(lpos);
    lPos.y = 1024 - lPos.y;
//    lPos += _fillNode->getContentSize()/2;
    
    Color4B color = {0, 0, 0, 0};
    if ( lPos.x < 0 || lPos.x >= _pickupImage->getWidth() || lPos.y < 0 || lPos.y >= _pickupImage->getHeight() ) {
        return color;
    }
    auto data = _pickupImage->getData();
    auto pixel = (unsigned int*) data;
    auto x = (int)lPos.x;
    auto y = (int)lPos.y;
    pixel = pixel + (y * _pickupImage->getWidth() ) * 1 + x * 1;
    //R通道
    color.r = *pixel & 0xff;
    //G通道
    color.g = (*pixel >> 8) & 0xff;
    //B通过
    color.b = (*pixel >> 16) & 0xff;
    //Alpha通道
    color.a = (*pixel >> 24) & 0xff;
    
//    log("pickup color= (%d, %d, %d, %d)", color.r, color.g, color.b, color.a);

    if (_pickupSprite)
    {
        _pickupSprite->setColor(Color3B(color));
    }
    
    return color;
    
}

void GameScene::changePalletteName()
{
//    if (_palletteLayer)
//    {
//        _palletteLayer->waitUnlockSuc();
//    }
    
}

void GameScene::setWaitBeginVec(Vec2 pos)
{
    _waitBeginVec2 = pos;
    
    _preColorRecord = Color3B(getPixelColor(pos));
}

Vec2 GameScene::getWaitBeginVec()
{
    return _waitBeginVec2;
}

void GameScene::setEnableReturnColor(bool isEnable)
{
    _isEnableReturnColor = isEnable;
}
