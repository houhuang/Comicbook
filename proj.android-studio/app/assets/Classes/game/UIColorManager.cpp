//
//  UIColorManager.cpp
//  ColorGarden
//
//  Created by houhuang on 16/8/23.
//
//

#include "UIColorManager.hpp"
#include "CSVParse.h"

static UIColorManager* _instance = nullptr;

UIColorManager* UIColorManager::getInstance()
{
    if (!_instance)
    {
        _instance = new UIColorManager();
    }
    
    return _instance;
}

UIColorManager::UIColorManager()
{
    this->initUIColorData();
}

void UIColorManager::initUIColorData()
{
    if (!_UIColorData.empty())
    {
        return;
    }
    
    CSVParse* lCsv = CSVParse::create("ui_color.csv");
    const unsigned row = lCsv->getRows();
    
    for (int i = 1; i < row; ++i)
    {
        string name = lCsv->getDatas(i, "ui_name");
        int r = stoi(lCsv->getDatas(i, "R"));
        int g = stoi(lCsv->getDatas(i, "G"));
        int b = stoi(lCsv->getDatas(i, "B"));
        
        UIColorInfo info;
        info.name = name;
        info.color = Color3B(r, g, b);

        _UIColorData.push_back(info);
    }
}

Color3B UIColorManager::findUIColor(string name)
{
    for (int i = 0; i < _UIColorData.size(); ++i)
    {
        UIColorInfo info = _UIColorData.at(i);
        if (strcmp(info.name.c_str(), name.c_str()) == 0)
        {
            return info.color;
        }
    }
    
    return Color3B(255, 255, 255);
}








