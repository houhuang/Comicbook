//
//  ColorLoopScrollLayer.cpp
//  ColorGarden
//
//  Created by houhuang on 17/10/23.
//
//

#include "ColorLoopScrollLayer.hpp"
#include "STVisibleRect.h"
#include "UIColorManager.hpp"
#include "UICpp.h"

#define COLOR_SPACE (V::isIpad()? 3 : 5)

ColorLoopScrollLayer* ColorLoopScrollLayer::create()
{
    ColorLoopScrollLayer* layer = new ColorLoopScrollLayer();
    if (layer && layer->init())
    {
        layer->autorelease();
        return layer;
    }
    
    CC_SAFE_DELETE(layer);
    return nullptr;
}

ColorLoopScrollLayer::ColorLoopScrollLayer()
{
    _leftLayer1 = nullptr;
    _leftLayer = nullptr;
    _centerLayer = nullptr;
    _rightLayer = nullptr;
    _rightLayer1 = nullptr;
    
    _currentChooseSprite = nullptr;
    
    _isMoveTouchEnded = true;
    
    _vColorDatas = xPallette->getData();
    _iCurrentPage = xPallette->getPalletteIndex();
    _iMaxPage = (int)_vColorDatas.size() - 1;
    
    _currentChooseColor = xPallette->getCurrentColor();
    _currentChooseSpriteWherePage = xPallette->getPalletteIndex();
}

ColorLoopScrollLayer::~ColorLoopScrollLayer()
{
    _eventDispatcher->removeEventListenersForTarget(this);
}

bool ColorLoopScrollLayer::init()
{
    if (!LayerColor::initWithColor(Color4B(0, 100, 0, 0), PALETTE_SIZE.width, PALETTE_SIZE.height)) return false;
//    this->setScale(V::getRealDesignSize().width/640);
    this->ignoreAnchorPointForPosition(false);
    this->setAnchorPoint(Vec2(0.5, 1));
    
    this->initUILayer();
    
    this->addListener();
    return true;
}

void ColorLoopScrollLayer::initUILayer()
{
    LayerColor* leftLayer1 = LayerColor::create(Color4B(100, 100, 0, 0), PALETTE_SIZE.width, PALETTE_SIZE.height);
    leftLayer1->ignoreAnchorPointForPosition(false);
    leftLayer1->setAnchorPoint(Vec2(0.5, 0.5));
    leftLayer1->setPosition(Vec2(-1.5*PALETTE_SIZE.width, PALETTE_SIZE.height/2));
    this->addChild(leftLayer1);
    _leftLayer1 = leftLayer1;
    
    LayerColor* leftLayer = LayerColor::create(Color4B(100, 0, 0, 0), PALETTE_SIZE.width, PALETTE_SIZE.height);
    leftLayer->ignoreAnchorPointForPosition(false);
    leftLayer->setAnchorPoint(Vec2(0.5, 0.5));
    leftLayer->setPosition(Vec2(-0.5*PALETTE_SIZE.width, PALETTE_SIZE.height/2));
    this->addChild(leftLayer);
    _leftLayer = leftLayer;
    
    LayerColor* centerLayer = LayerColor::create(Color4B(0, 100, 0, 0), PALETTE_SIZE.width, PALETTE_SIZE.height);
    centerLayer->ignoreAnchorPointForPosition(false);
    centerLayer->setAnchorPoint(Vec2(0.5, 0.5));
    centerLayer->setPosition(this->getContentSize()/2);
    this->addChild(centerLayer);
    _centerLayer = centerLayer;
    
    
    LayerColor* rightLayer = LayerColor::create(Color4B(0, 0, 100, 0), PALETTE_SIZE.width, PALETTE_SIZE.height);
    rightLayer->ignoreAnchorPointForPosition(false);
    rightLayer->setAnchorPoint(Vec2(0.5, 0.5));
    rightLayer->setPosition(Vec2(1.5*PALETTE_SIZE.width, PALETTE_SIZE.height/2));
    this->addChild(rightLayer);
    _rightLayer = rightLayer;
    
    LayerColor* rightLayer1 = LayerColor::create(Color4B(0, 100, 100, 0), PALETTE_SIZE.width, PALETTE_SIZE.height);
    rightLayer1->ignoreAnchorPointForPosition(false);
    rightLayer1->setAnchorPoint(Vec2(0.5, 0.5));
    rightLayer1->setPosition(Vec2(2.5*PALETTE_SIZE.width, PALETTE_SIZE.height/2));
    this->addChild(rightLayer1);
    _rightLayer1 = rightLayer1;
    
    this->updateColorLayer(UpdateObj::ALL);
}

