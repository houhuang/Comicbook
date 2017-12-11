//
//  MyartPublishedLayer.cpp
//  ColorGarden
//
//  Created by houhuang on 17/10/31.
//
//

#include "MyartPublishedLayer.hpp"
#include "MyArtDataManager.hpp"
#include "MyArtSprite.hpp"
#include "AppConfigs.h"
#include "STVisibleRect.h"
#include "AnalyticsManager.hpp"

#define col (V::isIpad()? 3 : 2)

MyartPublishedLayer* MyartPublishedLayer::create(Size size)
{
    MyartPublishedLayer* layer = new MyartPublishedLayer();
    if (layer && layer->init(size))
    {
        layer->autorelease();
        return layer;
    }
    
    CC_SAFE_DELETE(layer);
    return nullptr;
}

MyartPublishedLayer::MyartPublishedLayer()
{
    _isEnableTouch = true;
    _deleteLayer = nullptr;
}

MyartPublishedLayer::~MyartPublishedLayer()
{
    
}

bool MyartPublishedLayer::init(Size size)
{
    if (!LayerColor::initWithColor(Color4B(100, 0, 0, 0), size.width, size.height))
        return false;
    
    auto deleteNo = EventListenerCustom::create("delete_button_no", CC_CALLBACK_1(MyartPublishedLayer::responseDeleteLayerNo, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(deleteNo, this);
    
    auto deleteYes = EventListenerCustom::create("delete_button_yes", CC_CALLBACK_1(MyartPublishedLayer::responseDeleteLayerYes, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(deleteYes, this);
    
    auto removeDeleteLayer = EventListenerCustom::create("remove_deletelayer", CC_CALLBACK_1(MyartPublishedLayer::responseDeleteLayerNo, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(removeDeleteLayer, this);
    
    return true;
}

void MyartPublishedLayer::onEnterTransitionDidFinish()
{
    LayerColor::onEnterTransitionDidFinish();
    
    
    this->createTableView();
}

void MyartPublishedLayer::createTableView()
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

void MyartPublishedLayer::responseSprite(Node* node)
{
    MyArtSprite* lSprite = (MyArtSprite*)node;
    
    if (_deleteLayer)
    {
        _deleteLayer->removeFromParentAndCleanup(true);
        _deleteLayer = nullptr;
    }
    
    DeleteLayer* layer = DeleteLayer::create();
    layer->setTag(67);
    lSprite->addChild(layer, 99);
    _deleteLayer = layer;
    
}

void MyartPublishedLayer::updateTableView()
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

void MyartPublishedLayer::responseDeleteLayerNo(EventCustom* event)
{
    if (_deleteLayer)
    {
        _deleteLayer->removeFromParentAndCleanup(true);
        _deleteLayer = nullptr;
    }
}

void MyartPublishedLayer::responseDeleteLayerYes(EventCustom* event)
{
    if (_deleteLayer)
    {
        MyArtSprite* lSprite = dynamic_cast<MyArtSprite*>(_deleteLayer->getParent());
        if (lSprite)
        {
            string name = lSprite->getPreName();
            lSprite->runAction(Sequence::create(EaseSineOut::create(ScaleTo::create(0.5, 0)), CallFunc::create([this, name](){

                for (int i = 0; i < xMyArtData->m_vMyArtPostDatas.size(); ++i)
                {
                    if (xMyArtData->m_vMyArtPostDatas.at(i).previewName == name)
                    {
                        std::vector<MyArtData>::iterator it = xMyArtData->m_vMyArtPostDatas.begin() + i;
                        xMyArtData->m_vMyArtPostDatas.erase(it);
                        break;
                    }
                }
                
                _deleteLayer->removeFromParentAndCleanup(true);
                _deleteLayer = nullptr;
                this->updateTableView();
                
            }), NULL));
            
            string event = lSprite->getSpriteName().substr(0, lSprite->getSpriteName().find(".png"));
            
            map<string, string> map;
            map.insert(pair<string, string>("image_id", event));
            ATMInstance->sendEvent("image_remove", map);
        }
    }
}

Size MyartPublishedLayer::tableCellSizeForIndex(TableView *table, ssize_t idx)
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

TableViewCell* MyartPublishedLayer::tableCellAtIndex(TableView* table, ssize_t idex)
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
        int index = (int)(xMyArtData->m_vMyArtPostDatas.size() - col*idex - 1 - i);
        if (index < 0)
        {
            break;
        }

        string preName = xMyArtData->m_vMyArtPostDatas.at(index).previewName;
        int preIndex = (int)preName.find("preview.png");
        string fileName = preName.substr(0, preIndex) + "save.png";
        
        MyArtSprite* lSprite = MyArtSprite::create(xMyArtData->m_vMyArtPostDatas.at(index).name, Color3B(0, 0, 0), xMyArtData->m_vMyArtPostDatas.at(index).serveId, true);
        lSprite->setLoadImageName(preName);
        lSprite->setPreName(preName);
        lSprite->setPosition(Vec2(width*(i+0.5) + SPACE*(i+1), width/2));
        lSprite->setScale(width/lSprite->getContentSize().width);
        lSprite->display();
        lSprite->setName("name"+to_string(i));
        cell->addChild(lSprite);
        lSprite->addTouchListener(this, callfuncN_selector(MyartPublishedLayer::responseSprite), false);
        if (idex == this->numberOfCellsInTableView(NULL) - 1)
        {
            lSprite->setPositionY(SPACE + width/2);
        }
    }
    
    return cell;
}

ssize_t MyartPublishedLayer::numberOfCellsInTableView(TableView* table)
{
    int length = (int)xMyArtData->m_vMyArtPostDatas.size();
    if (length%col == 0)
    {
        length /= col;
    }else
    {
        length = (int)(length/col) + 1;
    }
    
    return length;
}

void MyartPublishedLayer::tableCellTouched(TableView* table, TableViewCell* cell)
{
    
}

void MyartPublishedLayer::tableCellTouched(TableView* table, TableViewCell* cell,Touch* pTouch)
{
    
}

void MyartPublishedLayer::tableCellRemoveTexture(TableView* table, TableViewCell* cell)
{
    MyArtSprite* lSprite =dynamic_cast<MyArtSprite*>(cell->getChildByName("name0"));
    if (lSprite)
    {
        DeleteLayer* layer = dynamic_cast<DeleteLayer*>(lSprite->getChildByTag(67));
        if (layer)
        {
            layer->removeFromParentAndCleanup(true);
            _deleteLayer = nullptr;
        }
        lSprite->removeTexture();
    }
    
    MyArtSprite* rSprite = dynamic_cast<MyArtSprite*>(cell->getChildByName("name1"));
    if (rSprite)
    {
        DeleteLayer* layer = dynamic_cast<DeleteLayer*>(lSprite->getChildByTag(67));
        if (layer)
        {
            layer->removeFromParentAndCleanup(true);
            _deleteLayer = nullptr;
        }
        rSprite->removeTexture();
    }
    
    MyArtSprite* rSprite3 = dynamic_cast<MyArtSprite*>(cell->getChildByName("name2"));
    if (rSprite3)
    {
        DeleteLayer* layer = dynamic_cast<DeleteLayer*>(lSprite->getChildByTag(67));
        if (layer)
        {
            layer->removeFromParentAndCleanup(true);
            _deleteLayer = nullptr;
        }
        rSprite3->removeTexture();
    }
}

void MyartPublishedLayer::tableRequestNextPage(TableView* table,ssize_t idx)
{
    
}

ssize_t MyartPublishedLayer::tableLastID(TableView* table)
{
    
}

void MyartPublishedLayer::shieldEvent(bool isShield)
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

void MyartPublishedLayer::tableScrollToTop()
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