//
//  AnalyticsManager.cpp
//  ColorGarden
//
//  Created by houhuang on 17/9/15.
//
//

#include "AnalyticsManager.hpp"
#include "Analytics.h"
#include "STSystemFunction.h"
#include "MyArtDataManager.hpp"

static AnalyticsManager* _instance = nullptr;

AnalyticsManager* AnalyticsManager::getInstance()
{
    if (!_instance)
    {
        _instance = new AnalyticsManager();
    }
    
    return _instance;
}

void AnalyticsManager::sendEvent(string eventName)
{
    std::map<string, string> map;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    Analytics::getInstance()->sendScreenEvent(eventName);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    STSystemFunction sf;
    sf.sendFireBase(to_json_map(eventName, map));
#endif
    
    _eventVec.push_back(eventName);
}

void AnalyticsManager::sendBannerEvent(string eventName)
{
    this->sendEvent(eventName, "banner_location", _bannerEventParas);
}

void AnalyticsManager::sendInterstitialEvent(string eventName)
{
    this->sendEvent(eventName, "interstitial_location", "painting loading page");
}

void AnalyticsManager::sendEvent(string eventName, string key, string value)
{
    map<string, string> _map;
    _map.insert(pair<string, string>(key, value));
    
    this->sendEvent(eventName, _map);
}

void AnalyticsManager::sendEvent(string eventName, map<string, string> map)
{
//    map.insert(pair<string, string>("111", "qwe"));
//    map.insert(pair<string, string>("222", "asd"));
//    map.insert(pair<string, string>("333", "zxc"));
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    STSystemFunction sf;
    sf.sendEvent(eventName, map);
//    Analytics::getInstance()->sendEvent(eventName, map);

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    STSystemFunction sf;
    sf.sendFireBase(to_json_map(eventName, map));
#endif
    
    stringstream stream;
    stream<<eventName + " | ";
    int i = 0;
    for (auto iter : map)
    {
        stream<<iter.first + ":";
        stream<<iter.second;
        if (i < map.size() - 1)
        {
        stream<<" | ";
        }
        ++i;
    }
    
//    _eventVec.push_back(stream.str());
    _eventVec.push_back(stream.str());
    log("%s", stream.str().c_str());
}

void AnalyticsManager::sendOldDataToFireBase()
{
    bool isSend = UserDefault::getInstance()->getBoolForKey("isSendoldData", false);
    if (!isSend)
    {
        //send data
        for (int i = 0; i < xMyArtData->m_vMyArtDatas.size(); ++i)
        {
            MyArtData myArtData = xMyArtData->m_vMyArtDatas.at(i);
            string name = myArtData.name.substr(0, myArtData.name.find("."));
            if (myArtData.serveId == 0)
            {
                this->sendEvent("liveimage_edit", "image_id", name);
            }else
            {
                this->sendEvent("liveimage_publish", "image_id", name);
            }
        }
        
    
        UserDefault::getInstance()->setBoolForKey("isSendoldData", true);
        UserDefault::getInstance()->flush();
    }
}


std::string AnalyticsManager::to_json (std::string name, std::vector<std::string> value) {
    std::vector<std::string>::const_iterator itor, last = value.end();
    std::stringstream stream;
    stream << "{\"" << name << "\":[";
    int i = 0;
    for (itor = value.begin(); itor != last; ++itor) {
        stream << "{";
        stream << "\"index\":" << "\"" << i << "\",";
        stream << "\"value\":" << "\"" << *itor << "\"";
        stream << "}";
        if(itor != last -1) {
            stream << ",";
        }
        ++i;
    }
    stream << "]}";
    return stream.str();
}

string AnalyticsManager::to_json_map (string name, std::map<string, string> value) {
    std::stringstream stream;
    stream << "{\"eventName\":\"" << name << "\",\"eventValue\":[";
    int i = 0;
    for (auto val : value)
    {
        stream << "{\"";
        stream << val.first << "\":\"" << val.second << "\"";
        stream << "}";
        if (i < value.size()-1)
        {
            stream << ",";
        }
        ++i;
    }
    stream << "]}";
    return stream.str();
}