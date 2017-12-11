//
//  ProgressManager.h
//  ColorBook
//
//  Created by luoxiaopeng on 4/24/15.
//
//

#pragma once

#include "UIHeader.h"

#define xProgress       (ProgressManager::getInstance())

typedef std::function<float(int index)>           ccProgressCallback;

class ProgressManager : public Node
{
public:
    static ProgressManager* getInstance();
    void destroyInstance();
    
    void start(const ccProgressCallback &callback);
    void stop();
    void jump(int index);
    
public:
    void update(float dt);

private:
    ProgressManager();
    
private:
    bool                    _start = false;
    int                     _index = 0;
    float                   _progressAcc;
    ccProgressCallback      _callback = nullptr;
    float                   _progressInterval;
    bool                    _skipAcc = false;
};
