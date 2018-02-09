//
//  ComicScene.cpp
//  ComicBook
//
//  Created by houhuang on 17/12/4.
//
//

#include "ComicScene.h"
#include "STVisibleRect.h"
#include "CartoonManager.h"
#include "CoverSprite.h"
#include "HomeScene.h"
#include "STSystemFunction.h"

#define TOP_HEIGHT              (V::isIpad()? 65 : 86)
#define col                     (V::isIpad()? 4 : 3)
#define SPACE                   20

enum{
    st_button_back = 10,
    st_button_showAndHide,
};

ComicScene::~ComicScene()
{
    xCartoon->setCatagoryOffset(stoi(xCartoon->getCurrentCategory().id), _table->getContentOffset().y);
    UserDefault::getInstance()->setBoolForKey("isHideComic", _isShowAlready);
    UserDefault::getInstance()->flush();
}

ComicScene::ComicScene()
{
    _cartoonLayer = nullptr;
    _table = nullptr;
    _isShowAlready = false;
    _showBtnSpr = nullptr;
    _showButton = nullptr;
    _dialog = nullptr;
}

bool ComicScene::init()
{
    if (!Scene::init())  return false;
    
    setName("ComicScene");
    
    LayerColor* layer = LayerColor::create(Color4B(245, 245, 245, 255));
    this->addChild(layer);
    
    LayerColor* topLayer = LayerColor::create(Color4B(107, 97, 225, 255), this->getContentSize().width, TOP_HEIGHT);
    topLayer->ignoreAnchorPointForPosition(false);
    topLayer->setAnchorPoint(Vec2(0.5, 1));
    topLayer->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height));
    this->addChild(topLayer, 10);
    
    Sprite* topBar = Sprite::create("top_bar.png");
    topBar->setScaleX((this->getContentSize().width+10)/topBar->getContentSize().width);
    topBar->setAnchorPoint(Vec2(0.5, 0));
    topBar->setPosition(Vec2(topLayer->getContentSize().width/2, -5));
    topLayer->addChild(topBar, -1);
    
    float fontSize = V::isIpad()? 30:40;
    Label* title = Label::createWithTTF(xCartoon->getCurrentCategory().name, "fonts/font1.ttf", fontSize);
    title->setPosition(Vec2(topLayer->getContentSize()/2) + Vec2(0, -7));
    topLayer->addChild(title, 10);

    auto removeCartoonEvent = EventListenerCustom::create(st_remove_showCartoonLayer, CC_CALLBACK_1(ComicScene::responseRemoveCartoonLayer, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(removeCartoonEvent, this);
    
//    this->createTableView();
    
    MenuItemImage* back = MenuItemImage::create("back_bg.png", "back_bg.png", CC_CALLBACK_1(ComicScene::onButton, this));
    back->setPosition(Vec2(30, topLayer->getContentSize().height/2));
    back->setTag(st_button_back);
    back->setScale(topLayer->getContentSize().height*0.6/back->getContentSize().height);
    
    Sprite* back_sprite = Sprite::create("back.png");
    back_sprite->setPosition(back->getContentSize()/2);
    back->addChild(back_sprite);
    
    MenuItemImage* show = MenuItemImage::create("palette_color@3x.png", "palette_color@3x.png", CC_CALLBACK_1(ComicScene::onButton, this));
    show->setPosition(Vec2(this->getContentSize().width - 30, topLayer->getContentSize().height/2));
    show->setTag(st_button_showAndHide);
    show->setScale(topLayer->getContentSize().height*0.6/back->getContentSize().height);
    _showButton = show;
    
    _showBtnSpr = Sprite::create("yes.png");
    _showBtnSpr->setPosition(show->getContentSize()/2);
    show->addChild(_showBtnSpr);
    
    Menu* lMenu = Menu::create(back, show, NULL);
    lMenu->setPosition(Vec2::ZERO);
    topLayer->addChild(lMenu, 10);
    
    _isShowAlready = UserDefault::getInstance()->getBoolForKey("isHideComic", true);
    
    setShowButtonState();
    
    auto removeDialogEvent = EventListenerCustom::create(st_remove_dialog, CC_CALLBACK_1(ComicScene::removeDialog, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(removeDialogEvent, this);
    
    if (xCartoon->getPreSceneName() == "ReadScene")
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        STSystemFunction sf;
        sf.showFullScreen();
#endif
    }
    
    return true;
}

void ComicScene::onEnterTransitionDidFinish()
{
    Scene::onEnterTransitionDidFinish();
    
    createTableView();
    
    this->runAction(Sequence::create(DelayTime::create(0.2f), CallFunc::create([this](){
        this->addBackListener();
    }), NULL));
    
}

void ComicScene::createTableView()
{
    TableView* table = TableView::create(this, Size(this->getContentSize().width, this->getContentSize().height - TOP_HEIGHT + 8));
    table->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    table->setVerticalFillOrder(cocos2d::extension::TableView::VerticalFillOrder::TOP_DOWN);
    _table = table;
    
    table->setDelegate(this);
    table->ignoreAnchorPointForPosition(false);
    table->setAnchorPoint(Vec2(0.5, 1));
    table->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height - TOP_HEIGHT + 8));

    table->reloadData();
    this->addChild(table);
    
    if (xCartoon->getPreSceneName() == "ReadScene")
    {
        float offsetY = xCartoon->getCatagoryOffset(stoi(xCartoon->getCurrentCategory().id));
        if (offsetY > 0)
        {
            _table->setContentOffset(Vec2(_table->getContentOffset().x, _table->maxContainerOffset().y));
            
        }else
        {
            if (offsetY < _table->minContainerOffset().y)
            {
                _table->setContentOffset(Vec2(_table->getContentOffset().x, _table->minContainerOffset().y));
            }else
            {
                _table->setContentOffset(Vec2(_table->getContentOffset().x, offsetY));
            }
            
        }
        
    }
  

}

