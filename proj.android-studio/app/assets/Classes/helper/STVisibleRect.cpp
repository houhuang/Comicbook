#include "STVisibleRect.h"
#include "STSystemFunction.h"

USING_NS_CC;
USING_NS_ST;

static Size s_FrameSize;
static Size s_DesignSize;
static Size s_RealDesignSize;
static Vec2 s_TopLeftOffset;
static Vec2 s_DesignOffset;
//屏幕边与showall区域的偏移
static Vec2 s_FrameAndDesignOffset;
static float s_RealDesignScale = 1.0f;
static float _relativeScale;

static int s_RetainOffsetLeft = 0;
static int s_RetainOffsetTop = 0;
static int s_RetainOffsetRight = 0;
static int s_RetainOffsetBottom = 0;

void STVisibleRect::setupVisibleRect(Size frameSize, Size designSize)
{
	s_DesignSize = designSize;
	s_FrameSize = frameSize;

    float scaleX=frameSize.width/designSize.width;
    float scaleY=frameSize.height/designSize.height;
    
    float minScale=scaleX>scaleY? scaleY: scaleX;

    frameSize = Size(frameSize.width/minScale, frameSize.height/minScale);

	s_RealDesignScale = 1.0 / minScale;
	s_RealDesignSize = frameSize;
    s_RealDesignSize.width = roundf(s_RealDesignSize.width);
    s_RealDesignSize.height = roundf(s_RealDesignSize.height);

	s_DesignOffset = Vec2((s_RealDesignSize.width - s_DesignSize.width) / 2.0f,
		(s_RealDesignSize.height - s_DesignSize.height) / 2.0f);
    
    Director::getInstance()->getOpenGLView()->setDesignResolutionSize(s_RealDesignSize.width, s_RealDesignSize.height, ResolutionPolicy::SHOW_ALL);
    
    //add by luoxp, 得到实际的s_RealDesignSize的scale
    scaleX = s_RealDesignSize.width/designSize.width;
    scaleY = s_RealDesignSize.height/designSize.height;
    
    if (scaleX > scaleY) {
        _relativeScale = scaleX;
    }
    else
    {
        _relativeScale = scaleY;
    }
    
    //relativeScale 实际和下列代码一样
//    auto scaleX = _winSize.width / DESIGN_WIDTH;
//    auto scaleY = _winSize.height / DESIGN_HEIGHT;
//    _relativeScale = scaleX > scaleY ? scaleX : scaleY;
}

void STVisibleRect::initRetainOffset(int left, int top, int right, int bottom)
{
	s_RetainOffsetLeft = left * s_RealDesignSize.width / s_FrameSize.width;
	s_RetainOffsetTop = top * s_RealDesignSize.height / s_FrameSize.height;
	s_RetainOffsetRight = right * s_RealDesignSize.width / s_FrameSize.width;
	s_RetainOffsetBottom = bottom * s_RealDesignSize.height / s_FrameSize.height;
    NotificationCenter::getInstance()->postNotification("NotifyRetainOffset");
}

int STVisibleRect::getRetainOffsetTop()
{
    return s_RetainOffsetTop;
}

void STVisibleRect::setPosition(Node* node, float x, float y, bool bTop, bool bBottom, bool bLeft, bool bRight)
{
	node->setPosition(getRealPositionForDesignPosition(Vec2(x, y), bTop, bBottom, bLeft, bRight));
}

void STVisibleRect::setPosition(cocos2d::Node *node, cocos2d::Vec2 pt, bool bTop, bool bBottom, bool bLeft, bool bRight)
{
    node->setPosition(getRealPositionForDesignPosition(pt, bTop, bBottom, bLeft, bRight));
}

Vec2 STVisibleRect::getRealPositionForDesignPosition(cocos2d::Vec2 aPosition, bool bTop, bool bBottom, bool bLeft, bool bRight)
{
    float posX = aPosition.x + s_DesignOffset.x;
	float posY = aPosition.y + s_DesignOffset.y;
    
	if(bTop)
		posY += s_DesignOffset.y;
	else if(bBottom)
		posY -= s_DesignOffset.y;
    if(bLeft)
        posX -= s_DesignOffset.x;
    else if(bRight)
        posX += s_DesignOffset.x;
    return Vec2(posX, posY);
}


Size& STVisibleRect::getRealDesignSize()
{
	return s_RealDesignSize;
}

Size& STVisibleRect::getDesignSize()
{
	return s_DesignSize;
}

Vec2 STVisibleRect::getDesignCeneter()
{
    return Vec2(s_DesignSize.width * .5f, s_DesignSize.height * .5f);
}