void ColorLoopScrollLayer::updateColorLayer(int page, Color3B color)
{
    _currentChooseColor = color;
    _iCurrentPage = page;
    this->updateColorLayer(ColorLoopScrollLayer::UpdateObj::ALL);
}

void ColorLoopScrollLayer::updateColorLayer(UpdateObj updateObj)
{
    int centerIdx = _iCurrentPage;
    int leftIdx = (_iCurrentPage-1)<0 ? _iMaxPage : (_iCurrentPage-1);
    int rightIdx = (_iCurrentPage+1)>_iMaxPage ? 0 : (_iCurrentPage+1);
    int leftIdx1,rightIdx1;
    
    if (_iCurrentPage-2 < 0)
    {
        if (_iCurrentPage == 1)
        {
            leftIdx1 = _iMaxPage;
        }else
        {
            leftIdx1 = _iMaxPage-1;
        }
    }else
    {
        leftIdx1 = _iCurrentPage-2;
    }
    
    if (_iCurrentPage+2 > _iMaxPage)
    {
        if (_iCurrentPage == _iMaxPage-1)
        {
            rightIdx1 = 0;
        }else
        {
            rightIdx1 = 1;
        }
    }else
    {
        rightIdx1 = _iCurrentPage+2;
    }
    
    
    PalletteInfo leftColorInfo1 = _vColorDatas.at(leftIdx1);
    PalletteInfo leftColorInfo = _vColorDatas.at(leftIdx);
    PalletteInfo centerColorInfo = _vColorDatas.at(centerIdx);
    PalletteInfo rightColorInfo = _vColorDatas.at(rightIdx);
    PalletteInfo rightColorInfo1 = _vColorDatas.at(rightIdx1);
    
    if (updateObj == UpdateObj::ALL)
    {
        this->createColorLayer(leftColorInfo1, _leftLayer1);
        this->createColorLayer(leftColorInfo, _leftLayer);
        this->createColorLayer(centerColorInfo, _centerLayer);
        this->createColorLayer(rightColorInfo, _rightLayer);
        this->createColorLayer(rightColorInfo1, _rightLayer1);
    }else if (updateObj == UpdateObj::LEFT)
    {
        this->createColorLayer(leftColorInfo1, _leftLayer1);
    }else if (updateObj == UpdateObj::RIGHT)
    {
        this->createColorLayer(rightColorInfo1, _rightLayer1);
    }
    
}

