//
//  ServerSprite.hpp
//  IMG1001
//
//  Created by houhuang on 15/12/10.
//
//

#ifndef ServerSprite_hpp
#define ServerSprite_hpp

#include <stdio.h>
#include "cocos2d.h"
//#include "../define/Globalmacro.h"
//#include "../data/DataManager.h"
//#include "TargetSprite.h"
USING_NS_CC;
using namespace std;

#define st_property(varType, varName, funName)\
protected: varType varName;\
public: virtual varType get##funName(void)\
{\
return varName; \
}\
public: virtual void set##funName(varType var) \
{\
varName=var; \
}\

#define st_datamanager_dynamicLoadTexture   "st_datamanager_dynamicLoadTexture"

class ServerSprite : public Node
{
public:
    ~ServerSprite();
    ServerSprite();
    
    static ServerSprite* create(const string& imageUrl, int id, int likeNumber);
    bool init(const string& imageUrl, int id, int likeNumber);
    
    static ServerSprite* create(int id, int likeNumber);
    bool init(int id, int likeNumber);
    
    static ServerSprite* create(const int id);
    bool init(const int id);
    
public:
    void display();
    void imageLoadedCallback(Ref* pSender);
    void LoadMapSprite(Ref* pSender);
    
    void waitDownLoadSuc(Ref* ref);
    void reponseMenu(Ref* ref);
    string getImageFullPath();
    
    void hideThis();

    void removeTextureCache();
    
    void responseGetLikeSuc(Ref* ref);
public:
    st_property(int, _imageID, ImageID);
    st_property(int, _likeNumber, LikeNumber);
    st_property(bool, _isMylikeSprite, IsMylikeSprite);
    
public:
    void addListener(Ref* target, SEL_CallFuncN callBack, bool isSwallow = false);
    
protected:
    Sprite*     m_pRotateSprite;
    string      m_sImageUrl;
    bool        m_bIsclickLike;
    Sprite*     m_pLikeSprite;
    Label*      m_pLikeLabel;
    Sprite*     m_pTextureSprite;
    Texture2D*  m_pTargetTexture;
    MenuItemImage*  m_pMenuItem;
    
    bool        m_bIsDownloadFinish;
    bool        m_bIsHide;
    
    //listener
    SEL_CallFuncN   _callBack;
    Ref*            _target;
    Vec2            _firstPos;
    bool            _isClick;
    
    Sprite*         _likeSprite;
};

#endif /* ServerSprite_hpp */
