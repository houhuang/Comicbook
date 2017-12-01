//
//  DownloadManager.cpp
//  ALABS0010
//
//  Created by houhuang on 16/3/22.
//
//

#include "DownloadManager.hpp"
#include "CSVParse.h"
#include "GraphManager.hpp"
#include "SceneManager.h"
#include "ChapterManager.hpp"
#include "PalletteManager.hpp"
#include "Dialog.h"
#include <stdint.h>
#include "SearchPathManager.hpp"

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32) && (CC_TARGET_PLATFORM != CC_PLATFORM_WP8) && (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#include <vector>
#endif

using namespace cocos2d;
using namespace network;

static DownloadManager* _instance = NULL;
bool DownloadManager::_isDownloading = false;

#define st_csv_file_graph       "graph.csv"
#define st_csv_file_chapter     "chapter4.csv"
#define st_csv_file_pallette    "http://liveupdate.toyboxapps.com/coloring/ALABS0006/color/"
#define st_csv_file_xxx         "language.csv"

#define st_csv_file_name        "graph.csv"
#define st_version_file_name    "version_2.php"

#define st_versionCsvUrl "http://liveupdate.toyboxapps.com/coloring/ALABS0006/color/graph.csv"
#define st_versionUrl "http://liveupdate.toyboxapps.com/coloring/ALABS0006/color/version_2.php"
#define st_download_dir "http://liveupdate.toyboxapps.com/coloring/ALABS0006/color"

DownloadManager* DownloadManager::getInstance()
{
    if (!_instance)
    {
        _instance = new DownloadManager();
        _instance->init();
    }
    
    return _instance;
}

bool DownloadManager::init()
{
    m_sSavePath = FileUtils::getInstance()->getWritablePath();
    m_iDownloadCount = 0;
    _isAutoCheck = false;
    
    m_dCurrentDownState = ENABLE_STATE;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    m_sSavePath+="color";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    m_sSavePath+="/color";
#endif

    this->createDirectory(m_sSavePath.c_str());

    SearchPathManager::getInstance()->updateSearchPath();
    
    return true;
}

void DownloadManager::excuteFunc(DownloadState pState)
{
    _isAutoCheck = false;
    m_dCurrentDownState = pState;
    switch (m_dCurrentDownState)
    {
        case ENABLE_STATE:
        {
        
        }
            break;
            
        case CHECK_NEW_VERSION_STATE:
        {
            _isAutoCheck = true;
            log(".....check is new version");
            this->checkIsNeedUpdate();
        }
            break;
            
        case CHECK_NEW_VERSION_STATE2:
        {
            _isAutoCheck = false;
            log(".....check is new version");
            this->checkIsNeedUpdate();
        }
            break;
            
        case CHECK_NEW_VERSION_ERROR_STATE:
        {
            log(".....check new version error");
        }
            break;
            
        case FIND_NEW_VERSION_STATE:
        {
            log(".....find new version");
            if (!_hinted) {
                if (Director::getInstance()->getRunningScene()->getName() == "ChapterScene")
                {
                    _hinted = true;
                    xDialog->show("find_new", "cancel", "Download", CC_CALLBACK_1(DownloadManager::onDialog, this));
                }
            }
        }
            break;
            
        case FIND_NEW_VERSION_STATE2:
        {
            this->excuteFunc(DownloadState::START_DOWNLOAD_STATE);
        }
            break;
            
        case NO_NEW_VERSION_STATE:
        {
            log(".....not find new version");
        }
            break;
            
        case DOWNLOAD_RESOURCE_ERROR_STATE:
        {
            log(".....download resource error");
        }
            break;
            
        case START_DOWNLOAD_STATE:
        {
            m_iDownloadCsvCount = 0;
            for (auto update : _updates) {
                downloadCsv(update.filename + ".csv");
            }
            log(".....start download...");
        }
            break;
            
        case DOWNLOAD_FINISH_STATE:
        {
            if (m_iDownloadCsvCount >= _updates.size())
            {                
                xScene->updateCSV();
                log(".....download finish!");
                NotificationCenter::getInstance()->postNotification(st_download_finish);
            }
  
        }
            break;
        default:
            break;
    }
}

void DownloadManager::checkIsNeedUpdate()
{
    HttpRequest* lRequest=new HttpRequest;
    string lPath=st_download_dir;
    lPath+="/";
    lPath+=st_version_file_name;
    
    lRequest->setUrl(lPath.c_str());
    lRequest->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    lRequest->setResponseCallback([this,lRequest](network::HttpClient* client, network::HttpResponse* response) {
        if (!response)
        {
            CCLOG("response failed");
            lRequest->release();
            
            DownloadManager::getInstance()->excuteFunc(CHECK_NEW_VERSION_ERROR_STATE);
            
            return;
        }
        
        if (!response->isSucceed())
        {
            CCLOG("err0r=%s",response->getErrorBuffer());
            CCLOG("response failed   获取网络资源失败");
            lRequest->release();
            
            DownloadManager::getInstance()->excuteFunc(CHECK_NEW_VERSION_ERROR_STATE);
            return;
        }
        std::vector<char> *buffer = response->getResponseData();
        std::string bufffff(buffer->begin(),buffer->end());
        
        if(!bufffff.empty())
        {
            parsePHP(bufffff);
        }
        
        lRequest->release();
    });
    
    HttpClient::getInstance()->send(lRequest);
}

void DownloadManager::downloadCsv(const string& fileName)
{
    if (fileName == st_csv_file_chapter) {
        _downloadBlocked = true;
    }
    
    HttpRequest* lRequest = new HttpRequest;
    
    string lPath = st_download_dir;
    lPath+="/";
    lPath+=fileName;
    
    lRequest->setUrl(lPath.c_str());
    lRequest->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    lRequest->setResponseCallback([this,lRequest](network::HttpClient* client, network::HttpResponse* response) {
        
        if (!response)
        {
            CCLOG("response failed");
            lRequest->release();
            this->excuteFunc(DOWNLOAD_RESOURCE_ERROR_STATE);
            return;
        }
        
        if (!response->isSucceed())
        {
            CCLOG("error=%s",response->getErrorBuffer());
            CCLOG("response failed   获取网络资源失败 %s", lRequest->getTag());
            lRequest->release();
            this->excuteFunc(DOWNLOAD_RESOURCE_ERROR_STATE);
            return;
        }
        
        std::vector<char> *buffer = response->getResponseData();
        std::string bufffff(buffer->begin(),buffer->end());
        
        std::string lPath=m_sSavePath+"/" + "_" + lRequest->getTag();
        FILE *fp = fopen(lPath.c_str(), "wb+");
        fwrite(bufffff.c_str(),1,buffer->size(), fp);
        fclose(fp);
        
        ++m_iDownloadCsvCount;
        
        log("%s", lRequest->getTag());
        if (strcmp(lRequest->getTag(), st_csv_file_name) == 0)
        {
            m_sCsvDataBuffer=bufffff;
            m_iCsvDataSize = buffer->size();

            if (!_downloadBlocked) {
                readGraphData();
            }
            else {
                _delayDownload = true;
            }
        }
        
        if (strcmp(lRequest->getTag(), st_csv_file_chapter) == 0)
        {
            _downloadBlocked = false;
            xChapter->readChapterCsvInfoDownload();
            
            if (_delayDownload) {
                _delayDownload = false;
                readGraphData();
            }
        }
        
        lRequest->release();
        
        //判断如果没有graph.csv, 就不需要下载图片, 下载完成csv后可以立刻结束
        if (!_hasGraph && m_iDownloadCsvCount >= _updates.size()) {
            excuteFunc(DOWNLOAD_FINISH_STATE);
        }
    });
    lRequest->setTag(fileName.c_str());
    HttpClient::getInstance()->send(lRequest);
}

void DownloadManager::readGraphData()
{
    m_vGraphInfo.clear();
    
    std::string lPath=m_sSavePath+"/" + "_" + st_csv_file_name;
    
    CSVParse* lCsv=CSVParse::create(lPath.c_str());
    
    if(lCsv==NULL)
    {
        return;
    }
    
    const unsigned int row=lCsv->getRows();
    for (int r=1; r<row; r++)
    {
        Graph lInfo;
        lInfo.name = lCsv->getDatas(r, "name");
        lInfo.chapter_id = stoi(lCsv->getDatas(r, "chapter_id"));
        
        bool isOldImage = false;
        if ((string)lCsv->getDatas(r, "reDownload") == "1")
        {
            lInfo.reDownload = true;
        }
        else
        {
            for (int i = 0; i < GphM_getInstance->getData().size(); ++i)
            {
                if (GphM_getInstance->getData().at(i).name == lInfo.name)
                {
                    isOldImage = true;
                }
            }
        }
        
        if (!isOldImage)
        {
            Chapter & chapter = xChapter->getChapterData(lInfo.chapter_id);
            xUser->setBoolForKey(chapter.name.c_str(), true);
            xChapter->addTotal(lInfo.chapter_id);
            m_vGraphInfo.push_back(lInfo);
        }
        
    }
    
    excuteDownload();
}

void DownloadManager::excuteDownload()
{
//    m_iDownloadCount = 0;
//    
//    for (int i = 0 ; i < xChapter->getData().size(); ++i)
//    {
//        auto & xx = xChapter->getData().at(i);
//        
////        if (xChapter->getData().at(i).isNewChapter || xChapter->getData().at(i).isReDownloadCover)
//        {
//            string lPath = m_sSavePath + "/" + xChapter->getData().at(i).image;
//            if (!FileUtils::getInstance()->isFileExist(lPath) || xChapter->getData().at(i).isReDownloadCover)
//            {
//                log("%s",xChapter->getData().at(i).image.c_str());
//                xChapter->addCoverTotal(xChapter->getData().at(i).id);
//                m_iDownloadCount++;
//                this->startDownload(xChapter->getData().at(i).image, 0);
//            }
//        }
//        
//    }
//    
//    for (int i=0; i<m_vGraphInfo.size(); i++) {
//        Graph* lInfo=&m_vGraphInfo.at(i);
//        string lPath=m_sSavePath+"/" + lInfo->name + ".png";
//        if(!FileUtils::getInstance()->isFileExist(lPath) || lInfo->reDownload)
//        {
//            m_iDownloadCount++;
//            this->startDownload(lInfo->name + ".png", lInfo->chapter_id);
//        }
//        
//        string lPathW=m_sSavePath+"/" + lInfo->name + "_mask.png";
//        if(!FileUtils::getInstance()->isFileExist(lPathW) || lInfo->reDownload)
//        {
//            m_iDownloadCount++;
//            this->startDownload(lInfo->name + "_mask.png", lInfo->chapter_id);
//        }
//    }
//    
//    if(m_iDownloadCount==0)
//    {
//        this->excuteFunc(DOWNLOAD_FINISH_STATE);
//    }
}

int _chapterID = -1;

void DownloadManager::startDownload(const string& fileName, int chapter_id)
{
    string lPath=st_download_dir;
    lPath+="/";
    lPath+=fileName;
    
    HttpRequest* lRequest=new HttpRequest;
    lRequest->setUrl(lPath.c_str());
    lRequest->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    lRequest->setResponseCallback([this,lRequest](network::HttpClient* client, network::HttpResponse* response) {
        if (!response)
        {
            CCLOG("response failed");
            lRequest->release();
            this->excuteFunc(DOWNLOAD_RESOURCE_ERROR_STATE);
            return;
        }
        
        if (!response->isSucceed())
        {
            CCLOG("error=%s",response->getErrorBuffer());
            CCLOG("response failed   获取网络资源失败 %s", lRequest->getTag());
            if (_retryCount < 3) {
                _retryCount++;
                startDownload(lRequest->getTag(), (int)(intptr_t)lRequest->getUserData());
                ++m_iDownloadCount;
            }
            else {
                excuteFunc(DOWNLOAD_RESOURCE_ERROR_STATE);
                lRequest->release();
            }
                
            return;
        }
        
        std::vector<char> *buffer = response->getResponseData();
        std::string bufffff(buffer->begin(),buffer->end());
        
        std::string lPath=m_sSavePath+"/"+lRequest->getTag();
        FILE *fp = fopen(lPath.c_str(), "wb+");
        fwrite(bufffff.c_str(),1,buffer->size(), fp);
        fclose(fp);
        
        lRequest->release();
        
        --m_iDownloadCount;
        
    
        CCLOG("m_iDownloadCount=%d",m_iDownloadCount);
        if(m_iDownloadCount<=0)
        {
            this->excuteFunc(DOWNLOAD_FINISH_STATE);
        }else
        {
            if ((int)(intptr_t)(lRequest->getUserData()) >= 0) {
                xChapter->updateProgress((int)(intptr_t)(lRequest->getUserData()));
            }
        }
    });
    
    lRequest->setUserData((void*)chapter_id);
    lRequest->setTag(fileName.c_str());
    HttpClient::getInstance()->send(lRequest);
}

void DownloadManager::saveCsvFile()
{
    for (auto update : _updates) {
        auto oldName = FileUtils::getInstance()->getWritablePath() + "color/_" + update.filename + ".csv";
        auto newName = FileUtils::getInstance()->getWritablePath() + "color/" + update.filename + ".csv";
        rename(oldName.c_str(), newName.c_str());
    }
}

void DownloadManager::refreshData()
{
    xGraph->readGraphCsvInfo();
    xChapter->readChapterCsvInfo();
    xPallette->readPalletteCsvInfo();
}

void DownloadManager::setCurrentDownState(DownloadState pState)
{
    m_dCurrentDownState = pState;
}

DownloadState DownloadManager::getCurrentDownState()
{
    return m_dCurrentDownState;
}

void DownloadManager::setColorSystemVersion()
{
    for (auto update : _updates) {
        UserDefault::getInstance()->setFloatForKey(string(update.filename + "_version").c_str(), update.version);
    }
    UserDefault::getInstance()->flush();
}

float DownloadManager::getColorSystemVersion(const string & filename)
{
    return UserDefault::getInstance()->getFloatForKey(string(filename + "_version").c_str(), 1.0f);
}

bool DownloadManager::createDirectory(const char *path)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    mode_t processMask = umask(0);
    int ret = mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
    umask(processMask);
    if (ret != 0 && (errno != EEXIST))
    {
        return false;
    }
    
    return true;
#else
    BOOL ret = CreateDirectoryA(path, nullptr);
    if (!ret && ERROR_ALREADY_EXISTS != GetLastError())
    {
        return false;
    }
    return true;
#endif
    
}