void ComicScene::onButton(Ref* ref)
{
    MenuItemImage* lMenuItem = (MenuItemImage*)ref;
    switch (lMenuItem->getTag()) {
        case st_button_back:
        {
            xCartoon->setPreSceneName("ComicScene");
            Director::getInstance()->replaceScene(TransitionMoveInL::create(0.2f, HomeScene::create()));
        }
            break;
            
        case st_button_showAndHide:
        {
            _showButton->setEnabled(false);
            this->runAction(Sequence::create(DelayTime::create(0.1f), CallFunc::create([this](){
                _showButton->setEnabled(true);
            }), NULL));
            
            if (_isShowAlready)
            {
                _isShowAlready = false;
            }else
            {
                _isShowAlready = true;
            }
            
            setShowButtonState();
            _table->reloadData();
            
            bool firstClick = UserDefault::getInstance()->getBoolForKey("isFirstClickShowButton", true);
            if (firstClick)
            {
                NewDialog* lDialog = NewDialog::create("隐藏已经阅读过的漫画。", "", "关闭");
                lDialog->addButtonListener(CC_CALLBACK_1(ComicScene::onDialog, this));
                this->addChild(lDialog, 101);
                _dialog = lDialog;
            }
        }
            break;
        default:
            break;
    }
}

void ComicScene::onDialog(const string &name)
{
    if (name == "关闭")
    {
        UserDefault::getInstance()->setBoolForKey("isFirstClickShowButton", false);
        UserDefault::getInstance()->flush();
    }
}

void ComicScene::removeDialog(cocos2d::EventCustom *event)
{
    if (_dialog)
    {
        _dialog->removeFromParent();
        _dialog = nullptr;
    }
}

void ComicScene::responseRemoveCartoonLayer(EventCustom* event)
{
    if (_cartoonLayer)
    {
        _cartoonLayer->removeFromParentAndCleanup(true);
        _cartoonLayer = nullptr;
    }
}

void ComicScene::setShowButtonState()
{
    _cartoonInfoVec.clear();
    
    if (_isShowAlready)
    {
        _showBtnSpr->setTexture("yes.png");
    }else
    {
        _showBtnSpr->setTexture("no.png");
    }
    
    for (CartoonInfo info : xCartoon->getCurrentCategory()._cartoonVec)
    {
        string folder = info.folder + "pagenumber";
        int page = UserDefault::getInstance()->getIntegerForKey(folder.c_str(), 0);
        
        if (_isShowAlready)
        {
            _cartoonInfoVec.push_back(info);
        }else
        {
            if (page < 2)
            {
                _cartoonInfoVec.push_back(info);
            }
        }
    }

}

