//
//  UITools.cpp
//
//  Created by luoxp on 14-9-22.
//
//

#include "UICPP.h"
#include "UITools.h"
#include <random>
#include "Analytics.h"

Button * quickButtonColor(const string& name, Widget* root, const Widget::ccWidgetTouchCallback &callback)
{
    auto btn = quickButton(name, root, callback);
    btn->setColor(UI_COLOR);
    
    return btn;
}

Button * quickButton(const string& name, Widget* root, const Widget::ccWidgetTouchCallback &callback)
{
#if COCOS2D_DEBUG >= 1
    Button *btn = dynamic_cast<Button*>(Helper::seekNodeByName(root, name));
    CCASSERT(btn != nullptr, "btn is nullptr.");
#else
    Button *btn = (Button*)Helper::seekNodeByName(root, name);
#endif
    btn->setPressedActionEnabled(true);
    btn->setTouchEnabled(true);
    btn->addTouchEventListener(callback);
    
    return btn;
}

Button * quickButtonDisablePressed(const string& name, Widget* root, const Widget::ccWidgetTouchCallback &callback)
{
#if COCOS2D_DEBUG >= 1
    Button *btn = dynamic_cast<Button*>(Helper::seekNodeByName(root, name));
    CCASSERT(btn != nullptr, "btn is nullptr.");
#else
    Button *btn = (Button*)Helper::seekNodeByName(root, name);
#endif
    btn->setTouchEnabled(true);
    btn->addTouchEventListener(callback);
    
    return btn;
}

ImageView * quickImageView(const string& name, Widget* root)
{
    ImageView *image = (ImageView*)Helper::seekNodeByName(root, name);
    return image;
}

ImageView * quickImageView(const string& name, Widget* root, const Widget::ccWidgetTouchCallback &callback)
{
    ImageView *image = (ImageView*)Helper::seekNodeByName(root, name);
    image->setTouchEnabled(true);
    image->addTouchEventListener(callback);
    return image;
}

CheckBox * quickCheckBox(const string& name, Widget* root, const CheckBox::ccCheckBoxCallback &callback)
{
    CheckBox * check = (CheckBox*)Helper::seekNodeByName(root, name);
    check->setTouchEnabled(true);
    check->addEventListener(callback);
    return check;
}

Layout * quickLayout(const string& name, Widget* root, const Widget::ccWidgetTouchCallback &callback)
{
    Layout *layout = (Layout*)Helper::seekNodeByName(root, name);
    layout->addTouchEventListener(callback);
    
    return layout;
}

LayoutEx * quickLayoutEx(const string& name, Widget* root, const Widget::ccWidgetTouchCallback &callback)
{
    //复制所有属性
    Layout * original = (Layout*)Helper::seekNodeByName(root, name);
    LayoutEx * layoutEx = LayoutEx::create();
    layoutEx->setName(original->getName());
    layoutEx->setZOrder(original->getZOrder());
    layoutEx->setContentSize(original->getContentSize());
    layoutEx->setLayoutType(original->getLayoutType());
    layoutEx->setLayoutParameter(original->getLayoutParameter());
    layoutEx->setPosition(original->getPosition() - Vec2(layoutEx->getContentSize()/2));
    
    Vector<Node*>& children = original->getChildren();
    while (!children.empty()) {
        Node* child = children.front();
        child->retain();
        child->removeFromParent();
        layoutEx->addChild(child);
        child->release();
    }
    
    original->getParent()->addChild(layoutEx);
    original->removeFromParent();
    
    //改变锚点
    layoutEx->setAnchorPoint(Vec2(0.5, 0.5));
    layoutEx->ignoreAnchorPointForPosition(false);
    
    //设置事件
    layoutEx->setTouchEnabled(true);
    layoutEx->addTouchEventListener(callback);
    
    return layoutEx;
}

LayoutEx * quickLayoutColor(const string& name, Widget* root, const Widget::ccWidgetTouchCallback &callback, const Color3B & color)
{
    auto layoutEx = quickLayoutColor(name, root, callback, nullptr, color);
    layoutEx->setPresent(layoutEx);
    return layoutEx;
}

