;//
//  ChapterScene.cpp
//  BabyPlay
//
//  Created by luoxp on 3/18/15.
//
//

#include "ChapterScene.h"
#include "UICpp.h"
#include "ChapterManager.hpp"
#include "MyArtDataManager.hpp"
#include "DownloadManager.hpp"
#include "UIListViewEx.h"
#include "STSystemFunction.h"
#include "LockManager.h"
#include "DownLoadPage.h"
#include "UIColorManager.hpp"
#include "SwallowListenerLayer.hpp"
#include "SSCInternalLibManager.h"
#include "DownloadSprite.hpp"
#include "ThreadManager.hpp"
#include "NewDownloadManager.hpp"
#include "STVisibleRect.h"
#include "AnalyticsManager.hpp"
#include "FirebaseEventScene.hpp"
#include "STSystemFunction.h"
#include "GraphScene.h"

#define DEBUG_MODE              0

#define col (V::isIpad()? 3 : 2)

ChapterScene::~ChapterScene(){
    
//    xThread->removeAllNeedloadImage();
//    xNewDownLoad->removeAllNeedDownloadImage();

}

ChapterScene::ChapterScene()
{
    _tableView = nullptr;
    _isEnableprivacy = true;
    m_vProgress.clear();
    
    _titleLayer = nullptr;
    _titleBar = nullptr;
    _titleBarShadow = nullptr;
    _titleText = nullptr;
    _topMenu = nullptr;

}

bool ChapterScene::init(){
    
    if(BaseScene::init())
    {
        setName("ChapterScene");
        
        
        auto _root = CSLoader::createNode((V::isIpad()? "HomeScene_ipad.csb" : "HomeScene.csb"));
        _root->setContentSize(getContentSize());
        Helper::doLayout(_root);
        addChild(_root);
        _root->cocos2d::Node::visit();
        
        xChapter->updateChapterData();
        
        ui::Button* myArt = static_cast<Button*>(Helper::seekNodeByName(_root, "btn_myArt"));
        myArt->addClickEventListener(CC_CALLBACK_1(ChapterScene::onButton, this));


        ui::Button* insp = static_cast<Button*>(Helper::seekNodeByName(_root,"btn_inspiration"));
        insp->addClickEventListener(CC_CALLBACK_1(ChapterScene::onButton, this));
        
        
        xAnalytics->sendScreenEvent("ChapterScene");
        
//        this->scheduleOnce(schedule_selector(ChapterScene::checkUpdate), 1.0);
        
        xAds->removeAds(kTypeBannerAds);
        
        _tableBg = static_cast<Layout*>(Helper::seekNodeByName(_root, "Panel_1"));
        
        
        static bool _switch = false;
        if (!_switch)
        {
            _switch = true;
//            SSCInternalLibManager::getInstance()->doNewsBlast();
        }
    
        if (DEBUG_MODE)
        {
            MenuItemImage* btn = MenuItemImage::create("other/graph_lock.png", "other/graph_lock.png", CC_CALLBACK_1(ChapterScene::ontMenuButton, this));
            btn->setPosition(Vec2(this->getContentSize().width * 0.1, this->getContentSize().height * 0.95));
            
            Menu* menu = Menu::create(btn, NULL);
            menu->setPosition(Vec2::ZERO);
            this->addChild(menu, 105);
            
        }
        
        LibraryTitleLayer* layer = LibraryTitleLayer::create(Size(640, (V::isIpad()? 230 : 258)));
        layer->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height));
        layer->addListener(CC_CALLBACK_1(ChapterScene::onTitleLayer, this));
        this->addChild(layer, 99);
        _titleLayer = layer;
        
//        Sprite* shadow_white = Sprite::create("res/topbar_tab_bg_shadow@3x.png");
//        shadow_white->setAnchorPoint(Vec2(0.5, 0));
//        shadow_white->setScaleX((this->getContentSize().width + 10)/shadow_white->getContentSize().width);
//        shadow_white->setScaleY((TOP_HEIGHT + 5)/shadow_white->getContentSize().height);
//        shadow_white->setPosition(Vec2(layer->getContentSize().width/2, V::isIpad() ? -4:-7));
//        layer->addChild(shadow_white, -1);

        
        LayerColor* titleBar = LayerColor::create(Color4B(82, 145, 240, 255), this->getContentSize().width, TOP_HEIGHT);
        titleBar->ignoreAnchorPointForPosition(false);
        titleBar->setAnchorPoint(Vec2(0.5, 1));
        titleBar->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height+1));
        this->addChild(titleBar, 100);
        titleBar->setOpacity(0);
        _titleBar = titleBar;
        
        float fontSize, scale;
        if (V::isHighVersion())
        {
            fontSize = (V::isIpad()? 40 : 60) * 0.8;
            scale = 0.6/0.8;
        }else
        {
            fontSize = (V::isIpad()? 40 : 60);
            scale = 0.6;
        }