void ColorLoopScrollLayer::createColorLayer(PalletteInfo info, LayerColor* layer)
{
    
    layer->removeAllChildrenWithCleanup(true);
    
    if (V::isIpad())
    {
        float fontSize, scale;
        if (V::isHighVersion())
        {
            fontSize = 35 * 0.8;
            scale = 0.5/0.8;
        }else
        {
            fontSize = 35;
            scale = 0.5;
        }
        
        Label* text = Label::createWithTTF(info.name, FONT_NAME, fontSize);
        text->setAnchorPoint(Vec2(0.5, 0));
        text->setPosition(Vec2(PALETTE_SIZE.width/2, 2));
        text->setScale(scale);
        text->setColor(xUcm->findUIColor("game_colorboard"));
        layer->addChild(text);
        
        for (int i = 0; i < 11; ++i)
        {
            ColorSprite* lSprite = ColorSprite::create("cocos/other/color01.png");
            lSprite->setAnchorPoint(Vec2(0.5, 0.5));
            lSprite->setIndex(i);
            lSprite->setScale(0.3278f );
            
            log("%f", V::getRealDesignScale());
            
            float width = lSprite->getBoundingBox().size.width;
            
            float x = (PALETTE_SIZE.width - 10 * COLOR_SPACE - 11*width)/2 + (i + 0.5)*width + i*COLOR_SPACE;

            lSprite->setPosition(Vec2(x , PALETTE_SIZE.height/2));
            
            int index = i;
//            if (i == 10 && (info.name == "Skin Tones" || info.name == "Blacks&Whites" || info.name == "Reds" || info.name == "Greens" || info.name == "Oranges" || info.name == "Purples" || info.name == "Blues" || info.name == "Dreams" || info.name == "Sunset" || info.name == "Sky" || info.name == "Wildlife" || info.name == "Paradise" || info.name == "Cyans"))
//            {
//                index = 15;
//            }
//            
//            if (i == 12 && info.name == "Yellows")
//            {
//                index = 16;
//            }
            
            lSprite->addListener(callfuncN_selector(ColorLoopScrollLayer::reponseColorSpriteTouch), this);
            lSprite->setColor(Color3B(info.vRGBInfo.at(index).r, info.vRGBInfo.at(index).g, info.vRGBInfo.at(index).b));
            lSprite->setPalletteInfo(info);
            layer->addChild(lSprite);
            
            lSprite->setClickSpite(false);
            if (info.vRGBInfo.at(index).r == _currentChooseColor.r && info.vRGBInfo.at(index).g == _currentChooseColor.g && info.vRGBInfo.at(index).b == _currentChooseColor.b)
            {
                //            lSprite->responseTouchEvent(Vec2(0, 0));
                lSprite->selected(true);
                _currentChooseSprite = lSprite;
            }
            
        }
    }else
    {
        float fontSize, scale;
        if (V::isHighVersion())
        {
            fontSize = 60 * 0.5;
            scale = 0.5/0.5;
        }else
        {
            fontSize = 60;
            scale = 0.5;
        }
        
        Label* text = Label::createWithTTF(info.name, FONT_NAME, fontSize);
        text->setAnchorPoint(Vec2(0.5, 0.5));
        text->setPosition(Vec2(PALETTE_SIZE.width/2, 40));
        text->setScale(scale);
        text->setColor(xUcm->findUIColor("game_colorboard"));
        layer->addChild(text);
        
        for (int i = 0; i < 11; ++i)
        {
            ColorSprite* lSprite = ColorSprite::create("cocos/other/color01.png");
            lSprite->setAnchorPoint(Vec2(0.5, 0.5));
            lSprite->setIndex(i);
            lSprite->setScale(0.5f);
            
            float width = lSprite->getBoundingBox().size.width;
            
            float lX,lY;
            
            float offset = (PALETTE_SIZE.width - 6 * width - 5 * COLOR_SPACE)/2;
            if (i < 5)
            {
                lX = offset + 35 + width/2 - 1 + i * (width + COLOR_SPACE);
                lY = PALETTE_SIZE.height/3*2 + 15 - 2;
            }else if (i < 11)
            {
                lX = offset + width/2 + (i-5) * (width + COLOR_SPACE);
                lY = PALETTE_SIZE.height/3 + 15 + 9 - 6;
            }
            
            
            lSprite->setPosition(Vec2(lX , lY + 5));
            
            int index = i;
//            if (i == 10 && (info.name == "Skin Tones" || info.name == "Blacks&Whites" || info.name == "Reds" || info.name == "Greens" || info.name == "Oranges" || info.name == "Purples" || info.name == "Blues" || info.name == "Dreams" || info.name == "Sunset" || info.name == "Sky" || info.name == "Wildlife" || info.name == "Paradise" || info.name == "Cyans"))
//            {
//                index = 15;
//            }
//            
//            if (i == 12 && info.name == "Yellows")
//            {
//                index = 16;
//            }
            
            lSprite->addListener(callfuncN_selector(ColorLoopScrollLayer::reponseColorSpriteTouch), this);
            lSprite->setColor(Color3B(info.vRGBInfo.at(index).r, info.vRGBInfo.at(index).g, info.vRGBInfo.at(index).b));
            lSprite->setPalletteInfo(info);
            layer->addChild(lSprite);
            
            lSprite->setClickSpite(false);
            if (info.vRGBInfo.at(index).r == _currentChooseColor.r && info.vRGBInfo.at(index).g == _currentChooseColor.g && info.vRGBInfo.at(index).b == _currentChooseColor.b)
            {
                //            lSprite->responseTouchEvent(Vec2(0, 0));
                lSprite->selected(true, true);
                _currentChooseSprite = lSprite;
            }
            
        }
    }
    
    
}

