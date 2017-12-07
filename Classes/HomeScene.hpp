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
using namespace std;

class HomeScene : public Scene, public TableViewDelegate, public TableViewDataSource
{
public:
    CREATE_FUNC(HomeScene);
    virtual bool init();
    
    HomeScene();
public:
    void createTable();
    
    void checkProgress();
    void onDialog(const string& name);
public:
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    
private:
    float topBar_realHeight;
};

#endif /* HomeScene_hpp */
