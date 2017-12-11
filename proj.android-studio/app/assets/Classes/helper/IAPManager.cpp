#include "IAPManager.h"
#include "AppConfigs.h"
#include "AudioHelper.h"


static IAPManager *_sharedIAP = nullptr;
IAPManager* IAPManager::getInstance()
{
    if (! _sharedIAP)
    {
        _sharedIAP = new IAPManager();
    }
    
    return _sharedIAP;
}

void IAPManager::destroyInstance()
{
    CC_SAFE_DELETE(_sharedIAP);
}

IAPManager::IAPManager()
{
    _userDefault = UserDefault::getInstance();
    _appBilling.setIABDelegate(this);
    
    string identifier = IAP_PREMIUM_SUBSCRIPTION;
    if(_userDefault->getBoolForKey(identifier.c_str(), false))
    {
        _purchased.push_back(IAP_PREMIUM_SUBSCRIPTION);
    }
}

void IAPManager::purchase(const string& iapid)
{
    _appBilling.purchase(iapid.c_str());
}

void IAPManager::restore()
{
    _appBilling.restore();
}

void IAPManager::purchaseSuccessful(const char* identifier)
{
    string iap = identifier;
    
    if (!isPurchased(iap)) {
        _purchased.push_back(iap);
        
        //写如文件保存, 后面可以来做加密
        string identifier = iap;
        _userDefault->setBoolForKey(identifier.c_str(), true);
        _userDefault->flush();
    }
    
    Value id(identifier);
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVENT_IAP, (void*)&id);
}

void IAPManager::purchaseFailed(const char *pid, int errorCode)
{
}

void IAPManager::restoreSuccessful(const char* pid)
{
    purchaseSuccessful(pid);
}

void IAPManager::restoreFailed(const char* pid, int errorCode)
{
}

bool IAPManager::isPurchased(const string& iapid)
{
    for (auto id : _purchased) {
        if (id == iapid) {
            return true;
        }
    }

    return false;
}

bool IAPManager::isUnlockAll()
{
    //临时这样处理
    return !_purchased.empty();
}