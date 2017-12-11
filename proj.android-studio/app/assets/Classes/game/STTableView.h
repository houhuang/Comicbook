//
//  STTableView.hpp
//  ColorGarden
//
//  Created by lidong on 15/12/29.
//
//

#ifndef STTableView_hpp
#define STTableView_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
using namespace cocos2d;
USING_NS_CC_EXT;


class STTableViewDelegate : public TableViewDelegate {
    
    
public:
    virtual void tableCellTouched(TableView* table, TableViewCell* cell,Touch* pTouch){};
    virtual void tableCellRemoveTexture(TableView* table, TableViewCell* cell){}
    virtual void tableRequestNextPage(TableView* table,ssize_t idx){}
    virtual ssize_t tableLastID(TableView* table){ return 0;}
    
    virtual void tableUpdateFunc(TableView* table,TableViewCell* cell){}
    
    virtual void tableMoveEnded(){};
};

class STTableView : public TableView {
    
protected:
    bool _isUpdateShow;
    
public:
    static STTableView* create(TableViewDataSource* dataSource, Size size);
    static STTableView* create(TableViewDataSource* dataSource, Size size, Node *container);

    void initSetting();
    
    virtual void onTouchEnded(Touch *pTouch, Event *pEvent) override;
    
    bool initWithViewSize(Size size, Node* container = nullptr);
    // Overrides
    virtual void scrollViewDidScroll(ScrollView* view) override;
    
    virtual void scrollViewMoveEnded();
    
    void updateContentSize();
    
    void setIsUpdateShow(bool pBool);
    
    void update(float dt);
    
    void setOffsetY(float offsetY);
    
private:
    float _offsetY;
    
};


#endif /* STTableView_hpp */
