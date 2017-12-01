//
//  LibraryTitleLayer.hpp
//  ColorGarden
//
//  Created by houhuang on 17/10/25.
//
//

#ifndef LibraryTitleLayer_hpp
#define LibraryTitleLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;
using namespace std;

enum class Mode
{
    LEFT,
    CENTER,
    RIGHT
};

typedef function<void(Mode)> titleLayerCallback;

class LibraryTitleLayer : public LayerColor
{
public:
    LibraryTitleLayer();
    ~LibraryTitleLayer();

    static LibraryTitleLayer* create(Size size);
    virtual bool init(Size size);
    
public:
    void initUILayer();
    
    void toWardLeft(float xOffset);
    void toWardRight(float xOffset);
    void autoTowardLeft();
    
    void updatePointerFromLeft();
    void updatePointerFromRight();
    
public:
    void addListener(titleLayerCallback callback);
    
    void timer(float dt);
private:
    Sprite* _leftSprite1;
    Sprite* _leftSprite;
    Sprite* _centerSprite;
    Sprite* _rightSprite;
    Sprite* _rightSprite1;
    
    Vec2    _firstPos;
    float   _accelerationX;
    bool    _isMoveTouchEnded;
    float   _timer;
    
    titleLayerCallback _callback;
    
    vector<string> _SpriteName;
    
    int _index;
    bool    _isClickCenterSprite;
};

#endif /* LibraryTitleLayer_hpp */