void ColorLoopScrollLayer::reponseColorSpriteTouch(Node* pSender)
{
    ColorSprite* lSprite = (ColorSprite*)pSender;
    ColorIndex colorIndex;
    colorIndex.page = _iCurrentPage;
    colorIndex.index = lSprite->getIndex();
    _currentChooseColor = xPallette->getColor(_iCurrentPage, lSprite->getIndex());
    _eventDispatcher->dispatchCustomEvent("event_color", (void*)&colorIndex);
    

    if (this->currentPageIsExist(_currentChooseSpriteWherePage))
    {
        _currentChooseSprite->selected(false);
        _currentChooseSprite = nullptr;
    }
    
    _currentChooseSprite = lSprite;
    _currentChooseSpriteWherePage = _iCurrentPage;
    lSprite->selected(true);
}

void ColorLoopScrollLayer::addListener()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch* touch, Event* event)->bool{
        
        Vec2 lPos = touch->getLocation();
        _firstPos = lPos;
        lPos = this->getParent()->convertToNodeSpace(lPos);
        if (this->boundingBox().containsPoint(lPos))
        {
            if (_isMoveTouchEnded)
            {
                _isMoveTouchEnded = false;
                return true;
            }else
            {
                return false;
            }
            
        }
        
        return false;
    };
    
    listener->onTouchMoved = [this](Touch* touch, Event* event){
        Vec2 offset = touch->getDelta();
        _leftLayer1->setPosition(_leftLayer1->getPosition() + Vec2(offset.x, 0));
        _leftLayer->setPosition(_leftLayer->getPosition() + Vec2(offset.x, 0));
        _centerLayer->setPosition(_centerLayer->getPosition() + Vec2(offset.x, 0));
        _rightLayer->setPosition(_rightLayer->getPosition() + Vec2(offset.x, 0));
        _rightLayer1->setPosition(_rightLayer1->getPosition() + Vec2(offset.x, 0));
        
        _accelerationX = abs(offset.x);
    };
    
    listener->onTouchEnded = [this](Touch* touch, Event* event){
        float offset = touch->getLocation().x - _firstPos.x;
        if (offset < 0)
        {
            this->towardLeft(offset);
        }else
        {
            this->towardRight(offset);
        }

    };
    
    listener->onTouchCancelled = [this](Touch* touch, Event* event){
        float offset = touch->getLocation().x - _firstPos.x;
        if (offset < 0)
        {
            this->towardLeft(offset);
        }else
        {
            this->towardRight(offset);
        }
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void ColorLoopScrollLayer::towardLeft(float xReadyOffset)
{
    
    if (xReadyOffset < -PALETTE_SIZE.width/2)
    {
        float time = (PALETTE_SIZE.width + xReadyOffset)/PALETTE_SIZE.width * 0.7f;
        ++_iCurrentPage;
        if (_iCurrentPage > _iMaxPage)
        {
            _iCurrentPage = 0;
        }
        
        _leftLayer1->setPosition(Vec2(2.5*PALETTE_SIZE.width, PALETTE_SIZE.height/2));
        _leftLayer->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(-1.5*PALETTE_SIZE.width, PALETTE_SIZE.height/2))));
        _centerLayer->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(-0.5*PALETTE_SIZE.width, PALETTE_SIZE.height/2))));
        _rightLayer1->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(1.5*PALETTE_SIZE.width, PALETTE_SIZE.height/2))));
        _rightLayer->runAction(Sequence::create(EaseSineOut::create(MoveTo::create(time, Vec2(0.5*PALETTE_SIZE.width ,PALETTE_SIZE.height/2))), CallFunc::create([this](){
            this->updateLayerPointerFromLeft();
            _isMoveTouchEnded = true;
        }), NULL));
        
    }else
    {
        if (_accelerationX > ACCELERATION_X)
        {
            float time = (PALETTE_SIZE.width + xReadyOffset)/PALETTE_SIZE.width * 0.7f/2;
            ++_iCurrentPage;
            if (_iCurrentPage > _iMaxPage)
            {
                _iCurrentPage = 0;
            }
            
            _leftLayer1->setPosition(Vec2(2.5*PALETTE_SIZE.width, PALETTE_SIZE.height/2));
            _leftLayer->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(-1.5*PALETTE_SIZE.width, PALETTE_SIZE.height/2))));
            _centerLayer->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(-0.5*PALETTE_SIZE.width, PALETTE_SIZE.height/2))));
            _rightLayer1->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(1.5*PALETTE_SIZE.width, PALETTE_SIZE.height/2))));
            _rightLayer->runAction(Sequence::create(EaseSineOut::create(MoveTo::create(time, Vec2(0.5*PALETTE_SIZE.width ,PALETTE_SIZE.height/2))), CallFunc::create([this](){
                this->updateLayerPointerFromLeft();
                _isMoveTouchEnded = true;
            }), NULL));
        }else
        {
            float time = std::abs(xReadyOffset)/PALETTE_SIZE.width * 0.7f;
            
            _leftLayer1->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(-1.5*PALETTE_SIZE.width, PALETTE_SIZE.height/2))));
            _leftLayer->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(-0.5*PALETTE_SIZE.width, PALETTE_SIZE.height/2))));
            _centerLayer->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(0.5*PALETTE_SIZE.width,PALETTE_SIZE.height/2))));
            _rightLayer1->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(2.5*PALETTE_SIZE.width, PALETTE_SIZE.height/2))));
            _rightLayer->runAction(Sequence::create(EaseSineOut::create(MoveTo::create(time, Vec2(1.5*PALETTE_SIZE.width ,PALETTE_SIZE.height/2))), CallFunc::create([this](){
                _isMoveTouchEnded = true;
                _accelerationX = 0.0f;
            }), NULL) );
        }
    }
    
}

