//
//  PalletteManager.cpp
//  ALABS0010
//
//  Created by houhuang on 16/3/22.
//
//

#include "PalletteManager.hpp"
#include "CSVParse.h"
#include "UITools.h"
#include "cocos-ext.h"

static PalletteManager* _instance = NULL;
bool PalletteManager::isAlreadySelectedColor = false;

PalletteManager* PalletteManager::getInstance()
{
    if (!_instance)
    {
        _instance = new PalletteManager();
    }
    
    return _instance;
}

PalletteManager::PalletteManager()
{
    readPalletteCsvInfo();
    _currentColor = m_vPalletteInfo.at(0).vRGBInfo.at(0);
}

PalletteManager::~PalletteManager()
{

}

void PalletteManager::readPalletteCsvInfo()
{
    m_vPalletteInfo.clear();
    
    string jsonPath = FileUtils::getInstance()->getStringFromFile("pub_gte_2.2.json");
    rapidjson::Document oDoc;
    oDoc.Parse<0>(jsonPath.c_str());
    
    if (oDoc.HasParseError())
    {
        log("Parse Error: %d\n", oDoc.GetParseError());
    }
    
    if (oDoc.IsNull())
    {
        log("NULL......");
    }
    
    rapidjson::Value& lPallette = oDoc["palettes"];
    
    for (int i = 0; i < lPallette.Size(); ++i)
    {
        PalletteInfo lPalletteInfo;
        lPalletteInfo.name = lPallette[i]["name"].GetString();
        string isLock = lPallette[i]["islock"].GetString();
        if (isLock == "FALSE")
        {
            lPalletteInfo.isLock = false;
        }else
        {
            lPalletteInfo.isLock = false;
        }
        
        rapidjson::Value& rgbInfo = lPallette[i]["rgbs"];
        for (int j = 0; j < rgbInfo.Size(); ++j)
        {
            string rgbStr = rgbInfo[j].GetString();
            
            auto rIndex = rgbStr.find(",");
            int rValue = stoi(rgbStr.substr(0,rIndex));
            
            auto gIndex = rgbStr.find(",", rIndex + 1);
            int gValue = stoi(rgbStr.substr(rIndex + 1, gIndex));
            
            int bValue = stoi(rgbStr.substr(gIndex + 1, rgbStr.length()));
            
            lPalletteInfo.vRGBInfo.push_back(Color3B(rValue, gValue, bValue));
        }
        
        m_vPalletteInfo.push_back(lPalletteInfo);
    }
    
    log("");
//    CSVParse* lCsv = CSVParse::create("pallette4.csv");
//    const unsigned int row = lCsv->getRows();
//    const unsigned int col = lCsv->getCols();
//    for (int i = 1; i < row; ++i)
//    {
//        PalletteInfo lPalletteInfo;
//        lPalletteInfo.name = lCsv->getDatas(i, "name");
//        
//        if (lPalletteInfo.name.empty()) break;
//            
//        string lIslock = lCsv->getDatas(i, "islock");
//        if (lIslock == "TRUE" || lIslock == "TURE")
//        {
//            lPalletteInfo.isLock = true;
//            
//        }else
//        {
//            lPalletteInfo.isLock = false;
//        }
//        
//        
//        for (int j = 2; j < col; j++)
//        {
//            string lCsv_rgbInfo = lCsv->getData(i, j);
//            if (lCsv_rgbInfo == "")
//            {
//                break;
//            }
//            
//            auto rIndex = lCsv_rgbInfo.find(".");
//            int rValue = stoi(lCsv_rgbInfo.substr(0,rIndex));
//            
//            auto gIndex = lCsv_rgbInfo.find(".", rIndex + 1);
//            int gValue = stoi(lCsv_rgbInfo.substr(rIndex + 1, gIndex));
//            
//            int bValue = stoi(lCsv_rgbInfo.substr(gIndex + 1, lCsv_rgbInfo.length()));
//            
//            lPalletteInfo.vRGBInfo.push_back(Color3B(rValue, gValue, bValue));
//        }
//        
//        m_vPalletteInfo.push_back(lPalletteInfo);
//    }
    
}

const vector<PalletteInfo> & PalletteManager::getData()
{
    return m_vPalletteInfo;
}

const Color3B & PalletteManager::getColor(size_t palletteIndex, int colorIndex)
{
    _palletteIndex = palletteIndex;
    _colorIndex = colorIndex;
    _currentColor = m_vPalletteInfo.at(palletteIndex).vRGBInfo.at(colorIndex);
    return _currentColor;
}

Color4B  PalletteManager::getRandomColor(size_t palletteIndex)
{
    auto colorInfo = m_vPalletteInfo.at(palletteIndex).vRGBInfo;
    _currentColor = colorInfo.at(quickRandom(0, (int)colorInfo.size() - 1));
    return std::move(Color4B(_currentColor));
}

const Color3B & PalletteManager::getCurrentColor()
{
    return _currentColor;
}

void PalletteManager::saveOldColor()
{
    _oldColor = _currentColor;
}

Color4B PalletteManager::getOldColor()
{
    _currentColor = _oldColor;
    return std::move(Color4B(_currentColor));
}

int PalletteManager::getPalletteIndex()
{
    return _palletteIndex;
}

int PalletteManager::getColorIndex()
{
    return _colorIndex;
}

void PalletteManager::reset()
{
    _palletteIndex = 0;
    _colorIndex = 0;
    _currentColor = m_vPalletteInfo.at(0).vRGBInfo.at(0);
}

