//
//  FirebaseEventScene.hpp
//  ColorGarden
//
//  Created by houhuang on 17/9/18.
//
//

#ifndef FirebaseEventScene_hpp
#define FirebaseEventScene_hpp

#include <stdio.h>
#include "UITools.h"
#include "cocos-ext.h"
USING_NS_CC;
using namespace cocos2d::extension;
class FirebaseEventScene : public Scene, public TableViewDelegate, public TableViewDataSource
{
public:
    CREATE_FUNC(FirebaseEventScene);
    bool init();
    
    void ontMenuButton(Ref* ref);
    void initTableView();
    
public:
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);
};

#endif /* FirebaseEventScene_hpp */
