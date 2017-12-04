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

class Category
{
public:
    string name;
    string id;
    string cover;
    vector<CartoonInfo> _cartoonVec;
};

class CartoonManager
{
public:
    static CartoonManager* getInstance();
    
public:
    void readCategoryCsv();
    vector<Category>& getCategoryInfo();
    
    Category& getCurrentCategory();
    void setCurrentCategory(int index);
protected:
    vector<CartoonInfo> _cartoonInfo;

private:
    void readCartoonCsv();
    vector<Category>  _categoryInfo;
    
    int _cateIndex;
};

#endif /* CartoonManager_hpp */
