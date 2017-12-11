//
//  ChapterScene.h
//
//
//  Created by luoxp on 3/18/15.
//
//

#pragma once

#include "UITools.h"
#include "ui/UIScrollView.h"
#include "ui/UISlider.h"
#include "DataPromotionManager.hpp"
#include "cocos-ext.h"
#include "STTableView.h"
#include "BaseScene.hpp"
#include "LibraryTitleLayer.hpp"

using namespace cocos2d::extension;
class  ChapterScene : public BaseScene , public STTableViewDelegate, public TableViewDataSource
{
public :
    virtual ~ChapterScene();
    ChapterScene();
    
    CREATE_FUNC(ChapterScene);
    
    virtual void onEnterTransitionDidFinish() override;
    
    virtual bool init() override;
    void onButton(Ref* pSender);
    void onDialog(const string& name);
    void createListView();
    
    void updateProgress(Ref* ref);
    void waitDownloadFinish(Ref* ref);
    void onListView(Ref* pSender, ui::ScrollView::EventType type);
    void onSlider(Ref *pSender, Slider::EventType type);
    void checkTableOffset();
    void refresh(float dt);
    
    void checkUpdate(float dt);
    void showRateUs(EventCustom* event);

    void reponseList(Ref* pSender, Widget::TouchEventType type);
    void downloadDialog(const string& name);
    void networkError(const string& name);
    
    void enablePricavy(float dt);
    void ontMenuButton(Ref* ref);
    
    void onNewDialog(const string& name);
    
    void onTitleLayer(Mode mode);
    void responseSpriteEvent(Node* ref);
    
    void initUI();
public:
    void createTableView();
    virtual void tableCellTouched(TableView* table, TableViewCell* cell) override;
    virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx) override;
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx) override;
    virtual ssize_t numberOfCellsInTableView(TableView *table) override;
    
    virtual void tableCellTouched(TableView* table, TableViewCell* cell,Touch* pTouch) override;
    virtual void tableCellRemoveTexture(TableView* table, TableViewCell* cell) override;
    
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view) override;
    virtual void tableMoveEnded() override;
    virtual void scrollViewMoveEnded() override;
private:
    ListView * _listview;
    Layout * _root;
    std::vector<ProgressTimer*> m_vProgress;
    Size m_sLayoutSize;
    Slider * _slider;
    Text *   _text;
    float    _oldY;
    
    Text*   _text_pre;
    Text*   _text_nex;
    
    TableView* _tableView;
    bool    _isEnableprivacy;
    
    LibraryTitleLayer*  _titleLayer;
    LayerColor*         _titleBar;
    Sprite*             _titleBarShadow;
    Label*              _titleText;
    
    Layout*    _tableBg;
    Menu*      _topMenu;
};