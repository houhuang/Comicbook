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

vector<CartoonInfo>& CartoonManager::getCartoonInfo()
{
    return _cartoonInfo;
}