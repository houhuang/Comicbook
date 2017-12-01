//
//  BannerManager.hpp
//  ColorGarden
//
//  Created by houhuang on 17/9/26.
//
//

#ifndef BannerManager_hpp
#define BannerManager_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "AdsManager.h"
USING_NS_CC;

#define xBanner BannerManager::getInstance()

class BannerManager : public BannerAdsDelegate
{
public:
    static BannerManager* getInstance();
    
public:
    void show(ADS_TYPE adType = kTypeBannerAds);
    
};

#endif /* BannerManager_hpp */
