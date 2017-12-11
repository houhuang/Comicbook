//
//  GraphSpriteLayer.hpp
//  ColorGarden
//
//  Created by houhuang on 17/11/7.
//
//

#ifndef GraphSpriteLayer_hpp
#define GraphSpriteLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "PostingLayer.hpp"

USING_NS_CC;
using namespace std;

class GraphSpriteLayer : public LayerColor
{
public:
    static GraphSpriteLayer* create(const string& preName, string spriteName);
    bool init(const string& preName, string spriteName);
    
    GraphSpriteLayer();
    ~GraphSpriteLayer();
    
    void onExit();
public:
    void addListener();
    void createContentFromiPhone();
    void createContentFromiPad();
    
    void onButton(Ref* ref);
    
    
    void scaleContent(const float duration, bool isEnlarge);
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

#endif /* GraphSpriteLayer_hpp */
