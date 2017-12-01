//
//  NoneMode.h
//  ColorBook
//
//  Created by luoxiaopeng on 4/24/15.
//
//

#ifndef __ColorBook__NoneMode__
#define __ColorBook__NoneMode__

#include "cocos2d.h"
#include "BaseMode.h"

class NoneMode : public BaseMode
{
public:
    NoneMode(GameScene * layer);
    virtual void onTouchesBegan(const vector<Touch*>& touches, Event *unused_event);
    void start();
};

#endif /* defined(__ColorBook__NoneMode__) */
