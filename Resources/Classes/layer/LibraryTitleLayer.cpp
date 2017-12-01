//
//  LibraryTitleLayer.cpp
//  ColorGarden
//
//  Created by houhuang on 17/10/25.
//
//

#include "LibraryTitleLayer.hpp"
#include "STVisibleRect.h"
#include "AppConfigs.h"

#define THIS_SIZE this->getContentSize()
#define ANIMATE_TOTALTIME 0.75f

#define iapd_Space (V::isIpad()? 20:0)

LibraryTitleLayer* LibraryTitleLayer::create(Size size)
{
    LibraryTitleLayer* layer = new LibraryTitleLayer();
    if (layer && layer->init(size))
    {
        layer->autorelease();
        return layer;
    }
    
    CC_SAFE_DELETE(layer);
    return nullptr;
}

LibraryTitleLayer::LibraryTitleLayer()
{
    _leftSprite1 = nullptr;
    _leftSprite = nullptr;
    _centerSprite = nullptr;
    _rightSprite = nullptr;
    _rightSprite1 = nullptr;
    
    _isMoveTouchEnded = true;
    _timer = 0.0f;
    
    _SpriteName.push_back("other/tLeft.png");
    _SpriteName.push_back("other/tCenter.png");
    _SpriteName.push_back("other/tRight.png");
    
    _index = 1;
}

LibraryTitleLayer::~LibraryTitleLayer()
{
    _eventDispatcher->removeEventListenersForTarget(this);
}

bool LibraryTitleLayer::init(Size size)
{
    if (!LayerColor::initWithColor(Color4B(0, 100, 0, 0), size.height*2.481, size.height)) return  false;
//    this->setContentSize(size);
    this->ignoreAnchorPointForPosition(false);
    this->setAnchorPoint(Vec2(0.5, 1));
    
    
    this->initUILayer();
    this->schedule(schedule_selector(LibraryTitleLayer::timer), 0.0);
    return true;
}

void LibraryTitleLayer::initUILayer()
{
    Sprite* leftSprite1 = Sprite::create("other/tRight.png");
    leftSprite1->setScaleY(THIS_SIZE.height/leftSprite1->getContentSize().height);
    leftSprite1->setScaleX((THIS_SIZE.width-iapd_Space)/leftSprite1->getContentSize().width);
    leftSprite1->setPosition(Vec2(-1.5*THIS_SIZE.width, THIS_SIZE.height/2));
    this->addChild(leftSprite1);
    _leftSprite1 = leftSprite1;
    
    Sprite* leftSprite = Sprite::create("other/tLeft.png");
    leftSprite->setScaleY(THIS_SIZE.height/leftSprite->getContentSize().height);
    leftSprite->setScaleX((THIS_SIZE.width-iapd_Space)/leftSprite->getContentSize().width);
    leftSprite->setPosition(Vec2(-0.5*THIS_SIZE.width, THIS_SIZE.height/2));
    this->addChild(leftSprite);
    _leftSprite = leftSprite;
    
    Sprite* centerSprite = Sprite::create("other/tCenter.png");
    centerSprite->setScaleY(THIS_SIZE.height/centerSprite->getContentSize().height);
    centerSprite->setScaleX((THIS_SIZE.width-iapd_Space)/centerSprite->getContentSize().width);
    centerSprite->setPosition(Vec2(0.5*THIS_SIZE.width, THIS_SIZE.height/2));
    centerSprite->setTag(1);
    this->addChild(centerSprite);
    _centerSprite = centerSprite;
    
    Sprite* rightSprite = Sprite::create("other/tRight.png");
    rightSprite->setScaleY(THIS_SIZE.height/rightSprite->getContentSize().height);
    rightSprite->setScaleX((THIS_SIZE.width-iapd_Space)/rightSprite->getContentSize().width);
    rightSprite->setPosition(Vec2(1.5*THIS_SIZE.width, THIS_SIZE.height/2));
    this->addChild(rightSprite);
    _rightSprite = rightSprite;
    
    Sprite* rightSprite1 = Sprite::create("other/tLeft.png");
    rightSprite1->setScaleY(THIS_SIZE.height/rightSprite1->getContentSize().height);
    rightSprite1->setScaleX((THIS_SIZE.width-iapd_Space)/rightSprite1->getContentSize().width);
    rightSprite1->setPosition(Vec2(2.5*THIS_SIZE.width, THIS_SIZE.height/2));
    this->addChild(rightSprite1);
    _rightSprite1 = rightSprite1;
    
    Sprite* centerBg = Sprite::create("other/feature bg 1@2x.png");
    centerBg->setPosition(centerSprite->getContentSize()/2);
    centerSprite->addChild(centerBg, -1);
    
    Sprite* leftBG = Sprite::create("other/feature bg 1@2x.png");
    leftBG->setPosition(leftSprite->getContentSize()/2);
    leftSprite->addChild(leftBG, -1);
    
    Sprite* left1BG = Sprite::create("other/feature bg 1@2x.png");
    left1BG->setPosition(leftSprite1->getContentSize()/2);
    leftSprite1->addChild(left1BG, -1);
    
    Sprite* rightBG = Sprite::create("other/feature bg 1@2x.png");
    rightBG->setPosition(rightSprite->getContentSize()/2);
    rightSprite->addChild(rightBG, -1);
    
    Sprite* right1BG = Sprite::create("other/feature bg 1@2x.png");
    right1BG->setPosition(rightSprite1->getContentSize()/2);
    rightSprite1->addChild(right1BG, -1);
}

