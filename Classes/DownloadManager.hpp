//
//  DownloadManager.hpp
//  ComicBook
//
//  Created by houhuang on 17/12/1.
//
//

#ifndef DownloadManager_hpp
#define DownloadManager_hpp

#include <stdio.h>
#include "network/HttpRequest.h"
#include "network/HttpClient.h"
#include "cocos-ext.h"
#include "CartoonManager.hpp"
USING_NS_CC;

#define xDownload DownloadManager::getInstance()

#define st_download_cover_finish    "st_download_cover_finish"
#define st_download_picture_suc     "st_download_picture_suc"

using namespace std;
using namespace network;

typedef struct _DownloadCoverInfo
{
    string url;
    string folder;
}DownloadCoverInfo;

class DownloadManager
{
public:
    static DownloadManager* getInstance();
    DownloadManager();
    
public:
    void downloadCarttonCsv();
    
    void downloadPictureCsv(string folder);
    
    //download cover
    void addNeedDownloadCover(DownloadCoverInfo info);
    void startDownloadCover();
    void downloadCover(string url, string folder);
    vector<DownloadCoverInfo> _downloadCoverInfo;
    bool _isDownloadingCover;
    
    //download picture
    void downloadPicture(Picture picture);
private:
    bool createDirectory(const char *path);
    string UrlGB2312(const char * str);
};

#endif /* DownloadManager_hpp */
