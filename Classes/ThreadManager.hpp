//
//  ThreadManager.hpp
//  ALABS0010
//
//  Created by houhuang on 16/3/29.
//
//

#ifndef ThreadManager_hpp
#define ThreadManager_hpp

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;
using namespace std;

#define st_multiThread_loadImage_finish "st_MultiThread_loadImage_finish"
#define xThread ThreadManager::getInstance()

class ThreadManager
{
public:
    ThreadManager();
    ~ThreadManager();
    static ThreadManager* getInstance();
public:
    void startLoadImage();
    void dynamicLoadImage(const string& imageName);
    void imageLoadedCallback(Ref* ref);
    
    void addNeedloadImage(const string& name);
    void removeNeedloadImage(const string& name);
    void removeAllNeedloadImage();
public:
    deque<string> m_vNeedLoadImage;
    string  m_sCurrentLoadImage;
    bool    m_bIsload;
};

#endif /* ThreadManager_hpp */
