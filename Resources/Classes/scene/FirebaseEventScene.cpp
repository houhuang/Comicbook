//
//  FirebaseEventScene.cpp
//  ColorGarden
//
//  Created by houhuang on 17/9/18.
//
//

#include "FirebaseEventScene.hpp"
#include "AnalyticsManager.hpp"
#include "ChapterScene.h"
#include "InspirationRecentLayer.hpp"
#include "InspirationLikeLayer.hpp"
#include "MyartAllLayer.hpp"
#include "MyartPublishedLayer.hpp"

bool FirebaseEventScene::init()
{
    if (!Scene::init()) return false;
    
    LayerColor* layer = LayerColor::create(Color4B(248, 248, 248, 255));
    this->addChild(layer);
    
    MenuItemImage* btn = MenuItemImage::create("other/graph_lock.png", "other/graph_lock.png", CC_CALLBACK_1(FirebaseEventScene::ontMenuButton, this));
    btn->setPosition(Vec2(this->getContentSize().width * 0.1, this->getContentSize().height * 0.95));
    
    Menu* menu = Menu::create(btn, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 105);
    
    this->initTableView();
//    MyartPublishedLayer* insLayer = MyartPublishedLayer::create(Size(this->getContentSize().width, 600));
//    insLayer->ignoreAnchorPointForPosition(false);
//    insLayer->setAnchorPoint(Vec2(0.5, 0.5));
//    insLayer->setPosition(this->getContentSize()/2);
//    this->addChild(insLayer, 101);
    
    return true;
}

void FirebaseEventScene::initTableView()
{
    TableView* table = TableView::create(this, this->getContentSize());
    table->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    table->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    this->addChild(table);
    table->ignoreAnchorPointForPosition(false);
    table->setAnchorPoint(Vec2(0.5, 0.5));
    table->setPosition(this->getContentSize()/2);
    
    table->setDelegate(this);
    table->reloadData();
}

void FirebaseEventScene::ontMenuButton(Ref* ref)
{
    Director::getInstance()->replaceScene(ChapterScene::create());
}

void FirebaseEventScene::tableCellTouched(TableView* table, TableViewCell* cell)
{

}

TableViewCell* FirebaseEventScene::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell* cell = table->dequeueCell();
    if (!cell)
    {
        cell = new TableViewCell();
        Label* label = Label::createWithTTF(ATMInstance->_eventVec.at(ATMInstance->_eventVec.size() - idx - 1), "res/kalinga.ttf", 20 * 2);
        label->setPosition(this->tableCellSizeForIndex(NULL, 0)/2);
        label->setAlignment(cocos2d::TextHAlignment::LEFT);
        cell->addChild(label);
        label->setColor(Color3B::BLACK);
        label->setScale(0.5);
        label->setTag(10);
    }
    
    Label* label = (Label*)cell->getChildByTag(10);
    label->setString(ATMInstance->_eventVec.at(ATMInstance->_eventVec.size() - idx - 1));

    
    return cell;
}

ssize_t FirebaseEventScene::numberOfCellsInTableView(TableView *table)
{
    return ATMInstance->_eventVec.size();
}

Size FirebaseEventScene::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(this->getContentSize().width, 50);
}