//        fontSize = (V::isIpad()? 40 : 60);
//        scale = 0.6;
//        Label* lbl = (Label*)txt->getVirtualRenderer();
//        lbl->getFontAtlas()->setAliasTexParameters();
        
        Label* title = Label::createWithTTF("LIBRARY", "res/Muli-SemiBold.ttf", fontSize);
        title->setPosition(titleBar->getContentSize()/2);
        title->setScale(scale);
        titleBar->addChild(title);
        title->setOpacity(0);
        _titleText = title;
//        title->getFontAtlas()->setAntiAliasTexParameters();
        
        Sprite* shadow = Sprite::create("res/topbar_tab_bg_shadow_blue@3x.png");
        shadow->setAnchorPoint(Vec2(0.5, 0));
        shadow->setScaleX((this->getContentSize().width + 10)/shadow->getContentSize().width);
        shadow->setScaleY((TOP_HEIGHT + 10)/shadow->getContentSize().height);
        shadow->setPosition(Vec2(titleBar->getContentSize().width/2, V::isIpad() ? -5:-7));
        titleBar->addChild(shadow, -1);
        shadow->setOpacity(0);
        _titleBarShadow = shadow;
        
        MenuItemImage* topMenu = MenuItemImage::create("other/topbtn.png", "other/topbtn.png", CC_CALLBACK_1(ChapterScene::onButton, this));
        topMenu->setScaleX(this->getContentSize().width/topMenu->getContentSize().width);
        topMenu->setScaleY(TOP_HEIGHT/topMenu->getContentSize().height);
        topMenu->setName("topMenu");
        topMenu->setAnchorPoint(Vec2(0.5, 1));
        topMenu->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height));
        
        Menu* lMenu = Menu::create(topMenu, NULL);
        lMenu->setPosition(Vec2::ZERO);
        this->addChild(lMenu, 99);
        _topMenu = lMenu;

//        NewDialog* dialog = NewDialog::create("network_err", "", "ok");
//        dialog->addButtonListener(CC_CALLBACK_1(ChapterScene::networkError, this));
//        this->addChild(dialog, 101);
//
//        NewDialog* dialog1 = NewDialog::create("post fail", "later", "tryAgain");
//        dialog1->addButtonListener(CC_CALLBACK_1(ChapterScene::onDialog, this));
//        this->addChild(dialog1, 101);
//
//        NewDialog* dialog2 = NewDialog::create("quit", "cancel", "ok");
//        dialog2->addButtonListener(CC_CALLBACK_1(ChapterScene::responseBack, this));
//        this->addChild(dialog2, 101);
        

        this->initUI();
        
        return true;
    }

    return false;
}


void ChapterScene::onTitleLayer(Mode mode)
{
    
    if (mode == Mode::LEFT)
    {
        xChapter->enterChapter("Birds");
    }else if (mode == Mode::CENTER)
    {
        xChapter->enterChapter("Christmas");
    }else if(mode == Mode::RIGHT)
    {
        xChapter->enterChapter("Portraits");
    }
    
    xChapter->setTableOffset(_tableView->getContentOffset().y);

    string userName = xChapter->getCurrentChapter().name;
    std::transform(userName.begin(), userName.end(), userName.begin(), ::toupper);
    
    xUser->setBoolForKey(userName.c_str(), true);
    xScene->forward(SceneManager::Tag::GRAPH, SceneManager::SceneAnimation::INLEFT);
    ATMInstance->sendEvent("view_category", "category_name", xChapter->getCurrentChapter().name + "_feature");

}

void ChapterScene::onNewDialog(const string& name)
{

}

void ChapterScene::ontMenuButton(Ref* ref)
{
    Director::getInstance()->replaceScene(FirebaseEventScene::create());
}

