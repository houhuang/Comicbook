//
//  CoverSprite.h
//  ComicBook
//
//  Created by houhuang on 17/12/1.
//
//

#ifndef CoverSprite_h
#define CoverSprite_h

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;
using namespace std;

class CoverSprite : public Node
{
public:
    static CoverSprite* create(const string folder, string url, Size size);
    virtual bool init(const string folder, string url, Size size);
    
    ~CoverSprite();
    CoverSprite();
    
public:
    void diaplay();
    void imageLoadedCallback(Texture2D* texture);
    void LoadMapSprite(EventCustom* event);
    
    void responseDownloadSuc(EventCustom* event);
    
public:
    void addListener(Ref* target, SEL_CallFuncO callBack, bool isSwallow = false);
protected:
    string getFullPathFromFolder();
    
protected:
    string _folder;
    string _url;

    Ref* _target;
    SEL_CallFuncO _callBack;
    Vec2    _firstVec;
    bool    _loadedSuc;
};

#endif /* CoverSprite_h */
