//
//  InspirationRecentLayer.hpp
//  ColorGarden
//
//  Created by houhuang on 17/10/26.
//
//

#ifndef InspirationRecentLayer_hpp
#define InspirationRecentLayer_hpp

#include <stdio.h>
#include "UITools.h"
#include "extensions/cocos-ext.h"
#include "STTableView.h"
#include "ServerSprite.hpp"
#include "SwallowListenerLayer.hpp"
using namespace cocos2d::extension;
using namespace std;

class InspirationRecentLayer : public LayerColor, public STTableViewDelegate, TableViewDataSource
{
public:
    static InspirationRecentLayer* create(Size size);
    InspirationRecentLayer();
    ~InspirationRecentLayer();
    bool init(Size size);
    
    void onEnterTransitionDidFinish();
public:
    void createTableView();
    
    void firstRequestDatas();
    
public:
    void onDialog(const string& name);
    void reponseServerSprite(Node* ref);
    
    void waitDownloadJsonSuc(Ref* ref);
    void waitDownloadJsonFai(Ref* ref);
    
    //load new page
    void updateContentSize(Ref* ref);
    
    void reponseUpdateTable(EventCustom* event);
public:
    void shieldEvent(bool isShield);
    
    void tableScrollToTop();
    
    int getlookCount();
public:
    Size tableCellSizeForIndex(TableView *table, ssize_t idx) override;
    TableViewCell* tableCellAtIndex(TableView* table, ssize_t idex)override;
    ssize_t numberOfCellsInTableView(TableView* table)override;
    void tableCellTouched(TableView* table, TableViewCell* cell)override;
    
    void tableCellTouched(TableView* table, TableViewCell* cell,Touch* pTouch)override;
    void tableCellRemoveTexture(TableView* table, TableViewCell* cell)override;
    void tableRequestNextPage(TableView* table,ssize_t idx)override;
    ssize_t tableLastID(TableView* table)override;
    
    void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override;
    virtual void tableMoveEnded() override;
public:
    SwallowListenerLayer*   _swallowLayer;
    Sprite*                 _rotateSprite;
    Sprite*                 _updateRotate;
    STTableView* _tableView;
    
    bool    _isEnableTouch;
    
    int _lookCount;
};
#endif /* InspirationRecentLayer_hpp */
