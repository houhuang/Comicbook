#ifndef _STVISIBLE_H_
#define _STVISIBLE_H_

#include "cocos2d.h"

#ifdef __cplusplus
#define NS_ST_BEGIN                     namespace namespaceST {
#define NS_ST_END                       }
#define USING_NS_ST                     using namespace namespaceST
#else
#define NS_ST_BEGIN
#define NS_ST_END
#define USING_NS_ST
#endif

NS_ST_BEGIN
USING_NS_CC;


#define V   namespaceST::STVisibleRect

class STVisibleRect {
public:
	// initialization for STVisibleRect
	static void setupVisibleRect(Size frameSize, Size designSize);

	// initialization offset for STVisibleRect
	static void initRetainOffset(int left, int top, int right, int bottom);
    
    static int getRetainOffsetTop();

	// real design size(bigger than the designed size of game)
	static Size& getRealDesignSize();
	
	// the designed size of game
	static Size& getDesignSize();
    
    //the center of design frame.
    static Vec2 getDesignCeneter();
	
	// get frame size(can consider as screen size)
	static Size& getFrameSize();
    
	// the offset between game designed size and real design size
	static Vec2 getDesignOffset();

	// the ratio = RealDesignSize / DesignSize
	static float getRealDesignScale();
    static float getRelativeScale();
    
    //the center coordindate of the screen
    static Vec2 getCenterOfScene();
    static Vec2 getPointOfSceneLeftBottom();
    static Vec2 getPointOfSceneRightUp();

	static void setPosition(Node* node, float x, float y, bool bTop = false, bool bBottom = false, bool bLeft = false, bool bRight = false);
    
    static void setPosition(Node* node, Vec2 pt, bool bTop = false, bool bBottom = false, bool bLeft = false, bool bRight = false);
    
    static Vec2 getRealPositionForDesignPosition(Vec2 aPosition, bool bTop = false, bool bBottom = false, bool bLeft = false, bool bRight = false);
    
    //限制node底部不超出屏幕底部的区域
    static Rect getBottomMoveRect(Node* apNode);
    //限制node顶部不超出屏幕顶部的区域
    static Rect getTopMoveRect(Node* apNode);
    //限制node左部不超出屏幕左部的区域
    static Rect getLeftMoveRect(Node* apNode);
    //限制node右部不超出屏幕右部的区域
    static Rect getRightMoveRect(Node* apNode);
    
    static bool isIpad();
    static bool isHighVersion();
    
    static ClippingNode* drawRoundRect(Node *newNode, float radius, unsigned int segments);
    static Sprite* maskedSprite(Sprite *textureSprite);
};

NS_ST_END
#endif