void ChapterScene::onEnterTransitionDidFinish()
{
    Scene::onEnterTransitionDidFinish();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    xNewDownLoad->downloadJsonFile();
#endif

}

void ChapterScene::initUI()
{
    xThread->removeAllNeedloadImage();
    xNewDownLoad->removeAllNeedDownloadImage();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    this->runAction(Sequence::create(DelayTime::create(0.2f), CallFunc::create([](){
//        Director::getInstance()->getTextureCache()->removeUnusedTextures();
    }), NULL));
    
    if (!SSCInternalLibManager::getInstance()->checkNetworkAvailable())
    {
        static bool _switch = false;
        if (!_switch)
        {
            _switch = true;
            NewDialog* dialog = NewDialog::create("network_err", "", "ok");
            dialog->addButtonListener(CC_CALLBACK_1(ChapterScene::networkError, this));
            this->addChild(dialog, 101);
        }

    }
    
    this->createTableView();
    
    if (xChapter->getTableOffset() > 0)
    {
        _tableView->setContentOffset(Vec2(_tableView->getContentOffset().x, _tableView->minContainerOffset().y));
    }else
    {
        if (xChapter->getTableOffset() < _tableView->minContainerOffset().y)
        {
            _tableView->setContentOffset(Vec2(_tableView->getContentOffset().x, _tableView->minContainerOffset().y));
        }else if (xChapter->getTableOffset() > _tableView->maxContainerOffset().y)
        {
            _tableView->setContentOffset(Vec2(_tableView->getContentOffset().x, _tableView->maxContainerOffset().y));
        }else
        {
            _tableView->setContentOffset(Vec2(_tableView->getContentOffset().x, xChapter->getTableOffset()));
        }
    }
}

void ChapterScene::showRateUs(EventCustom* event)
{
    STSystemFunction sf;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    sf.rating("1040802159", "Enjoyed our app? We are looking forward to your encouragements!^_^");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (!SSCInternalLibManager::getInstance()->hasRated())
    {
        SSCInternalLibManager::getInstance()->rateUs();
    }
#endif
    
}

bool isShowDownload = false;
void ChapterScene::checkUpdate(float dt)
{
    
    if (Director::getInstance()->getRunningScene() == this) {
//        if (xLock->_isShowRateUs)
//        {
//            xLock->_isShowRateUs = false;
//            this->showRateUs(NULL);
//        }
        
        if (!isShowDownload && xDPM_getInstance->isEverDownloadFinsh())
        {
            isShowDownload = true;
            xDownload->excuteFunc(DownloadState::CHECK_NEW_VERSION_STATE);
        }
    }
}

void ChapterScene::onDialog(const string& name)
{
    if(name == "ok"){
        
        xMyArtData->saveMyArtData();
        DownLoadPage::getInstance()->saveClickLikeInfo();
        xAudio->stopMusic();
        Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        exit(0);
#endif
    }
}

void ChapterScene::networkError(const string& name)
{

}



