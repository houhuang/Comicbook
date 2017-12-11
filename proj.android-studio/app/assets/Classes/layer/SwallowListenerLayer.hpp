//
//  SwallowListenerLayer.hpp
//  IMG1001
//
//  Created by houhuang on 15/12/1.
//
//

#ifndef SwallowListenerLayer_hpp
#define SwallowListenerLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;

class SwallowListenerLayer : public LayerColor
{
public:
    
    static SwallowListenerLayer* create(const Color4B& color);
    bool init(const Color4B& color);
    
    void setTouchEnable(bool pBool);
    void addTouchListener();
    void removeTouchListener();

    ~SwallowListenerLayer();
    
public:
    bool onTouchBegan(Touch* touch, Event* unused_event);
};


#endif /* SwallowListenerLayer_hpp */
