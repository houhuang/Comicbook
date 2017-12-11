//
//  SearchPathManager.h
//  ColorGarden
//
//  Created by houhuang on 17/3/16.
//
//

#ifndef SearchPathManager_h
#define SearchPathManager_h

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;
using namespace std;

class SearchPathManager
{
public:
    static SearchPathManager* getInstance();
    
    void updateSearchPath();

};

#endif /* SearchPathManager_h */
