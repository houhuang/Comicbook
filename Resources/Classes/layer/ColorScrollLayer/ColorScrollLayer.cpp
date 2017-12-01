//
//  ColorScrollLayer.cpp
//  ColorFree
//
//  Created by houhuang on 15/8/31.
//
//

#include "ColorScrollLayer.h"
//#include "../../define/Globalmacro.h"
//#include "../../helper/ScreenAdapterHelper.h"
#include "ColorSprite.h"
#include "ColorScrollLayerMask.h"
#include "UIColorManager.hpp"
#include "STVisibleRect.h"

#define COLOR_SPACE 10

enum
{
    st_btn_updown_tag = 10,
    st_btn_left_tag,
    st_btn_right_tag,
    
    st_resently_color_tag = 77,
    
    st_color_pack_newIcon_tag,

};

ColorScrollLayer::ColorScrollLayer()
{
    _index          = 0;
    m_iMaxPage      = 0;
    m_iCurrentPage  = 0;
    m_vFirstPoint   = Vec2(0, 0);
    m_vOffset       = Vec2(0, 0);
    m_sViewSize     = Size(0, 0);
    
    m_pScrollView   = NULL;
    m_pPageDot      = NULL;
    m_pContentLayer = NULL;
    m_pLeft         = NULL;
    m_pRight        = NULL;
    m_pMenu         = NULL;
    
    m_pPreColorSprite   = NULL;
    m_pColorGalleryText = NULL;

    
    m_bIsUpTowardsMove  = true;
    m_bIsInitColorSpriteVector = false;
    m_bIsMoveFinish     = true;
    m_bIsClickScroll    = false;
    _isEnableClickMenu  = true;
    
    m_vColorDatas = PalletteManager::getInstance()->getData();

}

ColorScrollLayer::~ColorScrollLayer()
{
    m_pPreColorSprite = NULL;

    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListenersForTarget(this);

}

ColorScrollLayer* ColorScrollLayer::create(Size size)
{
    ColorScrollLayer* layer = new ColorScrollLayer();
    if (layer && layer->init(size))
    {
        layer->autorelease();
        return layer;
    }
    
    CC_SAFE_DELETE(layer);
    return NULL;
}

bool ColorScrollLayer::init(Size size)
{
    if (!LayerColor::initWithColor(Color4B(248, 248, 248, 0))) return false;

    m_bIsFirstCreateLayer = true;
    
    this->ignoreAnchorPointForPosition(false);
    this->setAnchorPoint(Vec2(0.5, 0.5));
    this->setContentSize(Size(768, 200));
    
//    ColorScrollLayerMask* maskLayer = ColorScrollLayerMask::create(Size(this->getContentSize().width, 200));
//    maskLayer->ignoreAnchorPointForPosition(false);
//    maskLayer->setAnchorPoint(Vec2(0.5, 1));
//    maskLayer->setPosition(Vec2(this->getContentSize().width/2, 250));
//    this->addChild(maskLayer);
    
    m_pScrollView = cocos2d::extension::ScrollView::create();
//    m_pScrollView->setContentSize(Size(ScreenAdapterHelper::getValue(700), ScreenAdapterHelper::getValue(300)));
    m_pScrollView->setViewSize(Size(768, 200));
    m_pScrollView->setDirection(cocos2d::extension::ScrollView::Direction::HORIZONTAL);
    m_pScrollView->ignoreAnchorPointForPosition(false);
    m_pScrollView->setAnchorPoint(Vec2(0.5, 1));
    m_pScrollView->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height));
    log("%f",20/V::getRealDesignScale());
    m_pScrollView->setTouchEnabled(false);
    this->addChild(m_pScrollView);
    m_pScrollView->setColor(Color3B(248, 248, 248));
//    m_pScrollView->setSwallowsTouches(true);
    
    m_pPageDot = PageDot::create("cocos/other/page_turn2.png", "cocos/other/page_turn1.png");
    m_pPageDot->setPageCount((int)m_vColorDatas.size());
    m_pPageDot->setSelectPage(xPallette->getPalletteIndex());
    m_pPageDot->setPosition(Vec2(this->getContentSize().width/2 - 80, 130));
    this->addChild(m_pPageDot);
    
    m_iMaxPage = m_pPageDot->getPageCount();
    
    m_pContentLayer = Layer::create();
