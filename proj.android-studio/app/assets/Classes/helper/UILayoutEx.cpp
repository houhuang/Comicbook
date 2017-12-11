/****************************************************************************
Copyright (c) 2013-2014 Chukong Technologies Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "UILayoutEx.h"
#include "ui/UIHelper.h"
#include "ui/UIScale9Sprite.h"
#include "renderer/CCGLProgram.h"
#include "renderer/CCGLProgramCache.h"
#include "renderer/ccGLStateCache.h"
#include "renderer/CCRenderState.h"
#include "base/CCDirector.h"
#include "2d/CCDrawingPrimitives.h"
#include "renderer/CCRenderer.h"
#include "ui/UILayoutManager.h"
#include "2d/CCDrawNode.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCEventFocus.h"

NS_CC_BEGIN

namespace ui {
    
LayoutEx::LayoutEx()
{
    _present = nullptr;
}
    
LayoutEx* LayoutEx::create()
{
    LayoutEx* layout = new LayoutEx();
    if (layout && layout->init())
    {
        layout->autorelease();
        return layout;
    }
    CC_SAFE_DELETE(layout);
    return nullptr;
}
    
void LayoutEx::setTouchEnabled(bool enable)
{
    if (enable == _touchEnabled)
    {
        return;
    }
    _touchEnabled = enable;
    if (_touchEnabled)
    {
        _touchListener = EventListenerTouchOneByOne::create();
        CC_SAFE_RETAIN(_touchListener);
        _touchListener->setSwallowTouches(true);
        _touchListener->onTouchBegan = CC_CALLBACK_2(LayoutEx::onTouchBegan, this);
        _touchListener->onTouchMoved = CC_CALLBACK_2(LayoutEx::onTouchMoved, this);
        _touchListener->onTouchEnded = CC_CALLBACK_2(LayoutEx::onTouchEnded, this);
        _touchListener->onTouchCancelled = CC_CALLBACK_2(LayoutEx::onTouchCancelled, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener, this);
    }
    else
    {
        _eventDispatcher->removeEventListener(_touchListener);
        CC_SAFE_RELEASE_NULL(_touchListener);
    }
}
    
bool LayoutEx::onTouchBegan(Touch *touch, Event *unusedEvent)
{
    _hitted = false;
    if (isVisible() && isEnabled() && isAncestorsEnabled() && isAncestorsVisible(this) )
    {
        _touchBeganPosition = touch->getLocation();
        auto camera = Camera::getVisitingCamera();
        if(hitTest(_touchBeganPosition, camera, nullptr))
        {
            if (isClippingParentContainsPoint(_touchBeganPosition)) {
                _hittedByCamera = camera;
                _hitted = true;
            }
        }
    }
    if (!_hitted)
    {
        return false;
    }
    setHighlighted(true);
    
    /*
     * Propagate touch events to its parents
     */
    if (_propagateTouchEvents)
    {
        this->propagateTouchEvent(TouchEventType::BEGAN, this, touch);
    }
    
    pushDownEvent();
    return true;
}


void LayoutEx::onTouchMoved(Touch *touch, Event *unusedEvent)
{
    _touchMovePosition = touch->getLocation();
    
    setHighlighted(hitTest(_touchMovePosition, _hittedByCamera, nullptr));
    
    /*
     * Propagate touch events to its parents
     */
    if (_propagateTouchEvents)
    {
        this->propagateTouchEvent(TouchEventType::MOVED, this, touch);
    }
    
    moveEvent();
}

void LayoutEx::onTouchEnded(Touch *touch, Event *unusedEvent)
{
    _touchEndPosition = touch->getLocation();
    
    /*
     * Propagate touch events to its parents
     */
    if (_propagateTouchEvents)
    {
        this->propagateTouchEvent(TouchEventType::ENDED, this, touch);
    }
    
    bool highlight = _highlight;
    setHighlighted(false);
    
    if (highlight)
    {
        releaseUpEvent();
    }
    else
    {
        cancelUpEvent();
    }
}

void LayoutEx::onTouchCancelled(Touch *touch, Event *unusedEvent)
{
    setHighlighted(false);
    cancelUpEvent();
}
    
void LayoutEx::setHighlighted(bool hilight)
{
    _highlight = hilight;
    if (_bright)
    {
        if (_highlight)
        {
            setBrightStyle(BrightStyle::HIGHLIGHT);
        }
        else
        {
            setBrightStyle(BrightStyle::NORMAL);
        }
    }
    else
    {
        onPressStateChangedToDisabled();
    }
    
    if (_highlight) {
        if(_present)
            _present->setScale(1.2f);
    }
    else
    {
        if(_present)
            _present->setScale(1.f);
    }
}
    
}
NS_CC_END
