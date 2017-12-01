//
//  ColorSprite.cpp
//  ColorGarden
//
//  Created by lidong on 15/8/4.
//
//

#include "ColorSprite.h"
#include "AppConfigs.h"
#include "STVisibleRect.h"
//#include "../helper/ScreenAdapterHelper.h"

ColorSprite* ColorSprite::create(string filename)
{
    ColorSprite* lColorSprite =new ColorSprite();
    if(lColorSprite && lColorSprite->initWithFile(filename))
    {
        lColorSprite->autorelease();
        return lColorSprite;
    }
    else
    {
        delete lColorSprite;
        lColorSprite = NULL;
        return NULL;
    }
}

ColorSprite::ColorSprite()
{

}

ColorSprite::~ColorSprite()
{
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListenersForTarget(this);
}

bool ColorSprite::initWithFile(const std::string& filename)
{
    if (!Sprite::initWithFile(filename)) return false;
    _isSelected=false;
    _selectSprite=NULL;
  
    return true;
}

void ColorSprite::setClickSpite(bool isShow)
{
    if (_selectSprite)
    {
        return;
    }

    Color3B color = this->getColor();
    
    if (color.r >= 250 && color.g >= 250 && color.b >= 250)
    {
        Sprite* lBg = Sprite::create("cocos/other/white.png");
        lBg->setPosition(this->getContentSize()/2);
//        this->addChild(lBg);
    }

    if (color.r >= 238 && color.g >= 238 && color.b >= 238)
    {
        _selectSprite=Sprite::create("cocos/other/clickColor.png");
//        _selectSprite->setColor(Color3B::BLACK);
    }else
    {
        _selectSprite=Sprite::create("cocos/other/clickColor.png");
    }

    _selectSprite->setPosition(Vec2(this->getContentSize().width/2,this->getContentSize().height/2));
    this->addChild(_selectSprite);
    _selectSprite->setVisible(isShow);
}

void ColorSprite::addListener(SEL_CallFuncN callBackFun, Node* target, bool isSwallow /* = false */)
{
    _selectedCallBack = callBackFun;
    _target = target;
    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(ColorSprite::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(ColorSprite::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(ColorSprite::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(ColorSprite::onTouchCancelled, this);
    listener->setSwallowTouches(isSwallow);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool ColorSprite::onTouchBegan(Touch *touch, Event *unused_event)
{
    Vec2 lPos = touch->getLocationInView();
    m_vFirstPoint = lPos;
    lPos = Director::getInstance()->convertToGL(lPos);
    lPos = this->getParent()->convertToNodeSpace(lPos);
    
    Vec2 viewPos = touch->getLocationInView();
    
    float minX = (V::getRealDesignSize().width- PALETTE_SIZE.width)/2;
    float maxX = minX + PALETTE_SIZE.width;
    if (viewPos.x < minX || viewPos.x > maxX)
    {
        return false;
    }
    
    
//    if (m_bIsEnableTouch)
    {
        if (this->boundingBox().containsPoint(lPos))
        {
            
            return true;
        }
    }
    
    return false;
}

void ColorSprite::onTouchMoved(Touch *touch, Event *unused_event)
{
    
}

void ColorSprite::onTouchEnded(Touch *touch, Event *unused_event)
{
    Vec2 lPos = touch->getLocationInView();
    //    lPos = Director::getInstance()->convertToGL(lPos);
    //    lPos = this->getParent()->convertToNodeSpace(lPos);
    
    float distance = sqrt(pow(lPos.x - m_vFirstPoint.x, 2) + pow(lPos.y - m_vFirstPoint.y, 2));
    if (distance < 15)
    {
        //        this->selected(true);
        if (_target && _selectedCallBack)
        {
            (_target->*_selectedCallBack)(this);
        }
    }
}

void ColorSprite::onTouchCancelled(Touch *touch, Event *unused_event)
{
    
}

void ColorSprite::responseTouchEvent(Vec2 pPoint)
{
//    Vec2 lPosition=this->getParent()->convertToNodeSpace(pPoint);
//    if(this->boundingBox().containsPoint(lPosition))
    {
        this->selected(true);
        this->getDelegate()->reponseTouchEvent(this);
    }
 
}

void ColorSprite::selected(bool pBool)
{
    this->setIsSelected(pBool);
    
    if(pBool)
    {
        _selectSprite->setVisible(true);
        _selectSprite->setOpacity(0);
        _selectSprite->runAction(FadeIn::create(0.2f));
  
    }
    else
    {
        _selectSprite->setVisible(false);
    }
    
}

void ColorSprite::selected(bool pBool, bool isNotAnimation)
{
    this->setIsSelected(pBool);
    
    if(pBool)
    {
        _selectSprite->setVisible(true);
        
    }
    else
    {
        _selectSprite->setVisible(false);
    }
}