Size& STVisibleRect::getFrameSize()
{
	return s_FrameSize;
}

float STVisibleRect::getRealDesignScale()
{
	return s_RealDesignScale;
}

float STVisibleRect::getRelativeScale()
{
    return _relativeScale;
}

Vec2 STVisibleRect::getDesignOffset()
{
	return s_DesignOffset;
}

Vec2 STVisibleRect::getCenterOfScene()
{
    return Vec2(getPointOfSceneLeftBottom().x + s_FrameSize.width * .5f, getPointOfSceneLeftBottom().y + s_FrameSize.height* .5f);
}

Vec2 STVisibleRect::getPointOfSceneLeftBottom()
{
    return Director::getInstance()->getVisibleOrigin();
}

Vec2 STVisibleRect::getPointOfSceneRightUp()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    return Vec2(getPointOfSceneLeftBottom().x + visibleSize.width, getPointOfSceneLeftBottom().y + visibleSize.height);
}

Rect STVisibleRect::getBottomMoveRect(cocos2d::Node *apNode)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Rect rect(getPointOfSceneLeftBottom().x - apNode->getContentSize().width * (1.0f - apNode->getAnchorPoint().x),
                - apNode->getContentSize().height * (1.0f - apNode->getAnchorPoint().y),
                visibleSize.width + + apNode->getContentSize().width,
                apNode->getContentSize().height);
    return rect;
}

Rect STVisibleRect::getTopMoveRect(cocos2d::Node *apNode)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Rect rect(getPointOfSceneLeftBottom().x, visibleSize.height - apNode->getContentSize().height * (1.0f - apNode->getAnchorPoint().y),
                visibleSize.width, apNode->getContentSize().height);
    return rect;
}

Rect STVisibleRect::getLeftMoveRect(cocos2d::Node *apNode)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Rect rect(getPointOfSceneLeftBottom().x - apNode->getContentSize().width * (1.0f - apNode->getAnchorPoint().x),
                getPointOfSceneLeftBottom().y,
                apNode->getContentSize().width, visibleSize.height);
    return rect;
}

Rect STVisibleRect::getRightMoveRect(cocos2d::Node *apNode)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Rect rect(getPointOfSceneRightUp().x - apNode->getContentSize().width * (1.0f - apNode->getAnchorPoint().x),
               getPointOfSceneLeftBottom().y- apNode->getContentSize().height * (1.0f - apNode->getAnchorPoint().y),
                apNode->getContentSize().width, visibleSize.height);
    return rect;
}

bool STVisibleRect::isHighVersion()
{
    
    return false;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    STSystemFunction sf;
    if (sf.systemVersionHigh10())
    {
        return true;
    }else
    {
        return false;
    }
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return false;
#endif
}

bool STVisibleRect::isIpad()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    STSystemFunction sf;
    if (sf.isIpad())
    {
        return true;
    }else
    {
        return false;
    }
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    Size frameSize =  Director::getInstance()->getOpenGLView()->getFrameSize();
    float scale = frameSize.width/frameSize.height;
    if(scale > 0.7)
    {
        return true;
    }else
    {
        return false;
    }
#endif
}

/*
 * @brief        画圆角矩形模版，并裁剪节点
 * @param        origin            矩形开始点
 * @param        destination        矩形结束点
 * @param        radius            圆角半径
 * @param        segments        圆角等份数，等份越多，圆角越平滑
 * @attention
 */
