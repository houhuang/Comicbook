//
//  BlockLayer.h
//  ColorGarden
//
//  Created by luoxiaopeng on 15/8/4.
//
//

#pragma once

#include "UIHeader.h"

class BlockLayer : public LayerColor{

public:
    CREATE_FUNC(BlockLayer);
    virtual bool init();
    
    void setLoadingPosition(const Vec2 &pos);
};
