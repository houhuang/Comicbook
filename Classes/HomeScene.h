//
//  HomeScene.h
//  ComicBook
//
//  Created by houhuang on 17/12/1.
//
//

#ifndef HomeScene_h
#define HomeScene_h

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "NewDialog.h"
#include "SettingLayer.hpp"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class HomeScene : public Scene, public TableViewDelegate, public TableViewDataSource
{
public:
    CREATE_FUNC(HomeScene);
    virtual bool init();
    
    ~HomeScene();
    HomeScene();
    void onEnterTransitionDidFinish();
    
    void registerNotification();
public:
    void createTable();
    
    void checkProgress();
    void onDialog(const string& name);
    void onButton(Ref* ref);
    
    void responseCoverSprite(Node* node);
    void addBackListener();
    
    void removeDailog(EventCustom* event);
    void removeSettingLayer(EventCustom* event);
    
    void showClearDataDialog(EventCustom* event);
    void showContinueReadDialog(EventCustom* event);
public:
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    
    virtual void scrollViewDidScroll(ScrollView* view);
private:
    float   topBar_realHeight;
    NewDialog*      _dialog;
    
    TableView*      _tableView;
    Sprite*         _bannerSprite;
    SettingLayer*   _settingLayer;
};

#endif /* HomeScene_h */
