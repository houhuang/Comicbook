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
#include "NewDialog.h"
#include "ReadScene.h"
#include "EventSprite.hpp"

#define top_bar     228
#define SPACE       40
#define col         (V::isIpad()? 3:2)



HomeScene::HomeScene()
{
    topBar_realHeight = 0.0f;
}

bool HomeScene::init()
{
    if (!Scene::init()) return false;
    
    setName("HomeScene");
    
    LayerColor* layer = LayerColor::create(Color4B(245, 245, 245, 255));
    this->addChild(layer);
    
    Sprite* topBanner = Sprite::create("title.png");
    topBanner->setAnchorPoint(Vec2(0.5, 1));
    topBanner->setScale(this->getContentSize().width/topBanner->getContentSize().width);
    topBanner->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height));
    this->addChild(topBanner);
    topBar_realHeight = topBanner->getBoundingBox().size.height;
    
    createTable();
    checkProgress();
    return true;
}

void HomeScene::checkProgress()
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
        
        NewDialog* lDialog = NewDialog::create("继续阅读？", "否", "是");
        lDialog->addButtonListener(CC_CALLBACK_1(HomeScene::onDialog, this));
        this->addChild(lDialog, 101);
    }
}

void HomeScene::createTable()
{
    TableView* table = TableView::create(this, Size(this->getContentSize().width, this->getContentSize().height - topBar_realHeight));
    table->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    table->setVerticalFillOrder(cocos2d::extension::TableView::VerticalFillOrder::TOP_DOWN);
    
    table->setDelegate(this);
    table->ignoreAnchorPointForPosition(false);
    table->setAnchorPoint(Vec2(0.5, 1));
    table->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height - topBar_realHeight));
    table->reloadData();
    
    this->addChild(table);
}

void HomeScene::onDialog(const string& name)
{
    if (name == "right")
    {
        xCartoon->setCurrentFolder(xCartoon->getCurrentReadingCartoon().folder);
        xCartoon->readCurrentPictureCsv(xCartoon->getCurrentReadingCartoon().csvPath);
        
        Director::getInstance()->replaceScene(TransitionProgressInOut::create(0.2f, ReadScene::create(stoi(xCartoon->getCurrentReadingCartoon().pageNumber), "HomeScene")));
        
        
        xCartoon->setCurrentFolder("");
    }
}

void HomeScene::responseCoverSprite(Node* node)
{
    EventSprite* lSprite = (EventSprite*)node;
    if (lSprite)
    {
        xCartoon->setCurrentCategory(lSprite->getTag());
        Director::getInstance()->replaceScene(TransitionSlideInR::create(0.2f, ComicScene::create()));
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
        height += 100;
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
    
    cell->removeAllChildrenWithCleanup(true);
    
    float width = (this->getContentSize().width - (col+1)*SPACE)/col;
    
    
    for (int i = 0; i < col; ++i)
    {
        int index = (int)idx*col + i;
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
            lBg->setPosition(Vec2((i + 1)*SPACE + (i + 0.5)*width, 100));
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
    
    return length;
}










