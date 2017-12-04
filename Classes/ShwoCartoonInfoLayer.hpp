//
//  ShwoCartoonInfoLayer.hpp
//  ComicBook
//
//  Created by houhuang on 17/12/4.
//
//

#ifndef ShwoCartoonInfoLayer_hpp
#define ShwoCartoonInfoLayer_hpp

#include <stdio.h>
#include "CartoonManager.hpp"
USING_NS_CC;
class ShowCartoonInfoLayer : public LayerColor
{
public:
    static ShowCartoonInfoLayer* create(CartoonInfo& info);
    virtual bool init(CartoonInfo& info);
    
    ~ShowCartoonInfoLayer();
    ShowCartoonInfoLayer();
public:
    void initUI();
    
    void addListener();
protected:
    CartoonInfo _cartoonInfo;
    
    Sprite* _bgSprite;
    bool    _isClickThis;
    Vec2    _firstVec;
};

#endif /* ShwoCartoonInfoLayer_hpp */
