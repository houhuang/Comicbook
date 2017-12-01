//
//  InspirationLikeLayer.cpp
//  ColorGarden
//
//  Created by houhuang on 17/10/26.
//
//

#include "InspirationLikeLayer.hpp"
#include "DownLoadPage.h"
#include "ShowInspirationLayer.hpp"
#include "AppConfigs.h"
#include "STVisibleRect.h"
#include "ShowLargeImageLayer.hpp"

#define col     (V::isIpad()? 3:2)

InspirationLikeLayer* InspirationLikeLayer::create(Size size)
{
    InspirationLikeLayer* layer = new InspirationLikeLayer();
    if (layer && layer->init(size))
    {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return nullptr;
}

InspirationLikeLayer::~InspirationLikeLayer()
{

}

InspirationLikeLayer::InspirationLikeLayer()
{
    _table = nullptr;
    _isEnableTouch = true;
}

bool InspirationLikeLayer::init(Size size)
{
    if (!LayerColor::initWithColor(Color4B(100, 0, 0, 0), size.width, size.height)) return false;
    
    this->createTableView();
    
    auto updateTableViewEvent = EventListenerCustom::create("cancel_collection", CC_CALLBACK_1(InspirationLikeLayer::reponseUpdateTable, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(updateTableViewEvent, this);
    
    return true;
}

void InspirationLikeLayer::createTableView()
{
    if (_table)
    {
        _table->removeFromParentAndCleanup(true);
        _table = nullptr;
    }
    
    STTableView* table = STTableView::create(this, this->getContentSize());
    table->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    table->setVerticalFillOrder(cocos2d::extension::TableView::VerticalFillOrder::TOP_DOWN);
    table->setDelegate(this);
    table->setOffsetY(this->tableCellSizeForIndex(nullptr, 0).height);
    table->reloadData();
    _table = table;
    
    table->ignoreAnchorPointForPosition(false);
    table->setAnchorPoint(Vec2(0.5, 0.5));
    table->setPosition(this->getContentSize()/2);
    this->addChild(table);
}

void InspirationLikeLayer::reponseServerSprite(Node* ref)
{
    ServerSprite* lSprite = dynamic_cast<ServerSprite*>(ref);
    if (lSprite)
    {
//        ShowInspirationLayer* layer = ShowInspirationLayer::create(lSprite->getImageID(), lSprite->getLikeNumber(), lSprite->getIsMylikeSprite());
//        this->getParent()->addChild(layer, 100);
        
        float width = V::isIpad() ? this->getContentSize().width - 2*93 : this->getContentSize().width - 2*30;
        ShowLargeImageLayer* layer = ShowLargeImageLayer::create(lSprite->getImageID(), width);
        Director::getInstance()->getRunningScene()->addChild(layer, 100);
    }
}

void InspirationLikeLayer::reponseUpdateTable(EventCustom* event)
{
    float offset = _table->getContentOffset().y;
    
    _table->reloadData();

    if (this->numberOfCellsInTableView(nullptr) <=3)
    {
        return;
    }
    
    if (offset < _table->minContainerOffset().y)
    {
        _table->setContentOffset(_table->minContainerOffset());
    }else if (offset > _table->maxContainerOffset().y)
    {
        _table->setContentOffset(_table->maxContainerOffset());
    }else
    {
        _table->setContentOffset(Vec2( _table->getContentOffset().x, offset));
    }
}

Size InspirationLikeLayer::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    Size size = Size(this->getContentSize().width, (this->getContentSize().width-(col+1)*SPACE)/col + SPACE);
    if (idx == this->numberOfCellsInTableView(NULL)-1)
    {
        size.height += SPACE;
    }
    return size;
}

TableViewCell* InspirationLikeLayer::tableCellAtIndex(TableView* table, ssize_t idex)
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
        int index = DownLoadPage::getInstance()->m_vAlreadyClickLick.size() - col*idex - i - 1;
        if (index < 0)
        {
            break;
        }
        
        int likeImageName = DownLoadPage::getInstance()->m_vAlreadyClickLick.at(index);
        
        ServerSprite* lSprite = ServerSprite::create(likeImageName);
        lSprite->display();
        lSprite->setPosition(Vec2((i+0.5)*width + (i+1)*SPACE, width/2));
        lSprite->setScale(width/lSprite->getContentSize().width);
        lSprite->setName("name" + to_string(i));
        cell->addChild(lSprite);
        lSprite->addListener(this, callfuncN_selector(InspirationLikeLayer::reponseServerSprite));
        if (idex == this->numberOfCellsInTableView(NULL)-1)
        {
            lSprite->setPosition(Vec2((i+0.5)*width + (i+1)*SPACE, width/2 + SPACE));
        }
        
    }
    
    return cell;
}

ssize_t InspirationLikeLayer::numberOfCellsInTableView(TableView* table)
{
    int length = (int)DownLoadPage::getInstance()->m_vAlreadyClickLick.size();
    if (length%col == 0)
    {
        return length/col;
    }else
    {
        return (int)length/col + 1;
    }
}

void InspirationLikeLayer::tableCellTouched(TableView* table, TableViewCell* cell)
{

}

void InspirationLikeLayer::tableCellTouched(TableView* table, TableViewCell* cell,Touch* pTouch)
{

}

void InspirationLikeLayer::tableCellRemoveTexture(TableView* table, TableViewCell* cell)
{
    ServerSprite* lSprite =dynamic_cast<ServerSprite*>(cell->getChildByName("name0"));
    if (lSprite)
    {
        lSprite->removeTextureCache();
    }
    
    ServerSprite* rSprite = dynamic_cast<ServerSprite*>(cell->getChildByName("name1"));
    if (rSprite)
    {
        rSprite->removeTextureCache();
    }
    
    ServerSprite* rSprite3 = dynamic_cast<ServerSprite*>(cell->getChildByName("name2"));
    if (rSprite3)
    {
        rSprite3->removeTextureCache();
    }
}

void InspirationLikeLayer::tableRequestNextPage(TableView* table,ssize_t idx)
{

}

ssize_t InspirationLikeLayer::tableLastID(TableView* table)
{

}

void InspirationLikeLayer::tableScrollToTop()
{
    if (_table)
    {
        float time = 0;
        float offset = abs(_table->minContainerOffset().y - _table->getContentOffset().y);
        if (offset > 1000)
        {
            time = 0.3f;
        }else
        {
            time = offset/1000.0*0.3;
        }
        _table->setContentOffsetInDuration(_table->minContainerOffset(), time);
    }
}

void InspirationLikeLayer::shieldEvent(bool isShield)
{
    if (_table)
    {
        if (_isEnableTouch^isShield)
        {
            _isEnableTouch = isShield;
            _table->setTouchEnabled(isShield);
        }
    }
}
