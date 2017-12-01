//
//  ScribbleSpineNode.h
//
//
//  Created by renhong on 16/1/8.
//
//

#ifndef ScribbleSpineNode_h
#define ScribbleSpineNode_h
#ifdef SCRIBBLE_USE_SPINE
#include <stdio.h>
#include "spine/spine-cocos2dx.h"
#include "spine/extension.h"
#include "ScribbleNode.h"
#include "cocos2d.h"
using namespace cocos2d;
using namespace spine;

#define ScribbleSpineAtlasPage       "ScribbleSpineAtlasPage"

class ScribbleSpineNode : public ScribbleTouchNode{
public:
    static ScribbleSpineNode *create(Size pCanvasSize);
    
    ScribbleSpineNode();
    ~ScribbleSpineNode();
    bool init(Size pCanvasSize);
    void displaySkin();
    
protected:
    void update(float dt);
    spAtlasPage* createScribbleAtlasPage();
    void displayScribbleAtlasPage(spAtlasPage* atlasPage, spAtlasRegion* region);
private:
    float* _worldVertices;
    CC_SYNTHESIZE(SkeletonAnimation *, _skeletonAnimation, SkeletonAnimation);
    CC_SYNTHESIZE(spSlot *, _slot, Slot);
};
#endif
#endif /* ScribbleSpineNode_hpp */
