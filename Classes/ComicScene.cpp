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

#define TOP_HEIGHT              (V::isIpad()? 65 : 86)
#define col                     (V::isIpad()? 3 : 2)
#define SPACE                   40

enum{
    st_button_back = 10,
};

ComicScene::~ComicScene()
{
    xCartoon->setCatagoryOffset(stoi(xCartoon->getCurrentCategory().id), _table->getContentOffset().y);
}

ComicScene::ComicScene()
{
    _cartoonLayer = nullptr;
    _table = nullptr;
}

bool ComicScene::init()
{
    if (!Scene::init())  return false;
    
    LayerColor* layer = LayerColor::create(Color4B(245, 245, 245, 255));
    this->addChild(layer);
    
    LayerColor* topLayer = LayerColor::create(Color4B(82, 145, 240, 255), this->getContentSize().width, TOP_HEIGHT);
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

    auto removeCartoonEvent = EventListenerCustom::create("remove_cartoonLayer", CC_CALLBACK_1(ComicScene::responseRemoveCartoonLayer, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(removeCartoonEvent, this);
    this->createTableView();
    
    MenuItemImage* back = MenuItemImage::create("back_bg.png", "back_bg.png", CC_CALLBACK_1(ComicScene::onButton, this));
    back->setPosition(Vec2(30, topLayer->getContentSize().height/2));
    back->setTag(st_button_back);
    back->setScale(topLayer->getContentSize().height*0.6/back->getContentSize().height);
    
    Sprite* back_sprite = Sprite::create("back.png");
    back_sprite->setPosition(back->getContentSize()/2);
    back->addChild(back_sprite);
    
    Menu* lMenu = Menu::create(back, NULL);
    lMenu->setPosition(Vec2::ZERO);
    topLayer->addChild(lMenu, 10);
    
    
    
    return true;
}

void ComicScene::onEnterTransitionDidFinish()
{
    Scene::onEnterTransitionDidFinish();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
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

    float offsetY = xCartoon->getCatagoryOffset(stoi(xCartoon->getCurrentCategory().id));
    if (offsetY > 0)
    {
        table->setContentOffset(Vec2(table->getContentOffset().x, table->minContainerOffset().y));
    }else
    {
        table->setContentOffset(Vec2(table->getContentOffset().x, offsetY));
    }
    
    this->addChild(table);
}

void ComicScene::onButton(Ref* ref)
{
    MenuItemImage* lMenuItem = (MenuItemImage*)ref;
    switch (lMenuItem->getTag()) {
        case st_button_back:
        {
            Director::getInstance()->replaceScene(TransitionSlideInL::create(0.2f, HomeScene::create()));
        }
            break;
            
        default:
            break;
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

void ComicScene::responseSpriteClick(Ref* ref)
{
    CoverSprite* lSprite = (CoverSprite*)ref;
    int index = lSprite->getTag();
    xCartoon->setCurrentCartoon(index);
    
    ShowCartoonInfoLayer* layer = ShowCartoonInfoLayer::create(xCartoon->getCurrentCategory()._cartoonVec.at(index));
    this->addChild(layer, 100);
    _cartoonLayer = layer;
}

void ComicScene::tableCellTouched(TableView* table, TableViewCell* cell)
{

}

Size ComicScene::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    float height = ((this->getContentSize().width - (col + 1)*SPACE)/col)*608./346 + SPACE;
    if (idx == this->numberOfCellsInTableView(nullptr) -1)
    {
        height += 120;
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
        if (index >= xCartoon->getCurrentCategory()._cartoonVec.size())
        {
            break;
        }
        
        float width = (this->getContentSize().width - (col+1)*SPACE)/col;
        CartoonInfo info = xCartoon->getCurrentCategory()._cartoonVec.at(index);
        
        CoverSprite* lSprite = CoverSprite::create(info.folder, info.coverUrl, Size(0, 0));
        lSprite->setTag(index);
        lSprite->diaplay();
        lSprite->setAnchorPoint(Vec2(0.5, 0));
        lSprite->setScale(width/lSprite->getContentSize().width);
        lSprite->setPosition(Vec2((i + 1)*SPACE + (i + 0.5)*width, 0));
        lSprite->addListener(this, callfuncO_selector(ComicScene::responseSpriteClick));
        
        Label* name = Label::createWithTTF(info.name, "fonts/d2.ttf", 40);
        name->setPosition(Vec2(lSprite->getContentSize().width/2, 52));
        name->setColor(Color3B(43, 43, 43));
        lSprite->addChild(name, 11);
        
        if (name->getContentSize().width > (width + 20))
        {
            name->setScale((width + 20)/name->getContentSize().width);
        }

        if (idx == this->numberOfCellsInTableView(NULL)-1)
        {
            lSprite->setPosition(Vec2((i + 1)*SPACE + (i + 0.5)*width, 100));
        }

        cell->addChild(lSprite);
        
    }
    
    return cell;
}

ssize_t ComicScene::numberOfCellsInTableView(TableView *table)
{
    int length = (int)xCartoon->getCurrentCategory()._cartoonVec.size();
    if (length%col == 0)
    {
        length /= col;
    }else
    {
        length = length/col + 1;
    }
    
    return length;
}