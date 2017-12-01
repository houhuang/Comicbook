//
//  DownloadManager.hpp
//  ALABS0010
//
//  Created by houhuang on 16/3/22.
//
//

#ifndef DownloadManager_hpp
#define DownloadManager_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "GraphManager.hpp"
#include "network/HttpRequest.h"
#include "network/HttpClient.h"
USING_NS_CC;
using namespace std;

#define DldM_getInstance    DownloadManager::getInstance()
#define xDownload           DownloadManager::getInstance()

#define st_download_finish  "st_download_finish"

typedef enum _DownLoadState
{
    ENABLE_STATE = 0, //初始状态
    CHECK_NEW_VERSION_STATE, //检查是否有新版本
    CHECK_NEW_VERSION_STATE2, //手动检测是否有新版本
    CHECK_NEW_VERSION_ERROR_STATE,  //网络错误(从服务器上获取数据出错，并非指没网)
    FIND_NEW_VERSION_STATE, //发现新版本（自动检测，每次游戏弹一次）
    FIND_NEW_VERSION_STATE2, //手动检测
    NO_NEW_VERSION_STATE,   //没有新版本
    START_DOWNLOAD_STATE,   //开始下载
    DOWNLOAD_RESOURCE_ERROR_STATE,  //下载资源途中网络错误
    DOWNLOAD_FINISH_STATE,  //下载完成
}DownloadState;

class DownloadManager
{
public:
    struct UpdateInfo
    {
        string filename;
        float   version;
    };
    
public:
    static DownloadManager* getInstance();
    bool init();
    
public:
    void setCurrentDownState(DownloadState pState);
    DownloadState getCurrentDownState();
    
    void setColorSystemVersion();
    float getColorSystemVersion(const string & filename);
    void refreshData();
    void excuteFunc(DownloadState pState);
    void saveCsvFile();
    void onDialog(const string & name);
    void onHttp(cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response);
    
protected:
    void checkIsNeedUpdate();
    bool createDirectory(const char *path);
    void downloadCsv(const string& fileName);
    void readGraphData();
    
    void excuteDownload();
    void startDownload(const string& fileName, int chapter_id);
    void parsePHP(string version);
    	
public:
    DownloadState m_dCurrentDownState;
    string m_sSavePath;
    vector<Graph> m_vGraphInfo;
    
    float m_fTargetVersion;
    string m_sCsvDataBuffer;
    int m_iCsvDataSize;
    
    int m_iDownloadCount;
    int m_iDownloadCsvCount;
    vector<UpdateInfo>  _updates;
    bool _hinted = false;
    int _retryCount = 0;
    bool _downloadBlocked = false;
    bool _delayDownload = false;
    
    bool _hasGraph;
    
    vector<string> m_vNeedDownloadChapter;
    bool _isAutoCheck;
    static bool _isDownloading;
};

#endif /* DownloadManager_hpp */