void ChapterScene::onButton(Ref* pSender)
{
//    if (type != Widget::TouchEventType::ENDED) return;
    
//    if (((Node*)pSender)->getTag() == 1) return;

    string name = ((Node*)pSender)->getName();
    
    if (name == "btn_myArt")
    {
        log("click myart");
        xScene->forward(SceneManager::Tag::MY_ARTS, SceneManager::SceneAnimation::NONE);
    }else if (name == "btn_inspiration")
    {
        DownLoadPage::m_bIsShareByInsp = false;
        xScene->forward(SceneManager::Tag::INSPIRATION, SceneManager::SceneAnimation::NONE);
    }else if (name == "topMenu")
    {
        int xx = _titleBar->getOpacity();
        
        if (_tableView && xx == 255)
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
    
    xChapter->setTableOffset(_tableView->getContentOffset().y);
}

void ChapterScene::createListView()
{
    auto & data = xChapter->getData();

    for(int i = 0; i < data.size(); i ++)
    {
        auto layout = Layout::create();
        layout->setContentSize(Size(550, 600));
        auto vGraphInfo = data.at(i).vGraphInfo;
        layout->setBackGroundColor(Color3B(0, 100, 0));

        
        auto background = Sprite::create("other/Gallery_2.png");
        background->setPosition(layout->getContentSize()/2);
        background->setColor(data.at(i).frame);
        layout->addChild(background,2);
        _listview->pushBackCustomItem(layout);
        
        //添加Text
        string textContent = xStr->getStringDefault(data.at(i).name);
        auto title = Text::create(textContent, FONT_NAME, 47);
        title->setPosition(Vec2(background->getContentSize().width * 0.5, background->getContentSize().height * 0.24));
        title->setFontName("res/kalinga.ttf");
        background->addChild(title, 1);
        
        if (UserDefault::getInstance()->getBoolForKey(data.at(i).name.c_str(), false))
        {
            Sprite* lSprite = Sprite::create("other/new.png");
            lSprite->setAnchorPoint(Vec2(1, 1));
            lSprite->setPosition(layout->getContentSize() + Size(-25, -20));
            layout->addChild(lSprite, 2);
        }
        
            
        layout->setName("chapter");
        layout->setTouchEnabled(true);
        layout->addTouchEventListener(CC_CALLBACK_2(ChapterScene::reponseList, this));
            
        DownloadSprite* lDownloadSpr = DownloadSprite::create(data.at(i).coverUrl, data.at(i).image, data.at(i).reDownload);
        lDownloadSpr->display_cover();
        lDownloadSpr->setName("cover");
        lDownloadSpr->setPosition(layout->getContentSize()/2);
        lDownloadSpr->setScale(0.485);
        layout->addChild(lDownloadSpr, -1);
            
    }
    _listview->setScrollBarEnabled(false);
//    _listview->addTouchEventListener(CC_CALLBACK_2(ChapterScene::reponseList, this));
        }
        
void ChapterScene::createTableView()
{

    STTableView* table = STTableView::create(this, Size(getContentSize().width, Director::getInstance()->getVisibleSize().height - BOTTOM_HEIGHT));
    table->setDirection(extension::ScrollView::Direction::VERTICAL);
    _tableView = table;
    
    table->setDelegate(this);
 
    table->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    table->setColor(Color3B::RED);
        
    table->ignoreAnchorPointForPosition(false);
    table->setAnchorPoint(Vec2(0.5, 0));
    table->setPosition(Vec2(this->getContentSize().width/2, BOTTOM_HEIGHT + (V::isIpad()? 1 : 0)));
    _tableBg->addChild(table,10);
    
    table->reloadData();
}

void ChapterScene::tableCellTouched(TableView* table, TableViewCell* cell)
{
        
}

void ChapterScene::enablePricavy(float dt)
{
    _isEnableprivacy = true;
}

void ChapterScene::scrollViewDidScroll(cocos2d::extension::ScrollView* view)
{
    float topBarHeight = TOP_HEIGHT - 1;

    if (_tableView->getContentOffset().y > _tableView->minContainerOffset().y)
    {
        float offsetY = abs(_tableView->getContentOffset().y - _tableView->minContainerOffset().y);
        _titleLayer->setPositionY(this->getContentSize().height + offsetY);

    }else
    {
        _titleLayer->setPositionY(this->getContentSize().height);
    }

    float topOffset = _tableView->minContainerOffset().y + _titleLayer->getContentSize().height - 2*topBarHeight;
    if (_tableView->getContentOffset().y > topOffset &&  _tableView->getContentOffset().y < topOffset + topBarHeight)
    {
        float percent = 1 - abs(_tableView->getContentOffset().y - (topOffset + topBarHeight))/topBarHeight;
        log("+++++++++++    %f", percent);
        if (_titleBar)
        {
            _titleBar->setOpacity(percent * 255);
            _titleBarShadow->setOpacity(percent * 255);
            _titleText->setOpacity(percent * 255);
        }
    }else if (_tableView->getContentOffset().y >= topOffset + topBarHeight)
    {
        _titleBar->setOpacity(255);
        _titleBarShadow->setOpacity( 255);
        _titleText->setOpacity( 255);
    }else if (_tableView->getContentOffset().y < topOffset)
    {
        _titleBar->setOpacity(0);
        _titleBarShadow->setOpacity(0);
        _titleText->setOpacity(0);
    }
}

void ChapterScene::scrollViewMoveEnded()
{
    float topOffset = _tableView->minContainerOffset().y + _titleLayer->getContentSize().height - 2*(TOP_HEIGHT-1);
    float bottomOffset = _tableView->minContainerOffset().y;
    float time;
    
    if (_tableView->getContentOffset().y > bottomOffset && _tableView->getContentOffset().y < topOffset)
    {
        time = 0.35f * (abs(_tableView->minContainerOffset().y) - abs(_tableView->getContentOffset().y))/(_titleLayer->getContentSize().height - (TOP_HEIGHT-1));
        _tableView->setContentOffsetInDuration(Vec2(_tableView->getContentOffset().x, _tableView->minContainerOffset().y), time);
        this->runAction(Sequence::create(DelayTime::create(time), CallFunc::create([this](){
            _titleBar->setOpacity(0);
            _titleBarShadow->setOpacity(0);
            _titleText->setOpacity(0);
        }), NULL));
        

    }else if (_tableView->getContentOffset().y > topOffset &&  _tableView->getContentOffset().y < _tableView->minContainerOffset().y + _titleLayer->getContentSize().height - TOP_HEIGHT)
    {
        time = 0.35f * abs(abs(_tableView->minContainerOffset().y + _titleLayer->getContentSize().height - TOP_HEIGHT) - abs(_tableView->getContentOffset().y))/(_titleLayer->getContentSize().height - TOP_HEIGHT);
        _tableView->setContentOffsetInDuration(Vec2(_tableView->getContentOffset().x, topOffset + TOP_HEIGHT), time);
        
        this->runAction(Sequence::create(DelayTime::create(time), CallFunc::create([this](){
            _titleBar->setOpacity(255);
            _titleBarShadow->setOpacity(255);
            _titleText->setOpacity(255);
        }), NULL));
    }
}

void ChapterScene::tableMoveEnded()
{
//    log("---: %f,  %f,  %f",_tableView->getContentOffset().y, _tableView->minContainerOffset().y, _tableView->maxContainerOffset().y);
    
    return;
    
    float topOffset = _tableView->minContainerOffset().y + _titleLayer->getContentSize().height - TOP_HEIGHT;
    float bottomOffset = _tableView->minContainerOffset().y;
    
    if (_tableView->getContentOffset().y > bottomOffset && _tableView->getContentOffset().y < topOffset)
    {
        _tableView->setContentOffset(Vec2(_tableView->getContentOffset().x, _tableView->minContainerOffset().y));
    }else if (_tableView->getContentOffset().y > topOffset &&  _tableView->getContentOffset().y < _tableView->minContainerOffset().y + _titleLayer->getContentSize().height)
    {
        _tableView->setContentOffset(Vec2(_tableView->getContentOffset().x, _tableView->minContainerOffset().y + _titleLayer->getContentSize().height));
    }
    
}

void ChapterScene::tableCellTouched(TableView* table, TableViewCell* cell,Touch* pTouch)
{
    if (cell->getIdx() == 0)
    {
        return;
    }
    
    if (cell->getIdx() == this->numberOfCellsInTableView(NULL) - 1)
    {
        if (_isEnableprivacy)
        {
            _isEnableprivacy = false;
            STSystemFunction sf;
            sf.go2PrivacyPage();
            this->scheduleOnce(schedule_selector(ChapterScene::enablePricavy), 1.0f);
        }
        
    }
    
    return;
        
    Sprite* lWeekly = static_cast<Sprite*>(cell->getChildByName("weekly"));
    if (lWeekly)
    {
        xChapter->setTableOffset(_tableView->getContentOffset().y);
        xChapter->enterChapter((int)cell->getIdx());
        
        xUser->setBoolForKey(xChapter->getCurrentChapter().name.c_str(), false);
        xScene->forward(SceneManager::Tag::GRAPH);
    }

    DownloadSprite* lSprite = static_cast<DownloadSprite*>(cell->getChildByName("cover"));
    if (lSprite)
    {
        if (!lSprite->isDownloaded())
        {
            return;
        }
        
        xChapter->setTableOffset(_tableView->getContentOffset().y);
        xChapter->enterChapter((int)cell->getIdx()-1);
        
        xUser->setBoolForKey(xChapter->getCurrentChapter().name.c_str(), false);
        xScene->forward(SceneManager::Tag::GRAPH);

        ATMInstance->sendEvent("view_category", "category_name", xChapter->getCurrentChapter().name);
        
        
    }
}

void ChapterScene::responseSpriteEvent(Node* ref)
{
    DownloadSprite* lSprite = static_cast<DownloadSprite*>(ref);
    if (lSprite)
    {
        if (!lSprite->isDownloaded())
        {
            return;
        }
        
        xChapter->setTableOffset(_tableView->getContentOffset().y);
        xChapter->enterChapter((int)lSprite->getTag());
        
        string userName = xChapter->getCurrentChapter().name;
        std::transform(userName.begin(), userName.end(), userName.begin(), ::toupper);
        
        xUser->setBoolForKey(userName.c_str(), true);
        xScene->forward(SceneManager::Tag::GRAPH, SceneManager::SceneAnimation::INLEFT);
        
        ATMInstance->sendEvent("view_category", "category_name", xChapter->getCurrentChapter().name);
        
        
    }
}
        
void ChapterScene::tableCellRemoveTexture(TableView* table, TableViewCell* cell)
{
    DownloadSprite* lSprite = dynamic_cast<DownloadSprite*>(cell->getChildByName("cover"));
    if (lSprite)
    {
        lSprite->removeTextureCache();
    }
}
        
Size ChapterScene::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    if (idx == 0)
    {
        return Size(640, (V::isIpad() ? 233 : 263));
    }
    
    if (idx == this->numberOfCellsInTableView(NULL) - 1)
    {
        return Size(350, V::isIpad()? 45:61);
    }
    
    if (xChapter->getData().at(idx-1).rule == "weekly")
    {
        return Size(this->getContentSize().width, 150);
    }
    
    float width = (this->getContentSize().width - (col + 1) * SPACE)/col;
    float height = 607./506 * width + SPACE;
    return Size(this->getContentSize().width, height);
}
        
