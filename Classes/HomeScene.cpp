//
//  HomeScene.cpp
//  ComicBook
//
//  Created by houhuang on 17/12/1.
//
//

#include "HomeScene.h"
#include "CartoonManager.h"
#include "CoverSprite.h"
#include "STVisibleRect.h"
#include "ComicScene.h"
#include "ReadScene.h"
#include "EventSprite.hpp"
#include "DownloadManager.h"
#include "SearchPathManager.h"

#define top_bar     228
#define SPACE       40
#define col         (V::isIpad()? 3:2)

enum{
    st_button_setting = 10,
};

HomeScene::~HomeScene()
{
    _eventDispatcher->removeEventListenersForTarget(this);
}

HomeScene::HomeScene()
{
    topBar_realHeight = 0.0f;
    _dialog = nullptr;
    _bannerSprite = nullptr;
    _tableView = nullptr;
    _settingLayer = nullptr;
}

void HomeScene::registerNotification()
{
    auto removeDialogEvent = EventListenerCustom::create(st_remove_dialog, CC_CALLBACK_1(HomeScene::removeDailog, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(removeDialogEvent, this);
    
    auto removeSettingLayerEvent = EventListenerCustom::create(st_remove_settingLayer, CC_CALLBACK_1(HomeScene::removeSettingLayer, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(removeSettingLayerEvent, this);
    
    
    auto showClearDataDialogEvent = EventListenerCustom::create(st_showDialog_clearDataDialog, CC_CALLBACK_1(HomeScene::showClearDataDialog, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(showClearDataDialogEvent, this);
    
    auto showContinueReadDialogEvent = EventListenerCustom::create(st_showDialog_continueRead, CC_CALLBACK_1(HomeScene::showContinueReadDialog, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(showContinueReadDialogEvent, this);
    
    auto getResourceFaildEvent = EventListenerCustom::create(st_download_rootCsv_faild, CC_CALLBACK_1(HomeScene::responseGetResourceFaild, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(getResourceFaildEvent, this);
}

bool HomeScene::init()
{
    if (!Scene::init()) return false;
    
    setName("HomeScene");
    
    LayerColor* layer = LayerColor::create(Color4B(245, 245, 245, 255));
    this->addChild(layer);
    
    createTable();
//    checkProgress();
    registerNotification();
    
    this->runAction(Sequence::create(DelayTime::create(0.2f), CallFunc::create([this](){
        this->addBackListener();
    }), NULL));
    return true;
}

void HomeScene::onEnterTransitionDidFinish()
{
    Scene::onEnterTransitionDidFinish();
    
    this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([](){
        Director::getInstance()->getTextureCache()->removeUnusedTextures();
    }), NULL));
    
}

void HomeScene::checkProgress()
{
    bool isFirstContinueRead = UserDefault::getInstance()->getBoolForKey("isFirstContinueRead", true);
    if (isFirstContinueRead)
    {
        string data = UserDefault::getInstance()->getStringForKey("CurrentCartoonProgress", "");
        if (data != "")
        {
            UserDefault::getInstance()->setBoolForKey("isFirstContinueRead", false);
            UserDefault::getInstance()->flush();
            
            ReadingCartoonInfo info;
            info.folder     = data.substr(0, data.find("@"));
            string pa       = data.substr(data.find("@") + 1, data.length());
            info.csvPath    = pa.substr(0, pa.find("@"));
            info.pageNumber = pa.substr(pa.find("@") + 1, pa.length());
            
            xCartoon->getCurrentReadingCartoon() = info;
            
            this->runAction(Sequence::create(DelayTime::create(0.05f), CallFunc::create([this](){
                NewDialog* lDialog = NewDialog::create("是否继续上次的阅读？", "关闭", "继续");
                lDialog->addButtonListener(CC_CALLBACK_1(HomeScene::onDialog, this));
                this->addChild(lDialog, 101);
                _dialog = lDialog;
            }), NULL));
        }else
        {
            this->runAction(Sequence::create(DelayTime::create(0.05f), CallFunc::create([this](){
                NewDialog* lDialog = NewDialog::create("你还没有阅读记录。", "", "关闭");
                lDialog->addButtonListener(CC_CALLBACK_1(HomeScene::onDialog, this));
                this->addChild(lDialog, 101);
                _dialog = lDialog;
            }), NULL));
        }
    }else
    {
        string data = UserDefault::getInstance()->getStringForKey("CurrentCartoonProgress", "");
        if (data != "")
        {
            ReadingCartoonInfo info;
            info.folder = data.substr(0, data.find("@"));
            string pa = data.substr(data.find("@") + 1, data.length());
            info.csvPath = pa.substr(0, pa.find("@"));
            info.pageNumber = pa.substr(pa.find("@") + 1, pa.length());
            
            xCartoon->getCurrentReadingCartoon() = info;
            
            xCartoon->setCurrentFolder(xCartoon->getCurrentReadingCartoon().folder);
            xCartoon->readCurrentPictureCsv(xCartoon->getCurrentReadingCartoon().csvPath);
            
            xCartoon->setPreSceneName("HomeScene");
            Director::getInstance()->replaceScene(TransitionProgressInOut::create(0.2f, ReadScene::create(stoi(xCartoon->getCurrentReadingCartoon().pageNumber), "HomeScene")));
            
            
//            xCartoon->setCurrentFolder("");
        }else
        {
            this->runAction(Sequence::create(DelayTime::create(0.05f), CallFunc::create([this](){
                NewDialog* lDialog = NewDialog::create("你还没有阅读记录。", "", "关闭");
                lDialog->addButtonListener(CC_CALLBACK_1(HomeScene::onDialog, this));
                this->addChild(lDialog, 101);
                _dialog = lDialog;
            }), NULL));
        }
        
    }
    
    
}

void HomeScene::removeDailog(EventCustom* event)
{
    if (_dialog)
    {
        _dialog->removeFromParentAndCleanup(true);
        _dialog = nullptr;
    }
}

void HomeScene::removeSettingLayer(EventCustom* event)
{
    if (_settingLayer)
    {
        _settingLayer->removeFromParentAndCleanup(true);
        _settingLayer = nullptr;
    }
}

void HomeScene::showClearDataDialog(EventCustom* event)
{
    NewDialog* lDialog = NewDialog::create("长期使用会占用大量磁盘空间，是否释放磁盘空间？", "关闭", "释放");
    lDialog->addButtonListener(CC_CALLBACK_1(HomeScene::onDialog, this));
    this->addChild(lDialog, 101);
    _dialog = lDialog;
}

void HomeScene::showContinueReadDialog(EventCustom* event)
{
    checkProgress();
}

void HomeScene::responseGetResourceFaild(EventCustom* event)
{
    if (!_dialog)
    {
        NewDialog* lDialog = NewDialog::create("获取服务器资源失败，请检查你的网络连接是否正确！", "", "关闭");
        lDialog->addButtonListener(CC_CALLBACK_1(HomeScene::onDialog, this));
        this->addChild(lDialog, 101);
        _dialog = lDialog;
    }
}

void HomeScene::createTable()
{
    TableView* table = TableView::create(this, Size(this->getContentSize().width, this->getContentSize().height));
    table->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    table->setVerticalFillOrder(cocos2d::extension::TableView::VerticalFillOrder::TOP_DOWN);
    
    table->setDelegate(this);
    table->ignoreAnchorPointForPosition(false);
    table->setAnchorPoint(Vec2(0.5, 1));
    table->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height));
    table->reloadData();
    _tableView = table;
    
    this->addChild(table);
}

void HomeScene::onDialog(const string& name)
{
    if (name == "继续")
    {
        xCartoon->setCurrentFolder(xCartoon->getCurrentReadingCartoon().folder);
        xCartoon->readCurrentPictureCsv(xCartoon->getCurrentReadingCartoon().csvPath);
        
        xCartoon->setPreSceneName("HomeScene");
        Director::getInstance()->replaceScene(TransitionProgressInOut::create(0.2f, ReadScene::create(stoi(xCartoon->getCurrentReadingCartoon().pageNumber), "HomeScene")));
        
        
//        xCartoon->setCurrentFolder("");
    }else if (name == "释放")
    {
        string path = FileUtils::getInstance()->getWritablePath() + "picture/";
        FileUtils::getInstance()->removeDirectory(path);
//        FileUtils::getInstance()->destroyInstance();
        SearchPathManager::getInstance()->updateSearchPath();
    }
    else if (name == "退出")
    {
        Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        exit(0);
#endif
    }
}

void HomeScene::onButton(Ref* ref)
{
    MenuItemImage* lMenuItem = (MenuItemImage*)ref;
    
    switch (lMenuItem->getTag())
    {
        case st_button_setting:
        {
            if (lMenuItem)
            {
                Vec2 lPos = lMenuItem->convertToWorldSpace(lMenuItem->getContentSize()/2);
                SettingLayer* layer = SettingLayer::create(lPos);
                this->addChild(layer, 90);
                _settingLayer = layer;
            }
        }
            break;
            
        default:
            break;
    }
}

void HomeScene::responseCoverSprite(Node* node)
{
    EventSprite* lSprite = (EventSprite*)node;
    if (lSprite)
    {
        xCartoon->setPreSceneName("HomeScene");
        xCartoon->setCurrentCategory(lSprite->getTag());
        Director::getInstance()->replaceScene(TransitionMoveInR::create(0.2f, ComicScene::create()));
    }
    
}

void HomeScene::tableCellTouched(TableView* table, TableViewCell* cell)
{
    
}

Size HomeScene::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    float height = ((this->getContentSize().width - (col + 1)*SPACE)/col)*608./346 + SPACE;
    if (idx == this->numberOfCellsInTableView(nullptr) -1)
    {
        height += SPACE;
    }
    
    if (idx == 0)
    {
        height = this->getContentSize().width/1125.0*456.0;
    }

    return Size(this->getContentSize().width, height);
}

TableViewCell* HomeScene::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell* cell = table->dequeueCell();
    if (!cell)
    {
        cell = new TableViewCell();
        cell->autorelease();
    }
    
    Sprite* lSprite = (Sprite*)cell->getChildByTag(99);
    if (lSprite)
    {
        lSprite->removeFromParent();
        _bannerSprite = nullptr;
    }
    
    cell->removeAllChildrenWithCleanup(true);
    
    if (idx == 0)
    {
        Sprite* topBanner = Sprite::create("title.png");
        topBanner->setAnchorPoint(Vec2(0.5, 0));
        topBanner->setScale(this->getContentSize().width/topBanner->getContentSize().width);
        topBanner->setPosition(Vec2(this->getContentSize().width/2, 0));
        topBanner->setTag(99);
        cell->addChild(topBanner);
        _bannerSprite = topBanner;
        
        Sprite* shadow = Sprite::create("shadow.png");
        shadow->setAnchorPoint(Vec2(0.5, 0));
        shadow->setScaleX((topBanner->getContentSize().width+ 20)/shadow->getContentSize().width);
        shadow->setPosition(Vec2(topBanner->getContentSize().width/2, -5));
        topBanner->addChild(shadow, -1);
        
        MenuItemImage* lSetting = MenuItemImage::create("menu_btn.png", "menu_btn.png", CC_CALLBACK_1(HomeScene::onButton, this));
        lSetting->setAnchorPoint(Vec2(0, 1));
        lSetting->setPosition(Vec2(20, 366));
        lSetting->setTag(st_button_setting);
        lSetting->setScale(0.9f);
        
        Menu* lMenu = Menu::create(lSetting, NULL);
        lMenu->setPosition(Vec2::ZERO);
        topBanner->addChild(lMenu);
        
        return cell;
    }
    
    float width = (this->getContentSize().width - (col+1)*SPACE)/col;
    
    int newIdx = idx - 1;
    for (int i = 0; i < col; ++i)
    {
        int index = (int)newIdx*col + i;
        if (index >= xCartoon->getCategoryInfo().size())
        {
            break;
        }
        
        Sprite* lBg = Sprite::create("library_mask.png");
        lBg->setScale(width/346.);
        lBg->setAnchorPoint(Vec2(0.5, 0.0174));
        lBg->setPosition(Vec2((i + 1)*SPACE + (i + 0.5)*width, 0));
        
        EventSprite* cover = EventSprite::create(xCartoon->getCategoryInfo().at(index).cover);
        cover->setAnchorPoint(Vec2(0.5, 1));
        cover->setScaleX(346./cover->getContentSize().width);
        cover->setScaleY(502./cover->getContentSize().height);
        cover->setPosition(Vec2(lBg->getContentSize().width/2, lBg->getContentSize().height - 11));
        lBg->addChild(cover, -1);
        cover->setTag(index);
        
        Label* name = Label::createWithTTF(xCartoon->getCategoryInfo().at(index).name, "fonts/d2.ttf", 50);
        name->setPosition(Vec2(lBg->getContentSize().width/2, 62));
        name->setColor(Color3B(43, 43, 43));
        lBg->addChild(name, 11);
        
        cover->addListener(callfuncN_selector(HomeScene::responseCoverSprite), this);
        
        if (idx == this->numberOfCellsInTableView(NULL)-1)
        {
            lBg->setPosition(Vec2((i + 1)*SPACE + (i + 0.5)*width, SPACE));
        }
        
        cell->addChild(lBg);
        
    }
    
    return cell;
}

ssize_t HomeScene::numberOfCellsInTableView(TableView *table)
{
    int length = (int)xCartoon->getCategoryInfo().size();
    if (length%col == 0)
    {
        length /= col;
    }else
    {
        length = length/col + 1;
    }
    
    return length + 1;
}


void HomeScene::addBackListener()
{
    auto listener = EventListenerKeyboard::create();
    listener->onKeyReleased = [this](EventKeyboard::KeyCode code, Event* event){
        
        if (code == EventKeyboard::KeyCode::KEY_BACK)
        {
            if (_dialog)
            {
                _dialog->removeFromParentAndCleanup(true);
                _dialog = nullptr;
            }else if (_settingLayer)
            {
                _settingLayer->removeFromParentAndCleanup(true);
                _settingLayer = nullptr;
            }else
            {
                NewDialog* lDialog = NewDialog::create("是否退出游戏？", "关闭", "退出");
                lDialog->addButtonListener(CC_CALLBACK_1(HomeScene::onDialog, this));
                this->addChild(lDialog, 101);
                _dialog = lDialog;
            }
        }
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void HomeScene::scrollViewDidScroll(ScrollView* view)
{
    if (_bannerSprite && _tableView)
    {
        float offsetY = _tableView->minContainerOffset().y - _tableView->getContentOffset().y;
        
        if (offsetY > 0)
        {
            _bannerSprite->setPositionY(offsetY);
        }else
        {
            _bannerSprite->setPositionY(0);
        }
    }
    
}




