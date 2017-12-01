//
//  HomeScene.cpp
//  ComicBook
//
//  Created by houhuang on 17/12/1.
//
//

#include "HomeScene.hpp"
#include "CartoonManager.hpp"
#include "CoverSprite.hpp"

bool HomeScene::init()
{
    if (!Scene::init()) return false;
    
    LayerColor* layer = LayerColor::create(Color4B::WHITE);
    this->addChild(layer);

    xCartoon->readCartoonCsv();
    
    this->createTable();
    return true;
}

void HomeScene::createTable()
{
    TableView* table = TableView::create(this, Size(this->getContentSize().width, this->getContentSize().height));
    table->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    table->setVerticalFillOrder(cocos2d::extension::TableView::VerticalFillOrder::TOP_DOWN);
    
    table->setDelegate(this);
    table->ignoreAnchorPointForPosition(false);
    table->setAnchorPoint(Vec2(0.5, 0.5));
    table->setPosition(this->getContentSize()/2);
    table->reloadData();
    
    this->addChild(table);
}

void HomeScene::tableCellTouched(TableView* table, TableViewCell* cell)
{
    
}

Size HomeScene::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(this->getContentSize().width, 300);
}

TableViewCell* HomeScene::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell* cell = table->dequeueCell();
    if (!cell)
    {
        cell = new TableViewCell();
        cell->autorelease();
    }
    
    cell->removeAllChildrenWithCleanup(true);
    
    CartoonInfo info = xCartoon->getCartoonInfo().at(idx);
    CoverSprite* lSprite = CoverSprite::create(info.folder, info.coverUrl, Size(200, 300));
    lSprite->setPosition(Vec2(this->getContentSize().width/2, 150));
    lSprite->diaplay();
    cell->addChild(lSprite);
    
    return cell;
}

ssize_t HomeScene::numberOfCellsInTableView(TableView *table)
{
    return xCartoon->getCartoonInfo().size();
}










