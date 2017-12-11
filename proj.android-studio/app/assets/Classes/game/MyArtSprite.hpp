//
//  MyArtSprite.hpp
//  ALABS0010
//
//  Created by houhuang on 16/3/31.
//
//

#ifndef MyArtSprite_hpp
#define MyArtSprite_hpp

#include "UIHeader.h"

#define st_deleteMyArtSprite    "st_deleteMyArtSprite"

class MyArtSprite : public Node
{
public:
    static MyArtSprite* create(const string& imageName, const Color3B& bgColor, const int serveId, bool isShowLike = true);
    
    MyArtSprite();
    ~MyArtSprite();
    
    bool init(const string& imageName, const Color3B& bgColor, const int serveId, bool isShowLike);
    void onButton(Ref* pSender, Widget::TouchEventType type);
    
public:
    void display();
    void imageLoadedCallback(Ref* ref);
    void loadGraphSprite(Ref* ref);

    void waitGetlikeSuc(Ref* ref);
public:
    void removeTexture();
    void onDialog(const string& name);
    string getSpriteName();
    
    void setLoadImageName(const string& name);
    void setPreName(const string& name);
    string getPreName();
    bool isShared();
    
    Color3B getBGColor();
    
    //多处用到MyartSprite 但里面图片尺寸不同 用此方法调节图片尺寸
    void setImageScale(float scale);
public:

    void addTouchListener(Ref* target, SEL_CallFuncN callBack, bool isSwallow = true);
    void removeTouchListener();

    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    virtual void onTouchCancelled(Touch *touch, Event *unused_event);

protected:
    string m_sNeedLoadImage;
    string m_sNeedLoadImageT;
    string m_sNeedLoadImageP;
    Sprite* m_pRotateSprite;
    Texture2D* m_pTexture2D;
    Color3B m_cBgColor;
    
    SEL_CallFuncN _selector;
    Ref* _target;
    bool _isClick;
    
    Vec2 firstPos;

    Label* m_pLikeLabel;
    int m_iServeId;
    string m_sPreName;
    
    float  _ImageScale;
};

#endif /* MyArtSprite_hpp */
