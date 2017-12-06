//
//  ReadScene.hpp
//  ComicBook
//
//  Created by houhuang on 17/12/6.
//
//

#ifndef ReadScene_hpp
#define ReadScene_hpp

#include <stdio.h>
USING_NS_CC;
using namespace std;

class ReadScene : public Scene
{
public:
    CREATE_FUNC(ReadScene);
    virtual bool init();
    
    ReadScene();
    void initUILayer();
public:
    void addListener();
    void onButton(Ref* ref);
    
public:
    void towardLeftMove();
    void towardRightMove();
    
    void resetLayerPointer(bool towardLeftMove);
private:
    LayerColor* _leftLayer;
    LayerColor* _centerLayer;
    LayerColor* _rightLayer;
    
    bool    _isMoving;
};

#endif /* ReadScene_hpp */
