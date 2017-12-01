//
//  ProgressManager.cpp
//  ColorBook
//
//  Created by luoxiaopeng on 4/24/15.
//
//

#include "ProgressManager.h"

static ProgressManager *_sharedProgress = nullptr;

ProgressManager* ProgressManager::getInstance()
{
    if (! _sharedProgress)
    {
        _sharedProgress = new ProgressManager();
    }
    
    return _sharedProgress;
}

void ProgressManager::destroyInstance()
{
    xScheduler->unscheduleAllForTarget(this);
    CC_SAFE_DELETE(_sharedProgress);
}

ProgressManager::ProgressManager()
{
    xScheduler->scheduleUpdate(this, 0, false);
}

void ProgressManager::start(const ccProgressCallback &callback)
{
    _start = true;
    _index = 0;
    _progressAcc = 0;
    _progressInterval = 0;
    _skipAcc = false;
    _callback = callback;
}

void ProgressManager::stop()
{
    _start = false;
}


void ProgressManager::update(float dt)
{
    if (_start) {
        if (!_skipAcc) {
            _progressAcc += dt;
        }
        else
        {
            _skipAcc = false;
        }
        
        if (_progressAcc > _progressInterval) {
            _progressAcc = 0;
            if (_callback)
                _progressInterval = _callback(_index);
            _index++;
            //_skipAcc = true;
        }
    }
}

void ProgressManager::jump(int index)
{
    _index = index - 1;
}