//    m_pContentLayer = LayerColor::create(Color4B(0, 250, 0, 100));
    m_pContentLayer->setContentSize(Size(m_pScrollView->getViewSize().width * m_iMaxPage, m_pScrollView->getViewSize().height));
    m_pScrollView->addChild(m_pContentLayer);

    m_pScrollView->setDelegate(this);
    

    this->initUILayer();
    
    m_iCurrentPage = xPallette->getPalletteIndex();
    this->doAction(NULL);
    
    
    this->initContentLayer();
    
    this->addListener();
    
    _isFirstLoad=false;
    
    return true;
}

void ColorScrollLayer::onEnter()
{
    LayerColor::onEnter();
    
    if(!_isFirstLoad)
    {
        _isFirstLoad=true;
        _orginPos=this->getPosition();
    }
}

void ColorScrollLayer::initUILayer()
{

    m_pLeft = MenuItemSprite::create(Sprite::create("cocos/other/btn_no.png"), NULL, CC_CALLBACK_1(ColorScrollLayer::doAction, this));
    m_pLeft->ignoreAnchorPointForPosition(false);
    m_pLeft->setAnchorPoint(Vec2(0.5, 0.5));
    m_pLeft->setPosition(Vec2(70, 100));
    m_pLeft->setTag(st_btn_left_tag);
    m_pLeft->setScale(0.85f);
//    m_pLeft->setVisible(false);
    m_pLeft->setColor(Color3B(136, 136, 136));
    
    m_pRight = MenuItemSprite::create(Sprite::create("cocos/other/btn_right.png"), NULL, CC_CALLBACK_1(ColorScrollLayer::doAction, this));
    m_pRight->ignoreAnchorPointForPosition(false);
    m_pRight->setAnchorPoint(Vec2(0.5, 0.5));
    m_pRight->setPosition(Vec2(this->getContentSize().width - 70, 100));
    m_pRight->setTag(st_btn_right_tag);
    m_pRight->setScale(0.85f);
//    m_pRight->setVisible(false);
    m_pRight->setColor(Color3B(136, 136, 136));
    
    m_pMenu = Menu::create( m_pLeft, m_pRight, NULL);
    m_pMenu->setPosition(Vec2::ZERO);
    this->addChild(m_pMenu);
    
    string str = m_vColorDatas.at(0).name;
    m_pColorGalleryText = Label::createWithSystemFont(str.c_str(), "res/kalinga.ttf", 34 * 2);
//    m_pColorGalleryText = Label::createWithTTF(str.c_str(), "Arial", ScreenAdapterHelper::getValue(34));
    m_pColorGalleryText->setPosition(Vec2(this->getContentSize().width/2, 25));
//    m_pColorGalleryText->setColor(Color3B::WHITE);
    this->addChild(m_pColorGalleryText);
    m_pColorGalleryText->setColor(xUcm->findUIColor("game_colorboard"));
    m_pColorGalleryText->setScale(0.5);

    Sprite* lock = Sprite::create("other/lock.png");
    lock->setPosition(Vec2(m_pColorGalleryText->getBoundingBox().size.width + 20, m_pColorGalleryText->getContentSize().height/2));
    m_pColorGalleryText->addChild(lock, 10);
    lock->setScale(0.6f);
    lock->setVisible(false);
//    lock->setColor(xUcm->findUIColor("game_colorboard"));
    _lock = lock;
    
    if (m_vColorDatas.at(0).isLock && xLock->isLocked(m_vColorDatas.at(0).name, LockManager::Type::PALLETTE))
    {
        lock->setVisible(true);
    }
}

