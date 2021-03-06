//
//  ContentLayer.h
//  ComicBook
//
//  Created by houhuang on 17/12/6.
//
//

#ifndef ContentLayer_h
#define ContentLayer_h

#include <stdio.h>
#include "cocos2d.h"
#include "CartoonManager.h"
USING_NS_CC;
using namespace std;


class ContentLayer : public LayerColor
{
public:
    static ContentLayer* create(Picture& picture);
    virtual bool init(Picture& picture);
    
    ContentLayer();
    
    void onEnterTransitionDidFinish();
    
    void display();
    void imageLoadedCallback(Texture2D* texture);
    void LoadMapSprite(EventCustom* event);
    
    void responseDownloadSuc(EventCustom* event);
    
    void updateContent(Picture& picture);
    
private:
    string getPath();
    Picture _picture;
    Sprite* _contentSprite;
    Sprite* _rotateSprite;
    
    bool    _alreadyLoaded;
    string  _preImagePath;
    
};

#endif /* ContentLayer_h */
