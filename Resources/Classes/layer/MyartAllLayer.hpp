//
//  MyartAllLayer.hpp
//  ColorGarden
//
//  Created by houhuang on 17/10/31.
//
//

#ifndef MyartAllLayer_hpp
#define MyartAllLayer_hpp

#include <stdio.h>
#include "UITools.h"
#include "extensions/cocos-ext.h"
#include "STTableView.h"
using namespace cocos2d::extension;
using namespace std;

class MyartAllLayer : public LayerColor, public STTableViewDelegate, public TableViewDataSource
{
public:
    static MyartAllLayer* create(Size size);
    MyartAllLayer();
    ~MyartAllLayer();
    
    bool init(Size size);
    void onEnterTransitionDidFinish();
public:
    void createTableView();
    void tableScrollToTop();
public:
    void responseSprite(Node* node);
    
    void shieldEvent(bool isShield);
    void updateTableView(EventCustom* event);
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
};

#endif /* MyartAllLayer_hpp */
