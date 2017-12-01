//
//  AnalyticsManager.hpp
//  ColorGarden
//
//  Created by houhuang on 17/9/15.
//
//

#ifndef AnalyticsManager_hpp
#define AnalyticsManager_hpp

#include "UIHeader.h"
using namespace std;

#define ATMInstance AnalyticsManager::getInstance()

class AnalyticsManager
{
public:
    static AnalyticsManager* getInstance();
    void sendEvent(string eventName);
    void sendBannerEvent(string eventName);
    void sendInterstitialEvent(string eventName);
    void sendEvent(string eventName, string key, string value);
    void sendEvent(string eventName, map<string, string> map);
    
    std::string to_json(std::string name, std::vector<std::string> value);
    string to_json_map (string name, std::map<string, string> value);
    
    //收集老数据 只执行一次
    void sendOldDataToFireBase();
    
public:
    vector<string> _eventVec;
    
    string _bannerEventParas;
    string _interstitialEventParas;
    
    int _paintMode = 0;
};

#endif /* AnalyticsManager_hpp */
