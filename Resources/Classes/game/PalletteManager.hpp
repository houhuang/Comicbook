//
//  PalletteManager.hpp
//  ALABS0010
//
//  Created by houhuang on 16/3/22.
//
//

#ifndef PalletteManager_hpp
#define PalletteManager_hpp

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;
using namespace std;

#define PleM_getInstance    PalletteManager::getInstance()
#define xPallette           PalletteManager::getInstance()


typedef struct _PalletteInfo{
    string name;
    bool isLock;
    vector<Color3B> vRGBInfo;
}PalletteInfo;

class PalletteManager
{
public:
    ~PalletteManager();
    
    static PalletteManager* getInstance();
    const vector<PalletteInfo> & getData();
    const Color3B & getColor(size_t palletteIndex, int colorIndex);
    Color4B getRandomColor(size_t palletteIndex);
    const Color3B & getCurrentColor();
    void readPalletteCsvInfo();
    void saveOldColor();
    Color4B getOldColor();
    int getPalletteIndex();
    int getColorIndex();
    void reset();
    
private:
    PalletteManager();
    
private:
    vector<PalletteInfo> m_vPalletteInfo;
    Color3B _currentColor;
    Color3B _oldColor;
    int     _palletteIndex = 0;
    int     _colorIndex = 0;
    
public:
    static bool isAlreadySelectedColor;
};

#endif /* PalletteManager_hpp */
