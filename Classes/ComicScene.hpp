//
//  ComicScene.hpp
//  ComicBook
//
//  Created by houhuang on 17/12/4.
//
//

#ifndef ComicScene_hpp
#define ComicScene_hpp

#include <stdio.h>
#include "cocos-ext.h"
#include "ShwoCartoonInfoLayer.hpp"
USING_NS_CC;
using namespace std;
USING_NS_CC_EXT;

class ComicScene : public Scene, public TableViewDelegate, public TableViewDataSource
{
public:
    CREATE_FUNC(ComicScene);
    ComicScene();
    virtual bool init();
    
public:
    void createTableView();
    
    void responseSpriteClick(Ref* ref);
    void responseRemoveCartoonLayer(EventCustom* event);
    
    void onButton(Ref* ref);
public:
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    
protected:
    ShowCartoonInfoLayer*   _cartoonLayer;
    
};


#endif /* ComicScene_hpp */
