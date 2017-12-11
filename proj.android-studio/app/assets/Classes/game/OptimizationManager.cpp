//
//  OptimizationManager.cpp
//  ColorGarden
//
//  Created by houhuang on 17/3/6.
//
//

#include "OptimizationManager.hpp"
#include "UITools.h"
#include "DownLoadPage.h"

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32) && (CC_TARGET_PLATFORM != CC_PLATFORM_WP8) && (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#endif

#define ClearInspirationTime    "ClearInspirationTime"

static OptimizationManager* _instance = nullptr;

OptimizationManager* OptimizationManager::getInstance()
{
    if (!_instance)
    {
        _instance = new OptimizationManager();
    }
    
    return _instance;
}

OptimizationManager::OptimizationManager()
{
    
}

void OptimizationManager::clearInspirationResoure_deleteFolder()
{

    string preClearTime = UserDefault::getInstance()->getStringForKey(ClearInspirationTime, "0000-00-00 00-00-00");

    if (isAlreadyOneWeek(preClearTime))
    {
        std::string path = FileUtils::getInstance()->getWritablePath() + "Inspiration/";
        FileUtils::getInstance()->removeDirectory(path);
        
        UserDefault::getInstance()->setStringForKey(ClearInspirationTime, getCurrentTime());
        UserDefault::getInstance()->flush();
    }
    
}

void OptimizationManager::clearInspirationResoure_deleteFile()
{
    std::thread t1(&OptimizationManager::deleteInspirationFile, this);
//    t1.join();     //让主线程等待
    t1.detach();
    
}
void OptimizationManager::deleteInspirationFile()
{
    string path = FileUtils::getInstance()->getWritablePath() + "Inspiration/";
    
    if (!FileUtils::getInstance()->isFileExist(path)) return;
    
    vector<string> vFileData = this->getFilePathAtVec(path);
    //    log("+++++++++++++ %lu",vFileData.size());
    
    int m = 0, n = 0;
    for (int i = 0; i < vFileData.size(); ++i)
    {
        //每次最多循环2000次或删除100张图
        //        if (m > 5000 || n > 400) return;
        
        bool isDelete = true;
        for (int j = 0; j < DownLoadPage::getInstance()->m_vAlreadyClickLick.size(); ++j)
        {
            ++m;
            
            string fileName = vFileData.at(i);
            if ((int)fileName.find(".png") < 0)
            {
                isDelete = false;
                break;
            }
            
            stringstream ss;
            ss<<DownLoadPage::getInstance()->m_vAlreadyClickLick.at(j);
            ss<<".png";
            
            if (fileName == ss.str())
            {
                isDelete = false;
                break;
            };
        }
        
        if (isDelete)
        {
            ++n;
            string filePath = path + vFileData.at(i);
            std::remove(filePath.c_str());
        }
    }
}

std::vector<std::string> OptimizationManager::getFilePathAtVec(std::string filePath)
{
    std::vector<std::string> path_vec;
    
    
    const char* path = filePath.c_str();

    char *dir = (char*)malloc(filePath.size() + 1);

    sprintf(dir, "%s", path);
    
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    int i=0;
    
    if((dp=opendir(dir))==NULL)
    {
        fprintf(stderr,"cannot open %s",dir);
        exit(1);
    }
    chdir(dir);
    
    while((entry=readdir(dp))!=NULL&&i<255)
    {
        stat(entry->d_name,&statbuf);
        if(!S_ISREG(statbuf.st_mode))
            continue;
        path_vec.push_back(StringUtils::format("%s",entry->d_name));
    }
    return path_vec;
}

bool OptimizationManager::isAlreadyOneWeek(const string& preTime)
{
    struct tm now;
    struct tm pre_now;

    std::string now_time = getCurrentTime();
    now.tm_year = stod(now_time.substr(0, 4));
    now.tm_mon  = stod(now_time.substr(5, 2));
    now.tm_mday = stod(now_time.substr(8, 2));
    
    pre_now.tm_year = stod(preTime.substr(0, 4));
    pre_now.tm_mon  = stod(preTime.substr(5, 2));
    pre_now.tm_mday = stod(preTime.substr(8, 2));

    
    if (now.tm_year - pre_now.tm_year == 0)
    {
        if (now.tm_mon - pre_now.tm_mon == 0)
        {
            if (now.tm_mday - pre_now.tm_mday >= 7)
            {
                return true;
            }
        }else if (now.tm_mon - pre_now.tm_mon == 1)
        {
            if ((now.tm_mday + 30 - pre_now.tm_mday) >= 7)
            {
                return true;
            }
        }else
        {
            return true;
        }
    }else
    {
        if (now.tm_mon == 1)
        {
            if ((now.tm_mday + 30 - pre_now.tm_mday) >= 7)
            {
                return true;
            }
        }else
        {
            return true;
        }
    }
    
    return false;
}


