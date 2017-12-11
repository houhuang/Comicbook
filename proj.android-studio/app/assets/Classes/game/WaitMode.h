//
//  WaitMode.h
//  ColorBook
//
//  Created by luoxiaopeng on 4/24/15.
//
//

#ifndef __ColorBook__WaitMode__
#define __ColorBook__WaitMode__

#include "cocos2d.h"
#include "BaseMode.h"

class WaitMode : public BaseMode
{
public:
    WaitMode(GameScene * layer);
    virtual ~WaitMode();
    
    virtual void onTouchesBegan(const vector<Touch*>& touches, Event *unused_event);
    virtual void onTouchesMoved(const vector<Touch*>& touches, Event *unused_event);
    virtual void start();
    virtual void stop();
    
    void timeout(float dt);
    bool isOutOfRange(const vector<Touch*>& touches);
    void gotoPickup(float dt);
    
    
};

#endif /* defined(__ColorBook__WaitMode__) */