TableViewCell* ChapterScene::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell* cell = table->dequeueCell();
    if (!cell)
        {
        cell = new TableViewCell();
        cell->autorelease();
        }else
        {
        cell->removeAllChildrenWithCleanup(true);
        }
    
    if (idx == 0)
    {
        return cell;
    }
    
    if (idx == this->numberOfCellsInTableView(NULL)-1)
    {
        Label* label = Label::createWithTTF("Privacy Policy", "res/Muli-Regular.ttf", V::isIpad()? 34:44);
        label->setPosition(Vec2(this->getContentSize().width/2, V::isIpad()? 22 : 30));
        cell->addChild(label);
        label->setColor(Color3B(160, 160, 160));
        label->setScale(0.5f);
        
        DrawNode* node = DrawNode::create();
        node->drawLine(Vec2(0, 0), Vec2(label->getContentSize().width/2, 0), Color4F(Color4B(160, 160, 160, 255)));
        node->setPosition(Vec2( this->getContentSize().width/2 - label->getContentSize().width/4, V::isIpad()? 12 : 18));
        node->setLineWidth(1);
        cell->addChild(node);
        
        return cell;
    }
    
    auto& data = xChapter->getData();

    if (xChapter->getData().at(idx-1).rule == "weekly")
    {
        Sprite* lSprite = Sprite::create("other/weekly.png");
        lSprite->setPosition(Vec2(this->getContentSize().width/2, 50));
        cell->addChild(lSprite);
        lSprite->setName("weekly");
        
        Label* label = Label::createWithTTF(data.at(idx-1).name, "res/kalinga.ttf", 35);
        label->setPosition(lSprite->getContentSize()/2);
        lSprite->addChild(label);
        
        return cell;
    }
    
    int width = (this->getContentSize().width - SPACE * (col + 1))/col;
    
    for (int i = 0; i < col; ++i)
    {
        int index = col*((int)idx-1) + i;
        if (index >= data.size())
        {
            break;
        }
        
        DownloadSprite* lDownloadSpr = DownloadSprite::create(data.at(index).coverUrl, data.at(index).image, data.at(index).reDownload);
        lDownloadSpr->display_cover();
        lDownloadSpr->setTag(index);
        lDownloadSpr->setPosition(Vec2((i + 1)*SPACE + (i + 0.5)*width, 0));
        lDownloadSpr->setScale(width/lDownloadSpr->getContentSize().width);
        cell->addChild(lDownloadSpr, -1);
        lDownloadSpr->addListener(this, callfuncN_selector(ChapterScene::responseSpriteEvent));
    
        float fontSize, scale;
        if (V::isHighVersion())
        {
            fontSize = 45*0.7;
            scale = 1.0/0.7;
        }else
        {
            fontSize = 67;
            scale = 0.67;
        }
        
        //添加Text
        string textContent = xStr->getStringDefault(data.at(index).name);
        Label* text = Label::createWithTTF(textContent, "res/Muli-Regular.ttf", fontSize);
        text->ignoreAnchorPointForPosition(false);
        text->setAnchorPoint(Vec2(0, 0.5));
        text->setPosition(Vec2(30, lDownloadSpr->getContentSize().height * 0.088));
        text->setColor(Color3B(43, 43, 43));
        text->setScale(scale);
        lDownloadSpr->addChild(text, 10);

        
        string userName = data.at(index).name;
        std::transform(userName.begin(), userName.end(), userName.begin(), ::toupper);
        
        if (!UserDefault::getInstance()->getBoolForKey(userName.c_str(), false))
        {
            Sprite* lSprite = Sprite::create("other/new_lg.png");
            lSprite->setPosition(Vec2(55, lDownloadSpr->getContentSize().height + 12 - lSprite->getContentSize().height/2));
            lSprite->setScale(0.69);
            lDownloadSpr->addChild(lSprite, 10);
        }
    }
    
    return cell;
}
    