ClippingNode* STVisibleRect::drawRoundRect(Node* newNode, float radius, unsigned int segments){
//    Point origin = newNode->getPosition();
//    Point destination = Point(newNode->getPosition().x + newNode->getContentSize().width, newNode->getPosition().y + newNode->getContentSize().height);
    
    Point origin = newNode->getBoundingBox().origin;
    Point destination = Vec2(newNode->getBoundingBox().getMaxX(), newNode->getBoundingBox().getMaxY());
    
    ClippingNode* pClip = ClippingNode::create();
    
    pClip->setInverted(false);//设置是否反向，将决定画出来的圆是透明的还是黑色的
    pClip->setAnchorPoint(Point(0, 0));
    
    
    //算出1/4圆
    const float coef = 0.5f * (float)M_PI / segments;
    Point * vertices = new Point[segments + 1];
    Point * thisVertices = vertices;
    for (unsigned int i = 0; i <= segments; ++i, ++thisVertices)
    {
        float rads = (segments - i)*coef;
        thisVertices->x = (int)(radius * sinf(rads));
        thisVertices->y = (int)(radius * cosf(rads));
    }
    //
    Point tagCenter;
    float minX = MIN(origin.x, destination.x);
    float maxX = MAX(origin.x, destination.x);
    float minY = MIN(origin.y, destination.y);
    float maxY = MAX(origin.y, destination.y);
    
    unsigned int dwPolygonPtMax = (segments + 1) * 4;
    Point * pPolygonPtArr = new Point[dwPolygonPtMax];
    Point * thisPolygonPt = pPolygonPtArr;
    int aa = 0;
    //左上角
    tagCenter.x = minX + radius;
    tagCenter.y = maxY - radius;
    thisVertices = vertices;
    for (unsigned int i = 0; i <= segments; ++i, ++thisPolygonPt, ++thisVertices)
    {
        thisPolygonPt->x = tagCenter.x - thisVertices->x;
        thisPolygonPt->y = tagCenter.y + thisVertices->y;
        // log("%f , %f", thisPolygonPt->x, thisPolygonPt->y);
        ++aa;
    }
    //右上角
    tagCenter.x = maxX - radius;
    tagCenter.y = maxY - radius;
    thisVertices = vertices + segments;
    for (unsigned int i = 0; i <= segments; ++i, ++thisPolygonPt, --thisVertices)
    {
        thisPolygonPt->x = tagCenter.x + thisVertices->x;
        thisPolygonPt->y = tagCenter.y + thisVertices->y;
        // log("%f , %f", thisPolygonPt->x, thisPolygonPt->y);
        ++aa;
    }
    //右下角
    tagCenter.x = maxX - radius;
    tagCenter.y = minY + radius;
    thisVertices = vertices;
    for (unsigned int i = 0; i <= segments; ++i, ++thisPolygonPt, ++thisVertices)
    {
        thisPolygonPt->x = tagCenter.x + thisVertices->x;
        thisPolygonPt->y = tagCenter.y - thisVertices->y;
        // log("%f , %f", thisPolygonPt->x, thisPolygonPt->y);
        ++aa;
    }
    //左下角
    tagCenter.x = minX + radius;
    tagCenter.y = minY + radius;
    thisVertices = vertices + segments;
    for (unsigned int i = 0; i <= segments; ++i, ++thisPolygonPt, --thisVertices)
    {
        thisPolygonPt->x = tagCenter.x - thisVertices->x;
        thisPolygonPt->y = tagCenter.y - thisVertices->y;
        // log("%f , %f", thisPolygonPt->x, thisPolygonPt->y);
        ++aa;
    }
    
    //设置参数
    static Color4F red(0, 0, 0, 1);//顶点颜色设置为红色，参数是R,G,B,透明度
    
    //注意不要将pStencil addChild
    DrawNode *pStencil = DrawNode::create();
    
    
    pStencil->drawPolygon(pPolygonPtArr, dwPolygonPtMax, red, 0, red);//绘制这个多边形
    
    pStencil->setPosition(Point(0, 0));
    
    pClip->setStencil(pStencil);
    
    
    pClip->addChild(newNode, 1);
    pClip->setContentSize(newNode->getContentSize());
    

    CC_SAFE_DELETE_ARRAY(vertices);
    CC_SAFE_DELETE_ARRAY(pPolygonPtArr);
    

    
    return pClip;
}

Sprite * STVisibleRect::maskedSprite(Sprite *textureSprite)
{
    Sprite * maskSprite = Sprite::create("other/preview_pic_bg@2x.png");
    textureSprite->setScale(maskSprite->getContentSize().width/textureSprite->getContentSize().width);
    
    RenderTexture * renderTexture = RenderTexture::create(maskSprite->getContentSize().width, maskSprite->getContentSize().height);
    maskSprite->setPosition(Vec2(maskSprite->getContentSize().width / 2, maskSprite->getContentSize().height / 2));
    textureSprite->setPosition(Vec2(maskSprite->getContentSize().width / 2, maskSprite->getContentSize().height / 2));
   
    maskSprite->setBlendFunc((BlendFunc){GL_ONE, GL_ZERO});
    textureSprite->setBlendFunc((BlendFunc){GL_DST_ALPHA, GL_ZERO});

    renderTexture->begin();
    maskSprite->visit();
    textureSprite->visit();
    renderTexture->end();
    
    renderTexture->getSprite()->getTexture()->setAntiAliasTexParameters();
    
    Sprite * retval = Sprite::createWithTexture(renderTexture->getSprite()->getTexture());
//    retval->setFlipY(true);
    retval->setFlippedY(true);
    return retval;
}
