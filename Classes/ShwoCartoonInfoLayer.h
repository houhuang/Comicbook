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
#include "cocos2d.h"
#include "CartoonManager.h"
USING_NS_CC;

#define st_remove_showCartoonLayer  "remove_cartoonLayer"

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
    
    void loadPictureCsv();
public:
    void responseDownloadPicCsvSuc(EventCustom* event);
    void onButton(Ref* ref);
protected:
    CartoonInfo _cartoonInfo;
    
    Sprite* _bgSprite;
    bool    _isClickThis;
    Vec2    _firstVec;
    
    Label*  _label;
    bool    _isDownloadSuc;
};

#endif /* ShwoCartoonInfoLayer_hpp */
