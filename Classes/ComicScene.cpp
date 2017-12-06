//
//  ComicScene.cpp
//  ComicBook
//
//  Created by houhuang on 17/12/4.
//
//

#include "ComicScene.hpp"
#include "STVisibleRect.h"
#include "CartoonManager.hpp"
#include "CoverSprite.hpp"


#define TOP_HEIGHT              (V::isIpad()? 55 : 86)
#define col                     (V::isIpad()? 3 : 2)
#define SPACE                   40

ComicScene::ComicScene()
{
    _cartoonLayer = nullptr;
}

bool ComicScene::init()
{
    if (!Scene::init())  return false;
    
    LayerColor* layer = LayerColor::create(Color4B(245, 245, 245, 255));
    this->addChild(layer);
    
    Sprite* topBar = Sprite::create("top_bar.png");
    topBar->setScaleX((this->getContentSize().width+10)/topBar->getContentSize().width);
    topBar->setScaleY((TOP_HEIGHT+10)/topBar->getContentSize().height);
    topBar->setAnchorPoint(Vec2(0.5, 1));
    topBar->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height + 5));
    this->addChild(topBar);
    
    Label* title = Label::createWithTTF(xCartoon->getCurrentCategory().name, "fonts/font1.ttf", 50);
    title->setPosition(Vec2(topBar->getContentSize().width/2, topBar->getContentSize().height/2));
    topBar->addChild(title);

    auto removeCartoonEvent = EventListenerCustom::create("remove_cartoonLayer", CC_CALLBACK_1(ComicScene::responseRemoveCartoonLayer, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(removeCartoonEvent, this);
    this->createTableView();
    
    return true;
}

void ComicScene::createTableView()
{
    TableView* table = TableView::create(this, Size(this->getContentSize().width, this->getContentSize().height - TOP_HEIGHT));
    table->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    table->setVerticalFillOrder(cocos2d::extension::TableView::VerticalFillOrder::TOP_DOWN);
    
    table->setDelegate(this);
    table->ignoreAnchorPointForPosition(false);
    table->setAnchorPoint(Vec2(0.5, 1));
    table->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height - TOP_HEIGHT));
    table->reloadData();

    this->addChild(table);
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
    this->addChild(layer);
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
        height += 100;
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
        
        Label* name = Label::createWithSystemFont(info.name, "Arial", 40);
        name->setPosition(Vec2(lSprite->getContentSize().width/2, 52));
        name->setColor(Color3B(43, 43, 43));
        lSprite->addChild(name, 11);
        
        
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