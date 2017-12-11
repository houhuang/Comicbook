//
//  ColorAdsManager.h
//  ColorBook
//
//  Created by luoxiaopeng on 4/24/15.
//
//

#ifndef __ColorBook__ColorAdsManager__
#define __ColorBook__ColorAdsManager__

#include "UIHeader.h"

#define xColorAds  (ColorAdsManager::getInstance())

class ColorAdsManager : public Node
{
public:
    static ColorAdsManager* getInstance();
    void destroyInstance();
    
    void share();
    void changeScene();

protected:
    ColorAdsManager();
    void loadFinish();
    
protected:
    CC_SYNTHESIZE(bool, _showed, Showed);
    CC_SYNTHESIZE(bool, _shared, Shared);
    CC_SYNTHESIZE(bool, _showCrossed, showCrossed);
    CC_SYNTHESIZE(bool, _isEnableShowInterstitialAds, IsEnableShowInterstitialAds);
};

#endif /* defined(__ColorBook__ColorAdsManager__) */
