//
//  SettingLayer.hpp
//  ComicBook
//
//  Created by houhuang on 17/12/14.
//
//

#ifndef SettingLayer_hpp
#define SettingLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;

#define st_remove_settingLayer          "st_remove_settingLayer"
#define st_showDialog_clearDataDialog   "st_showDialog_clearDataDialog"

class SettingLayer : public LayerColor
{
public:
    static SettingLayer* create(Vec2 lPos);
    bool init(Vec2 lPos);
    
    ~SettingLayer();
    SettingLayer();
    
    void initUI(Vec2 lPos);
    
public:
    void addListener();
    
    void onButton(Ref* ref);
private:
    Sprite*     _contentSprite;
    bool        _isClickedContent;
    Vec2        _firstPos;
};

#endif /* SettingLayer_hpp */
