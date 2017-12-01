//
//  CartoonManager.hpp
//  ComicBook
//
//  Created by houhuang on 17/12/1.
//
//

#ifndef CartoonManager_hpp
#define CartoonManager_hpp

#include <stdio.h>
USING_NS_CC;
using namespace std;

#define xCartoon CartoonManager::getInstance()

class CartoonInfo
{
public:
    string isLock;
    string isNew;
    string folder;
    string name;
    string cateId;
    string area;
    string des;
    string coverUrl;
    string isfinish;
};

class CartoonManager
{
public:
    static CartoonManager* getInstance();
    
public:
    void readCartoonCsv();
    vector<CartoonInfo>& getCartoonInfo();
protected:
    vector<CartoonInfo> _cartoonInfo;
    
};

#endif /* CartoonManager_hpp */
