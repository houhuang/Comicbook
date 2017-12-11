//
//  DataPromotionManager.hpp
//  ALABS0002NEW
//
//  Created by houhuang on 16/6/2.
//
//

#ifndef DataPromotionManager_hpp
#define DataPromotionManager_hpp

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;
using namespace std;

#define xDPM_getInstance    DataPromotionManager::getInstance()

class DataPromotionManager
{
public:
    static DataPromotionManager* getInstance();
    void registerMessage();
    
    void promotionToV2_0();
    
    //revise Myart csv
    void promotionToV2_1();
    
    void promotionToV4_0();
    
    void checkForcedUpdate();
public:
    bool isEverDownloadFinsh();
    

private:
    void saveCSVDataTofile(const vector<list<string>>& lCsvData);
    

};

#endif /* DataPromotionManager_hpp */