ssize_t ChapterScene::numberOfCellsInTableView(TableView *table)
{
    int count = (int)xChapter->getData().size();
    
    if (count%col == 0)
    {
        count = count / col;
    }else
    {
        count = (count/col) + 1;
    }
    
    return count + 1 + 1;
}

void ChapterScene::reponseList(Ref* pSender, Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED) return;
    
    if (((Node*)pSender)->getTag() == 1) return;
    
    string name = ((Node*)pSender)->getName();
    if (name == "chapter")
    {
        DownloadSprite* lSprite = (DownloadSprite*)((Node*)pSender)->getChildByName("cover");
        if (!lSprite->isDownloaded())
        {
            return;
        }
        
        xChapter->enterChapter((int)_listview->getCurSelectedIndex());
        
        xUser->setBoolForKey(xChapter->getCurrentChapter().name.c_str(), false);
        xScene->forward(SceneManager::Tag::GRAPH);
    }
    
    if (name == "download_chapter")
    {
        if (SSCInternalLibManager::getInstance()->checkNetworkAvailable())
        {
//            xDialog->show("find_new", "cancel", "Download", CC_CALLBACK_1(ChapterScene::downloadDialog, this));
        }else
        {
//            xDialog->show("network_err", "ok", CC_CALLBACK_1(ChapterScene::downloadDialog, this));
        }
    }
    
    xChapter->setTableOffset(_listview->getInnerContainerPosition().y);
}

