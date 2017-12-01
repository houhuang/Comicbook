//
//  NewDialog.cpp
//  ColorGarden
//
//  Created by houhuang on 17/9/20.
//
//

#include "NewDialog.hpp"
#include "STVisibleRect.h"
#include "StringsManager.h"
#include "BaseScene.hpp"
#include "SceneManager.h"

enum
{
    st_dialog_left = 1,
    st_dialog_right
};

NewDialog* NewDialog::create(const string& tText, const string& lText, const string rText, float fontTSize)
{
    NewDialog* layer = new NewDialog();
    if (layer && layer->init(tText, lText, rText, fontTSize))
    {
        layer->autorelease();
        return layer;
    }
    
    CC_SAFE_DELETE(layer);
    return nullptr;
}

NewDialog::~NewDialog()
{
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("setIsResponseBackToTrue");
    _eventDispatcher->removeEventListenersForTarget(this);
}

bool NewDialog::init(const string& tText, const string& lText, const string rText, float fontTSize)
{
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 210))) return false;
    
    _leftText = lText;
    _rightText = rText;
    
    V::isIpad()? this->initUIWithiPad(tText, lText, rText, fontTSize) :
                 this->initUIWithiPhone(tText, lText, rText, fontTSize);
    
    xScene->setIsResponseBack(false);
    log("qqqqqq:%d",xScene->getIsResponseBack());
    return true;
}

void NewDialog::onEnterTransitionDidFinish()
{
    LayerColor::onEnterTransitionDidFinish();

    this->addListener();
}

void NewDialog::onExit()
{
    LayerColor::onExit();
    
    BaseScene* lScene = dynamic_cast<BaseScene*>(Director::getInstance()->getRunningScene());
    if (lScene)
    {
        lScene->setTouchEnable(true);
    }

}

void NewDialog::initUIWithiPhone(const string& tText, const string& lText, const string rText, float fontTSize)
{
    Sprite* bg = Sprite::create("other/popup_small_bg@2x.png");
    bg->setPosition(this->getContentSize()/2);
    bg->setScale(this->getContentSize().width*0.6/452);
    this->addChild(bg);
    _dialogBG = bg;
    
    Label* tl = Label::createWithTTF(xStr->getString("MSG_" + tText), "res/Muli-Regular.ttf", 13 * 2);
    tl->setColor(Color3B(103, 103, 103));
    tl->setDimensions(bg->getContentSize().width * 0.8 * 0.9, bg->getContentSize().height * 0.7);
    tl->setAlignment(cocos2d::TextHAlignment::CENTER);
    tl->setVerticalAlignment(cocos2d::TextVAlignment::CENTER);
    tl->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height*0.6));
    bg->addChild(tl, 111);
    tl->setLineHeight(43);
//    tl->setScale(0.5);
    
    
    
    if (lText == "")
    {
        MenuItemImage* right = MenuItemImage::create("dialog/left.png", "dialog/left.png", CC_CALLBACK_1(NewDialog::onButton, this));
        right->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height*0.25));
        right->setTag(st_dialog_right);
        
        Label* right_label = Label::createWithTTF(xStr->getString("BTN_" + rText), "res/Muli-Regular.ttf", 13*2);
        right_label->setPosition(right->getContentSize()/2);
        right->addChild(right_label);
        right_label->setColor(Color3B(103, 103, 103));
//        right_label->setScale(0.5);
        
        Menu* lMenu = Menu::create( right, NULL);
        lMenu->setPosition(Vec2::ZERO);
        bg->addChild(lMenu);
    }else
    {
        bg->setTexture("other/popup_small@3x.png");
        
        MenuItemImage* left = MenuItemImage::create("dialog/left.png", "dialog/left.png", CC_CALLBACK_1(NewDialog::onButton, this));
        left->setPosition(Vec2(bg->getContentSize().width/4 + 30, bg->getContentSize().height*0.25));
        left->setTag(st_dialog_left);
        
        Label* left_label = Label::createWithTTF(xStr->getString("BTN_" + lText), "res/Muli-Regular.ttf", 13*2);
        left_label->setPosition(left->getContentSize()/2);
        left->addChild(left_label, 101);
        left_label->setColor(Color3B(103, 103, 103));
//        left_label->setScale(0.5);
        
        
        MenuItemImage* right = MenuItemImage::create("dialog/left.png", "dialog/left.png", CC_CALLBACK_1(NewDialog::onButton, this));
        right->setPosition(Vec2(bg->getContentSize().width/4*3 - 30, bg->getContentSize().height*0.25));
        right->setTag(st_dialog_right);
        
        Label* right_label = Label::createWithTTF(xStr->getString("BTN_" + rText), "res/Muli-Regular.ttf", 13*2);
        right_label->setPosition(right->getContentSize()/2);
        right->addChild(right_label, 101);
        right_label->setColor(Color3B(82, 145, 240));
//        right_label->setScale(0.5);
        
        Menu* lMenu = Menu::create(left, right, NULL);
        lMenu->setPosition(Vec2::ZERO);
        bg->addChild(lMenu);
    }
}

