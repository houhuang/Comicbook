#ifndef __IAPMANAGER_H__
#define __IAPMANAGER_H__

#include "cocos2d.h"
#include "STInAppBilling.h"

USING_NS_CC;

using std::deque;
using std::string;
using std::vector;

#define xIAP                (IAPManager::getInstance())
#define EVENT_IAP           "event_iap"

class IAPManager : public STIABDelegate{
public:
    static IAPManager* getInstance();
    void destroyInstance();
    
    void purchase(const string& iapid);
    void restore();
    bool isPurchased(const string& iapid);
    bool isUnlockAll();
    
private:
    IAPManager();
    
    virtual void purchaseSuccessful(const char* pid) override;
    virtual void purchaseFailed(const char *pid, int errorCode) override;
    virtual void restoreSuccessful(const char* pid) override;
    virtual void restoreFailed(const char* pid, int errorCode) override;
    
    UserDefault *   _userDefault;
    STInAppBilling  _appBilling;
    
    vector<string>  _purchased;         //已经购买
};


#endif