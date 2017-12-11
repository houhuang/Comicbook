//
//  NewDownloadManager.hpp
//  ColorGarden
//
//  Created by houhuang on 17/2/16.
//
//

#ifndef NewDownloadManager_hpp
#define NewDownloadManager_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "network/HttpRequest.h"
#include "network/HttpClient.h"
USING_NS_CC;
using namespace network;

#define xNewDownLoad    NewDownloadManager::getInstance()

typedef enum _ImageType
{
    PNG_THUMB_512   = 11,
    PNG_NOMAL_1024,
    PNG_MASK_1024,
    PNG_COVER_512,
}ImageType;

typedef struct _DownloadSpriteInfo
{
    std::string url;
    std::string name;
    ImageType   imageType;
}DownloadSpriteInfo;

class NewDownloadManager
{
public:
    static NewDownloadManager* getInstance();
    NewDownloadManager();
    ~NewDownloadManager();
    
public:
    void downloadJsonFile();
    void startDownloadImage(const int& index = 0);
    
    void addNeedDownloadImage(DownloadSpriteInfo& info);
    void removeNeedDownloadImage(const std::string& name, ImageType imageType = ImageType::PNG_THUMB_512);
    void removeAllNeedDownloadImage();
    
    void getServiceData(const std::string& url);
protected:
    void downloadImage(std::string url, std::string name, ImageType imageType);

public:
    std::vector<DownloadSpriteInfo> m_vNeedDownloadSpr;
    bool    m_bIsDownloading;
    
    static bool m_bIsdownloadFail;
protected:
    bool createDirectory(const char *path);
    
};

#endif /* NewDownloadManager_hpp */
