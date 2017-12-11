//
//  ColorLoopScrollLayer.hpp
//  ColorGarden
//
//  Created by houhuang on 17/10/23.
//
//

#ifndef ColorLoopScrollLayer_hpp
#define ColorLoopScrollLayer_hpp

#include <stdio.h>
#include "PageDot.h"
#include "PalletteManager.hpp"
#include "ColorSprite.h"
USING_NS_CC;
using namespace std;

struct ColorIndex
{
    int page;
    int index;
};

class ColorLoopScrollLayer : public LayerColor
{
public:
    ColorLoopScrollLayer();
    ~ColorLoopScrollLayer();
    
    static ColorLoopScrollLayer* create();
    virtual bool init();
    
public:
    void initUILayer();
    void towardLeft(float xReadyOffset);
    void towardRight(float xReadyOffset);
    
public:
    void addListener();
    void reponseColorSpriteTouch(Node* pSender);
    
public:
    enum class UpdateObj
    {
        LEFT,
        RIGHT,
        ALL
    };

public:
    void updateColorLayer(int page, Color3B color);
    
private:
    void updateLayerPointerFromLeft();
    void updateLayerPointerFromRight();

    void updateColorLayer(UpdateObj updateObj);
    void createColorLayer(PalletteInfo info, LayerColor* layer);
    
    bool currentPageIsExist(int page);
protected:
    LayerColor* _leftLayer1;
    LayerColor* _leftLayer;
    LayerColor* _centerLayer;
    LayerColor* _rightLayer;
    LayerColor* _rightLayer1;
    
    Vec2    _firstPos;
    bool    _isMoveTouchEnded;
    
    vector<PalletteInfo> _vColorDatas;
    int     _iCurrentPage;
    int     _iMaxPage;
    
    Color3B _currentChooseColor;
    ColorSprite*    _currentChooseSprite;
    int _currentChooseSpriteWherePage;
    
    vector<ColorSprite*>    _vColorSpriteDatas;
    float   _accelerationX;
};

#endif /* ColorLoopScrollLayer_hpp */
