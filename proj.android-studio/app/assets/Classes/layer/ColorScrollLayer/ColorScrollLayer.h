//
//  ColorScrollLayer.h
//  ColorFree
//
//  Created by houhuang on 15/8/31.
//
//

#ifndef __ColorFree__ColorScrollLayer__
#define __ColorFree__ColorScrollLayer__

#include <stdio.h>
//#include "cocos2d.h"
#include "LockManager.h"
#include "cocos-ext.h"
#include "PageDot.h"
#include "ColorSprite.h"
#include "PalletteManager.hpp"


USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

struct ColorIndex {
    int page;
    int index;
};

class ColorScrollLayer : public LayerColor , ScrollViewDelegate, ColorSpriteDelegate
{
public:
    
    ColorScrollLayer();
    ~ColorScrollLayer();

    static ColorScrollLayer* create(Size size);
    virtual bool init(Size size);
    void onEnter();
public:
    void addListener();
    
    void initContentLayer();
    void initUILayer();
    
    void reponseColorSpriteTouch(Node* pSender);
    void reponseUnlockColorButton(Node* pNode);
    void doAction(Ref* pSender);
    
    void menuDelayEnable(float dt);
    
    void scrollMoveAction();
    
    void reponsePurchseSuccessfulNofiction(Ref* ref);
    
    
    void removeDialog(Node* pNode);
    void unlockColorPack(Node* pNode);
    
    void updataColorPackText();
    
    void resumeMenuEnable();
    void areadyMoveFinish(float dt);
    
    void deleteCurrentColor();
 
    void reponseTouchEvent(ColorSprite* pColorSprite);
    
    void waitUnlockSuc();
    
public:
    void scrollViewDidScroll(extension::ScrollView* view);

    void scrollViewDidZoom(extension::ScrollView* view);
    
protected:
    int     m_iMaxPage;
    int     m_iCurrentPage;
    Vec2    m_vFirstPoint;
    Vec2    m_vOffset;
    Size    m_sViewSize;
    
    extension::ScrollView* m_pScrollView;
    PageDot*    m_pPageDot;
    Layer*      m_pContentLayer;

    LayerColor*     m_pPreColorSprite;
    
    MenuItemSprite* m_pLeft;
    MenuItemSprite* m_pRight;
    MenuItemSprite* m_pHideScroll;
    Menu*           m_pMenu;
    Label*          m_pColorGalleryText;
    
    bool    m_bIsUpTowardsMove;
    bool    m_bIsMoveFinish;
    
    bool    m_bIsClickScroll;
    
    
    //颜色数据
    std::vector<PalletteInfo> m_vColorDatas;
    
//    //最近使用的三种颜色
//    std::vector<ColorSprite*> m_pRensentlyColor;
    
    //vector下标
    int _index;
    
    bool m_bIsInitColorSpriteVector;
    bool m_bIsFirstCreateLayer;

    Vec2 _orginPos;
    bool _isFirstLoad;
    
    std::vector<LayerColor*>    m_vColorBGLayer;
//    ColorSprite*                m_pColorSprite;
    std::vector<ColorSprite*>   m_vColorSprite;
    ColorIndex  _colorIndex;
    
    Sprite* _lock;
    
    bool _isEnableClickMenu;
};

#endif /* defined(__ColorFree__ColorScrollLayer__) */
