//
//  RateUsLayer.hpp
//  ColorGarden
//
//  Created by houhuang on 17/9/18.
//
//

#ifndef RateUsLayer_hpp
#define RateUsLayer_hpp
#include "UITools.h"
USING_NS_CC;

#include <stdio.h>
class RateUsLayer : public LayerColor
{
public:
    static RateUsLayer* create();
    bool init();
    ~RateUsLayer();
    
    void initUILayer();
    void addListener();
public:
    void onButton(Ref* ref);
    
    void setButtonEnable();
    
protected:
    Sprite* _dialogBG;
    Vec2 _firstPos;
};

#endif /* RateUsLayer_hpp */
