//
//  PageDot.cpp
//  ColorFree
//
//  Created by houhuang on 15/8/31.
//
//

#include "PageDot.h"
//#include "../../helper/ScreenAdapterHelper.h"
//#include "../../helper/DeviceHelper.h"
#define INTERVAL 8

PageDot* PageDot::create(const std::string normalImage, const std::string selectdImage)
{
    PageDot* lNode = new PageDot();
    if (lNode && lNode->init(normalImage, selectdImage))
    {
        lNode->autorelease();
        return lNode;
    }
    
    CC_SAFE_DELETE(lNode);
    return NULL;
}

bool PageDot::init(const std::string normalImage, const std::string selectdImage)
{
    if (!Node::create())
    {
        return false;
    }
    
    m_sNormalImageName = normalImage;
    m_sSelectImageName = selectdImage;
   
    
    return true;
}

void PageDot::setPageCount(const int pagesCount)
{
    m_iPageCount = pagesCount;
    float offset = 0.0f;
    
    for (int i = 0; i < m_iPageCount; ++i)
    {
        Sprite* normal = Sprite::create(m_sNormalImageName);
        normal->setPosition(Vec2(offset + normal->getContentSize().width/2, 20));
        offset += normal->getContentSize().width + 5;
        this->addChild(normal);
//        normal->setVisible(false);
    }
    
    m_pSelctedCoutrol = Sprite::create(m_sSelectImageName);
    m_pSelctedCoutrol->setPosition(Vec2(m_pSelctedCoutrol->getContentSize().width/2, m_pSelctedCoutrol->getContentSize().height/2));
    this->addChild(m_pSelctedCoutrol);
//    m_pSelctedCoutrol->setVisible(false);
    
    this->setContentSize(Size(m_iPageCount * (m_pSelctedCoutrol->getContentSize().width + INTERVAL), m_pSelctedCoutrol->getContentSize().height));
}

void PageDot::setSelectPage(const int selectPage)
{
    
    m_pSelctedCoutrol->setPosition(Vec2(selectPage * m_pSelctedCoutrol->getContentSize().width + 5 * (selectPage + 1) + 2.0f, 20));

}

int PageDot::getPageCount()
{
    return m_iPageCount;
}