void LibraryTitleLayer::timer(float dt)
{
    if (_isMoveTouchEnded)
    {
        _timer += dt;
        if (_timer > 4)
        {
            _timer = 0;
            this->autoTowardLeft();
        }
    }
}

void LibraryTitleLayer::addListener(titleLayerCallback callback)
{
    _callback = callback;
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch* touch, Event* event)->bool{
        Vec2 lPos = touch->getLocation();
        _firstPos = lPos;
        lPos = this->getParent()->convertToNodeSpace(lPos);
        
        _accelerationX = 0;
        _isClickCenterSprite = false;
        
        float x = (V::getRealDesignSize().width - THIS_SIZE.width)/2;
        Rect rect = Rect(this->getBoundingBox().origin.x - x,
                         this->getBoundingBox().origin.y,
                         this->getBoundingBox().size.width + 2*x,
                         this->getBoundingBox().size.height);
        
        if (this->getBoundingBox().containsPoint(lPos))
        {
            _isClickCenterSprite = true;
        }
        
        if (rect.containsPoint(lPos))
        {
            if (_isMoveTouchEnded)
            {
                _isMoveTouchEnded = false;
                return true;
            }
 
        }
        
        return false;
    };
    
    listener->onTouchMoved = [this](Touch* touch, Event* event){
        Vec2 detal = touch->getDelta();
        _leftSprite1->setPosition(_leftSprite1->getPosition() + Vec2(detal.x ,0));
        _leftSprite->setPosition(_leftSprite->getPosition() + Vec2(detal.x ,0));
        _centerSprite->setPosition(_centerSprite->getPosition() + Vec2(detal.x ,0));
        _rightSprite->setPosition(_rightSprite->getPosition() + Vec2(detal.x ,0));
        _rightSprite1->setPosition(_rightSprite1->getPosition() + Vec2(detal.x ,0));
        _accelerationX = abs(detal.x);
    };
    
    listener->onTouchEnded = [this](Touch* touch, Event* event){
        float xOffset = touch->getLocation().x - _firstPos.x ;
        if (abs(xOffset) < 5 && _isClickCenterSprite)
        {
            if (_callback)
            {
                switch (_centerSprite->getTag())
                {
                    case 0:
                    {
                        _callback(Mode::LEFT);
                    }
                        break;
                    case 1:
                    {
                        _callback(Mode::CENTER);
                    }
                        break;
                    case 2:
                    {
                        _callback(Mode::RIGHT);
                    }
                        break;
                    default:
                        break;
                }
                
            }
        }
        
        if (xOffset < 0)
        {
            this->toWardLeft(xOffset);
        }else
        {
            this->toWardRight(xOffset);
        }
    };
    
    listener->onTouchCancelled = [this](Touch* touch, Event* event){
        float xOffset = _firstPos.x - touch->getLocation().x;
        if (xOffset < 0)
        {
            this->toWardLeft(xOffset);
        }else
        {
            this->toWardRight(xOffset);
        }
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void LibraryTitleLayer::autoTowardLeft()
{
    _isMoveTouchEnded = false;
    float time = ANIMATE_TOTALTIME/2;
    
    
    _leftSprite1->setPosition(Vec2(2.5*THIS_SIZE.width, THIS_SIZE.height/2));
    _leftSprite->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(-1.5*THIS_SIZE.width, THIS_SIZE.height/2))));
    _rightSprite1->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(1.5*THIS_SIZE.width, THIS_SIZE.height/2))));
    
    _centerSprite->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(-0.5*THIS_SIZE.width, THIS_SIZE.height/2))));
    _rightSprite->runAction(Sequence::create(EaseSineOut::create(MoveTo::create(time, Vec2(THIS_SIZE/2))), CallFunc::create([this](){
        this->updatePointerFromLeft();
    }),NULL));
}

