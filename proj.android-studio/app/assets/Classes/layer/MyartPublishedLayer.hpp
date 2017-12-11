//
//  MyartPublishedLayer.hpp
//  ColorGarden
//
//  Created by houhuang on 17/10/31.
//
//

#ifndef MyartPublishedLayer_hpp
#define MyartPublishedLayer_hpp

#include <stdio.h>
#include "UITools.h"
#include "extensions/cocos-ext.h"
#include "STTableView.h"
#include "DeleteLayer.hpp"
using namespace cocos2d::extension;
using namespace std;

class MyartPublishedLayer : public LayerColor, public STTableViewDelegate, public TableViewDataSource
{
public:
    static MyartPublishedLayer* create(Size size);
    MyartPublishedLayer();
    ~MyartPublishedLayer();
    
    bool init(Size size);
    void onEnterTransitionDidFinish();
public:
    void createTableView();
    void tableScrollToTop();
public:
    void responseSprite(Node* node);
    
    void shieldEvent(bool isShield);
    
    void responseDeleteLayerNo(EventCustom* event);
    void responseDeleteLayerYes(EventCustom* event);
    
    void updateTableView();
public:
    Size tableCellSizeForIndex(TableView *table, ssize_t idx);
    TableViewCell* tableCellAtIndex(TableView* table, ssize_t idex);
    ssize_t numberOfCellsInTableView(TableView* table);
    void tableCellTouched(TableView* table, TableViewCell* cell);
    
    void tableCellTouched(TableView* table, TableViewCell* cell,Touch* pTouch);
    void tableCellRemoveTexture(TableView* table, TableViewCell* cell);
    void tableRequestNextPage(TableView* table,ssize_t idx);
    ssize_t tableLastID(TableView* table);
    
private:
    STTableView*    _tableView;
    bool    _isEnableTouch;
    DeleteLayer*    _deleteLayer;
};

#endif /* MyartPublishedLayer_hpp */
