//
//  HomeScene.hpp
//  ComicBook
//
//  Created by houhuang on 17/12/1.
//
//

#ifndef HomeScene_hpp
#define HomeScene_hpp

#include <stdio.h>
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

class HomeScene : public Scene, public TableViewDelegate, public TableViewDataSource
{
public:
    CREATE_FUNC(HomeScene);
    
    virtual bool init();
    
public:
    void createTable();
    
public:
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
};

#endif /* HomeScene_hpp */