void ColorLoopScrollLayer::towardRight(float xReadyOffset)
{
    if (xReadyOffset > PALETTE_SIZE.width/3)
    {
        float time = (PALETTE_SIZE.width - xReadyOffset)/PALETTE_SIZE.width * 0.7f;
        --_iCurrentPage;
        if (_iCurrentPage < 0)
        {
            _iCurrentPage = _iMaxPage;
        }
        
        _rightLayer1->setPosition(Vec2(-1.5*PALETTE_SIZE.width, PALETTE_SIZE.height/2));
        _rightLayer->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(2.5*PALETTE_SIZE.width, PALETTE_SIZE.height/2))));
        _leftLayer1->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(-0.5*PALETTE_SIZE.width, PALETTE_SIZE.height/2))));
        _centerLayer->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(1.5*PALETTE_SIZE.width, PALETTE_SIZE.height/2))));
        _leftLayer->runAction(Sequence::create(EaseSineOut::create(MoveTo::create(time, Vec2(0.5*PALETTE_SIZE.width, PALETTE_SIZE.height/2))), CallFunc::create([this](){
            this->updateLayerPointerFromRight();
            _isMoveTouchEnded = true;
        }), NULL));
        
    }else
    {
        if (_accelerationX > ACCELERATION_X)
        {
            float time = (PALETTE_SIZE.width - xReadyOffset)/PALETTE_SIZE.width * 0.7f/2;
            --_iCurrentPage;
            if (_iCurrentPage < 0)
            {
                _iCurrentPage = _iMaxPage;
            }
            
            _rightLayer1->setPosition(Vec2(-1.5*PALETTE_SIZE.width, PALETTE_SIZE.height/2));
            _rightLayer->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(2.5*PALETTE_SIZE.width, PALETTE_SIZE.height/2))));
            _leftLayer1->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(-0.5*PALETTE_SIZE.width, PALETTE_SIZE.height/2))));
            _centerLayer->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(1.5*PALETTE_SIZE.width, PALETTE_SIZE.height/2))));
            _leftLayer->runAction(Sequence::create(EaseSineOut::create(MoveTo::create(time, Vec2(0.5*PALETTE_SIZE.width, PALETTE_SIZE.height/2))), CallFunc::create([this](){
                this->updateLayerPointerFromRight();
                _isMoveTouchEnded = true;
            }), NULL));
        }else
        {
            float time = std::abs(xReadyOffset)/PALETTE_SIZE.width * 0.7f;
            
            _rightLayer1->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(2.5*PALETTE_SIZE.width, PALETTE_SIZE.height/2))));
            _rightLayer->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(1.5*PALETTE_SIZE.width, PALETTE_SIZE.height/2))));
            _leftLayer1->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(-1.5*PALETTE_SIZE.width, PALETTE_SIZE.height/2))));
            _centerLayer->runAction(EaseSineOut::create(MoveTo::create(time, Vec2(0.5*PALETTE_SIZE.width, PALETTE_SIZE.height/2))));
            _leftLayer->runAction(Sequence::create(EaseSineOut::create(MoveTo::create(time, Vec2(-0.5*PALETTE_SIZE.width, PALETTE_SIZE.height/2))), CallFunc::create([this](){
                _isMoveTouchEnded = true;
                _accelerationX = 0.0f;
            }), NULL));
        }
    }
}