void ComicScene::responseSpriteClick(Ref* ref)
{
    CoverSprite* lSprite = (CoverSprite*)ref;
    int index = lSprite->getTag();
    xCartoon->setCurrentCartoon(index);
    
    ShowCartoonInfoLayer* layer = ShowCartoonInfoLayer::create(_cartoonInfoVec.at(index));
    this->addChild(layer, 100);
    _cartoonLayer = layer;

    string userText = _cartoonInfoVec.at(index).folder + "isNewCartoon";
    UserDefault::getInstance()->setBoolForKey(userText.c_str(), false);
}

void ComicScene::tableCellTouched(TableView* table, TableViewCell* cell)
{

}

Size ComicScene::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    float height = ((this->getContentSize().width - (col + 1)*SPACE)/col)*608./346 + SPACE;
    if (idx == this->numberOfCellsInTableView(nullptr) -1)
    {
        height += SPACE;
    }
    
    if (idx == 0)
    {
        height += 10;
    }
    
    return Size(this->getContentSize().width, height);
}

TableViewCell* ComicScene::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell* cell = table->dequeueCell();
    if (!cell)
    {
        cell = new TableViewCell();
        cell->autorelease();
    }
    
    cell->removeAllChildrenWithCleanup(true);
    
    for (int i = 0; i < col; ++i)
    {
        int index = (int)idx*col + i;
        if (index >= _cartoonInfoVec.size())
        {
            break;
        }
        
        float width = (this->getContentSize().width - (col+1)*SPACE)/col;
        CartoonInfo info = _cartoonInfoVec.at(index);
        
        CoverSprite* lSprite = CoverSprite::create(info.folder, info.coverUrl, Size(0, 0));
        lSprite->setTag(index);
        lSprite->diaplay();
        lSprite->setAnchorPoint(Vec2(0.5, 0));
        lSprite->setScale(width/lSprite->getContentSize().width);
        lSprite->setPosition(Vec2((i + 1)*SPACE + (i + 0.5)*width, 0));
        lSprite->addListener(this, callfuncO_selector(ComicScene::responseSpriteClick));
        
        string userText = info.folder + "isNewCartoon";
        bool isNew = UserDefault::getInstance()->getBoolForKey(userText.c_str(), true);
        
        bool isNewCartoon = false;
        if (info.isNew == "true")
        {
            isNewCartoon = true;
        }
        
        if (isNew && isNewCartoon)
        {
            Sprite* newSprite = Sprite::create("new.png");
            newSprite->setPosition(Vec2(0, lSprite->getContentSize().height) + Vec2(8, -8));
            newSprite->setScale(0.6f);
            lSprite->addChild(newSprite, 10);
        }
        
        Label* name = Label::createWithTTF(info.name, "fonts/d2.ttf", 50);
        name->setPosition(Vec2(lSprite->getContentSize().width/2, 52));
        name->setColor(Color3B(43, 43, 43));
        lSprite->addChild(name, 11);
        
        if (name->getContentSize().width > (width + 20))
        {
            name->setScale((width + 100)/name->getContentSize().width);
        }

        if (idx == this->numberOfCellsInTableView(NULL)-1)
        {
            lSprite->setPosition(Vec2((i + 1)*SPACE + (i + 0.5)*width, SPACE));
        }

        cell->addChild(lSprite);
        
    }
    
    return cell;
}

ssize_t ComicScene::numberOfCellsInTableView(TableView *table)
{
    int length = (int)_cartoonInfoVec.size();
    if (length%col == 0)
    {
        length /= col;
    }else
    {
        length = length/col + 1;
    }
    
    return length;
}

void ComicScene::addBackListener()
{
    auto listener = EventListenerKeyboard::create();
    listener->onKeyReleased = [this](EventKeyboard::KeyCode code, Event* event){
        
        if (code == EventKeyboard::KeyCode::KEY_BACK)
        {
            if (_dialog)
            {
                _dialog->removeFromParent();
                _dialog = nullptr;
            }
            else if (_cartoonLayer)
            {
                _cartoonLayer->removeFromParentAndCleanup(true);
                _cartoonLayer = nullptr;
            }else
            {
                xCartoon->setPreSceneName("ComicScene");
                Director::getInstance()->replaceScene(TransitionMoveInL::create(0.2f, HomeScene::create()));
            }
        }
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}