void ColorScrollLayer::doAction(Ref* pSender)
{
    if (!_isEnableClickMenu)
    {
        return;
    }
    _isEnableClickMenu = false;
    MenuItemImage* clickMenu = (MenuItemImage*)pSender;
    if (!clickMenu)
    {
        m_pScrollView->setContentOffsetInDuration(Vec2(-m_pScrollView->getViewSize().width * m_iCurrentPage, 0), 0.0f);
        
        m_pPageDot->setSelectPage(m_iCurrentPage);
        
        string str = m_vColorDatas.at(m_iCurrentPage).name;
        m_pColorGalleryText->setString(str);
        _lock->setPosition(Vec2(m_pColorGalleryText->getBoundingBox().size.width + 20, _lock->getPosition().y));
        
        log("%d,%d",m_vColorDatas.at(m_iCurrentPage).isLock, xLock->isLocked(m_vColorDatas.at(m_iCurrentPage).name, LockManager::Type::PALLETTE));
        if (m_vColorDatas.at(m_iCurrentPage).isLock && xLock->isLocked(m_vColorDatas.at(m_iCurrentPage).name, LockManager::Type::PALLETTE))
        {
            _lock->setVisible(true);
        }else
        {
            _lock->setVisible(false);
        }
        
        this->updataColorPackText();
        
        scheduleOnce(schedule_selector(ColorScrollLayer::menuDelayEnable), 1.0f);
        return;
    }
    
    switch (clickMenu->getTag())
    {
        case st_btn_left_tag:
        {
//            m_pMenu->setEnabled(false);
            m_bIsMoveFinish = false;
            --m_iCurrentPage;
            
            if (m_iCurrentPage < 1)
            {
                m_pLeft->setNormalImage(Sprite::create("cocos/other/btn_no.png"));
            }else
            {
                m_pRight->setNormalImage(Sprite::create("cocos/other/btn_right.png"));
            }
            
            if (m_iCurrentPage < 0)
            {
                m_iCurrentPage = 0;
                
            }else
            {
                
                m_pScrollView->setContentOffsetInDuration(Vec2(-m_pScrollView->getViewSize().width * m_iCurrentPage, 0), 0.5f);
            }
            
            m_pPageDot->setSelectPage(m_iCurrentPage);
            
            
            string str = m_vColorDatas.at(m_iCurrentPage).name;
            m_pColorGalleryText->setString(str);
            _lock->setPosition(Vec2(m_pColorGalleryText->getBoundingBox().size.width + 20, _lock->getPosition().y));
            //
            log("%d,%d",m_vColorDatas.at(m_iCurrentPage).isLock, xLock->isLocked(m_vColorDatas.at(m_iCurrentPage).name, LockManager::Type::PALLETTE));
            if (m_vColorDatas.at(m_iCurrentPage).isLock && xLock->isLocked(m_vColorDatas.at(m_iCurrentPage).name, LockManager::Type::PALLETTE))
            {
                _lock->setVisible(true);
            }else
            {
                _lock->setVisible(false);
            }
            
            this->updataColorPackText();
            
            scheduleOnce(schedule_selector(ColorScrollLayer::menuDelayEnable), 1.0f);
        }
            break;
        case st_btn_right_tag:
        {
//            m_pMenu->setEnabled(false);
            ++m_iCurrentPage;
            
            if (m_iCurrentPage >= m_iMaxPage - 1)
            {
                m_pRight->setNormalImage(Sprite::create("cocos/other/btn_no.png"));
            }else
            {
                m_pLeft->setNormalImage(Sprite::create("cocos/other/btn_left.png"));
            }
            
            if (m_iCurrentPage >= m_iMaxPage)
            {
                m_iCurrentPage = m_iMaxPage - 1;
                
            }else
            {
                m_pScrollView->setContentOffsetInDuration(Vec2(-m_pScrollView->getViewSize().width * m_iCurrentPage, 0), 0.5f);
            }
            
            m_pPageDot->setSelectPage(m_iCurrentPage);
            
            string str = m_vColorDatas.at(m_iCurrentPage).name;
            m_pColorGalleryText->setString(str);
            _lock->setPosition(Vec2(m_pColorGalleryText->getBoundingBox().size.width + 20, _lock->getPosition().y));
            
            log("%d,%d",m_vColorDatas.at(m_iCurrentPage).isLock, xLock->isLocked(m_vColorDatas.at(m_iCurrentPage).name, LockManager::Type::PALLETTE));
            if (m_vColorDatas.at(m_iCurrentPage).isLock && xLock->isLocked(m_vColorDatas.at(m_iCurrentPage).name, LockManager::Type::PALLETTE))
            {
                _lock->setVisible(true);
            }else
            {
                _lock->setVisible(false);
            }
            
            this->updataColorPackText();
            
            MenuItemImage* lMenuItem = MenuItemImage::create("", "", [this](Ref*){
                
            });

            
            scheduleOnce(schedule_selector(ColorScrollLayer::menuDelayEnable), 1.0f);
        }
            break;
            
        default:
            break;
    }

}

