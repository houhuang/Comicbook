//
//  InspirationLikeLayer.hpp
//  ColorGarden
//
//  Created by houhuang on 17/10/26.
//
//

#ifndef InspirationLikeLayer_hpp
#define InspirationLikeLayer_hpp

#include <stdio.h>
#include "UITools.h"
#include "extensions/cocos-ext.h"
#include "STTableView.h"
#include "ServerSprite.hpp"
#include "SwallowListenerLayer.hpp"
using namespace cocos2d::extension;
using namespace std;

class InspirationLikeLayer : public LayerColor, public STTableViewDelegate, public TableViewDataSource
{
public:
    static InspirationLikeLayer* create(Size size);
    InspirationLikeLayer();
    ~InspirationLikeLayer();
    bool init(Size size);
    
public:
    void createTableView();
    
    void reponseServerSprite(Node* ref);
    void reponseUpdateTable(EventCustom* event);
    
public:
    void shieldEvent(bool isShield);
    void tableScrollToTop();
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
    TableView* _table;

    bool _isEnableTouch;
};

#endif /* InspirationLikeLayer_hpp */

