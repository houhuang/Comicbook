//
//  BannerManager.cpp
//  ColorGarden
//
//  Created by houhuang on 17/9/26.
//
//

#include "BannerManager.hpp"
#include "AnalyticsManager.hpp"

static BannerManager* instance = nullptr;

BannerManager* BannerManager::getInstance()
{
    if (!instance)
    {
        instance = new BannerManager();
    }
    
    return instance;
}


void BannerManager::show(ADS_TYPE adType)
{

//    BannerAds::getInstance()->remove();
    BannerAds::getInstance()->show();
    BannerAds::getInstance()->onBannerLoadFailed = [this](std::string error)
    {
        
    };

    BannerAds::getInstance()->onBannerExpanded = [this](){
        
        ATMInstance->sendBannerEvent("ads_banner");
    };
    BannerAds::getInstance()->onBannerLoaded = [this]()
    {
        
    };
    BannerAds::getInstance()->onBannerCollapsed = [this]()
    {
        
    };

}