void ChapterScene::downloadDialog(const string& name)
{
    if (name == "Download")
    {
        DownloadManager::_isDownloading = true;
        DownloadManager::getInstance()->excuteFunc(DownloadState::CHECK_NEW_VERSION_STATE2);
        
//        this->runAction(Sequence::create(DelayTime::create(0.5), CallFunc::create([this](){
//            this->waitDownloadFinish(NULL);
//        }), NULL)  );
        _listview->removeLastItem();
        
    }else if (name == "cancel")
    {
    
    }
}

void ChapterScene::updateProgress(Ref* ref)
{
    for (int i = 0; i < m_vProgress.size(); ++i)
    {
        ProgressTimer* lTimer = m_vProgress.at(i);
        lTimer->setPercentage(xChapter->getData().at(lTimer->getTag()).getPercent());
        log("%d",lTimer->getTag());
        if (xChapter->getData().at(lTimer->getTag()).getPercent() >= 100)
        {
            xChapter->getData().at(lTimer->getTag()).isNewChapter = false;
            this->waitDownloadFinish(NULL);
        }
    }
}

void ChapterScene::waitDownloadFinish(Ref* ref)
{
    m_vProgress.clear();
    _listview->removeAllChildren();
    createListView();
}

void ChapterScene::checkTableOffset()
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            {
    if (xChapter->getTableOffset() < INT_MAX) {
        _oldY = _tableView->getPositionY();
//        _listview->setPositionY(_oldY + xChapter->getTableOffset());
        scheduleOnce(schedule_selector(ChapterScene::refresh), 0);
    }
}

void ChapterScene::refresh(float dt)
{
//    _listview->setPositionY(_oldY);
    
    //这个类只是为了使用protected的方法
    ListViewEx listViewEx(_listview);
    
    //log(" %f %f", , );
    listViewEx.jumpToDestinationEx(Vec2(0, xChapter->getTableOffset()));
}
