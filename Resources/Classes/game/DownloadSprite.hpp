//
//  DownloadSprite.hpp
//  ColorGarden
//
//  Created by houhuang on 17/2/16.
//
//

#ifndef DownloadSprite_hpp
#define DownloadSprite_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "GraphManager.hpp"
#include "UITools.h"
USING_NS_CC;

class DownloadSprite : public Node
{
public:
    //graph download
    static DownloadSprite* create(Graph& graph);
    bool init(Graph& graph);
    
    static DownloadSprite* create(std::string& url, std::string& name, std::string& redownload);
    bool init(std::string& url, std::string& name, std::string& redownload);
    
    DownloadSprite();
    ~DownloadSprite();
    
public:
    void display();
    void display_cover();
    
    void imageLoadedCallback();
    void imageLoadedCallback_cover();
    
    void loadGraphSprite(Ref* ref);
    void responseImageDownloaded(EventCustom* event);
    
    bool getCurrentGraphIslock();
    bool isDownloaded();

    void removeTextureCache();
    
public:
    void addListener(Ref* target, SEL_CallFuncN selecter, bool isSwallow = false);
public:
    Graph   m_gGraphInfo;

protected:
    bool    m_bIsLock;
    bool    m_bIsCover;
    bool    m_bIsLoaded;
    
    //优化性能,保证每个Sprite下载和多线程加载后的逻辑只进行一次
    bool    m_bIsThreaded;
    bool    m_bIsDownloaded;
    
    Sprite* m_pRotateSprite;
    
    std::string m_sUrl;
    std::string m_sName;
    
    std::string m_sThumb;
    std::string m_sSave;
    std::string m_sRedownload;
    
    Texture2D*  m_pThumbTexture;
    Texture2D*  m_pSaveTexture;
    
    SEL_CallFuncN _callBack;
    Ref*          _target;
    Vec2          _firstPos;
    
    Size          _thisSize;
    LayerColor*   _bgLayer;
};

#endif /* DownloadSprite_hpp */
