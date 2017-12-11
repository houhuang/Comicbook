//
//  AdsLoadingLayer.h
//  SpotDifferences
//
//  Created by luoxiaopeng on 6/12/15.
//
//

#ifndef __SpotDifferences__AdsLoadingLayer__
#define __SpotDifferences__AdsLoadingLayer__

#include "cocos2d.h"

using namespace cocos2d;

class AdsLoadingLayer : public Layer
{
public:
    static AdsLoadingLayer* create(bool isRewards);
    virtual bool init(bool isRewards);
    void loadStudio();
    void onEnter();
    void onExit();
    
private:
    bool _isRewards;
};

#endif /* defined(__SpotDifferences__AdsLoadingLayer__) */
