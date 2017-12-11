//
//  EventSprite.hpp
//  ComicBook
//
//  Created by houhuang on 17/12/8.
//
//

#ifndef EventSprite_hpp
#define EventSprite_hpp

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;
using namespace std;

class EventSprite : public Sprite
{
public:
    static EventSprite* create(const string& name);
    virtual bool init(const string& name);
    

public:
    void addListener(SEL_CallFuncN callBack, Node* target);
    
private:
    SEL_CallFuncN _callBack;
    Node*   _target;
    
    Vec2    _firstPos;
};

#endif /* EventSprite_hpp */
