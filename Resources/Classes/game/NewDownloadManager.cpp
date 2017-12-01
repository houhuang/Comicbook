//
//  NewDownloadManager.cpp
//  ColorGarden
//
//  Created by houhuang on 17/2/16.
//
//

#include "NewDownloadManager.hpp"
#include "SearchPathManager.hpp"
#include "STSystemFunction.h"

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32) && (CC_TARGET_PLATFORM != CC_PLATFORM_WP8) && (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#include <vector>
#endif

#define jsonFileUrl "http://liveupdate.toyboxapps.com/coloring/ALABS0006/color/pub_gte_2.2.json"

static NewDownloadManager* _instance = nullptr;

bool NewDownloadManager::m_bIsdownloadFail = false;

NewDownloadManager* NewDownloadManager::getInstance()
{
    if (!_instance)
    {
        _instance = new NewDownloadManager();
    }
    
    return _instance;
}

NewDownloadManager::NewDownloadManager()
{
    m_bIsDownloading = false;
    
    std::string colorPath = FileUtils::getInstance()->getWritablePath() + "color";
    this->createDirectory(colorPath.c_str());
    SearchPathManager::getInstance()->updateSearchPath();
}

NewDownloadManager::~NewDownloadManager()
{

}

void NewDownloadManager::addNeedDownloadImage(DownloadSpriteInfo& info)
{
    if (info.name.empty()) return;
    
    for (int i = 0; i < m_vNeedDownloadSpr.size(); ++i)
    {
        if ((m_vNeedDownloadSpr.at(i).name == info.name) && (m_vNeedDownloadSpr.at(i).imageType == info.imageType))
        {
            return;
        }
    }
    
    m_vNeedDownloadSpr.push_back(info);
    this->startDownloadImage();
    
}

void NewDownloadManager::removeAllNeedDownloadImage()
{
    m_vNeedDownloadSpr.clear();
}

void NewDownloadManager::removeNeedDownloadImage(const std::string& name, ImageType imageType)
{
    if (name.empty()) return;
    
    for (std::vector<DownloadSpriteInfo>::iterator iter = m_vNeedDownloadSpr.begin(); iter != m_vNeedDownloadSpr.end(); ++iter)
    {
        if ((*iter).name == name && (*iter).imageType == imageType)
        {
            m_vNeedDownloadSpr.erase(iter);
            break;
        }
    }
    
}

void NewDownloadManager::startDownloadImage(const int& index)
{
    if (m_vNeedDownloadSpr.size() > 0)
    {
        DownloadSpriteInfo info = m_vNeedDownloadSpr.at(index);
        this->downloadImage(info.url, info.name, info.imageType);
    }
}

void NewDownloadManager::downloadImage(std::string url, std::string name, ImageType imageType)
{
    if (m_bIsDownloading)
    {
        return;
    }
    
    m_bIsDownloading = true;
    
    std::string saveImageName, realUrl;
    switch (imageType)
    {
        case ImageType::PNG_THUMB_512:
        {
//            saveImageName = name + "_thumb.png";
//            realUrl = url + "/thumb.png";
            
            saveImageName = name + "_thumb.png";
            realUrl = url + "/" + saveImageName;
        }
            break;
            
        case ImageType::PNG_NOMAL_1024:
        {
//            saveImageName = name + ".png";
//            realUrl = url + "/1024.png";
            saveImageName = name + ".png";
            realUrl = url + "/" + saveImageName;
        }
            break;
            
        case ImageType::PNG_MASK_1024:
        {
//            saveImageName = name + "_mask.png";
//            realUrl = url + "/1024_mask.png";
            
            saveImageName = name + "_mask.png";
            realUrl = url + "/" + saveImageName;
        }
            break;
            
        case ImageType::PNG_COVER_512:
        {
            saveImageName = name;
            realUrl = url;
        }
            break;
            
        default:
            break;
    }
    
    HttpRequest* lRequest = new HttpRequest();
    lRequest->setUrl(realUrl.c_str());
    lRequest->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    lRequest->setResponseCallback([this, name, lRequest](network::HttpClient* client, network::HttpResponse* response){

        if (!response || !response->isSucceed())
        {
            log("response failed!   获取网络资源失败！");
            lRequest->release();
            
            this->startDownloadImage();
            m_bIsDownloading = false;
            
            return;
        }
        
        std::vector<char>* buffer = response->getResponseData();
        std::string buffff(buffer->begin(), buffer->end());
        
        std::string realName = lRequest->getTag();
        long int index = buffff.find("404 Not Found");
        if (index < 0)
        {
            std::string path = FileUtils::getInstance()->getWritablePath() + "color/" + std::string(lRequest->getTag());
            long int coverIndex = realName.find("cover");
            if (coverIndex < 0)
            {
                FILE* fp = fopen(path.c_str(), "wb+");
                fwrite(buffff.c_str(), 1, buffer->size(), fp);
                fclose(fp);
            }else
            {
                std::vector<char> *buffer = response->getResponseData();
                std::string bufffff(buffer->begin(),buffer->end());
                
                
                unsigned char* buffer2 = (unsigned char*)malloc(buffer->size()*sizeof(unsigned char));
                unsigned char*ptr=buffer2;
                
                for ( std::vector<char>::iterator iter=buffer->begin(); iter!=buffer->end(); iter++)
                {
                    *buffer2 = (unsigned char)*iter;
                    buffer2++;
                }
                
                Image* lImage=new Image();
                lImage->initWithImageData(ptr, buffer->size());
                
                Texture2D* text = new Texture2D();
                text->initWithImage(lImage);
                
                Sprite* lSprite = Sprite::createWithTexture(text);
                lSprite->setScale(0.5);
                lSprite->setPosition(lSprite->getContentSize()/4);
                
                RenderTexture* lRender = RenderTexture::create(512, 512);
                lRender->begin();
                lSprite->visit();
                lRender->end();
                lRender->getSprite()->getTexture()->setAntiAliasTexParameters();
                Director::getInstance()->getRenderer()->render();
                
                Image* image = lRender->newImage();
                image->saveToFile(path);
                
                lImage->release();
                Director::getInstance()->getTextureCache()->removeTextureForKey(path);
                
                free(ptr);
                ptr = NULL;
                text->release();
                image->release();
            }
        }
        

        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("imageDownloaded",(void*)&realName);

        m_bIsDownloading = false;
        
//        if (m_vNeedDownloadSpr.size() > 0)
//        {
//            m_vNeedDownloadSpr.erase(m_vNeedDownloadSpr.begin());
//        }
        
        for (vector<DownloadSpriteInfo>::iterator iter = m_vNeedDownloadSpr.begin(); iter != m_vNeedDownloadSpr.end(); ++iter)
        {
            if ((*iter).name == name)
            {
                m_vNeedDownloadSpr.erase(iter);
                break;
            }
        }

        this->startDownloadImage();
        lRequest->release();
        
    });
    
    lRequest->setTag(saveImageName.c_str());
    HttpClient::getInstance()->send(lRequest);
}

void NewDownloadManager::downloadJsonFile()
{
    HttpRequest* lRequest=new HttpRequest;
    lRequest->setUrl(jsonFileUrl);
    lRequest->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    lRequest->setResponseCallback([this, lRequest](network::HttpClient* client, network::HttpResponse* response){
        
        if (!response || !response->isSucceed())
        {
            log("reponse failed!    获取网络资源失败！");
        }else
        {
            std::vector<char> *buffer = response->getResponseData();
            std::string buffff(buffer->begin(), buffer->end());
//            log("json file data is %s", buffff.c_str());
            
            long int index = buffff.find("404 Not Found");
            if (index < 0)
            {
                //check forced update
                STSystemFunction sf;
                std::string localBundleId = sf.getPlatformBundleID();
                std::string localVersion = sf.getPlatformVersion();
                
                rapidjson::Document _doc;
                _doc.Parse<0>(buffff.c_str());

                if (_doc.HasMember(localBundleId.c_str()))
                {
                    rapidjson::Value& updateObj = _doc[localBundleId.c_str()];
                    std::string serveVerison = updateObj["version"].GetString();
                    if (localVersion < serveVerison)
                    {
                        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("forcedUpdate");
                    }else
                    {
                        std::string lPath = FileUtils::getInstance()->getWritablePath() + "color/pub_gte_2.2.json";
                        FILE* fp = fopen(lPath.c_str(), "wb+");
                        fwrite(buffff.c_str(), 1, buffer->size(), fp);
                        fclose(fp);
                    }
                }else
                {
                    std::string lPath = FileUtils::getInstance()->getWritablePath() + "color/pub_gte_2.2.json";
                    FILE* fp = fopen(lPath.c_str(), "wb+");
                    fwrite(buffff.c_str(), 1, buffer->size(), fp);
                    fclose(fp);
                }

            }
        }
        
        lRequest->release();
    });
    
    HttpClient::getInstance()->send(lRequest);
}

void NewDownloadManager::getServiceData(const std::string& url)
{
    HttpRequest* lRequest=new HttpRequest;
    lRequest->setUrl(url.c_str());
    lRequest->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    lRequest->setResponseCallback([this, lRequest](network::HttpClient* client, network::HttpResponse* response){
        
        if (!response || !response->isSucceed())
        {
            log("reponse failed!    获取网络资源失败！");
            lRequest->release();
            return;
        }else
        {
            std::vector<char> *buffer = response->getResponseData();
            std::string buffff(buffer->begin(), buffer->end());
            log("json file data is %s", buffff.c_str());
            
            rapidjson::Document _doc;
            _doc.Parse<0>(buffff.c_str());
            
            if (_doc.HasParseError())
            {
                log("Parse Error: %d\n", _doc.GetParseError());
                lRequest->release();
                return;
            }
            
            if (_doc.IsNull())
            {
                log("json file is NULL......");
                lRequest->release();
                return;
            }
            
            rapidjson::Value& lStatus = _doc["status"];

            if (lStatus.GetBool())
            {
//                std::string url = _doc["url"].GetString();
                std::string url = "";
                Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("forcedUpdate",(void*)&url);
            }
        }
        
        lRequest->release();
    });
    
    HttpClient::getInstance()->send(lRequest);
}

bool NewDownloadManager::createDirectory(const char *path)
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