void ColorLoopScrollLayer::updateLayerPointerFromLeft()
{
    LayerColor* temp = _leftLayer1;
    _leftLayer1 = _leftLayer;
    _leftLayer = _centerLayer;
    _centerLayer = _rightLayer;
    _rightLayer = _rightLayer1;
    _rightLayer1 = temp;
    
    _accelerationX = 0.0f;
    this->updateColorLayer(UpdateObj::RIGHT);
}

void ColorLoopScrollLayer::updateLayerPointerFromRight()
{
    LayerColor* temp = _rightLayer1;
    _rightLayer1 = _rightLayer;
    _rightLayer = _centerLayer;
    _centerLayer = _leftLayer;
    _leftLayer = _leftLayer1;
    _leftLayer1 = temp;
    
    _accelerationX = 0.0f;
    this->updateColorLayer(UpdateObj::LEFT);
}

bool ColorLoopScrollLayer::currentPageIsExist(int page)
{
    if (_iCurrentPage >= 2 && _iCurrentPage <= _iMaxPage - 2)
    {
        if ((page >= _iCurrentPage - 2) && (page <= _iCurrentPage + 2))
        {
            return true;
        }
    }else if (_iCurrentPage == 0)
    {
        if (page == (_iMaxPage - 1) || page == (_iMaxPage - 0) || page == _iCurrentPage || page == 1 || page == 2)
        {
            return true;
        }
    }else if (_iCurrentPage == 1)
    {
        if (page == (_iMaxPage - 0) || page == 0 || page == _iCurrentPage || page == 2 || page == 3)
        {
            return true;
        }
    }else if (_iCurrentPage == _iMaxPage - 1)
    {
        if (page == (_iCurrentPage - 2) || page == (_iCurrentPage - 1) || page == _iCurrentPage || page == (_iCurrentPage + 1) || page == 0)
        {
            return true;
        }
    }else if (_iCurrentPage == _iMaxPage)
    {
        if (page == (_iCurrentPage - 2) || page == (_iCurrentPage - 1) || page == _iCurrentPage || page == 0 || page == 1)
        {
            return true;
        }
    }
    
    return false;
    
}