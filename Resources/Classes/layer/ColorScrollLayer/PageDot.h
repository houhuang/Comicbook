//
//  PageDot.h
//  ColorFree
//
//  Created by houhuang on 15/8/31.
//
//

#ifndef __ColorFree__PageDot__
#define __ColorFree__PageDot__

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;

class PageDot : public Node
{
public:
    static PageDot* create(const std::string normalImage, const std::string selectdImage);
    bool init(const std::string normalImage, const std::string selectdImage);
    
    void setPageCount(const int pagesCount);
    void setSelectPage(const int selectPage);
    int getPageCount();
    
protected:
    std::string m_sNormalImageName;
    std::string m_sSelectImageName;
    int         m_iPageCount;
    Sprite*     m_pSelctedCoutrol;
};

#endif /* defined(__ColorFree__PageDot__) */
