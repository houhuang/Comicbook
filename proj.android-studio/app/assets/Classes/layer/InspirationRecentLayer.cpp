//
//  InspirationRecentLayer.cpp
//  ColorGarden
//
//  Created by houhuang on 17/10/26.
//
//

#include "InspirationRecentLayer.hpp"
#include "SSCInternalLibManager.h"
#include "NewDialog.hpp"
#include "SceneManager.h"
#include "DownLoadPage.h"
#include "ShowInspirationLayer.hpp"
#include "AppConfigs.h"
#include "STVisibleRect.h"
#include "ShowLargeImageLayer.hpp"

#define col (V::isIpad()? 3 : 2)

InspirationRecentLayer* InspirationRecentLayer::create(Size size)
{
    InspirationRecentLayer* layer = new InspirationRecentLayer();
    if (layer && layer->init(size))
    {
        layer->autorelease();
        return layer;
    }
    
    CC_SAFE_DELETE(layer);
    return nullptr;
}

InspirationRecentLayer::~InspirationRecentLayer()
{
    NotificationCenter::getInstance()->removeObserver(this, st_inspirationLayer_downLoadJson_fai);
    NotificationCenter::getInstance()->removeObserver(this, st_inspirationLayer_firstDownLoadJson_suc);
    NotificationCenter::getInstance()->removeObserver(this, st_inspirationLayer_downLoadJson_suc);
    
}

InspirationRecentLayer::InspirationRecentLayer()
{
    _swallowLayer = nullptr;
    _tableView = nullptr;
    _rotateSprite = nullptr;
    _updateRotate = nullptr;
    
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(InspirationRecentLayer::waitDownloadJsonSuc), st_inspirationLayer_firstDownLoadJson_suc, NULL);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(InspirationRecentLayer::waitDownloadJsonFai), st_inspirationLayer_downLoadJson_fai, NULL);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(InspirationRecentLayer::updateContentSize), st_inspirationLayer_downLoadJson_suc, NULL);
    
    _isEnableTouch = true;
}

bool InspirationRecentLayer::init(Size size)
{
    if (!LayerColor::initWithColor(Color4B(0, 0, 100, 0), size.width, size.height)) return false;
    
    auto updateTableViewEvent = EventListenerCustom::create("update_recent", CC_CALLBACK_1(InspirationRecentLayer::reponseUpdateTable, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(updateTableViewEvent, this);
    
    Sprite* updateRorate = Sprite::create("cocos/other/load_rotate_icon.png");
    updateRorate->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height - 50));
    this->addChild(updateRorate, 10);
    updateRorate->setScale(0.5f);
    updateRorate->setVisible(false);
    updateRorate->runAction(RepeatForever::create(RotateBy::create(1.0f, 360)));
    _updateRotate = updateRorate;
    
    return true;
}

void InspirationRecentLayer::reponseUpdateTable(EventCustom* event)
{
    if (_tableView)
    {
        Vec2 vec = _tableView->getContentOffset();
        _tableView->reloadData();
        _tableView->setContentOffset(vec);
    }
}

void InspirationRecentLayer::onEnterTransitionDidFinish()
{
    this->firstRequestDatas();
}

void InspirationRecentLayer::scrollViewDidScroll(cocos2d::extension::ScrollView* view)
{
    if (_tableView)
    {
//        log("---%f",_tableView->getContentOffset().y);
        if (_tableView->getContentOffset().y < _tableView->minContainerOffset().y - 100)
        {
            _updateRotate->setVisible(true);
            _updateRotate->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height - abs(_tableView->getContentOffset().y - _tableView->minContainerOffset().y)/2));
            _updateRotate->pause();
        }else
        {
            _updateRotate->setVisible(false);
        }
    }
    
}

void InspirationRecentLayer::tableScrollToTop()
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

void InspirationRecentLayer::tableMoveEnded()
{
    if (_tableView && _tableView->getContentOffset().y < _tableView->minContainerOffset().y - 100)
    {
        DownLoadPage::getInstance()->firstDownLoadInspirationInfo();
        
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("createUpdateLayer");
        
        _updateRotate->resume();
        _tableView->springBack = false;
        _updateRotate->setVisible(true);
    }else
    {
        _tableView->springBack = true;
        _updateRotate->setVisible(false);
    }
}

void InspirationRecentLayer::firstRequestDatas()
{
    if (SSCInternalLibManager::getInstance()->checkNetworkAvailable())
    {
        DownLoadPage::getInstance()->firstDownLoadInspirationInfo();
        SwallowListenerLayer* layer = SwallowListenerLayer::create(Color4B(0, 0, 0, 0));
        layer->setPosition(this->getPosition()/2);
        this->addChild(layer,100);
        layer->setTouchEnable(false);
        _swallowLayer = layer;
        
        Sprite* rotateSprite = Sprite::create("cocos/other/load_rotate_icon.png");
        rotateSprite->setPosition(this->getContentSize()/2);
        this->addChild(rotateSprite, 10);
        rotateSprite->setScale(0.5f);
        rotateSprite->runAction(RepeatForever::create(RotateBy::create(1.0f, 360)));
        _rotateSprite = rotateSprite;
    }else
    {
//        NewDialog* dialog = NewDialog::create("network_err", "", "ok");
//        dialog->addButtonListener(CC_CALLBACK_1(InspirationRecentLayer::onDialog, this));
//        this->getParent()->addChild(dialog, 101);
    }
}

