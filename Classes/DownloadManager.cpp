//
//  DownloadManager.cpp
//  ComicBook
//
//  Created by houhuang on 17/12/1.
//
//

#include "DownloadManager.h"

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32) && (CC_TARGET_PLATFORM != CC_PLATFORM_WP8) && (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#include <vector>
#endif

#define st_cartooncsv_url   "https://firebasestorage.googleapis.com/v0/b/cartoonbook-f83f9.appspot.com/o/cartoon.csv?alt=media&token=b8209a81-c652-429d-91fc-37003f38d251"

static DownloadManager* _instance = nullptr;

DownloadManager* DownloadManager::getInstance()
{
    if (!_instance)
    {
        _instance = new DownloadManager();
    }
    
    return _instance;
}

DownloadManager::DownloadManager()
{
    _isDownloadingCover = false;
}


//download cover
void DownloadManager::addNeedDownloadCover(DownloadCoverInfo info)
{
    for (int i = 0 ; i < _downloadCoverInfo.size(); ++i)
    {
        if (_downloadCoverInfo.at(i).url == info.url)
        {
            return;
        }
    }
    
    _downloadCoverInfo.push_back(info);
    this->startDownloadCover();
}

void DownloadManager::startDownloadCover()
{
    if (_downloadCoverInfo.size() > 0)
    {
        DownloadCoverInfo imageInfo = _downloadCoverInfo.at(0);
        this->downloadCover(imageInfo.url, imageInfo.folder);
    }
}

void DownloadManager::downloadCover(string url, string folder)
{
    if (_isDownloadingCover) return;
    
    _isDownloadingCover = true;
 
    HttpRequest* lRequest = new HttpRequest();
    lRequest->setUrl(url.c_str());
    lRequest->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    lRequest->setResponseCallback([this, lRequest](network::HttpClient* client, network::HttpResponse* response){
        
        if (!response || !response->isSucceed())
        {
            log("response failed!   获取网络资源失败！");
            _isDownloadingCover = false;
        }
        
        std::vector<char>* buffer = response->getResponseData();
        std::string buffff(buffer->begin(), buffer->end());
        
        string path = FileUtils::getInstance()->getWritablePath() + "cover/";
        if (!FileUtils::getInstance()->isFileExist(path))
        {
            this->createDirectory(path.c_str());
        }
        
        string name = path + lRequest->getTag() + "_cover.png";
        
        FILE* fp = fopen(name.c_str(), "wb+");
        fwrite(buffff.c_str(), 1, buffer->size(), fp);
        fclose(fp);
        
        _isDownloadingCover = false;
        
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(st_download_cover_finish);
        
        _downloadCoverInfo.erase(_downloadCoverInfo.begin());
        if (_downloadCoverInfo.size() > 0)
        {
            this->startDownloadCover();
        }

        lRequest->release();
        
    });
    
    lRequest->setTag(folder.c_str());
    HttpClient::getInstance()->send(lRequest);
}

void DownloadManager::downloadCarttonCsv()
{
    HttpRequest* lRequest = new HttpRequest();
    lRequest->setUrl(st_cartooncsv_url);
    lRequest->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    lRequest->setResponseCallback([this, lRequest](network::HttpClient* client, network::HttpResponse* response){
        
        if (!response || !response->isSucceed())
        {
            log("response failed!   获取网络资源失败！");
            Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(st_download_rootCsv_faild);
            
            lRequest->release();
            return ;
        }
        
        std::vector<char>* buffer = response->getResponseData();
        std::string buffff(buffer->begin(), buffer->end());
        
        long int index = (long int)buffff.find("NoSuchKey");
        if (index > 0)
        {
            lRequest->release();
            return ;
        }
        
        string path = FileUtils::getInstance()->getWritablePath() + "cartoon.csv";
        
        FILE* fp = fopen(path.c_str(), "wb+");
        fwrite(buffff.c_str(), 1, buffer->size(), fp);
        fclose(fp);
        
        lRequest->release();
        
    });

    HttpClient::getInstance()->send(lRequest);
}

void DownloadManager::downloadPictureCsv(string folder)
{

    string url;
    url.append("https://firebasestorage.googleapis.com/v0/b/cartoonbook-f83f9.appspot.com/o/piccsv%2F").append(folder).append("_picture.csv?alt=media");
    
    HttpRequest* lRequest = new HttpRequest();
    lRequest->setUrl(url.c_str());
    lRequest->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    lRequest->setResponseCallback([this, lRequest](network::HttpClient* client, network::HttpResponse* response){
        
        if (!response || !response->isSucceed())
        {
            log("response failed!   获取网络资源失败！");
        }
        
        std::vector<char>* buffer = response->getResponseData();
        std::string buffff(buffer->begin(), buffer->end());
        
        string path = FileUtils::getInstance()->getWritablePath() + "piccsv/";
        if (!FileUtils::getInstance()->isFileExist(path))
        {
            this->createDirectory(path.c_str());
        }
        
        string name = path + lRequest->getTag() + "_picture.csv";
        
        FILE* fp = fopen(name.c_str(), "wb+");
        fwrite(buffff.c_str(), 1, buffer->size(), fp);
        fclose(fp);
        
        
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(st_download_picture_suc);
        
        
        lRequest->release();
        
    });
    
    lRequest->setTag(folder.c_str());
    HttpClient::getInstance()->send(lRequest);
}

void DownloadManager::downloadPicture(Picture picture)
{
    HttpRequest* lRequest = new HttpRequest();
    lRequest->setUrl(picture.url.c_str());
    lRequest->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    lRequest->setResponseCallback([this, lRequest](network::HttpClient* client, network::HttpResponse* response){
        
        if (!response || !response->isSucceed())
        {
            log("response failed!   获取网络资源失败！");
        }
        
        std::vector<char>* buffer = response->getResponseData();
        std::string buffff(buffer->begin(), buffer->end());
        
        long int index = (long int)buffff.find("NoSuchKey");
        if (index > 0)
        {
            lRequest->release();
            return ;
        }
        
        string rr = lRequest->getTag();
        rr = rr.substr(0, rr.find("/") + 1);
        string path = FileUtils::getInstance()->getWritablePath() + "picture/";
        if (!FileUtils::getInstance()->isFileExist(path))
        {
            this->createDirectory(path.c_str());
        }
        
        if (!FileUtils::getInstance()->isFileExist(path + rr))
        {
            this->createDirectory((path + rr).c_str());
        }
        
        string name = path + lRequest->getTag();
        
        FILE* fp = fopen(name.c_str(), "wb+");
        fwrite(buffff.c_str(), 1, buffer->size(), fp);
        fclose(fp);
        
        
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(st_download_picture_suc);
        
        
        lRequest->release();
        
    });
    
    string name = xCartoon->getCurrentFolder() + "/" + picture.chaID + "_" + picture.index + ".jpg";
    lRequest->setTag(name.c_str());
    HttpClient::getInstance()->send(lRequest);
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

string DownloadManager::UrlGB2312(const char * str)
{
    string dd;
    size_t len = strlen(str);
    for (size_t i=0;i<len;i++)
    {
        if(isalnum((unsigned char)str[i]))
        {
            char tempbuff[2];
            sprintf(tempbuff,"%c",str[i]);
            dd.append(tempbuff);
        }
        else if (isspace((unsigned char)str[i]))
        {
            dd.append("+");
        }
        else
        {
            char tempbuff[4];
            sprintf(tempbuff,"%%%X%X",((unsigned char*)str)[i] >>4,((unsigned char*)str)[i] %16);
            dd.append(tempbuff);
        }
        
    }
    return dd;
}












