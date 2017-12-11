//
//  MyArtsScene.h
//
//
//  Created by luoxp on 3/18/15.
//
//

#pragma once

#include "UITools.h"

#include "SwallowListenerLayer.hpp"
#include "PostingLayer.hpp"
#include "BaseScene.hpp"
#include "MyartAllLayer.hpp"
#include "MyartPublishedLayer.hpp"

class  MyArtsScene : public BaseScene{
public:
    virtual ~MyArtsScene();
    MyArtsScene();
    
    CREATE_FUNC(MyArtsScene);
    
    virtual bool init();
    void onButton(Ref* pSender);
    
    void onEnterTransitionDidFinish();

public:

    void onDialog(const string& name);
    void enterNextScene();
    
    void doAction(Ref* ref);
    
    void onPermissionGrantedResult(int requestCode,bool bGranted);
    
    void waitPostFaild(Ref* ref);

    void waitPostSuccessful(Ref* ref);

    void hideDialog();
    
    void responseUpdloadImage(EventCustom* event);
public:
    void addListener();
    
    void move();
    void moveFromClickButton(bool isClickLeft);
    void moveEnded();
    
    void btnLineTowardLeftMove();
    void btnLineTowardRightMove();
private:

    Layout* _placeholder;
    Layout* _root;
    
    int _offset;
    
    SwallowListenerLayer*   _swallowLayer;
    string delete_NeedLoadImage;
    string delete_PreName;
    Color3B spriteBGColor;
    Image* saveImage;
    
    PostingLayer*  m_pPostingLayer;
    
    Button*  _All;
    Button*  _Pub;

    Text*   _All_btn_text;
    Text*   _Pub_btn_text;
    
    bool    _isShowAll;
    string name;
    
    
    MyartAllLayer*  _allLayer;
    MyartPublishedLayer*    _publishedLayer;
    Vec2    _firstPos;
    float   _detalX;
    bool    _moveDirectionIsH;
    bool    _switch;
    bool    _isTouchEnable;
    
    Sprite* _btnBottomLine;
    bool    _btnIsTowardRightMoving;
    bool    _btnIsTowardLeftMoving;
    
    Layout* _tableBg;
    Menu*   _topMenu;
};