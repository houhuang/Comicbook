//
//  ComicScene.h
//  ComicBook
//
//  Created by houhuang on 17/12/4.
//
//

#ifndef ComicScene_h
#define ComicScene_h

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "ShwoCartoonInfoLayer.h"
#include "NewDialog.h"
USING_NS_CC;
using namespace std;
USING_NS_CC_EXT;

class ComicScene : public Scene, public TableViewDelegate, public TableViewDataSource
{
public:
    CREATE_FUNC(ComicScene);
    ~ComicScene();
    ComicScene();
    virtual bool init();
    
    void onEnterTransitionDidFinish();
public:
    void createTableView();
    
    void responseSpriteClick(Ref* ref);
    void responseRemoveCartoonLayer(EventCustom* event);
    
    void onButton(Ref* ref);
    void addBackListener();
    
    void setShowButtonState();
    void removeDialog(EventCustom* event);
    void onDialog(const string& name);
public:
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    
protected:
    ShowCartoonInfoLayer*   _cartoonLayer;
    
    TableView*  _table;
    bool    _isShowAlready;
    
    Sprite*  _showBtnSpr;
    vector<CartoonInfo> _cartoonInfoVec;
    
    MenuItemImage*  _showButton;
    NewDialog*  _dialog;
};


#endif /* ComicScene_h */