void ColorScrollLayer::addListener()
{
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [this](Touch* touch, Event* event)
    {
        if(touch->getID() > 0)
        {
            return false;
        }
        
        if (!_isEnableClickMenu)
        {
            return false;
        }
        
        m_vOffset = m_pScrollView->getContentOffset();
        
        Vec2 lPos = touch->getLocationInView();
        lPos = Director::getInstance()->convertToGL(lPos);
        lPos = this->convertToNodeSpace(lPos);
        m_vFirstPoint = lPos;
        
        
        if (m_bIsMoveFinish)
        {
            //避免多点触摸问题
            if (!m_bIsClickScroll)
            {
                if (m_pScrollView->boundingBox().containsPoint(lPos))
                {
                    m_bIsClickScroll = true;
                    m_bIsMoveFinish = false;
                    m_pMenu->setEnabled(false);
                    
                    return true;
                }
            }
            
        }
        
        return false;
    };
    
    listener->onTouchMoved = [this](Touch* touch, Event* event)
    {
        Vec2 lPos = touch->getLocationInView();
        lPos = Director::getInstance()->convertToGL(lPos);
        lPos = this->convertToNodeSpace(lPos);
        
        float distance = lPos.x - m_vFirstPoint.x;
        
        m_pScrollView->setContentOffset(Vec2(distance + m_vOffset.x, 0));
    };
    
    listener->onTouchEnded = [this](Touch* touch, Event* event)
    {
        Vec2 endPoint = touch->getLocationInView();
        endPoint = Director::getInstance()->convertToGL(endPoint);
        endPoint = this->convertToNodeSpace(endPoint);
        
        float distance = endPoint.x - m_vFirstPoint.x;
        
        if (fabs(distance) > 200)
        {
            if (distance > 0)
            {
                --m_iCurrentPage;
                m_pRight->setNormalImage(Sprite::create("cocos/other/btn_right.png"));
            }
            else if (distance < 0)
            {
                ++m_iCurrentPage;
                m_pLeft->setNormalImage(Sprite::create("cocos/other/btn_left.png"));
            }
            
            if (m_iCurrentPage == m_iMaxPage - 1)
            {
                m_pRight->setNormalImage(Sprite::create("cocos/other/btn_no.png"));
            }
            if (m_iCurrentPage == 0)
            {
                m_pLeft->setNormalImage(Sprite::create("cocos/other/btn_no.png"));
            }
        }
        
        if (m_iCurrentPage < 0)
        {
            m_iCurrentPage = 0;
        }else if (m_iCurrentPage >= m_iMaxPage)
        {
            m_iCurrentPage = m_iCurrentPage - 1;
        }
        
        float moveTime = (768 - abs(distance))/768 * 0.5;
        
        m_pScrollView->setContentOffsetInDuration(Vec2(-m_pScrollView->getViewSize().width * m_iCurrentPage, 0), moveTime);
        m_pPageDot->setSelectPage(m_iCurrentPage);
        
        string str = m_vColorDatas.at(m_iCurrentPage).name;
        m_pColorGalleryText->setString(str);
        _lock->setPosition(Vec2(m_pColorGalleryText->getBoundingBox().size.width + 20, _lock->getPosition().y));
        
        log("%d,%d",m_vColorDatas.at(m_iCurrentPage).isLock, xLock->isLocked(m_vColorDatas.at(m_iCurrentPage).name, LockManager::Type::PALLETTE));
        if (m_vColorDatas.at(m_iCurrentPage).isLock && xLock->isLocked(m_vColorDatas.at(m_iCurrentPage).name, LockManager::Type::PALLETTE))
        {
            _lock->setVisible(true);
        }else
        {
            _lock->setVisible(false);
        }
        
        this->updataColorPackText();
        
        this->scheduleOnce(schedule_selector(ColorScrollLayer::areadyMoveFinish), moveTime);
        
        m_bIsClickScroll = false;
    };
    
    
    
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void ColorScrollLayer::areadyMoveFinish(float dt)
{
    m_bIsMoveFinish = true;
    m_pMenu->setEnabled(true);
}

void ColorScrollLayer::initContentLayer()
{
    m_vColorBGLayer.clear();
    m_vColorSprite.clear();
    
    
    for (int j = 0; j < m_vColorDatas.size(); j++)
    {
        LayerColor* layer = LayerColor::create(Color4B(248, 248, 248, 255), 768, 200);
        layer->ignoreAnchorPointForPosition(false);
        layer->setAnchorPoint(Vec2(0.5, 0.5));
        layer->setPosition(Vec2(384 + 768 * j, this->getContentSize().height * 0.5));
        m_pContentLayer->addChild(layer);
        
        m_vColorBGLayer.push_back(layer);

        log("%d",(int)m_vColorDatas.at(j).vRGBInfo.size());
        
        
//        if (m_vColorDatas.at(j).vRGBInfo.size() < 20)
//        {
//            for (int i = 0 ; i < m_vColorDatas.at(j).vRGBInfo.size(); i++)
//            {
//                PalletteInfo colorData = m_vColorDatas.at(j);
//                
//                ColorSprite* lSprite = ColorSprite::create("cocos/other/color01.png");
//                lSprite->setAnchorPoint(Vec2(0.5, 0.5));
//                lSprite->setDelegate(this);
//                lSprite->setIndex(i);
//                //            lSprite->setScale(5.0/((int)(m_vColorDatas.at(j).vRGBInfo.size()/2)));
//                
//                float sacle = 9.0/m_vColorDatas.at(j).vRGBInfo.size() + (m_vColorDatas.at(j).vRGBInfo.size()/9.0-1.0) * 0.25;
//                lSprite->setScale(sacle);
//                
//                log("%f",sacle + (m_vColorDatas.at(j).vRGBInfo.size()/9.0-1.0) * 0.5);
//                
//                float lX;
//                if (m_vColorDatas.at(j).vRGBInfo.size()%2 == 0)
//                {
//                    lX = 70 + (this->getContentSize().width-100)/(m_vColorDatas.at(j).vRGBInfo.size() - 1) * i + (18-m_vColorDatas.at(j).vRGBInfo.size())*1.8;
//                    if ((i+1)%2 == 0)
//                    {
//                        lX = 70 + (this->getContentSize().width-100)/(m_vColorDatas.at(j).vRGBInfo.size() - 1) * (i - 1) + (18-m_vColorDatas.at(j).vRGBInfo.size())*1.5;
//                    }
//                }else
//                {
//                    lX = 70 + (this->getContentSize().width-100)/m_vColorDatas.at(j).vRGBInfo.size() * i + (18-m_vColorDatas.at(j).vRGBInfo.size())*1.5;
//                }
//                
//                
//                float lY = (i % 2 == 0)? 50 : lSprite->getContentSize().height + 52.2 - m_vColorDatas.at(j).vRGBInfo.size() * 1.2;
//                lSprite->setPosition(Vec2(lX, lY));
//                
//                
//                lSprite->addListener(callfuncN_selector(ColorScrollLayer::reponseColorSpriteTouch), this);
//                lSprite->setColor(Color3B(colorData.vRGBInfo.at(i).r, colorData.vRGBInfo.at(i).g, colorData.vRGBInfo.at(i).b));
//                lSprite->setPalletteInfo(colorData);
//                layer->addChild(lSprite);
//                
//                lSprite->setClickSpite(false);
//                if (colorData.vRGBInfo.at(i).r == xPallette->getCurrentColor().r && colorData.vRGBInfo.at(i).g == xPallette->getCurrentColor().g && colorData.vRGBInfo.at(i).b == xPallette->getCurrentColor().b && m_bIsFirstCreateLayer)
//                {
//                    m_bIsFirstCreateLayer = false;
//                    lSprite->responseTouchEvent(Vec2(0, 0));
//                }
//                
//                m_vColorSprite.push_back(lSprite);
//                ++_index;
//            }
//        }
//        else
        {
            int surplus = m_vColorDatas.at(j).vRGBInfo.size()%3;
            for (int i = 0 ; i < 13; i++)
            {
                PalletteInfo colorData = m_vColorDatas.at(j);
                
                ColorSprite* lSprite = ColorSprite::create("cocos/other/color_bg.png");
                lSprite->setAnchorPoint(Vec2(0.5, 0.5));
                lSprite->setDelegate(this);
                lSprite->setIndex(i);
                
                float lX,lY;
                
                int count = (int)m_vColorDatas.at(j).vRGBInfo.size()/3;
                float offset = (9 - count)*(lSprite->getContentSize().width+COLOR_SPACE)/2;
                if (i < count)
                {
                    lX = offset + 34 + lSprite->getContentSize().width/2 - 1 + i * (lSprite->getContentSize().width - 2 + COLOR_SPACE);
                    lY = layer->getContentSize().height/3*2 + 15;
                }else if (i < 2*count + 1)
                {
                    lX = offset + 33 + lSprite->getContentSize().width-2 + (i - count - 1) * (lSprite->getContentSize().width-2 + COLOR_SPACE) + 5;
                    lY = layer->getContentSize().height/3 + 15 + 9;
                }
                
                
                lSprite->setPosition(Vec2(lX + 45, lY));
                
                int index = i;
                if (i == 12 && (colorData.name == "Skin Tones" || colorData.name == "Blacks&Whites" || colorData.name == "Reds" || colorData.name == "Greens" || colorData.name == "Oranges" || colorData.name == "Purples" || colorData.name == "Blues" || colorData.name == "Dreams" || colorData.name == "Sunset" || colorData.name == "Sky" || colorData.name == "Wildlife" || colorData.name == "Paradise" || colorData.name == "Cyans"))
                {
                    index = 17;
                }
                
                if (i == 12 && colorData.name == "Yellows")
                {
                    index = 16;
                }
                
                lSprite->addListener(callfuncN_selector(ColorScrollLayer::reponseColorSpriteTouch), this);
                lSprite->setColor(Color3B(colorData.vRGBInfo.at(index).r, colorData.vRGBInfo.at(index).g, colorData.vRGBInfo.at(index).b));
                lSprite->setPalletteInfo(colorData);
                layer->addChild(lSprite);
                
                lSprite->setClickSpite(false);
                if (colorData.vRGBInfo.at(index).r == xPallette->getCurrentColor().r && colorData.vRGBInfo.at(index).g == xPallette->getCurrentColor().g && colorData.vRGBInfo.at(index).b == xPallette->getCurrentColor().b && m_bIsFirstCreateLayer)
                {
                    m_bIsFirstCreateLayer = false;
                    lSprite->responseTouchEvent(Vec2(0, 0));
                }
                
                m_vColorSprite.push_back(lSprite);
                ++_index;
            }
        }
    }
}

void ColorScrollLayer::reponseTouchEvent(ColorSprite* pColorSprite)
{
    if(pColorSprite->getIsSelected())
    {
        PalletteManager::isAlreadySelectedColor =true;
//        DataManager::getInstance()->selectedColorData=pColorSprite->getColorData();
    }
}

void ColorScrollLayer::scrollViewDidScroll(cocos2d::extension::ScrollView* view)
{
//    float scrollViewOffset = m_pScrollView->getContentOffset().x;
//    
////    log("%f",scrollViewOffset + m_vColorBGLayer.at(1)->getPosition().x - ScreenAdapterHelper::getValue(384));
//    
//    for (auto layer : m_vColorBGLayer)
//    {
//        float offset = layer->getPosition().x + scrollViewOffset - 384;
////        log("%f",offset);
//        
//        if (offset > -768 && offset < 768)
//        {
//            for (auto lSprite : layer->getChildren())
//            {
//                ColorSprite* sprite = dynamic_cast<ColorSprite*>(lSprite);
//                if (offset > 0 )
//                {
//                    float scale = offset / 768;
//                    float offsetX = ( - sprite->getOriPositionX()) * scale;
//                    float offsetY = (100 - sprite->getOriPositionY()) * scale;
//                    
//                    lSprite->setPosition(Vec2(sprite->getOriPositionX() + offsetX, sprite->getOriPositionY() + offsetY));
//                    
//                }
//               
//                else
//                {
//                    float scale = abs(offset / 768);
//                    float offsetX = (768 - sprite->getOriPositionX()) * scale;
//                    float offsetY = (100 - sprite->getOriPositionY()) * scale;
//                    lSprite->setPosition(Vec2(sprite->getOriPositionX() + offsetX, sprite->getOriPositionY() + offsetY));
//                    
//                    //                log("scale = %f , offset = %f", scale, offset);
//                }
//            }
//
//        }else
//        {
//            if (offset > 0)
//            {
//                for (auto lSprite : layer->getChildren())
//                {
//                    ColorSprite* sprite = dynamic_cast<ColorSprite*>(lSprite);
//                    
//                    sprite->setPosition(Vec2( 0, 100));
//                }
//            }else
//            {
//                for (auto lSprite : layer->getChildren())
//                {
//
//                    ColorSprite* sprite = dynamic_cast<ColorSprite*>(lSprite);
//                    sprite->setPosition(Vec2( 768, 100));
//
//                }
//            }
//        
//        }
//    }
    
}

void ColorScrollLayer::scrollViewDidZoom(cocos2d::extension::ScrollView* view)
{

}

void ColorScrollLayer::menuDelayEnable(float dt)
{
    _isEnableClickMenu = true;
//    m_pMenu->setEnabled(true);
    m_bIsMoveFinish = true;
}

void ColorScrollLayer::removeDialog(Node* pNode)
{
    pNode->removeFromParentAndCleanup(true);
}

void ColorScrollLayer::reponseColorSpriteTouch(Node* pSender)
{
    ColorSprite* lSprite = (ColorSprite*)pSender;

    _colorIndex.page = m_iCurrentPage;
    _colorIndex.index = lSprite->getIndex();
    _eventDispatcher->dispatchCustomEvent("event_color", (void*)&_colorIndex);
 
    if (m_vColorDatas.at(m_iCurrentPage).isLock && xLock->isLocked(m_vColorDatas.at(m_iCurrentPage).name, LockManager::Type::PALLETTE))
    {
        return;
    }

    
    for (int i = 0 ; i < m_vColorSprite.size(); ++i)
    {
        ColorSprite* lSprite2 = m_vColorSprite.at(i);
        if (lSprite2)
        {
            lSprite2->selected(false);
        }
    }

    lSprite->responseTouchEvent(Vec2(0, 0));
    
}

void ColorScrollLayer::deleteCurrentColor()
{
    if (m_pPreColorSprite)
    {
        m_pPreColorSprite->removeFromParentAndCleanup(true);
        m_pPreColorSprite = NULL;
    }
}


void ColorScrollLayer::scrollMoveAction()
{
    
}

void ColorScrollLayer::resumeMenuEnable()
{
    m_pHideScroll->setEnabled(true);
}

void ColorScrollLayer::updataColorPackText()
{
    Sprite* _sprite = (Sprite*)this->getChildByTag(st_color_pack_newIcon_tag);
    if (_sprite)
    {
        _sprite->removeFromParentAndCleanup(true);
    }
    
//    m_pColorGalleryText->setColor(Color3B::WHITE);
}

void ColorScrollLayer::waitUnlockSuc()
{
    _lock->setVisible(false);
}
