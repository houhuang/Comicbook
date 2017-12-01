//
//  MyartAllLayer.cpp
//  ColorGarden
//
//  Created by houhuang on 17/10/31.
//
//

#include "MyartAllLayer.hpp"
#include "MyArtDataManager.hpp"
#include "MyArtSprite.hpp"
#include "AppConfigs.h"
#include "STVisibleRect.h"
#include "MyartSpriteLayer.hpp"

#define col (V::isIpad()? 3 : 2)

MyartAllLayer* MyartAllLayer::create(Size size)
{
    MyartAllLayer* layer = new MyartAllLayer();
    if (layer && layer->init(size))
    {
        layer->autorelease();
        return layer;
    }
    
    CC_SAFE_DELETE(layer);
    return nullptr;
}

MyartAllLayer::MyartAllLayer()
{
    _isEnableTouch = true;
}

MyartAllLayer::~MyartAllLayer()
{

}

bool MyartAllLayer::init(Size size)
{
    if (!LayerColor::initWithColor(Color4B(0, 100, 0, 0), size.width, size.height))
        return false;
    
    auto updateTable = EventListenerCustom::create("updateTalbe_myartall", CC_CALLBACK_1(MyartAllLayer::updateTableView, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(updateTable, this);
    
    return true;
}

void MyartAllLayer::onEnterTransitionDidFinish()
{
    LayerColor::onEnterTransitionDidFinish();
    
    
    this->createTableView();
}

void MyartAllLayer::createTableView()
{
    STTableView* table = STTableView::create(this, this->getContentSize());
    table->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    table->setVerticalFillOrder(cocos2d::extension::TableView::VerticalFillOrder::TOP_DOWN);
    table->setDelegate(this);
    table->reloadData();
    
    table->ignoreAnchorPointForPosition(false);
    table->setAnchorPoint(Vec2(0.5, 0.5));
    table->setPosition(this->getContentSize()/2);
    this->addChild(table);
    _tableView = table;
}

void MyartAllLayer::updateTableView(EventCustom* event)
{
    if (_tableView)
    {
        float offset = _tableView->getContentOffset().y;
        
        _tableView->reloadData();
        
        if (this->numberOfCellsInTableView(nullptr) <=3)
        {
            return;
        }
        
        if (offset < _tableView->minContainerOffset().y)
        {
            _tableView->setContentOffset(_tableView->minContainerOffset());
        }else if (offset > _tableView->maxContainerOffset().y)
        {
            _tableView->setContentOffset(_tableView->maxContainerOffset());
        }else
        {
            _tableView->setContentOffset(Vec2( _tableView->getContentOffset().x, offset));
        }
    }
}

void MyartAllLayer::responseSprite(Node* node)
{
    MyArtSprite* lSprite = (MyArtSprite*)node;
    
      Vec2 lPos =lSprite->convertToWorldSpace(lSprite->getContentSize()/2);
    
//    Vec2 lPos = Director::getInstance()->convertToGL(lSprite->getPosition());
    
    MyartSpriteLayer* layer = MyartSpriteLayer::create(lSprite->getPreName(), lSprite->getSpriteName(), lPos);
    
    Director::getInstance()->getRunningScene()->addChild(layer, 100);
//    this->getParent()->addChild(layer, 100);
}

Size MyartAllLayer::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    float width = (this->getContentSize().width - (col+1)*SPACE)/col;
    float height;
    if (idx == this->numberOfCellsInTableView(NULL) - 1)
    {
        height = width + SPACE * 2;
    }else
    {
        height = width + SPACE;
    }
    
    return Size(this->getContentSize().width, height);
}

TableViewCell* MyartAllLayer::tableCellAtIndex(TableView* table, ssize_t idex)
{
    TableViewCell* cell = table->dequeueCell();
    if (!cell)
    {
        cell = new TableViewCell();
        cell->autorelease();
    }
    
    cell->removeAllChildrenWithCleanup(true);
    
    float width = (this->getContentSize().width-(col+1)*SPACE)/col;
    
    for (int i = 0; i < col; ++i)
    {
        int index = (int)(xMyArtData->m_vMyArtDatas.size() - col*idex - 1 - i);
        if (index < 0)
        {
            break;
        }
        
        string preName = xMyArtData->m_vMyArtDatas.at(index).previewName;
        int preIndex = (int)preName.find("preview.png");
        string fileName = preName.substr(0, preIndex) + "save.png";
        
        MyArtSprite* lSprite = MyArtSprite::create(xMyArtData->m_vMyArtDatas.at(index).name, Color3B(0, 0, 0), xMyArtData->m_vMyArtDatas.at(index).serveId, false);
        lSprite->setLoadImageName(preName);
        lSprite->setPreName(preName);
        lSprite->setPosition(Vec2(width*(i+0.5) + SPACE*(i+1), width/2));
        lSprite->setScale(width/lSprite->getContentSize().width);
        lSprite->display();
        lSprite->setName("name"+to_string(i));
        cell->addChild(lSprite);
        lSprite->addTouchListener(this, callfuncN_selector(MyartAllLayer::responseSprite), false);
        if (idex == this->numberOfCellsInTableView(NULL) - 1)
        {
            lSprite->setPositionY(SPACE + width/2);
        }
    }
    
    return cell;
}

ssize_t MyartAllLayer::numberOfCellsInTableView(TableView* table)
{
    int length = (int)xMyArtData->m_vMyArtDatas.size();
    if (length%col == 0)
    {
        length /= col;
    }else
    {
        length = (int)(length/col) + 1;
    }
    
    return length;
}

void MyartAllLayer::tableCellTouched(TableView* table, TableViewCell* cell)
{

}

void MyartAllLayer::tableCellTouched(TableView* table, TableViewCell* cell,Touch* pTouch)
{

}

void MyartAllLayer::tableCellRemoveTexture(TableView* table, TableViewCell* cell)
{
    MyArtSprite* lSprite =dynamic_cast<MyArtSprite*>(cell->getChildByName("name0"));
    if (lSprite)
    {
        lSprite->removeTexture();
    }
    
    MyArtSprite* rSprite = dynamic_cast<MyArtSprite*>(cell->getChildByName("name1"));
    if (rSprite)
    {
        rSprite->removeTexture();
    }
    
    MyArtSprite* rSprite3 = dynamic_cast<MyArtSprite*>(cell->getChildByName("name2"));
    if (rSprite3)
    {
        rSprite3->removeTexture();
    }
    
    cell->removeFromParentAndCleanup(true);
}

void MyartAllLayer::tableRequestNextPage(TableView* table,ssize_t idx)
{

}

ssize_t MyartAllLayer::tableLastID(TableView* table)
{

}

void MyartAllLayer::shieldEvent(bool isShield)
{
    if (_tableView)
    {
        if (_isEnableTouch^isShield)
        {
            _isEnableTouch = isShield;
            _tableView->setTouchEnabled(isShield);
        }
    }
}

void MyartAllLayer::tableScrollToTop()
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
    }
}