//
//  GraphScene.h
//
//
//  Created by luoxp on 3/18/15.
//
//

#pragma once

#include "UITools.h"
#include "ui/UISlider.h"
#include "extensions/cocos-ext.h"
#include "STTableView.h"
#include "BaseScene.hpp"
#include "PostingLayer.hpp"
#include "GraphSpriteLayer.hpp"

using namespace cocos2d::extension;

class  GraphScene : public BaseScene, public TableViewDataSource, public STTableViewDelegate
{
public :
    virtual ~GraphScene();
    GraphScene();

    CREATE_FUNC(GraphScene);
    
    void onEnterTransitionDidFinish();
    
    virtual bool init() override;
    void onButton(Ref* pSender);
    void onDialog(const string& name);
    void downloadDialog(const string& name);
    void reloadData();
    void checkUpdate(float dt);
    void onSlider(Ref *pSender, Slider::EventType type);
    
    void waitDownloadFinish(Ref* ref);
    
    void responseDownloadSprite(Node* ref);
    
    //override BaseScene function
    void addBackListener();
    
    void updateTableView();
    
    void waitPostFaild(Ref* ref);
    void waitPostSuccessful(Ref* ref);
    void responseUpdloadImage(EventCustom* event);
    void removeMyartSpriteLayer(EventCustom* event);
public:
    void createTableView();
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx) override;
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx) override;
    virtual ssize_t numberOfCellsInTableView(TableView *table) override;
    virtual void scrollViewDidScroll(extension::ScrollView* view) override;
    
    virtual void tableCellTouched(TableView* table, TableViewCell* cell,Touch* pTouch) override;
    virtual void tableCellRemoveTexture(TableView* table, TableViewCell* cell) override;
    
    virtual void tableUpdateFunc(TableView* table,TableViewCell* cell) override;
    
private:
    ListView * _placeholder;
    STTableView * _tableView;
    int _graphIndex = 0;
    Slider * _slider;
    Text *   _text;
    
    Text*   _text_pre;
    Text*   _text_nex;
    
    Sprite* _ChapterMark;
    float   _ChapterMarkY;
    Label*  _ChapterText;
    string  _preChapterStr;
    
    PostingLayer*   _postingLayer;
    
    Layout* _tableBg;
    Menu*   _topMenu;
    
    GraphSpriteLayer*   _graphLayer;
};