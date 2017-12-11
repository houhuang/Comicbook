//
//  UIColorManager.hpp
//  ColorGarden
//
//  Created by houhuang on 16/8/23.
//
//

#ifndef UIColorManager_hpp
#define UIColorManager_hpp

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;
using namespace std;

#define xUcm    UIColorManager::getInstance()

typedef struct UIColorInfo
{
    std::string name;
    Color3B color;
}_UIColorInfo;

class UIColorManager
{
public:
    static UIColorManager* getInstance();
    
    UIColorManager();
    
public:
    void initUIColorData();
    
    Color3B findUIColor(string name);
    
public:
    std::vector<UIColorInfo> _UIColorData;
};

#endif /* UIColorManager_hpp */