LayoutEx * quickLayoutColor(const string& name, Widget* root, const Widget::ccWidgetTouchCallback &callback, Widget * present, const Color3B & color)
{
    //复制所有属性
    Layout * original = (Layout*)Helper::seekNodeByName(root, name);
    LayoutEx * layoutEx = LayoutEx::create();
    layoutEx->setName(original->getName());
    layoutEx->setZOrder(original->getZOrder());
    layoutEx->setContentSize(original->getContentSize());
    layoutEx->setLayoutType(original->getLayoutType());
    layoutEx->setLayoutParameter(original->getLayoutParameter());
    layoutEx->setPosition(original->getPosition() - Vec2(layoutEx->getContentSize()/2));
    
    Vector<Node*>& children = original->getChildren();
    while (!children.empty()) {
        Node* child = children.front();
        child->setColor(color);
        child->removeFromParent();
        layoutEx->addChild(child);
    }
    
    original->getParent()->addChild(layoutEx);
    original->removeFromParent();
    
    //改变锚点
    layoutEx->setAnchorPoint(Vec2(0.5, 0.5));
    layoutEx->ignoreAnchorPointForPosition(false);
    
    //设置事件
    layoutEx->setTouchEnabled(true);
    layoutEx->addTouchEventListener(callback);
    layoutEx->setPresent(present);
    
    return layoutEx;
}

Text * quickText(const string& name, Widget* root, const Widget::ccWidgetTouchCallback &callback, const Color3B & color)
{
    Text *text = (Text*)Helper::seekNodeByName(root, name);
    text->setTouchEnabled(true);
    text->addTouchEventListener(callback);
    text->setColor(color);

    //draw underline
    auto underline = DrawNode::create();
    text->getParent()->addChild(underline);
    underline->drawSegment(text->getPosition() - text->getContentSize()/2,
                           Vec2(text->getPosition().x + text->getContentSize().width/2, text->getPosition().y - text->getContentSize().height/2), 1, Color4F(Color3B(119, 119, 119)));
    text->setUserData(underline);
    
    return text;
}

void quickSetString(int iTag, Widget *itemRoot, const string &text)
{
    Text * ui = (Text*)Helper::seekNodeByTag(itemRoot, iTag);
    ui->setString(text);
}

void quickSetString(const string &itemName, Widget *itemRoot, const string &text)
{
    Text * ui = (Text*)Helper::seekNodeByName(itemRoot, itemName);
    ui->setString(text);
}

void quickSetImage(int iTag, Widget *itemRoot, const string &filename)
{
    ImageView * ui = (ImageView*)Helper::seekNodeByTag(itemRoot, iTag);
    ui->loadTexture(filename);
}

void quickSetImage(const string &itemName, Widget *itemRoot, const string &filename)
{
	ImageView * ui = (ImageView*)Helper::seekNodeByName(itemRoot, itemName);
    ui->loadTexture(filename);
}

void quickSetVisible(int iTag, Widget *itemRoot, bool bVisible)
{
	auto ui = Helper::seekNodeByTag(itemRoot, iTag);
    ui->setVisible(bVisible);
}

void quickSetVisible(const string& name , Widget *itemRoot, bool bVisible)
{
    auto ui = Helper::seekNodeByName(itemRoot, name);
    ui->setVisible(bVisible);
}

void quickAddEvent(const string& eventName, const function<void(EventCustom*)>& callback, Node *node)
{
    auto event = EventListenerCustom::create(eventName, callback);
    node->getEventDispatcher()->addEventListenerWithSceneGraphPriority(event, node);
}

void quickSetButtonTitle(int iTag, Widget *itemRoot, const string &itemName)
{
	Button * ui = (Button*)Helper::seekNodeByTag(itemRoot, iTag);
    ui->setTitleText(itemName);
}

void quickSetFontName(const string& name, Widget *itemRoot, const string& fontname)
{
    auto text = (Text*)Helper::seekNodeByName(itemRoot, name);
    text->setFontName(fontname);
}

void quickLogContentSize(Node *node)
{
    Size size = node->getContentSize();
    log("width: %f, height: %f", size.width, size.height);
}