void InspirationRecentLayer::createTableView()
{
    if (_tableView) return;
    
    STTableView* table = STTableView::create(this, this->getContentSize());
    table->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    table->setVerticalFillOrder(cocos2d::extension::TableView::VerticalFillOrder::TOP_DOWN);
    table->setDelegate(this);
    table->setOffsetY(this->tableCellSizeForIndex(nullptr, 0).height);
    table->reloadData();
    _tableView = table;
    
    table->ignoreAnchorPointForPosition(false);
    table->setAnchorPoint(Vec2(0.5, 0.5));
    table->setPosition(this->getContentSize()/2);
    this->addChild(table);
}

void InspirationRecentLayer::waitDownloadJsonSuc(Ref* ref)
{
    if (_swallowLayer)
    {
        _swallowLayer->removeFromParentAndCleanup(true);
        _swallowLayer = NULL;
    }
    
    if (_rotateSprite)
    {
        _rotateSprite->removeFromParentAndCleanup(true);
        _rotateSprite = nullptr;
    }
    
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("removeUpdateLayer");
    
    if (_updateRotate)
    {
        _updateRotate->setVisible(false);
    }
    
    //    m_iImageNumber = (int)DownLoadPage::getInstance()->m_vInspirationInfo.size();
    //    this->initContentLayer();
    if (DownLoadPage::getInstance()->m_vInspirationInfo.size() > 0)
    {
        if (_tableView)
        {
            _tableView->reloadData();
        }else
        {
            this->createTableView();
        }
        
    }

}

void InspirationRecentLayer::waitDownloadJsonFai(Ref* ref)
{
    if (_swallowLayer)
    {
        _swallowLayer->removeFromParentAndCleanup(true);
        _swallowLayer = NULL;
    }
    
    if (_rotateSprite)
    {
        _rotateSprite->removeFromParentAndCleanup(true);
        _rotateSprite = nullptr;
    }
    
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("removeUpdateLayer");
    
    if (_updateRotate)
    {
        _updateRotate->setVisible(false);
    }
    
    if (_tableView)
    {
        _tableView->reloadData();
    }
    
//    NewDialog* dialog = NewDialog::create("post fail", "", "ok");
//    dialog->addButtonListener(CC_CALLBACK_1(InspirationRecentLayer::onDialog, this));
//    this->addChild(dialog, 101);
}

void InspirationRecentLayer::updateContentSize(Ref* ref)
{
    _tableView->updateContentSize();
}

void InspirationRecentLayer::onDialog(const string& name)
{
    if (name == "ok")
    {
        xScene->forward(SceneManager::Tag::CHAPTER);
    }
}

void InspirationRecentLayer::reponseServerSprite(Node* ref)
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

Size InspirationRecentLayer::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    Size size = Size(this->getContentSize().width, (this->getContentSize().width-(col+1)*SPACE)/col + SPACE);
    
    if (idx == this->numberOfCellsInTableView(NULL)-1)
    {
        size.height += SPACE;
    }
    
    return size;
}

int InspirationRecentLayer::getlookCount()
{
    return _lookCount;
}

TableViewCell* InspirationRecentLayer::tableCellAtIndex(TableView* table, ssize_t idex)
{
    TableViewCell* cell = table->dequeueCell();
    if (!cell)
    {
        cell = new TableViewCell();
        cell->autorelease();
    }
    
    cell->removeAllChildrenWithCleanup(true);
    
    float width = (this->getContentSize().width-(col + 1)*SPACE)/col;
    
    if (idex > _lookCount)
    {
        _lookCount = (int)idex;
    }
    
    for (int i = 0; i < col; ++i)
    {
        int index = col* idex + i;
        if (index >= DownLoadPage::getInstance()->m_vInspirationInfo.size())
        {
            break;
        }
        
        InspirationInfo lInsInfo;
        lInsInfo = DownLoadPage::getInstance()->m_vInspirationInfo.at(index);
        
        ServerSprite* lSprite = ServerSprite::create(lInsInfo.imageUrl, lInsInfo.ID, lInsInfo.likeNumber);
        lSprite->display();
        lSprite->setPosition(Vec2((i+0.5)*width + (i+1)*SPACE, width/2));
        lSprite->setScale(width/lSprite->getContentSize().width);
        lSprite->setName("name" + to_string(i));
        cell->addChild(lSprite);
        lSprite->addListener(this, callfuncN_selector(InspirationRecentLayer::reponseServerSprite));
        
        if (idex == this->numberOfCellsInTableView(NULL)-1)
        {
            lSprite->setPosition((i+0.5)*width + (i+1)*SPACE, width/2 + SPACE);
        }
    }
    return cell;
}

ssize_t InspirationRecentLayer::numberOfCellsInTableView(TableView* table)
{
    int length = (int)DownLoadPage::getInstance()->m_vInspirationInfo.size();
    if (length%col == 0)
    {
        return length/col;
    }else
    {
        return (int)length/col + 1;
    }
}

void InspirationRecentLayer::tableCellTouched(TableView* table, TableViewCell* cell)
{

}

void InspirationRecentLayer::tableCellTouched(TableView* table, TableViewCell* cell,Touch* pTouch)
{

}

void InspirationRecentLayer::tableCellRemoveTexture(TableView* table, TableViewCell* cell)
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
    
    cell->removeFromParentAndCleanup(true);
}

void InspirationRecentLayer::tableRequestNextPage(TableView* table,ssize_t idx)
{
    DownLoadPage::getInstance()->downLoadInspirationInfo(idx * (V::isIpad()? 3:2));
}

ssize_t InspirationRecentLayer::tableLastID(TableView* table)
{
    return DownLoadPage::getInstance()->m_vInspirationInfo.at(DownLoadPage::getInstance()->m_vInspirationInfo.size() - 1).ID;
}

void InspirationRecentLayer::shieldEvent(bool isShield)
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




