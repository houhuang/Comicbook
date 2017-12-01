//
//  DeleteLayer.cpp
//  ColorGarden
//
//  Created by houhuang on 17/11/7.
//
//

#include "DeleteLayer.hpp"

enum
{
    st_button_no_tag = 11,
    st_button_yes_tag
};

DeleteLayer::~DeleteLayer()
{
    _eventDispatcher->removeEventListenersForTarget(this);
}

bool DeleteLayer::init()
{
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 210), 300, 300)) return false;
    
    Label* text = Label::createWithTTF("Remove from published?", "res/Muli-Regular.ttf", 50);
    text->setScale(0.5f);
    text->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height*0.8));
    this->addChild(text);
    
    Sprite* icon = Sprite::create("other/icon_remove_delete@3x.png");
    icon->setScale(0.67);
    icon->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height*0.48));
    this->addChild(icon);
    
    Sprite* bottom = Sprite::create("other/option@2x.png");
    bottom->setScale(this->getContentSize().width/bottom->getContentSize().width);
    bottom->setAnchorPoint(Vec2(0.5, 0));
    bottom->setPosition(Vec2(this->getContentSize().width/2, 0));
    this->addChild(bottom);
    
    MenuItemImage* no = MenuItemImage::create("other/left222.png", "other/left222.png", CC_CALLBACK_1(DeleteLayer::onButton, this));
    no->setAnchorPoint(Vec2(0.5, 0));
    no->setScale(this->getContentSize().width/2/no->getContentSize().width);
    no->setPosition(Vec2(this->getContentSize().width*0.26, 0));
    no->setTag(st_button_no_tag);
    
    MenuItemImage* yes = MenuItemImage::create("other/left222.png", "other/left222.png", CC_CALLBACK_1(DeleteLayer::onButton, this));
    yes->setAnchorPoint(Vec2(0.5, 0));
    yes->setScale(this->getContentSize().width/2/no->getContentSize().width);
    yes->setPosition(Vec2(this->getContentSize().width*0.74, 0));
    yes->setTag(st_button_yes_tag);
    
    Menu* lMenu = Menu::create(no, yes, NULL);
    lMenu->setPosition(Vec2::ZERO);
    this->addChild(lMenu);
    
    Sprite* lBg = Sprite::create("other/content_mask.png");
    lBg->setPosition(this->getContentSize()/2);
    lBg->setScale(this->getContentSize().width/355);
    this->addChild(lBg, 10);
    
    this->addListener();
    
    return true;
}

void DeleteLayer::onButton(Ref* ref)
{
    MenuItemImage* lMenu = (MenuItemImage*)ref;
    
    switch (lMenu->getTag())
    {
        case st_button_no_tag:
        {
            Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("delete_button_no");
        }
            break;
        case st_button_yes_tag:
        {
            Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("delete_button_yes");
        }
            break;
        default:
            break;
    }
}

void DeleteLayer::addListener()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch* touch, Event* event)->bool{
        Vec2 lPos = touch->getLocation();
        _firstPos = lPos;
        lPos = this->getParent()->convertToNodeSpace(lPos);
        if (this->getBoundingBox().containsPoint(lPos))
        {
            return true;
        }
        
        return false;
    };
    
    listener->onTouchMoved = [this](Touch* touch, Event* event){
        
    };
    
    listener->onTouchEnded = [this](Touch* touch, Event* event){
        float distance = _firstPos.distance(touch->getLocation());
        
        if (distance < 5)
        {
            Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("remove_deletelayer");
        }
    };
    
    listener->onTouchCancelled = [this](Touch* touch, Event* event){
        float distance = _firstPos.distance(touch->getLocation());
        
        if (distance < 5)
        {
            Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("remove_deletelayer");
        }
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}
