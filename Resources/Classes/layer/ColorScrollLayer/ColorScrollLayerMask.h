//
//  ColorScrollLayerMask.h
//  ColorFree
//
//  Created by houhuang on 15/9/1.
//
//

#ifndef __ColorFree__ColorScrollLayerMask__
#define __ColorFree__ColorScrollLayerMask__

#include <stdio.h>
#include "cocos2d.h"
//#include "../Dialog.h"
USING_NS_CC;

class ColorScrollLayerMask : public LayerColor
{
public:
    ~ColorScrollLayerMask();
    static ColorScrollLayerMask* create(Size maskSize);
    virtual bool init(Size maskSize);
    
private:
    void addListener();
};

#endif /* defined(__ColorFree__ColorScrollLayerMask__) */