void LibraryTitleLayer::toWardLeft(float xOffset)
{
    if (xOffset < -THIS_SIZE.width/2)
    {
        float time = (THIS_SIZE.width - abs(xOffset))/THIS_SIZE.width * ANIMATE_TOTALTIME;
        
        _leftSprite1->setPosition(Vec2(2.5*THIS_SIZE.width, THIS_SIZE.height/2));
        _leftSprite->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(-1.5*THIS_SIZE.width, THIS_SIZE.height/2))));
        _rightSprite1->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(1.5*THIS_SIZE.width, THIS_SIZE.height/2))));
        
        
        _centerSprite->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(-0.5*THIS_SIZE.width, THIS_SIZE.height/2))));
        _rightSprite->runAction(Sequence::create(EaseSineOut::create(MoveTo::create(time, Vec2(THIS_SIZE/2))), CallFunc::create([this](){
            this->updatePointerFromLeft();
        }),NULL));
    }else
    {
        if (_accelerationX > ACCELERATION_X)
        {
            float time = (THIS_SIZE.width - abs(xOffset))/THIS_SIZE.width * ANIMATE_TOTALTIME/2;
            
            _leftSprite1->setPosition(Vec2(2.5*THIS_SIZE.width, THIS_SIZE.height/2));
            _leftSprite->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(-1.5*THIS_SIZE.width, THIS_SIZE.height/2))));
            _rightSprite1->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(1.5*THIS_SIZE.width, THIS_SIZE.height/2))));
            
            _centerSprite->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(-0.5*THIS_SIZE.width, THIS_SIZE.height/2))));
            _rightSprite->runAction(Sequence::create(EaseSineOut::create(MoveTo::create(time, Vec2(THIS_SIZE/2))), CallFunc::create([this](){
                this->updatePointerFromLeft();
            }),NULL));
        }else
        {
            float time = abs(xOffset)/THIS_SIZE.width * ANIMATE_TOTALTIME;
            
            _leftSprite1->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(-1.5*THIS_SIZE.width, THIS_SIZE.height/2))));
            _rightSprite1->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(2.5*THIS_SIZE.width, THIS_SIZE.height/2))));
            
            _leftSprite->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(-0.5*THIS_SIZE.width, THIS_SIZE.height/2))));
            _centerSprite->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(0.5*THIS_SIZE.width, THIS_SIZE.height/2))));
            _rightSprite->runAction(Sequence::create(EaseSineOut::create(MoveTo::create(time, Vec2(1.5*THIS_SIZE.width ,THIS_SIZE.height/2))), CallFunc::create([this](){
                _isMoveTouchEnded = true;
                _timer = 0;
            }),NULL));
        }
        
        
    }
}

