//
//  GraphScene.cpp
//  BabyPlay
//
//  Created by luoxp on 3/18/15.
//
//

#include "GraphScene.h"
#include "UICpp.h"
#include "DownloadSprite.hpp"
#include "ChapterManager.hpp"
#include "LockManager.h"
#include "DownloadManager.hpp"
#include "MyArtDataManager.hpp"
#include "UIColorManager.hpp"
#include "DataPromotionManager.hpp"
#include "SSCInternalLibManager.h"
#include "DownloadSprite.hpp"
#include "GraphSprite.hpp"
#include "ThreadManager.hpp"
#include "NewDownloadManager.hpp"
#include "AnalyticsManager.hpp"
#include "BannerManager.hpp"
#include "DownLoadPage.h"

#define SubChapter_Switch   0
#define xVisibleSize        Director::getInstance()->getVisibleSize()
#define GRAPH_SPACE         (V::isIpad()? 36 : 61)
#define col                 (V::isIpad()? 2 : 1)

GraphScene::~GraphScene(){
    
    _tableView->setIsUpdateShow(false);
    NotificationCenter::getInstance()->removeObserver(this, st_download_finish);
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    
    NotificationCenter::getInstance()->removeObserver(this, st_myPost_failed);
    NotificationCenter::getInstance()->removeObserver(this, st_myPost_success);
}

GraphScene::GraphScene()
{
    _ChapterMark = NULL;
    _postingLayer = nullptr;
    _tableBg = nullptr;
    _topMenu = nullptr;
    _graphLayer = nullptr;
    
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GraphScene::waitDownloadFinish), st_download_finish, NULL);
    
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GraphScene::waitPostSuccessful), st_myPost_success, NULL);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GraphScene::waitPostFaild), st_myPost_failed, NULL);
}

void GraphScene::waitDownloadFinish(Ref* ref)
{
    _tableView->reloadData();
}

bool GraphScene::init(){
    
    if(BaseScene::init())
    {
        setName("GraphScene");
        
        auto _root = CSLoader::createNode((V::isIpad()? "GraphScene_ipad.csb" : "GraphScene.csb"));
        _root->setContentSize(this->getContentSize());
        Helper::doLayout(_root);
        this->addChild(_root);
        _root->visit();

        Director::getInstance()->getTextureCache()->addImage("cocos/other/graph_bg.png");
        
        ui::Button* back = (Button*)Helper::seekNodeByName(_root, "back");
        back->addClickEventListener(CC_CALLBACK_1(GraphScene::onButton, this));
        
        ui::Button* myArt = static_cast<Button*>(Helper::seekNodeByName(_root, "btn_myArt"));
        myArt->addClickEventListener(CC_CALLBACK_1(GraphScene::onButton, this));
        
        
        ui::Button* insp = static_cast<Button*>(Helper::seekNodeByName(_root,"btn_inspiration"));
        insp->addClickEventListener(CC_CALLBACK_1(GraphScene::onButton, this));
        
        ui::Button* gallery = static_cast<Button*>(Helper::seekNodeByName(_root,"btn_gallery"));
        gallery->addClickEventListener(CC_CALLBACK_1(GraphScene::onButton, this));
        
        auto tilte = (Text*)Helper::seekNodeByName(_root, "TEXT_graph");
        string userName = xStr->getStringDefault(xChapter->getCurrentChapter().name);
        std::transform(userName.begin(), userName.end(), userName.begin(), ::toupper);
        tilte->setString(userName);
        tilte->setColor(Color3B(255, 255, 255));
        
        if (V::isHighVersion())
        {
            tilte->setFontSize(tilte->getFontSize()*0.8);
            tilte->setScale(tilte->getScale()/0.8);
        }
        _tableBg = static_cast<Layout*>(Helper::seekNodeByName(_root, "Panel_3"));
        
        MenuItemImage* topMenu = MenuItemImage::create("other/topbtn.png", "other/topbtn.png", CC_CALLBACK_1(GraphScene::onButton, this));
        topMenu->setScaleX((this->getContentSize().width-85)/topMenu->getContentSize().width);
        topMenu->setScaleY(TOP_HEIGHT/topMenu->getContentSize().height);
        topMenu->setName("topMenu");
        topMenu->setAnchorPoint(Vec2(1, 1));
        topMenu->setPosition(Vec2(this->getContentSize().width, this->getContentSize().height));
        
        Menu* lMenu = Menu::create(topMenu, NULL);
        lMenu->setPosition(Vec2::ZERO);
        this->addChild(lMenu, 99);
        _topMenu = lMenu;
        
//        NewDialog* dialog1 = NewDialog::create("post fail", "later", "tryAgain");
//                dialog1->addButtonListener(CC_CALLBACK_1(GraphScene::onDialog, this));
//                this->addChild(dialog1, 101);
        
        
        auto eventRemoveSprite = EventListenerCustom::create("removeGraphSpriteLayer", CC_CALLBACK_1(GraphScene::removeMyartSpriteLayer, this));
        _eventDispatcher->addEventListenerWithSceneGraphPriority(eventRemoveSprite, this);
        
        
        
        xAnalytics->sendScreenEvent("GraphScene");
//        ATMInstance->_bannerEventParas = "category_page";
        xAds->removeAds(kTypeBannerAds);
        
        if (!SubChapter_Switch || xChapter->getCurrentChapter().rule == "weekly")
        {
            return true;
        }
        
        Sprite* lSprite = Sprite::create("other/chapter.png");
        lSprite->setAnchorPoint(Vec2(0, 0.5));
        V::setPosition(lSprite, -10, 810, true, false, true, false);
        this->addChild(lSprite, 11);
        _ChapterMark = lSprite;
        _ChapterMarkY = lSprite->getPosition().y;
        
        Chapter & lChapterInfo = xChapter->getCurrentChapter();
        Label* lable = Label::createWithTTF(lChapterInfo.vGraphInfo.at(0).belongToCN.c_str(), FONT_NAME, 20);
        lable->setPosition(lSprite->getContentSize()/2);
        lSprite->addChild(lable);
        _ChapterText = lable;
        _preChapterStr = lChapterInfo.vGraphInfo.at(0).belongToCN;
        
        return true;
    }

    return false;
}

