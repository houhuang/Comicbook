//
//  CoverSprite.hpp
//  ComicBook
//
//  Created by houhuang on 17/12/1.
//
//

#ifndef CoverSprite_hpp
#define CoverSprite_hpp

#include <stdio.h>
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
protected:
    string getFullPathFromFolder();
    
protected:
    string _folder;
    string _url;
};

#endif /* CoverSprite_hpp */
