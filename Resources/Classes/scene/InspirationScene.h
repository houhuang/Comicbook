//
//  InspirationScene.h
//
//
//  Created by luoxp on 3/18/15.
//
//

#pragma once

#include "UITools.h"
#include "extensions/cocos-ext.h"
#include "STTableView.h"
#include "ServerSprite.hpp"
#include "SwallowListenerLayer.hpp"
#include "BaseScene.hpp"
#include "InspirationRecentLayer.hpp"
#include "InspirationLikeLayer.hpp"

using namespace cocos2d::extension;

class  InspirationScene : public BaseScene
{
public:
    virtual ~InspirationScene();
    InspirationScene();
    
    CREATE_FUNC(InspirationScene);
    
    virtual bool init();
    void onButton(Ref* pSender);
    void onEnterTransitionDidFinish();
    
public:
    
    void onDialog(const string & name);
    
    void onExit();
    
    void removeDialog(Node* node);

    void rateUsDialog(const string& name);
    
    void setButtonEnable(bool isEnable);
    
public:
    void addListener();
    
    void move();
    void moveFromClickButton(bool isClickLeft);
    void moveEnded();
    
    void btnLineTowardRightMove();
    void btnLineTowardLeftMove();
    
    void createSwallowLayer(EventCustom* event);
    void removeSwallowLayer(EventCustom* event);
protected:
    int         m_iImageNumber;
    string      m_sImageName;
    
    STTableView*  m_pTableView;
    cocos2d::extension::ScrollView* m_pScrollView;
    
    bool        m_bIsEdit;
    bool        m_bIsNetwork;
    bool        m_bIsShowLoading;

    Layout*     _placeholder;
    Layout*     _root;
    
    Button*     _Recent;
    Button*     _MyLike;
    
    Button*     _gallery;
    Button*     _myart;

    Text*       _Recent_text;
    Text*       _MyLike_text;
    
    int         _ShowMode;
    int         _lookCount = 0;
    bool        _beginCount = false;
    
    InspirationLikeLayer*   _likeLayer;
    InspirationRecentLayer* _recentLayer;
    Vec2    _firstPos;
    float   _detalX;
    bool    _moveDirectionIsH;
    bool    _switch;
    bool    _isTouchEnable;
    
    Sprite* _btnBottomLine;
    bool    _btnIsTowardRightMoving;
    bool    _btnIsTowardLeftMoving;
    
    Layout* _tableBg;
    SwallowListenerLayer*   _updateLayer;
    Menu*   _topMenu;
};