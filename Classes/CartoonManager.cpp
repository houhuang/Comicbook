//
//  CartoonManager.cpp
//  ComicBook
//
//  Created by houhuang on 17/12/1.
//
//

#include "CartoonManager.hpp"
#include "CSVParse.h"

static CartoonManager* _instance = nullptr;

CartoonManager* CartoonManager::getInstance()
{
    if (!_instance)
    {
        _instance = new CartoonManager();

    }
    
    return _instance;
}

void CartoonManager::readCartoonCsv()
{
    CSVParse* lCsv = CSVParse::create("cartoon.csv");
    if (lCsv)
    {
        unsigned int row = lCsv->getRows();
        for (int i = 1; i < row; ++i)
        {
            CartoonInfo car;
            car.isLock = lCsv->getDatas(i, "isLock");
            car.isNew = lCsv->getDatas(i, "isNew");
            car.folder = lCsv->getDatas(i, "folder");
            car.name = lCsv->getDatas(i, "name");
            car.cateId = lCsv->getDatas(i, "cateId");
            car.area = lCsv->getDatas(i, "area");
            car.des = lCsv->getDatas(i, "des");
            car.coverUrl = lCsv->getDatas(i, "coverUrl");
            car.isfinish = lCsv->getDatas(i, "isfinsh");
            
            _cartoonInfo.push_back(car);
        }
    }
}

void CartoonManager::readCategoryCsv()
{
    CSVParse* lCsv = CSVParse::create("category.csv");
    if (lCsv)
    {
        unsigned int row = lCsv->getRows();
        for (int i = 1; i < row; ++i)
        {
            Category category;
            category.name = lCsv->getDatas(i, "name");
            category.id = lCsv->getDatas(i, "id");
            category.cover = lCsv->getDatas(i, "cover");
            _categoryInfo.push_back(category);
        }
    }
    
    this->readCartoonCsv();
    for (int i = 0; i < _categoryInfo.size(); ++i)
    {
        for (int j = 0; j < _cartoonInfo.size(); ++j)
        {
            if (_categoryInfo.at(i).id == _cartoonInfo.at(j).cateId)
            {
                _categoryInfo.at(i)._cartoonVec.push_back(_cartoonInfo.at(j));
            }
        }
    }
  
}

void CartoonManager::readCurrentPictureCsv(string path)
{
    _currentPictureInfo.clear();
    
    CSVParse* lCsv = CSVParse::create(path.c_str());
    if (lCsv)
    {
        unsigned int row = lCsv->getRows();
        for (int i = 0; i < row; ++i)
        {
            Picture picture;
            picture.chaID = lCsv->getData(i, 0);
            picture.index = lCsv->getData(i, 1);
            picture.url = lCsv->getData(i, 2);
            
            _currentPictureInfo.push_back(picture);
        }
    }
}

vector<Picture>& CartoonManager::getCurrentPictureInfo()
{
    return _currentPictureInfo;
}

Category& CartoonManager::getCurrentCategory()
{
    return _categoryInfo.at(_cateIndex);
}
void CartoonManager::setCurrentCategory(int index)
{
    _cateIndex = index;
}

CartoonInfo& CartoonManager::getCurrentCartoon()
{
    return getCurrentCategory()._cartoonVec.at(_cartIndex);
}

void CartoonManager::setCurrentCartoon(int index)
{
    _cartIndex = index;
}

vector<Category>& CartoonManager::getCategoryInfo()
{
    return _categoryInfo;
}

ReadingCartoonInfo& CartoonManager::getCurrentReadingCartoon()
{
    return _currentReadingCartoon;
}

void CartoonManager::setCurrentFolder(string folder)
{
    _currentFolder = folder;
}

string CartoonManager::getCurrentFolder()
{
    if (_currentFolder == "")
    {
        return getCurrentCartoon().folder;
    }else
    {
        return _currentFolder;
    }
}