void quickLogPosition(Node *node)
{
    Vec2 position = node->getPosition();
    log("x: %f, y: %f", position.x, position.y);
}

void quickBackToOriginal(Node * node, Vec2 &original)
{
    FiniteTimeAction* action = EaseElasticOut::create(MoveTo::create(0.5f, original), 0.8f);
    node->runAction(action);
}

bool quickIfAInB(Widget * a, Widget * b)
{
    Vec2 pos = b->getWorldPosition();
    Size contentSize = b->getContentSize();
    
    Rect rect = Rect(pos.x - contentSize.width/2, pos.y - contentSize.height/2, contentSize.width, contentSize.height);
    if (rect.containsPoint(a->getWorldPosition()))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void quickSetLocalZOrder(Widget * widget, int localZOrder)
{
    widget->setLocalZOrder(localZOrder);
    widget->getVirtualRenderer()->setLocalZOrder(localZOrder);
}

bool quickIsOutsideOfScreen(Widget * widget)
{
    Size win = Director::getInstance()->getVisibleSize();
    Size size = widget->getContentSize();
    Vec2 pos = widget->getWorldPosition();
    
    //先检查x
    if (pos.x - size.width/2 < 0 || pos.x + size.width/2 > win.width) {
        return true;
    }
    else if (pos.y - size.height/2 < 0 || pos.y + size.height/2 > win.height)
    {
        return true;
    }
    
    return false;
}

float quickAdaptScale(Node *src, Node *dest)
{
    return quickAdaptScale(src->getContentSize(), dest->getContentSize());
}

float quickAdaptScale(const Size &src, const Size &dest)
{
    float scaleX = dest.width / src.width;
    float scaleY = dest.height / src.height;
    
    if (scaleX < scaleY) {
        return scaleX;
    }
    else
    {
        return scaleY;
    }
}

void quickFlurry(string name)
{
    Analytics::getInstance()->sendEvent(name, name);
}

Vec2 rotateWithDegree(Vec2 &point, float fDegree)
{
    //角度转弧度
    float angle = M_PI * fDegree /180;
    return point.rotateByAngle(Vec2::ZERO, angle);
}

int cycleNum(int iCount, bool bAdd, int *idx)
{
    if (bAdd == true)
    {
        if (*idx >= iCount - 1)
        {
            *idx = 0;
        }
        else
        {
            (*idx)++;
        }
    }
    else if (bAdd == false)
    {
        if ((*idx) <= 0)
        {
            (*idx) = iCount - 1;
        }
        else
        {
            (*idx)--;
        }
    }
    
    return (*idx);
}

int quickRandom(int min, int max)
{
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_int_distribution<> distribution(min, max);
    return distribution(engine);
}

float quickRandomFloat(float min, float max)
{
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_real_distribution<> distribution(min, max);
    return distribution(engine);
}

Armature * quickGetAnimation(int animationIndex, const string &name, const Vec2 &pos)
{
    Armature *armature = Armature::create(name);
    armature->getAnimation()->playWithIndex(animationIndex);
    armature->setPosition(pos);
    return armature;
}

std::string getCurrentTime(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    struct timeval now;
    struct tm* time;
    
    gettimeofday(&now, NULL);
    
    
    time = localtime(&now.tv_sec);
    int year = time->tm_year + 1900;
    log("year = %d",year);
    
    char date[32] = {0};
    sprintf(date, "%d-%02d-%02d %02d-%02d-%02d",(int)time->tm_year + 1900,(int)time->tm_mon + 1,(int)time->tm_mday, (int)time->tm_hour, (int)time->tm_min, (int)time->tm_sec);
    log("%s",date);
    return StringUtils::format("%s",date);
    
#endif
    
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
    
    struct tm* tm;
    time_t timep;
    time(timep);
    
    tm = localtime(&timep);
    char date[32] = {0};
    sprintf(date, "%d-%02d-%02d %02d-%02d-%02d",(int)time->tm_year + 1900,(int)time->tm_mon + 1,(int)time->tm_mday, (int)time->tm_hour, (int)time->tm_min, (int)time->tm_sec);
    log("%s",date);
    return StringUtils::format("%s",date);
    
#endif
}
