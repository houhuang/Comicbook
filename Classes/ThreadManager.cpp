//
//  ThreadManager.cpp
//  ALABS0010
//
//  Created by houhuang on 16/3/29.
//
//

#include "ThreadManager.h"

static ThreadManager* _instance = NULL;

ThreadManager::ThreadManager()
{
    m_bIsload = false;
    m_sCurrentLoadImage = "";
}

ThreadManager::~ThreadManager()
{
    
}

ThreadManager* ThreadManager::getInstance()
{
    if (!_instance)
    {
        _instance = new ThreadManager();
    }
    return _instance;
}

void ThreadManager::startLoadImage()
{
    if (!m_vNeedLoadImage.empty())
    {
        string str = m_vNeedLoadImage.at(0);
        this->dynamicLoadImage(str);
    }
}

void ThreadManager::dynamicLoadImage(const string& imageName)
{
    if (!m_bIsload)
    {
        m_bIsload = true;
        m_sCurrentLoadImage = imageName;
        Director::getInstance()->getTextureCache()->addImageAsync(imageName, CC_CALLBACK_1(ThreadManager::imageLoadedCallback, this));
    }
}

void ThreadManager::imageLoadedCallback(Ref* ref)
{
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(st_multiThread_loadImage_finish);
    
    m_bIsload = false;
    
    if (m_vNeedLoadImage.empty())
    {
        return;
    }
    
    for ( deque<string>::iterator iter = m_vNeedLoadImage.begin(); iter!= m_vNeedLoadImage.end(); iter++) {
        if (*iter == m_sCurrentLoadImage) {
            iter = m_vNeedLoadImage.erase(iter);
            break;
        }
    }
    
    
    if (!m_vNeedLoadImage.empty()) {
        startLoadImage();
    }
}

void ThreadManager::addNeedloadImage(const string& name)
{
    if (name.empty()) return;
    
    
    for (int i = 0 ; i < m_vNeedLoadImage.size(); ++i)
    {
        if (m_vNeedLoadImage.at(i) == name)
        {
            return;
        }
    }
    
    m_vNeedLoadImage.push_back(name);
    this->startLoadImage();

}

void ThreadManager::removeAllNeedloadImage()
{
    m_vNeedLoadImage.clear();
}

void ThreadManager::removeNeedloadImage(const string& name)
{
    if (name.empty()) return;
    
    
    for (int i = 0; i < m_vNeedLoadImage.size(); ++i)
    {
        if (m_vNeedLoadImage.at(i) == name)
        {
            m_vNeedLoadImage.erase(m_vNeedLoadImage.begin() + i);
            break;
        }
    }
}
