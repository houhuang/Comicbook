//
//  PostingLayer.hpp
//  ColorGarden
//
//  Created by houhuang on 17/4/5.
//
//

#ifndef PostingLayer_hpp
#define PostingLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "SwallowListenerLayer.hpp"
USING_NS_CC;

class PostingLayer : public SwallowListenerLayer
{
public:
    static PostingLayer* create(const Color4B& color);
    bool init(const Color4B& color);
    
    PostingLayer();
    
public:
    void setPushSuccessed();
    
protected:
    void update(float dt);
    
private:
    float _time;
    Label*  _label;
};

#endif /* PostingLayer_hpp */
