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

#include "UIPageViewEx.h"
#include "UIHeader.h"

NS_CC_BEGIN

namespace ui {
    
IMPLEMENT_CLASS_GUI_INFO(PageViewEx)

PageViewEx::PageViewEx():
_curPageIdx(0),
_touchMoveDirection(TouchDirection::UP),
_bottomBoundaryChild(nullptr),
_topBoundaryChild(nullptr),
_topBoundary(0.0f),
_bottomBoundary(0.0f),
_isAutoScrolling(false),
_autoScrollDistance(0.0f),
_autoScrollSpeed(0.0f),
_autoScrollDirection(AutoScrollDirection::UP),
_childFocusCancelOffset(5.0f),
_pageViewEventListener(nullptr),
_pageViewEventSelector(nullptr)
{
    this->setTouchEnabled(true);
}

PageViewEx::~PageViewEx()
{
    _pageViewEventListener = nullptr;
    _pageViewEventSelector = nullptr;
}

PageViewEx* PageViewEx::create()
{
    PageViewEx* widget = new PageViewEx();
    if (widget && widget->init())
    {
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}
    
void PageViewEx::onEnter()
{
#if CC_ENABLE_SCRIPT_BINDING
    if (_scriptType == kScriptTypeJavascript)
    {
        if (ScriptEngineManager::sendNodeEventToJSExtended(this, kNodeOnEnter))
            return;
    }
#endif
    
    Layout::onEnter();
    scheduleUpdate();
}

bool PageViewEx::init()
{
    if (Layout::init())
    {
        setClippingEnabled(true);
        return true;
    }
    return false;
}

void PageViewEx::addWidgetToPage(Widget *widget, ssize_t pageIdx, bool forceCreate)
{
    if (!widget || pageIdx < 0)
    {
        return;
    }
   
    ssize_t pageCount = this->getPageCount();
    if (pageIdx < 0 || pageIdx >= pageCount)
    {
        if (forceCreate)
        {
            if (pageIdx > pageCount)
            {
                CCLOG("pageIdx is %d, it will be added as page id [%d]",static_cast<int>(pageIdx),static_cast<int>(pageCount));
            }
            Layout* newPage = createPage();
            newPage->addChild(widget);
            addPage(newPage);
        }
    }
    else
    {
        Node * page = _pages.at(pageIdx);
        page->addChild(widget);
    }
}

Layout* PageViewEx::createPage()
{
    Layout* newPage = Layout::create();
    newPage->setContentSize(getContentSize());
    return newPage;
}

void PageViewEx::addPage(Layout* page)
{
    if (!page || _pages.contains(page))
    {
        return;
    }

    
    addChild(page);
    _pages.pushBack(page);
    
    _doLayoutDirty = true;
}

void PageViewEx::insertPage(Layout* page, int idx)
{
    if (idx < 0 || !page || _pages.contains(page))
    {
        return;
    }
   
    
    ssize_t pageCount = this->getPageCount();
    if (idx >= pageCount)
    {
        addPage(page);
    }
    else
    {
        _pages.insert(idx, page);
        addChild(page);
        
    }
    
    _doLayoutDirty = true;
}

void PageViewEx::removePage(Layout* page)
{
    if (!page)
    {
        return;
    }
    removeChild(page);
    _pages.eraseObject(page);
    
    _doLayoutDirty = true;
}

void PageViewEx::removePageAtIndex(ssize_t index)
{
    if (index < 0 || index >= this->getPages().size())
    {
        return;
    }
    Layout* page = _pages.at(index);
    removePage(page);
}
    
void PageViewEx::removeAllPages()
{
    for(const auto& node : _pages)
    {
        removeChild(node);
    }
    _pages.clear();
}

void PageViewEx::updateBoundaryPages()
{
    if (_pages.size() <= 0)
    {
        _bottomBoundaryChild = nullptr;
        _topBoundaryChild = nullptr;
        return;
    }
    _bottomBoundaryChild = _pages.at(0);
    _topBoundaryChild = _pages.at(this->getPageCount()-1);
}

ssize_t PageViewEx::getPageCount()const
{
    return _pages.size();
}

float PageViewEx::getPositionXByIndex(ssize_t idx)const
{
    return (getContentSize().height * (idx-_curPageIdx));
}

void PageViewEx::onSizeChanged()
{
    Layout::onSizeChanged();
    _topBoundary = getContentSize().height;
    
    _doLayoutDirty = true;
}

void PageViewEx::updateAllPagesSize()
{
    Size selfSize = getContentSize();
    for (auto& page : _pages)
    {
        page->setContentSize(selfSize);
    }
}

void PageViewEx::updateAllPagesPosition()
{
    ssize_t pageCount = this->getPageCount();
    
    if (pageCount <= 0)
    {
        _curPageIdx = 0;
        return;
    }
    
    if (_curPageIdx >= pageCount)
    {
        _curPageIdx = pageCount-1;
    }
    
    float pageHeight = getContentSize().height;
    for (int i=0; i<pageCount; i++)
    {
        Layout* page = _pages.at(i);
        page->setPosition(Vec2(0, (i-_curPageIdx) * pageHeight));
        
    }
}


void PageViewEx::scrollToPage(ssize_t idx)
{
    if (idx < 0 || idx >= this->getPageCount())
    {
        return;
    }
    _curPageIdx = idx;
    Layout* curPage = _pages.at(idx);
    _autoScrollDistance = -(curPage->getPosition().y);
    _autoScrollSpeed = fabs(_autoScrollDistance)/0.2f;
    _autoScrollDirection = _autoScrollDistance > 0 ? AutoScrollDirection::DOWN : AutoScrollDirection::UP;
    _isAutoScrolling = true;
}

void PageViewEx::update(float dt)
{
    if (_isAutoScrolling)
    {
        this->autoScroll(dt);
    }
}
    
void PageViewEx::autoScroll(float dt)
    {
        switch (_autoScrollDirection)
        {
            case AutoScrollDirection::UP:
            {
                float step = _autoScrollSpeed*dt;
                if (_autoScrollDistance + step >= 0.0f)
                {
                    step = -_autoScrollDistance;
                    _autoScrollDistance = 0.0f;
                    _isAutoScrolling = false;
                }
                else
                {
                    _autoScrollDistance += step;
                }
                scrollPages(-step);
                if (!_isAutoScrolling)
                {
                    pageTurningEvent();
                }
                break;
            }
                break;
            case AutoScrollDirection::DOWN:
            {
                float step = _autoScrollSpeed*dt;
                if (_autoScrollDistance - step <= 0.0f)
                {
                    step = _autoScrollDistance;
                    _autoScrollDistance = 0.0f;
                    _isAutoScrolling = false;
                }
                else
                {
                    _autoScrollDistance -= step;
                }
                scrollPages(step);
                
                if (!_isAutoScrolling)
                {
                    pageTurningEvent();
                }
                
                break;
            }
            default:
                break;
        }

    }

void PageViewEx::doLayout()
{
    if (!_doLayoutDirty)
    {
        return;
    }
    
    updateAllPagesPosition();
    updateAllPagesSize();
    updateBoundaryPages();

    
    _doLayoutDirty = false;
}

void PageViewEx::movePages(float offset)
{
    for (auto& page : this->getPages())
    {
        page->setPosition(Vec2(page->getPosition().x,
                               page->getPosition().y + offset));
    }
    
    Widget* curPage = (Widget*)(getPages().at(_curPageIdx));
    if (curPage)
    {
        Vec2 curPagePos = curPage->getPosition();
        switch (_touchMoveDirection) {
            case TouchDirection::UP:
            {
                if (curPagePos.y > 0) {
                    _percent = curPagePos.y / getContentSize().height;
                }
                else
                {
                    _percent = (getContentSize().height + curPagePos.y) / getContentSize().height;
                }
            }
                break;
            case TouchDirection::DOWN:
            {
                if (curPagePos.y < 0) {
                    _percent = std::abs(curPagePos.y) / getContentSize().height;
                }
                else
                {
                    _percent = (getContentSize().height - curPagePos.y) / getContentSize().height;
                }
            }
                break;
            default:
                break;
        }
        
        if (_percent > 0.02 && _percent < 0.98) {
            _eventDispatcher->dispatchCustomEvent("event_move_pages");
        }
    }
}

bool PageViewEx::scrollPages(float touchOffset)
{
    if (this->getPageCount() <= 0)
    {
        return false;
    }
    
    if (!_bottomBoundaryChild || !_topBoundaryChild)
    {
        return false;
    }
    
    float realOffset = touchOffset;
   
    switch (_touchMoveDirection)
    {
        case TouchDirection::DOWN: // down
            if (_topBoundaryChild->getTopBoundary() + touchOffset <= _topBoundary)
            {
                realOffset = _topBoundary - _topBoundaryChild->getTopBoundary();
                movePages(realOffset);
                return false;
            }
            break;
        case TouchDirection::UP: // up
            if (_bottomBoundaryChild->getBottomBoundary() + touchOffset >= _bottomBoundary)
            {
                realOffset = _bottomBoundary - _bottomBoundaryChild->getBottomBoundary();
                movePages(realOffset);
                return false;
            }
            break;
        default:
            break;
    }
    
    movePages(realOffset);
    return true;
}


void PageViewEx::handleMoveLogic(Touch *touch)
{
    Vec2 touchPoint = touch->getLocation();
    
    float offset = 0.0;
    offset = touchPoint.y - touch->getPreviousLocation().y;
    
    if (offset < 0)
    {
        _touchMoveDirection = TouchDirection::DOWN;
    }
    else if (offset > 0)
    {
        _touchMoveDirection = TouchDirection::UP;
    }
    scrollPages(offset);
}

void PageViewEx::handleReleaseLogic(Touch *touch)
{
    if (this->getPageCount() <= 0)
    {
        return;
    }
    Widget* curPage = dynamic_cast<Widget*>(this->getPages().at(_curPageIdx));
    if (curPage)
    {
        Vec2 curPagePos = curPage->getPosition();
        ssize_t pageCount = this->getPageCount();
        float curPageLocation = curPagePos.y;
        float pageHeight = getContentSize().height;
        float boundary = pageHeight/2.0f;
        if (curPageLocation <= -boundary)
        {
            if (_curPageIdx >= pageCount-1)
            {
                scrollPages(-curPageLocation);
            }
            else
            {
                int idex = (curPage->getPosition().y / curPage->getContentSize().height);
                float offset = curPage->getPosition().y / curPage->getContentSize().height - idex;
                if (std::abs(offset) >= 0.5)
                {
                    scrollToPage(_curPageIdx+1 + abs(idex));
                }else
                {
                    scrollToPage(_curPageIdx+1 + abs(idex) - 1);
                }
            }
        }
        else if (curPageLocation >= boundary)
        {
            if (_curPageIdx <= 0)
            {
                scrollPages(-curPageLocation);
            }
            else
            {
                int idex = (curPage->getPosition().y / curPage->getContentSize().height);
                float offset = curPage->getPosition().y / curPage->getContentSize().height - idex;
                if (std::abs(offset) >= 0.5)
                {
                    scrollToPage(_curPageIdx-1 - abs(idex));
                }else
                {
                    scrollToPage(_curPageIdx-1 - abs(idex) + 1);
                }
            }
        }
        else
        {
            scrollToPage(_curPageIdx);
        }
    }
}


void PageViewEx::interceptTouchEvent(TouchEventType event, Widget *sender, Touch *touch)
{
    Vec2 touchPoint = touch->getLocation();
    
    switch (event)
    {
        case TouchEventType::BEGAN:
            //no-op
            break;
        case TouchEventType::MOVED:
        {
            float offset = 0;
            offset = fabs(sender->getTouchBeganPosition().y - touchPoint.y);
            if (offset > _childFocusCancelOffset)
            {
                sender->setHighlighted(false);
                handleMoveLogic(touch);
            }
        }
            break;
        case TouchEventType::CANCELED:
        case TouchEventType::ENDED:
            handleReleaseLogic(touch);
            break;
    }
}

ssize_t PageViewEx::getCurPageIndex() const
{
    return _curPageIdx;
}

Vector<Layout*>& PageViewEx::getPages()
{
    return _pages;
}
    
Layout* PageViewEx::getPage(ssize_t index)
{
    if (index < 0 || index >= this->getPages().size())
    {
        return nullptr;
    }
    return _pages.at(index);
}

std::string PageViewEx::getDescription() const
{
    return "PageViewEx";
}

Widget* PageViewEx::createCloneInstance()
{
    return PageViewEx::create();
}

void PageViewEx::copyClonedWidgetChildren(Widget* model)
{
    auto modelPages = static_cast<PageViewEx*>(model)->getPages();
    for (auto& page : modelPages)
    {
        addPage(static_cast<Layout*>(page->clone()));
    }
}

void PageViewEx::copySpecialProperties(Widget *widget)
{
    PageViewEx* pageView = dynamic_cast<PageViewEx*>(widget);
    if (pageView)
    {
        Layout::copySpecialProperties(widget);
        _eventCallback = pageView->_eventCallback;
        _pageViewEventListener = pageView->_pageViewEventListener;
        _pageViewEventSelector = pageView->_pageViewEventSelector;
    }
}

}

NS_CC_END
