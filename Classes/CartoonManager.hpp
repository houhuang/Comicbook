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

class ReadingCartoonInfo
{
public:
    string folder;
    string csvPath;
    string pageNumber;
};

class Picture
{
public:
    string chaID;
    string index;
    string url;
};

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
    
    void readCurrentPictureCsv(string path);
    vector<Picture>& getCurrentPictureInfo();
    
    Category& getCurrentCategory();
    void setCurrentCategory(int index);
    
    CartoonInfo& getCurrentCartoon();
    void setCurrentCartoon(int index);
    
    ReadingCartoonInfo& getCurrentReadingCartoon();
    
    void setCurrentFolder(string folder);
    string getCurrentFolder();
    
public:
    
protected:
    vector<CartoonInfo> _cartoonInfo;

private:
    void readCartoonCsv();
    vector<Category>  _categoryInfo;
    
    int _cateIndex;
    int _cartIndex;
    
    vector<Picture>   _currentPictureInfo;
    
    ReadingCartoonInfo  _currentReadingCartoon;
    
    string _currentFolder = "";
    
    
};

#endif /* CartoonManager_hpp */