void DownloadManager::parsePHP(string version)
{
    log("version : %s", version.c_str());
    
    _updates.clear();
    
    char *ver_cstr = new char[version.length() + 1];
    strcpy(ver_cstr, version.c_str());
    if (ver_cstr[0] == '\xef' && ver_cstr[1] == '\xbb' && ver_cstr[2] == '\xbf') {
        ver_cstr += 3;
    }
    version = string(ver_cstr);
    
    vector<string> elements;
    ssize_t start = 0;
    ssize_t end = 0;
    while (start < version.length()) {
        int token = 2;
        end = version.find("\r\n", start);
        if (end == string::npos) {
            end = version.find("\r", start);
            token = 1;
            if (end == string::npos)
                end = version.length();
        }
        
        elements.push_back(version.substr(start, end - start));
        start = end + token;
    }
    
    _hasGraph = false;
    for(auto element : elements)
    {
        try{
            auto space = element.find(" ");
            UpdateInfo update;
            update.filename = element.substr(0, space);
            
            if (update.filename == "pallette")
            {
                update.filename = "pallette4";
            }
            
            if (update.filename == "chapter")
            {
                update.filename = "chapter4";
            }
            
            update.version = std::stof(element.substr(space + 1));
            _updates.push_back(update);
        }
        catch(...)
        {
            return;
        }
    }
    
    bool startDownload = false;
    for(auto it = _updates.begin(); it != _updates.end(); )
    {
        auto oldversion = getColorSystemVersion((*it).filename);
        if (oldversion < (*it).version)
        {
            if ((*it).filename == "graph") {
                _hasGraph = true;
            }
            
            it++;
            startDownload = true;

        }
        else
        {
            it=_updates.erase(it);
        }
    }
    
    if (startDownload) {
        
        if (_isAutoCheck)
        {
            excuteFunc(FIND_NEW_VERSION_STATE);
        }else
        {
            excuteFunc(FIND_NEW_VERSION_STATE2);
        }
        
    }
    else
    {
        excuteFunc(NO_NEW_VERSION_STATE);
    }
}

void DownloadManager::onDialog(const string & name)
{
    if (name == "Download") {
        excuteFunc(START_DOWNLOAD_STATE);
    }
}

