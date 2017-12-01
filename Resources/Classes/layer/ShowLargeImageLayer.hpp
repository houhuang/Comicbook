//
//  ShowLargeImageLayer.hpp
//  ColorGarden
//
//  Created by houhuang on 17/11/7.
//
//

#ifndef ShowLargeImageLayer_hpp
#define ShowLargeImageLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;

class ShowLargeImageLayer : public LayerColor
{
public:
    static ShowLargeImageLayer* create(const int imageID, float width);
    bool init(const int imageID, float width);
    
    ShowLargeImageLayer();
    ~ShowLargeImageLayer();

public:
    void addListener();
    
    void scaleContent(const float duration, bool isEnlarge);
private:
    Sprite* _contentSprite;
    Vec2    _firstPos;
    
    bool    _isExcuteAnimation;
};

#endif /* ShowLargeImageLayer_hpp */
