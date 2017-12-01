//
//  ShowInspirationLayer.hpp
//  ColorGarden
//
//  Created by houhuang on 17/5/18.
//
//

#ifndef ShowInspirationLayer_hpp
#define ShowInspirationLayer_hpp

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;

class ShowInspirationLayer : public LayerColor
{
public:
    static ShowInspirationLayer* create(const int postID, const int likeNumber, bool isMylike = false);
    bool init(const int postID, const int likeNumber, bool isMylike = false);
    
    ShowInspirationLayer();
    ~ShowInspirationLayer();
public:
    void addListener();
    
    void setContentTouchEnbale(bool isEnble);
public:
    void onAction(Ref* ref);
    
    void responseGetLikeSuc(Ref* ref);
    
    void createContent();
    void destoryContent();

    void scaleContent(const float duration, bool isEnlarge);
public:
    bool    _isAlreadyLike;
    Sprite* _heart;
    Label*  _label;
    int     _likeNumber;
    int     _postID;
    
    Sprite* _bgSprite;
    Sprite* _contentSprite;
    
    Menu*   _heartMenu;
    Menu*   _backMenu;
    bool    _isListenerClick;
    Vec2    _firstPos;
    
    bool    _isMylike;
    
    //update position
    MenuItemImage*  _hearMenuItem;
};

#endif /* ShowInspirationLayer_hpp */