void GraphScene::onEnterTransitionDidFinish()
{
    Scene::onEnterTransitionDidFinish();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    xThread->removeAllNeedloadImage();
    xNewDownLoad->removeAllNeedDownloadImage();
//    this->runAction(Sequence::create(DelayTime::create(0.2f), CallFunc::create([](){
//        Director::getInstance()->getTextureCache()->removeUnusedTextures();
//    }), NULL));
    
    createTableView();
    
    if (!SSCInternalLibManager::getInstance()->checkNetworkAvailable())
    {
        NewDialog* dialog = NewDialog::create("network_err", "", "ok");
        dialog->addButtonListener(CC_CALLBACK_1(GraphScene::onDialog, this));
        this->addChild(dialog, 101);
    }
    
//    if (NewDownloadManager::m_bIsdownloadFail)
//    {
//        NewDialog* dialog = NewDialog::create("download_err", "", "ok");
//        dialog->addButtonListener(CC_CALLBACK_1(GraphScene::onDialog, this));
//        this->addChild(dialog, 101);
//    }
    
    auto responsUpload = EventListenerCustom::create("graph_upload", CC_CALLBACK_1(GraphScene::responseUpdloadImage, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(responsUpload, this);
}

void GraphScene::checkUpdate(float dt)
{
    if (Director::getInstance()->getRunningScene() == this) {
        xDownload->excuteFunc(DownloadState::CHECK_NEW_VERSION_STATE);
    }
}

void GraphScene::onButton(Ref* pSender)
{
    
    string name = ((Node*)pSender)->getName();
    
    if(name.compare("back") == 0)
    {
        xChapter->getCurrentChapter().tableOffset = _tableView->getContentOffset().y;
        xScene->back(SceneManager::SceneAnimation::OUTLEFT);
//        xScene->forward(SceneManager::Tag::CHAPTER, SceneManager::SceneAnimation::OUTLEFT);
    }
    else if (name == "btn_gallery") {
        
        xScene->forward(SceneManager::Tag::CHAPTER, SceneManager::SceneAnimation::NONE);
    }
    else if(name == "btn_myArt"){
        xMyArtData->m_bIsChapterEnterMyart = true;
        xScene->forward(SceneManager::Tag::MY_ARTS, SceneManager::SceneAnimation::NONE);
    }
    else if(name == "btn_inspiration"){
        xScene->forward(SceneManager::Tag::INSPIRATION, SceneManager::SceneAnimation::NONE);
    }else if (name == "topMenu")
    {
        if (_tableView)
        {
            float time = 0;
            float offset = abs(_tableView->minContainerOffset().y - _tableView->getContentOffset().y);
            if (offset > 1000)
            {
                time = 0.3f;
            }else
            {
                time = offset/1000.0*0.3;
            }
            _tableView->setContentOffsetInDuration(_tableView->minContainerOffset(), time);
            
            _topMenu->setEnabled(false);
            this->runAction(Sequence::create(DelayTime::create(0.3f), CallFunc::create([this](){
                _topMenu->setEnabled(true);
            }), NULL));
        }
    }
}

void GraphScene::createTableView()
{

    _tableView = STTableView::create(this, Size(getContentSize().width, Director::getInstance()->getVisibleSize().height - TOP_HEIGHT- BOTTOM_HEIGHT + 1));
    _tableView->setVerticalFillOrder(cocos2d::extension::TableView::VerticalFillOrder::TOP_DOWN);
    _tableView->setDirection(extension::ScrollView::Direction::VERTICAL);
    _tableView->setDelegate(this);
    _tableView->reloadData();
    
    if (xChapter->getCurrentChapter().tableOffset > 0.0f)
    {
        _tableView->setContentOffset(_tableView->minContainerOffset());
    }else
    {
        if (xChapter->getCurrentChapter().tableOffset <= _tableView->minContainerOffset().y)
        {
            _tableView->setContentOffset(Vec2(_tableView->getContentOffset().x, _tableView->minContainerOffset().y));
        }else
        {
            _tableView->setContentOffset(Vec2(_tableView->getContentOffset().x, xChapter->getCurrentChapter().tableOffset));
        }
        
    }
    
    _tableView->ignoreAnchorPointForPosition(false);
    _tableView->setAnchorPoint(Vec2(0.5, 1));
    _tableView->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height - TOP_HEIGHT + 1));
    _tableBg->addChild(_tableView);
    
    if (SubChapter_Switch)
        _tableView->setIsUpdateShow(true);
}