void NewDialog::initUIWithiPad(const string& tText, const string& lText, const string rText, float fontTSize)
{
    Sprite* bg = Sprite::create("other/popup_small_bg@3x.png");
    bg->setPosition(this->getContentSize()/2);
    bg->setScale(this->getContentSize().width*0.35/678);
    this->addChild(bg);
    _dialogBG = bg;
    
    Label* tl = Label::createWithTTF(xStr->getString("MSG_" + tText), "res/Muli-Regular.ttf", 20 * 2);
    tl->setColor(Color3B(103, 103, 103));
    tl->setDimensions(bg->getContentSize().width * 0.8 * 0.9, bg->getContentSize().height * 0.7);
    tl->setAlignment(cocos2d::TextHAlignment::CENTER);
    tl->setVerticalAlignment(cocos2d::TextVAlignment::CENTER);
    tl->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height*0.62));
    bg->addChild(tl, 111);
    tl->setLineHeight(60);
    //    tl->setScale(0.5);
    
    
    
    if (lText == "")
    {
        MenuItemImage* right = MenuItemImage::create("dialog/left.png", "dialog/left.png", CC_CALLBACK_1(NewDialog::onButton, this));
        right->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height*0.25));
        right->setTag(st_dialog_right);
        
        Label* right_label = Label::createWithTTF(xStr->getString("BTN_" + rText), "res/Muli-Regular.ttf", 20*2);
        right_label->setPosition(right->getContentSize()/2);
        right->addChild(right_label);
        right_label->setColor(Color3B(82, 145, 240));
        //        right_label->setScale(0.5);
        
        Menu* lMenu = Menu::create( right, NULL);
        lMenu->setPosition(Vec2::ZERO);
        bg->addChild(lMenu);
    }else
    {
        
        Sprite* lSprite = Sprite::create("other/popup_small_line@3x.png");
        lSprite->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height*0.25));
        bg->addChild(lSprite);
        
        MenuItemImage* left = MenuItemImage::create("dialog/left.png", "dialog/left.png", CC_CALLBACK_1(NewDialog::onButton, this));
        left->setPosition(Vec2(bg->getContentSize().width/4 + 40, bg->getContentSize().height*0.25));
        left->setTag(st_dialog_left);
        
        Label* left_label = Label::createWithTTF(xStr->getString("BTN_" + lText), "res/Muli-Regular.ttf", 20*2);
        left_label->setPosition(left->getContentSize()/2);
        left->addChild(left_label, 101);
        left_label->setColor(Color3B(103, 103, 103));
        //        left_label->setScale(0.5);
        
        
        MenuItemImage* right = MenuItemImage::create("dialog/left.png", "dialog/left.png", CC_CALLBACK_1(NewDialog::onButton, this));
        right->setPosition(Vec2(bg->getContentSize().width/4*3 - 40, bg->getContentSize().height*0.25));
        right->setTag(st_dialog_right);
        
        Label* right_label = Label::createWithTTF(xStr->getString("BTN_" + rText), "res/Muli-Regular.ttf", 20*2);
        right_label->setPosition(right->getContentSize()/2);
        right->addChild(right_label, 101);
        right_label->setColor(Color3B(82, 145, 240));
        //        right_label->setScale(0.5);
        
        Menu* lMenu = Menu::create(left, right, NULL);
        lMenu->setPosition(Vec2::ZERO);
        bg->addChild(lMenu);
    }
}

void NewDialog::onButton(Ref* ref)
{
    MenuItemImage* lmenu = (MenuItemImage*)ref;
    switch (lmenu->getTag())
    {
        case st_dialog_left:
        {
            if (_callBack)
            {
                _callBack(_leftText);
            }
            
            this->removeFromParentAndCleanup(true);
            
        }
            break;
            
        case st_dialog_right:
        {
            if (_callBack)
            {
                _callBack(_rightText);
            }
            
            this->removeFromParentAndCleanup(true);
        }
            break;
            
        default:
            break;
    }
}

void NewDialog::addButtonListener(const ccDialogCallback callBack)
{
    _callBack = callBack;
}

void NewDialog::addListener()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch* touch, Event* event)->bool{
        _firstPos = touch->getLocationInView();
        return true;
    };
    
    listener->onTouchMoved = [this](Touch* touch, Event* event){
        
    };
    
    listener->onTouchEnded = [this](Touch* touch, Event* event){
        Vec2 lPos = touch->getLocationInView();
        
        float distance = lPos.distance(_firstPos);
        
        lPos = Director::getInstance()->convertToGL(lPos);
        
        if (!_dialogBG->getBoundingBox().containsPoint(lPos) && distance < 50)
        {
            BaseScene* scene = (BaseScene*)this->getParent();
            if (scene)
            {
                scene->addBackListener();
            }
            
            this->removeFromParentAndCleanup(true);
        }
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    auto listener2 = EventListenerKeyboard::create();
    listener2->onKeyReleased = [this](EventKeyboard::KeyCode code, Event* event)
    {
        if(code == EventKeyboard::KeyCode::KEY_BACK)
        {
            this->removeFromParentAndCleanup(true);
        }
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener2, this);
}