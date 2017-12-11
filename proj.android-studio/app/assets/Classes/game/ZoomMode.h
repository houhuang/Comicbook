//
//  ZoomMode.h
//  ColorBook
//
//  Created by luoxiaopeng on 4/24/15.
//
//

#ifndef __ColorBook__ZoomMode__
#define __ColorBook__ZoomMode__

#include "cocos2d.h"
#include "BaseMode.h"

class ZoomMode : public BaseMode
{
public:
    ZoomMode(GameScene * layer);
    virtual ~ZoomMode();
    
    virtual void onTouchesBegan(const vector<Touch*>& touches, Event *unused_event);
    virtual void onTouchesMoved(const vector<Touch*>& touches, Event *unused_event);
    
    void resetPictureAnchorPoint(const std::vector<Touch *> &touches);
    void timeout(float dt);
    bool isOutOfRange(const vector<Touch*>& touches);
    bool checkIsMoveMode(const vector<Touch*>& touches);
    
    virtual void start();
    virtual void stop();
    
    void setMaxScale(float scale);
private:
    bool    _canChange;
    float _maxScale;
};

#endif /* defined(__ColorBook__ZoomMode__) */