void GraphScene::reloadData()
{
    xChapter->getCurrentChapter().tableOffset = _tableView->getContentOffset().y;
    xChapter->getCurrentChapter().enterGraph(_graphIndex);
    xChapter->getCurrentChapter().getCurrentGraph().setNew(false);
    xScene->forward(SceneManager::Tag::GAME);
}

void GraphScene::onDialog(const string& name)
{
    log("%s", xDialog->getUserString().c_str());
    if(name == "watch_ad"){
        xLock->unlock(xDialog->getUserString(), LockManager::Type::GRAPH);
        quickFlurry("watchAds_" + xDialog->getUserString());
    }
    else if (name == "new"){
        xChapter->getCurrentChapter().getCurrentGraph().setNew(true);
        xScene->forward(SceneManager::Tag::GAME);
        
        map<string, string> map;
        map.insert(pair<string, string>("image_id", xChapter->getCurrentChapter().getCurrentGraph().name));
        map.insert(pair<string, string>("restart_location", "library"));
        
        ATMInstance->sendEvent("image_restart", map);
    }
    else if (name == "continue"){
        xChapter->getCurrentChapter().getCurrentGraph().setNew(false);
        xScene->forward(SceneManager::Tag::GAME);
        
        map<string, string> map;
        map.insert(pair<string, string>("image_id", xChapter->getCurrentChapter().getCurrentGraph().name));
        map.insert(pair<string, string>("continue_location", "library"));
        ATMInstance->sendEvent("image_continue", map);
    }else if (name == "later")
    {
        if (_postingLayer)
        {
            _postingLayer->removeFromParentAndCleanup(true);
            _postingLayer = NULL;
        }
    }else if (name == "tryAgain")
    {
        PostingLayer* lSwallowLayer = PostingLayer::create(Color4B(0, 0, 0, 200));
        this->addChild(lSwallowLayer, 100);
        lSwallowLayer->setTouchEnable(true);
        _postingLayer = lSwallowLayer;
        
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

void GraphScene::removeMyartSpriteLayer(EventCustom* event)
{
    if (_graphLayer)
    {
        _graphLayer->removeFromParentAndCleanup(true);
        _graphLayer = nullptr;
    }
}

void GraphScene::addBackListener()
{
    auto listener = EventListenerKeyboard::create();
    listener->onKeyReleased = [this](EventKeyboard::KeyCode code, Event* event)
    {
        if(code == EventKeyboard::KeyCode::KEY_BACK)
        {
            if (_graphLayer)
            {
                _graphLayer->removeFromParentAndCleanup(true);
                _graphLayer = nullptr;
            }else
            {
                xChapter->getCurrentChapter().tableOffset = _tableView->getContentOffset().y;
                xScene->back(SceneManager::SceneAnimation::OUTLEFT);
            }
 
        }
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

}

void GraphScene::downloadDialog(const string &name)
{
    if (name == "Download")
    {
        float offset = _tableView->getContentOffset().y;
        DownloadManager::_isDownloading = true;
        DownloadManager::getInstance()->excuteFunc(DownloadState::CHECK_NEW_VERSION_STATE2);
        _tableView->reloadData();
        _tableView->setContentOffset(Vec2(_tableView->getContentOffset().x, offset));
    }
}

void GraphScene::tableCellTouched(TableView* table, TableViewCell* cell)
{
    
}

void GraphScene::responseDownloadSprite(Node* ref)
{
    DownloadSprite* lDownloadSprite = dynamic_cast<DownloadSprite*>(ref);
    
    if (lDownloadSprite == NULL) return;
    
    if (!lDownloadSprite->isDownloaded()) return;
    
    auto & graph = xChapter->getCurrentChapter().vGraphInfo.at(lDownloadSprite->getTag());
    
    if (!SSCInternalLibManager::getInstance()->checkNetworkAvailable() && !FileUtils::getInstance()->isFileExist(graph.getPngName()))
    {
        NewDialog* dialog = NewDialog::create("network_err", "", "ok");
        dialog->addButtonListener(CC_CALLBACK_1(GraphScene::onDialog, this));
        this->addChild(dialog, 101);
        return;
    }
    
    if (strcmp(lDownloadSprite->m_gGraphInfo.name.substr(0, 3).c_str(), "map") == 0)
    {
        stringstream ss;
        ss<<"";
        ss<<lDownloadSprite->m_gGraphInfo.name;
        ss<<"_white.png_spare";
        
        UserDefault::getInstance()->setBoolForKey(ss.str().c_str(), true);
        UserDefault::getInstance()->flush();
        
        
    }else if(lDownloadSprite->m_gGraphInfo.name.find("_1024") > 0 && graph.chapter_id > 0)
    {
        
        string userData = lDownloadSprite->m_gGraphInfo.name;
        string aa = userData.substr(0, 4);
        int index = stoi(aa);
        
        stringstream ss;
        ss<<"";
        ss<<"map";
        ss<<index + 20;
        ss<<"_white.png";
        
        UserDefault::getInstance()->setBoolForKey(ss.str().c_str(), true);
        UserDefault::getInstance()->flush();
    }else
    {
        UserDefault::getInstance()->setBoolForKey(lDownloadSprite->m_gGraphInfo.name.c_str(), true);
        UserDefault::getInstance()->flush();
    }
    
    
    xChapter->getCurrentChapter().tableOffset = _tableView->getContentOffset().y;
    xChapter->getCurrentChapter().enterGraph(lDownloadSprite->getTag());
    
    UserDefault::getInstance()->setBoolForKey(graph.name.c_str(), true);
    
    string path = FileUtils::getInstance()->getWritablePath() + graph.getHistoryName();
    bool isExsit = FileUtils::getInstance()->isFileExist(path);
    if (isExsit) {
        
        GraphSpriteLayer* layer = GraphSpriteLayer::create(graph.getPreviewName(), graph.getPngName());
        this->addChild(layer, 100);
        _graphLayer = layer;
        
//        if (xMyArtData->isShared(graph.getPreviewName())) {
//            NewDialog* dialog = NewDialog::create("shared", "cancel", "new");
//            dialog->addButtonListener(CC_CALLBACK_1(GraphScene::onDialog, this));
//            this->addChild(dialog, 101);
//        }
//        else {
//            NewDialog* dialog = NewDialog::create("continue", "new", "continue");
//            dialog->addButtonListener(CC_CALLBACK_1(GraphScene::onDialog, this));
//            this->addChild(dialog, 101);
//        }
    }
    else
    {
        map<string, string> map;
        map.insert(pair<string, string>("image_id", graph.name));
        map.insert(pair<string, string>("category_name", xChapter->getCurrentChapter().name));
        ATMInstance->sendEvent("image_start", map);
        
        graph.setNew(false);
        xScene->forward(SceneManager::Tag::GAME, SceneManager::SceneAnimation::NONE);
    }
    
 
    
    return;
    
    if (lDownloadSprite->getCurrentGraphIslock())
    {
        _graphIndex = lDownloadSprite->getTag();
        NewDialog* dialog = NewDialog::create("watch_ad", "cancel", "watch_ad");
        dialog->addButtonListener(CC_CALLBACK_1(GraphScene::onDialog, this));
        this->addChild(dialog, 101);
        xDialog->setUserString(lDownloadSprite->m_gGraphInfo.name);
    }
    else
    {
        xChapter->getCurrentChapter().tableOffset = _tableView->getContentOffset().y;
        xChapter->getCurrentChapter().enterGraph(lDownloadSprite->getTag());
        
        auto & graph = xChapter->getCurrentChapter().getCurrentGraph();
        UserDefault::getInstance()->setBoolForKey(graph.name.c_str(), true);
        
        string path = FileUtils::getInstance()->getWritablePath() + graph.getHistoryName();
        bool isExsit = FileUtils::getInstance()->isFileExist(path);
        if (isExsit) {
            if (xMyArtData->isShared(graph.getPreviewName())) {
                NewDialog* dialog = NewDialog::create("shared", "cancel", "new");
                dialog->addButtonListener(CC_CALLBACK_1(GraphScene::onDialog, this));
                this->addChild(dialog, 101);
            }
            else {
                NewDialog* dialog = NewDialog::create("continue", "new", "continue");
                dialog->addButtonListener(CC_CALLBACK_1(GraphScene::onDialog, this));
                this->addChild(dialog, 101);
            }
        }
        else
        {
            map<string, string> map;
            map.insert(pair<string, string>("image_id", graph.name));
            map.insert(pair<string, string>("category_name", xChapter->getCurrentChapter().name));
            ATMInstance->sendEvent("image_start", map);
            
            graph.setNew(false);
            xScene->forward(SceneManager::Tag::GAME);
        }
    }
}

void GraphScene::updateTableView()
{
    _tableView->reloadData();
    if (xChapter->getCurrentChapter().tableOffset > 0.0f)
    {
        _tableView->setContentOffset(_tableView->minContainerOffset());
    }else
    {
        if (xChapter->getCurrentChapter().tableOffset <= _tableView->minContainerOffset().y)
        {
            _tableView->setContentOffset(Vec2(_tableView->getContentOffset().x, _tableView->minContainerOffset().y));
        }else
        {
            _tableView->setContentOffset(Vec2(_tableView->getContentOffset().x, xChapter->getCurrentChapter().tableOffset));
        }
        
    }
}

void GraphScene::tableCellTouched(TableView* table, TableViewCell* cell,Touch* pTouch)
{
    return;
    
    
    if (cell->getIdx() == this->numberOfCellsInTableView(NULL) - 1) return;
    DownloadSprite* lDownloadSprite = dynamic_cast<DownloadSprite*>(cell->getChildren().at(0));
    
    if (lDownloadSprite == NULL) return;
    
    if (!lDownloadSprite->isDownloaded()) return;
    
    auto & graph = xChapter->getCurrentChapter().vGraphInfo.at(cell->getIdx());

    if (!SSCInternalLibManager::getInstance()->checkNetworkAvailable() && !FileUtils::getInstance()->isFileExist(graph.getPngName()))
    {
        NewDialog* dialog = NewDialog::create("network_err", "", "ok");
        dialog->addButtonListener(CC_CALLBACK_1(GraphScene::onDialog, this));
        this->addChild(dialog, 101);
        return;
    }
    
    if (strcmp(lDownloadSprite->m_gGraphInfo.name.substr(0, 3).c_str(), "map") == 0)
    {
        stringstream ss;
        ss<<"";
        ss<<lDownloadSprite->m_gGraphInfo.name;
        ss<<"_white.png_spare";
        
        UserDefault::getInstance()->setBoolForKey(ss.str().c_str(), true);
        UserDefault::getInstance()->flush();
        
        
    }else if(lDownloadSprite->m_gGraphInfo.name.find("_1024") > 0 && graph.chapter_id > 0)
    {
        
        string userData = lDownloadSprite->m_gGraphInfo.name;
        string aa = userData.substr(0, 4);
        int index = stoi(aa);
        
        stringstream ss;
        ss<<"";
        ss<<"map";
        ss<<index + 20;
        ss<<"_white.png";
        
        UserDefault::getInstance()->setBoolForKey(ss.str().c_str(), true);
        UserDefault::getInstance()->flush();
    }else
    {
        UserDefault::getInstance()->setBoolForKey(lDownloadSprite->m_gGraphInfo.name.c_str(), true);
        UserDefault::getInstance()->flush();
    }
    
    
    if (lDownloadSprite->getCurrentGraphIslock())
    {
        _graphIndex = lDownloadSprite->getTag();
        NewDialog* dialog = NewDialog::create("watch_ad", "cancel", "watch_ad");
        dialog->addButtonListener(CC_CALLBACK_1(GraphScene::onDialog, this));
        this->addChild(dialog, 101);
        xDialog->setUserString(lDownloadSprite->m_gGraphInfo.name);
    }
    else
    {
        xChapter->getCurrentChapter().tableOffset = _tableView->getContentOffset().y;
        xChapter->getCurrentChapter().enterGraph(lDownloadSprite->getTag());
        
        auto & graph = xChapter->getCurrentChapter().getCurrentGraph();
        UserDefault::getInstance()->setBoolForKey(graph.name.c_str(), true);
        
        string path = FileUtils::getInstance()->getWritablePath() + graph.getHistoryName();
        bool isExsit = FileUtils::getInstance()->isFileExist(path);
        if (isExsit) {
            if (xMyArtData->isShared(graph.getPreviewName())) {
                NewDialog* dialog = NewDialog::create("shared", "cancel", "new");
                dialog->addButtonListener(CC_CALLBACK_1(GraphScene::onDialog, this));
                this->addChild(dialog, 101);
            }
            else {
                NewDialog* dialog = NewDialog::create("continue", "new", "continue");
                dialog->addButtonListener(CC_CALLBACK_1(GraphScene::onDialog, this));
                this->addChild(dialog, 101);
            }
        }
        else
        {
            map<string, string> map;
            map.insert(pair<string, string>("image_id", graph.name));
            map.insert(pair<string, string>("category_name", xChapter->getCurrentChapter().name));
            ATMInstance->sendEvent("image_start", map);

            graph.setNew(false);
            xScene->forward(SceneManager::Tag::GAME);
        }
    }
}

void GraphScene::tableCellRemoveTexture(TableView* table, TableViewCell* cell)
{
    DownloadSprite* lSprite = dynamic_cast<DownloadSprite*>(cell->getChildByName("DownloadSrpite" + to_string(0)));
    if (lSprite)
    {
        lSprite->removeTextureCache();
    }
    
    DownloadSprite* lSprite2 = dynamic_cast<DownloadSprite*>(cell->getChildByName("DownloadSrpite" + to_string(1)));
    if (lSprite2)
    {
        lSprite2->removeTextureCache();
    }
}


Size GraphScene::tableCellSizeForIndex(TableView *table, ssize_t idx)
{

    Chapter & lChapterInfo = xChapter->getCurrentChapter();
    if (SubChapter_Switch && (idx == 0 || (idx > 0 && lChapterInfo.vGraphInfo.at(idx).belongToCN != lChapterInfo.vGraphInfo.at(idx-1).belongToCN)) && (lChapterInfo.rule != "weekly"))
    {
        return Size(this->getContentSize().width, xVisibleSize.width * 0.9 + 80);
    }else
    {
        float height = ((this->getContentSize().width - (col + 1)*GRAPH_SPACE))/col + GRAPH_SPACE;
        
        if (this->numberOfCellsInTableView(nullptr) - 1 == idx)
        {
            height += GRAPH_SPACE;
        }
        
        return Size(this->getContentSize().width, height);
    }
    
}

TableViewCell* GraphScene::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell* cell = table->dequeueCell();
    if (!cell)
    {
        cell = new TableViewCell();
        cell->autorelease();
    }
    cell->removeAllChildrenWithCleanup(true);
    
    
    Chapter & lChapterInfo = xChapter->getCurrentChapter();
    
    Color3B color1 = lChapterInfo.vGraphInfo.at(idx).bgColor;
    if (color1.r == 255 && color1.g == 255 && color1.b == 255)
    {
        lChapterInfo.vGraphInfo.at(idx).bgColor = Color3B(248, 248, 248);
    }

    
    float width = (this->getContentSize().width - (col + 1)*GRAPH_SPACE)/col;
    
    for (int i = 0; i < col; ++i)
    {
        int index = idx*col + i;
        if (index >= lChapterInfo.vGraphInfo.size())
        {
            break;
        }
        
        DownloadSprite* lDownloadSprite = DownloadSprite::create(lChapterInfo.vGraphInfo.at(index));
        lDownloadSprite->setTag(index);
        lDownloadSprite->setPosition(Vec2((i + 1)*GRAPH_SPACE + (i + 0.5)*width, 0));
        lDownloadSprite->display();
        cell->addChild(lDownloadSprite);
        lDownloadSprite->setName("DownloadSrpite" + to_string(i));
        lDownloadSprite->setScale(width/lDownloadSprite->getContentSize().width);
        lDownloadSprite->addListener(this, callfuncN_selector(GraphScene::responseDownloadSprite));
        if (idx == this->numberOfCellsInTableView(NULL)-1)
        {
            lDownloadSprite->setPosition(Vec2((i + 1)*GRAPH_SPACE + (i + 0.5)*width, GRAPH_SPACE));
        }
        
    }
//    if (SubChapter_Switch && (idx > 0 && lChapterInfo.rule != "weekly" && lChapterInfo.vGraphInfo.at(idx).belongToCN != lChapterInfo.vGraphInfo.at(idx-1).belongToCN))
//    {
//        Sprite* lSprite = Sprite::create("other/chapter.png");
//        lSprite->setAnchorPoint(Vec2(0, 0.5));
//        lSprite->setPosition(Vec2(0, xVisibleSize.width) + Vec2(-10, -23));
//        lSprite->setTag(555);
//        lSprite->setName(lChapterInfo.vGraphInfo.at(idx).belongToCN.c_str());
//        cell->addChild(lSprite, 10);
//
//        Label* lable = Label::createWithTTF(lChapterInfo.vGraphInfo.at(idx).belongToCN.c_str(), "res/kalinga.ttf", 20);
//        lable->setPosition(lSprite->getContentSize()/2);
//        lSprite->addChild(lable);
//    }
    
    return cell;
}

ssize_t GraphScene::numberOfCellsInTableView(TableView *table)
{
    int length = (int)xChapter->getCurrentChapter().vGraphInfo.size();
    if (length%col == 0)
    {
        length /= col;
    }else
    {
        length = length/col + 1;
    }
    
    return length;
}

void GraphScene::tableUpdateFunc(TableView* table,TableViewCell* cell)
{
    if (!_ChapterMark) return;
    
    float offset = fabs(_tableView->minContainerOffset().y) - fabs(_tableView->getContentOffset().y);
    if (offset <= 0)
    {
        _ChapterMark->setPosition(Vec2(_ChapterMark->getPosition().x, _ChapterMarkY + offset));
    }
    
    Sprite* lSprite = dynamic_cast<Sprite*>(cell->getChildByTag(555));
    if (lSprite)
    {
        Vec2 lPos = lSprite->getParent()->convertToWorldSpace(lSprite->getPosition());
        lPos = this->convertToNodeSpace(lPos);
        log("---(%f)",lPos.y);
        
        if (lPos.y >= _ChapterMark->getPosition().y)
        {
            lSprite->setVisible(false);
            _preChapterStr = _ChapterText->getString();
            _ChapterText->setString(lSprite->getName().c_str());
        }else
        {
            lSprite->setVisible(true);
            Chapter& lCha = xChapter->getCurrentChapter();
            
            int index = (int)cell->getIdx()-1;
            if (index < 0)
            {
                index = 0;
            }
            _ChapterText->setString(lCha.vGraphInfo.at(index).belongToCN.c_str());
        }
    }
}

void GraphScene::scrollViewDidScroll(extension::ScrollView* view)
{
//    auto percent = _tableView->getContentOffset().x / (_tableView->getViewSize().width - _tableView->getContainer()->getContentSize().width) * 100;
//    if (percent < 0) {
//        percent = 0;
//    }
//    else if (percent > 100) {
//        percent = 100;
//    }
//    _slider->setPercent(percent);
//    
//    auto count = numberOfCellsInTableView(_tableView) - 1;
//    int current = round((count - 1) * percent / 100) + 1;
//    _text->setString(std::to_string(current) + "/" + std::to_string(count));
//    _text_pre->setString(std::to_string(current));
//    _text_nex->setString("/" + std::to_string(count));
    
//    float offset = fabs(_tableView->minContainerOffset().y) - fabs(_tableView->getContentOffset().y);
////    log("%f--%f--%f",_tableView->maxContainerOffset().y,_tableView->getContentOffset().y, offset);
//    
//    if (offset <= 0)
//    {
//        _ChapterMark->setPosition(Vec2(_ChapterMark->getPosition().x, _ChapterMarkY + offset));
//    }
//    
//    
//    if (!_vSprite.empty() && _vSprite.at(0))
//    {
//        
//        log("(%f)-----(%f)(%f)(%f)", _tableView->getContainer()->getBoundingBox().size.height,_tableView->getContentOffset().y, _tableView->getChildren().at(0)->getPosition().y,_tableView->getContentOffset().y - _tableView->getChildren().at(0)->getPosition().y);
//    }
    
}

void GraphScene::onSlider(Ref *pSender, Slider::EventType type)
{
//    if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
//    {
//        Slider* slider = dynamic_cast<Slider*>(pSender);
//        int percent = slider->getPercent();
//        auto offsetx = (_tableView->getViewSize().width - _tableView->getContainer()->getContentSize().width) * percent / 100;
//        _tableView->setContentOffset(Vec2(offsetx, _tableView->getContentOffset().y));
//        
//        auto count = numberOfCellsInTableView(_tableView) - 1;
//        int current = round((count - 1) * percent / 100) + 1;
//        _text->setString(std::to_string(current) + "/" + std::to_string(count));
//        _text_pre->setString(std::to_string(current));
//        _text_nex->setString("/" + std::to_string(count));
//    }
}

void GraphScene::waitPostFaild(Ref* ref)
{
    if (_postingLayer)
    {
        _postingLayer->removeFromParentAndCleanup(true);
        _postingLayer = NULL;
    }
    
    
    NewDialog* dialog = NewDialog::create("postFail", "later", "postfail_ok");
    dialog->addButtonListener(CC_CALLBACK_1(GraphScene::onDialog, this));
    this->addChild(dialog, 101);
    
}

void GraphScene::waitPostSuccessful(Ref* ref)
{
    if (_postingLayer)
    {
        _postingLayer->setPushSuccessed();
        _postingLayer = NULL;
    }
    
    
//    NewDialog* dialog = NewDialog::create("post to insprision", "later", "go_ins");
//    dialog->addButtonListener(CC_CALLBACK_1(GraphScene::onDialog, this));
//    this->addChild(dialog, 101);
}

void GraphScene::responseUpdloadImage(EventCustom* event)
{
    if (!SSCInternalLibManager::getInstance()->checkNetworkAvailable())
    {
        NewDialog* dialog = NewDialog::create("network_err", "", "ok");
        dialog->addButtonListener(CC_CALLBACK_1(GraphScene::onDialog, this));
        this->addChild(dialog, 101);
        
        return;
    }
    
    if (_postingLayer)
    {
        _postingLayer->removeFromParentAndCleanup(true);
    }
    
    PostingLayer* lSwallowLayer = PostingLayer::create(Color4B(0, 0, 0, 200));
    this->addChild(lSwallowLayer, 100);
    lSwallowLayer->setTouchEnable(true);
    _postingLayer = lSwallowLayer;
    
    runAction(Sequence::create(DelayTime::create(0.1), CallFunc::create([&](){

        int pre_name_index = (int)DownLoadPage::m_sCurrentPostImageName.find_last_of("_");
        string post_name = DownLoadPage::m_sCurrentPostImageName.substr(0, pre_name_index+1) + "save.png";
        DownLoadPage::getInstance()->postImage(post_name);
        
    }), nullptr));
}