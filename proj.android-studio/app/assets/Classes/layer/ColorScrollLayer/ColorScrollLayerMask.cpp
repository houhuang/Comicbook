//
//  ColorScrollLayerMask.cpp
//  ColorFree
//
//  Created by houhuang on 15/9/1.
//
//

#include "ColorScrollLayerMask.h"
//#include "../../helper/ScreenAdapterHelper.h"
//#include "../../define/Globalmacro.h"

ColorScrollLayerMask* ColorScrollLayerMask::create(Size maskSize)
{
    ColorScrollLayerMask* layer = new ColorScrollLayerMask();
    if (layer && layer->init(maskSize))
    {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return NULL;
}

bool ColorScrollLayerMask::init(Size maskSize)
{
    if (!LayerColor::initWithColor(Color4B(248, 248, 248, 255))) return false;

    this->setContentSize(maskSize);
    this->ignoreAnchorPointForPosition(false);
    this->setAnchorPoint(Vec2(0.5, 0));
    
    this->addListener();
    
    return true;
}

ColorScrollLayerMask::~ColorScrollLayerMask()
{
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListenersForTarget(this);
}

void ColorScrollLayerMask::addListener()
{
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = [this](Touch* touch, Event* event)
    {
        Vec2 lPos = touch->getLocationInView();
        lPos = Director::getInstance()->convertToGL(lPos);
        lPos = this->getParent()->convertToNodeSpace(lPos);
        if (this->boundingBox().containsPoint(lPos))
        {
            return true;
        }
        return false;
    };
    
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}