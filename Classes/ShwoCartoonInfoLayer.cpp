//
//  ShwoCartoonInfoLayer.cpp
//  ComicBook
//
//  Created by houhuang on 17/12/4.
//
//

#include "ShwoCartoonInfoLayer.hpp"
#include "STVisibleRect.h"
#include "DownloadManager.hpp"

ShowCartoonInfoLayer* ShowCartoonInfoLayer::create(CartoonInfo& info)
{
    ShowCartoonInfoLayer* layer = new ShowCartoonInfoLayer();
    if (layer && layer->init(info))
    {
        layer->autorelease();
        return layer;
    }
    
    CC_SAFE_DELETE(layer);
    return nullptr;
}

ShowCartoonInfoLayer::~ShowCartoonInfoLayer()
{
    _eventDispatcher->removeEventListenersForTarget(this);
}

ShowCartoonInfoLayer::ShowCartoonInfoLayer()
{
    _bgSprite = nullptr;
    _firstVec = Vec2(0, 0);
    _label = nullptr;
}

bool ShowCartoonInfoLayer::init(CartoonInfo& info)
{
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 200))) return false;
    
    _cartoonInfo = info;
    
    initUI();
    this->addListener();
    
    auto downloadCsvSuc = EventListenerCustom::create(st_download_picture_suc, CC_CALLBACK_1(ShowCartoonInfoLayer::responseDownloadPicCsvSuc, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(downloadCsvSuc, this);
    return true;
}

void ShowCartoonInfoLayer::initUI()
{
    Sprite* lSprite = Sprite::create("menu_bg.png");
    float oriScale = V::isIpad()? (this->getContentSize().width*0.8):(this->getContentSize().width*0.9);
    lSprite->setScale(oriScale/1018.);
    lSprite->setPosition(this->getContentSize()/2);
    this->addChild(lSprite);
    _bgSprite = lSprite;
    
    string path = "cover/" +_cartoonInfo.folder + "_cover.png";
    Sprite* cover = Sprite::create(path);
    cover->setAnchorPoint(Vec2(0.5, 1));
    lSprite->addChild(cover);
    cover->setScale(1018./3/cover->getContentSize().width);
    cover->setPosition(Vec2(lSprite->getContentSize().width/4, lSprite->getContentSize().height - 85 - 56));
    
    
    string text = "名字:" + _cartoonInfo.name + "\n"
                    +"区域:" + _cartoonInfo.area + "\n"
                    +"类型:" + xCartoon->getCurrentCategory().name + "\n"
                    + "是否完结:是" + "\n"
                    + "页数:" + to_string(0);
    
    Label* label = Label::createWithTTF(text, "fonts/d2.ttf", 40);
    label->setAlignment(cocos2d::TextHAlignment::LEFT);
    label->setAnchorPoint(Vec2(0, 0.5));
    label->setPosition(Vec2(lSprite->getContentSize().width/2 - 60, lSprite->getContentSize().height * 0.75));
    label->setLineHeight(100);
    label->setColor(Color3B(76, 76, 76));
    lSprite->addChild(label);
    _label = label;
    
    
    
    string desStr = "   " + _cartoonInfo.des;
    int fontSize = 35;
    if (desStr.length() > 1300)
    {
        fontSize = 25;
    }
    
    Label* des = Label::createWithTTF(desStr, "fonts/d2.ttf", fontSize);
    des->setAnchorPoint(Vec2(0.5, 1));
    des->setAlignment(cocos2d::TextHAlignment::LEFT);
    des->setDimensions(950, 600);
    des->setPosition(Vec2(lSprite->getContentSize().width/2, lSprite->getContentSize().height*0.6));
    des->setColor(Color3B(76, 76, 76));
    lSprite->addChild(des);
    
    int x = desStr.length();
    
    this->loadPictureCsv();
}

void ShowCartoonInfoLayer::responseDownloadPicCsvSuc(EventCustom* event)
{
    this->loadPictureCsv();
}

void ShowCartoonInfoLayer::loadPictureCsv()
{
    string path = "piccsv/" + _cartoonInfo.folder + "_picture.csv";
    if (FileUtils::getInstance()->isFileExist(path))
    {
        xCartoon->readCurrentPictureCsv(path);
        
        string text = "名字:" + _cartoonInfo.name + "\n"
        +"区域:" + _cartoonInfo.area + "\n"
        +"类型:" + xCartoon->getCurrentCategory().name + "\n"
        + "是否完结:是" + "\n"
        + "页数:" + to_string(xCartoon->getCurrentPictureInfo().size());
        
        if (_label)
        {
            _label->setString(text);
        }
    }else
    {
        xDownload->downloadPictureCsv(_cartoonInfo.folder);
    }
}

void ShowCartoonInfoLayer::addListener()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [&](Touch* touch, Event* event)->bool{
    
        _isClickThis = false;
        _firstVec = touch->getLocation();
        Vec2 lPos = _bgSprite->getParent()->convertToNodeSpace(touch->getLocation());
        if (_bgSprite->getBoundingBox().containsPoint(lPos))
        {
            _isClickThis = true;
        }
        return true;
    };
    
    listener->onTouchMoved = [&](Touch* touch, Event* event){
        
    };
    
    listener->onTouchEnded = [&](Touch* touch, Event* event){
        float distance = _firstVec.distance(touch->getLocation());
        if (distance < 10 && !_isClickThis)
        {
            Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("remove_cartoonLayer");
        }
    };
    
    listener->onTouchCancelled = [&](Touch* touch, Event* event){
        float distance = _firstVec.distance(touch->getLocation());
        if (distance < 10 && !_isClickThis)
        {
            Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("remove_cartoonLayer");
        }
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}





