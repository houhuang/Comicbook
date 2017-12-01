//
//  MyartSpriteLayer.hpp
//  ColorGarden
//
//  Created by houhuang on 17/11/7.
//
//

#ifndef MyartSpriteLayer_hpp
#define MyartSpriteLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "PostingLayer.hpp"

USING_NS_CC;
using namespace std;

class MyartSpriteLayer : public LayerColor
{
public:
    static MyartSpriteLayer* create(const string& preName, string spriteName, Vec2 animationBeginPos);
    bool init(const string& preName, string spriteName, Vec2 animationBeginPos);
    
    MyartSpriteLayer();
    ~MyartSpriteLayer();

public:
    void addListener();
    void createContentFromiPhone();
    void createContentFromiPad();
    
    void onButton(Ref* ref);
    
    
    void scaleContent(const float duration, bool isEnlarge, Vec2 beginPos = Vec2(0, 0));
private:
    Sprite* _contentSprite;
    Vec2    _firstPos;
    
    bool    _isExcuteAnimation;
    bool    _isClickContent;
    bool    _isClickImage;
    
    string  _name;
    
    Sprite*         _deleteSprite;
    Label*          _deleteLabel;
    
    string  _SpriteName;
    PostingLayer*   _postingLayer;
    
    Sprite* _showImage;
};

#endif /* MyartSpriteLayer_hpp */
