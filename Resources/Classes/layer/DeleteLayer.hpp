//
//  DeleteLayer.hpp
//  ColorGarden
//
//  Created by houhuang on 17/11/7.
//
//

#ifndef DeleteLayer_hpp
#define DeleteLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;

class DeleteLayer :public LayerColor
{
public:
    CREATE_FUNC(DeleteLayer);
    bool init();

    ~DeleteLayer();
public:
    void onButton(Ref* ref);
    void addListener();
    
protected:
    Vec2    _firstPos;
};

#endif /* DeleteLayer_hpp */
