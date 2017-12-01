//
//  LockManager.h
//  ColorBook
//
//  Created by luoxp on 4/24/15.
//
//

#ifndef __ColorBook__LockManager__
#define __ColorBook__LockManager__

#include "UITools.h"

#define xLock   (LockManager::getInstance())

class RewardAdLoadingDelegate;

class LockManager : public Node
{
public:
    enum class Type{
        GRAPH,
        PALLETTE
    };
    
    static LockManager* getInstance();
    void destroyInstance();
    
    void unlock(const string & identifier, Type type);
    bool isLocked(const string & identifier, Type type);
    void showBanner(float dt);

public:
    void update(float dt);
    ~LockManager();
    
private:
    LockManager();
    
    void rewardAdSuccessful(EventCustom* event);
    void setUnLockTime();
    string getUnLockTime(const string & identifier, Type type);
    bool offsetTime(string time);
    bool offsetTime_pallette(string time);
    
private:
    string _identifier;
    Type _type;
    
public:
    static bool _isShowRateUs;
};

#endif /* defined(__ColorBook__LockManager__) */