void LibraryTitleLayer::toWardRight(float xOffset)
{
    if (xOffset > THIS_SIZE.width/2)
    {
        float time = (THIS_SIZE.width - abs(xOffset))/THIS_SIZE.width * ANIMATE_TOTALTIME;
        
        _leftSprite1->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(-0.5*THIS_SIZE.width, THIS_SIZE.height/2))));
        _rightSprite->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(2.5*THIS_SIZE.width, THIS_SIZE.height/2))));
        _rightSprite1->setPosition(Vec2(-1.5*THIS_SIZE.width, THIS_SIZE.height/2));
        
        _centerSprite->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(1.5*THIS_SIZE.width, THIS_SIZE.height/2))));
        _leftSprite->runAction(Sequence::create(EaseSineOut::create(MoveTo::create(time, Vec2(THIS_SIZE/2))), CallFunc::create([this](){
            this->updatePointerFromRight();
        }),NULL));
    }else
    {
        if (_accelerationX > ACCELERATION_X)
        {
            float time = (THIS_SIZE.width - abs(xOffset))/THIS_SIZE.width * ANIMATE_TOTALTIME/2;
            
            _leftSprite1->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(-0.5*THIS_SIZE.width, THIS_SIZE.height/2))));
            _rightSprite->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(2.5*THIS_SIZE.width, THIS_SIZE.height/2))));
            _rightSprite1->setPosition(Vec2(-1.5*THIS_SIZE.width, THIS_SIZE.height/2));
            
            
            _centerSprite->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(1.5*THIS_SIZE.width, THIS_SIZE.height/2))));
            _leftSprite->runAction(Sequence::create(EaseSineOut::create(MoveTo::create(time, Vec2(THIS_SIZE/2))), CallFunc::create([this](){
                this->updatePointerFromRight();
            }),NULL));
        }else
        {
            float time = abs(xOffset)/THIS_SIZE.width * ANIMATE_TOTALTIME;
            
            _leftSprite1->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(-1.5*THIS_SIZE.width, THIS_SIZE.height/2))));
            _rightSprite1->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(2.5*THIS_SIZE.width, THIS_SIZE.height/2))));
            
            
            _leftSprite->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(-0.5*THIS_SIZE.width, THIS_SIZE.height/2))));
            _centerSprite->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(0.5*THIS_SIZE.width, THIS_SIZE.height/2))));
            _rightSprite->runAction(Sequence::create(EaseSineOut::create(MoveTo::create(time, Vec2(1.5*THIS_SIZE.width ,THIS_SIZE.height/2))), CallFunc::create([this](){
                _isMoveTouchEnded = true;
                _timer = 0;
            }),NULL));
        }
        
        
    }
}

void LibraryTitleLayer::updatePointerFromLeft()
{
    Sprite* lSprite = _leftSprite1;
    _leftSprite1 = _leftSprite;
    _leftSprite = _centerSprite;
    _centerSprite = _rightSprite;
    _rightSprite = _rightSprite1;
    _rightSprite1 = lSprite;
    
    _isMoveTouchEnded = true;
    _timer = 0;
    ++_index;
    if (_index > 2)
    {
        _index = 0;
    }
    
    _centerSprite->setTexture(_SpriteName.at(_index));
    _centerSprite->setTag(_index);
    if (_index == 0)
    {
        _leftSprite->setTexture(_SpriteName.at(2));
        _leftSprite1->setTexture(_SpriteName.at(1));
        _rightSprite->setTexture(_SpriteName.at(1));
        _rightSprite1->setTexture(_SpriteName.at(2));
    }else if (_index == 1)
    {
        _leftSprite->setTexture(_SpriteName.at(0));
        _leftSprite1->setTexture(_SpriteName.at(2));
        _rightSprite->setTexture(_SpriteName.at(2));
        _rightSprite1->setTexture(_SpriteName.at(0));
    }else
    {
        _leftSprite->setTexture(_SpriteName.at(1));
        _leftSprite1->setTexture(_SpriteName.at(0));
        _rightSprite->setTexture(_SpriteName.at(0));
        _rightSprite1->setTexture(_SpriteName.at(1));
    }
}

void LibraryTitleLayer::updatePointerFromRight()
{
    Sprite* lSprite = _rightSprite1;
    _rightSprite1 = _rightSprite;
    _rightSprite = _centerSprite;
    _centerSprite = _leftSprite;
    _leftSprite = _leftSprite1;
    _leftSprite1 = lSprite;
    --_index;
    if (_index < 0)
    {
        _index = 2;
    }
    
    _centerSprite->setTexture(_SpriteName.at(_index));
    _centerSprite->setTag(_index);
    if (_index == 0)
    {
        _leftSprite->setTexture(_SpriteName.at(2));
        _leftSprite1->setTexture(_SpriteName.at(1));
        _rightSprite->setTexture(_SpriteName.at(1));
        _rightSprite1->setTexture(_SpriteName.at(2));
    }else if (_index == 1)
    {
        _leftSprite->setTexture(_SpriteName.at(0));
        _leftSprite1->setTexture(_SpriteName.at(2));
        _rightSprite->setTexture(_SpriteName.at(2));
        _rightSprite1->setTexture(_SpriteName.at(0));
    }else
    {
        _leftSprite->setTexture(_SpriteName.at(1));
        _leftSprite1->setTexture(_SpriteName.at(0));
        _rightSprite->setTexture(_SpriteName.at(0));
        _rightSprite1->setTexture(_SpriteName.at(1));
    }
    
    _isMoveTouchEnded = true;
    _timer = 0;
}
