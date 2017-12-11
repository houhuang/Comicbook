//
//  PickupMode.hpp
//  ColorGarden
//
//  Created by houhuang on 17/10/20.
//
//

#ifndef PickupMode_hpp
#define PickupMode_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "BaseMode.h"

class PickupMode : public BaseMode
{
public:
    PickupMode(GameScene* layer);
    virtual ~PickupMode();
    virtual void onTouchesBegan(const vector<Touch*>& touches, Event *unused_event);
    virtual void onTouchesMoved(const vector<Touch*>& touches, Event *unused_event);
    virtual void onTouchesEnded(const vector<Touch*>& touches, Event *unused_event);
    virtual void onTouchesCancelled(const vector<Touch*>& touches, Event *unused_event);
    
    virtual void start();
    virtual void stop();
    
public:
    Sprite* _pickupSpr;
    Color3B _endMoveColor;
};

#endif /* PickupMode_hpp */
