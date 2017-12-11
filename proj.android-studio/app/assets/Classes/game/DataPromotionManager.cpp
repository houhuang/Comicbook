//
//  DataPromotionManager.cpp
//  ALABS0002NEW
//
//  Created by houhuang on 16/6/2.
//
//

#include "DataPromotionManager.hpp"
#include "MyArtDataManager.hpp"
#include "CSVParse.h"
#include "ChapterManager.hpp"
#include "NewDownloadManager.hpp"
#include "STSystemFunction.h"

#define promotionTo2_0  "promotionTo2_0"
#define promotionTo2_1  "promotionTo2_1"
#define promotionTo4_0  "promotionTo4_0"

static DataPromotionManager* _instance = nullptr;

typedef struct _Chapter_spare
{
    string id;
    string name;
    string frame;
    string img;
}Chapter_spare;

DataPromotionManager* DataPromotionManager::getInstance()
{
    if (!_instance) {
        _instance = new DataPromotionManager();
    }
    return _instance;
}


void DataPromotionManager::promotionToV2_0()
{
    
    if (UserDefault::getInstance()->getBoolForKey(promotionTo2_0, false))
    {
        return;
    }
    
    for (int i = 0; i < 100; ++i)
    {
        char buffer[20];
        sprintf(buffer, "map%d_save.png",i);
        std::string filePath = FileUtils::getInstance()->getWritablePath() + buffer;
        
        if (FileUtils::getInstance()->isFileExist(filePath))
        {
            std::string name = buffer;
            int index = name.find("_");
            
            std::string str = name.substr(0, index);
            std::string newName = str + "_0_save.png";
            
            rename(filePath.c_str(), (FileUtils::getInstance()->getWritablePath() + newName).c_str());
        }
    }
    
    for (int i = 0; i < 300; ++i)
    {
        char buffer[20];
        sprintf(buffer, "%04d_1024_save.png",i);
        std::string filePath = FileUtils::getInstance()->getWritablePath() + buffer;
        
        if (FileUtils::getInstance()->isFileExist(filePath))
        {
            std::string name = buffer;
            std::string str = name.substr(0, 4);
            std::string newName = str + "_1024_0_save.png";
            
            rename(filePath.c_str(), (FileUtils::getInstance()->getWritablePath() + newName).c_str());
        }
    }
    
    UserDefault::getInstance()->setBoolForKey(promotionTo2_0, true);
    
    UserDefault::getInstance()->flush();
}

void DataPromotionManager::promotionToV2_1()
{
    if (UserDefault::getInstance()->getBoolForKey(promotionTo2_1, false))
    {
        return;
    }
    
    std::string lPath = FileUtils::getInstance()->getWritablePath() + "myArtFile.csv";
    
    if (FileUtils::getInstance()->isFileExist(lPath))
    {
        CSVParse* lCsv = CSVParse::create(lPath.c_str());
        if (lCsv == nullptr)
        {
            return;
        }
        
        //     CSVParse* lCsv = CSVParse::create(string(string("data/") + st_SaveMyArtDataFile).c_str());
        
        const unsigned int row = lCsv->getRows();
        for (int i = 0; i < row; ++i)
        {
            string lName = lCsv->getData(i, 0);
            string lId = lCsv->getData(i, 1);
            string lPreName = lCsv->getData(i, 2);
            
            MyArtData lMyArtData;
            lMyArtData.name = lName;
            lMyArtData.serveId = stoi(lId);
            lMyArtData.previewName = lPreName;
            
            xMyArtData->m_vMyArtDatas.push_back(lMyArtData);
        }
        
        xMyArtData->saveMyArtData();
    }
    
//    //revise chapter.csv
    std::string lChapter_Path = FileUtils::getInstance()->getWritablePath() + "color/chapter.csv";
    if (FileUtils::getInstance()->isFileExist(lChapter_Path))
    {
        std::vector<Chapter_spare> vChapter_spare;
        
        CSVParse* lChapter = CSVParse::create(lChapter_Path.c_str());
        
        
        const unsigned int row = lChapter->getRows();
        for (int i = 0; i < row; ++i)
        {
            Chapter_spare info;
            
            info.id = lChapter->getDatas(i, "id");
            info.name = lChapter->getDatas(i, "name");
            info.frame = lChapter->getDatas(i, "frame");
            
            if (info.id == "id")
            {
                info.img = "img";
            }
            
            if (info.name == "Mandala")
            {
                info.img = "mandala.png";
            }
            
            if (info.name == "Floral")
            {
                info.img = "flora.png";
            }
            
            if (info.name == "Animal")
            {
                info.img = "animal.png";
            }
            
            if (info.name == "Doodle")
            {
                info.img = "doodle.png";
            }
            
            if (info.name == "Pixel")
            {
                info.img = "pixel.png";
            }
            
            if (info.name == "Creative")
            {
                info.img = "creative.png";
            }
            
            vChapter_spare.push_back(info);
        }
        
        
        std::vector<list<std::string>> vList;
        
        for (int i = 0; i < vChapter_spare.size(); ++i)
        {
            list<std::string> lList;
            Chapter_spare info = vChapter_spare.at(i);
            lList.push_back(info.id);
            lList.push_back(info.name);
            lList.push_back(info.frame);
            lList.push_back(info.img);
            
            vList.push_back(lList);
        }
        
        this->saveCSVDataTofile(vList);
        
    }
    

    
    UserDefault::getInstance()->setBoolForKey(promotionTo2_1, true);
    
    UserDefault::getInstance()->flush();
}

void DataPromotionManager::promotionToV4_0()
{
    if (UserDefault::getInstance()->getBoolForKey(promotionTo4_0, false))
    {
        return;
    }
    
    UserDefault::getInstance()->setFloatForKey("graph_version", 1.0f);
    UserDefault::getInstance()->setFloatForKey("chapter_version", 1.0f);
    
    //revise chapter.csv
    std::string lChapter_Path = FileUtils::getInstance()->getWritablePath() + "color/chapter.csv";
    if (FileUtils::getInstance()->isFileExist(lChapter_Path))
    {
        std::vector<Chapter_spare> vChapter_spare;
        
        CSVParse* lChapter = CSVParse::create(lChapter_Path.c_str());
        
        
        const unsigned int row = lChapter->getRows();
        for (int i = 0; i < row; ++i)
        {
            Chapter_spare info;
            
            info.id = lChapter->getDatas(i, "id");
            info.name = lChapter->getDatas(i, "name");
            info.frame = lChapter->getDatas(i, "frame");
            info.img = lChapter->getDatas(i, "img");
//            
//            if (info.id == "id")
//            {
//                info.img = "img";
//            }
//            
//            if (info.name == "Mandala" || info.name == "MANDALA")
//            {
//                info.img = "mandala1.png";
//            }
//            
//            if (info.name == "Floral" || info.name == "FLORA" || info.name == "FLORAL")
//            {
//                info.img = "floral1.png";
//            }
//            
//            if (info.name == "Animal" || info.name == "ANIMAL")
//            {
//                info.img = "animal1.png";
//            }
//            
//            if (info.name == "BEAUTY" || info.name == "Beauty")
//            {
//                info.img = "beauty1.png";
//            }
//            
//            if (info.name == "FASHION" || info.name == "Fashion")
//            {
//                info.img = "fashion1.png";
//            }
//            
//            if (info.name == "FESTIVAL" || info.name == "Festival")
//            {
//                info.img = "festival1.png";
//            }
//            
//            if (info.name == "PATTERN" || info.name == "Pattern")
//            {
//                info.img = "pattern1.png";
//            }
//            
//            if (info.name == "CREATIVE" || info.name == "Creative")
//            {
//                info.img = "creative1.png";
//            }
//            
//            if (info.name == "ZODIAC" || info.name == "Zodiac")
//            {
//                info.img = "zodiac1.png";
//            }
//            
//            if (info.name == "FOOD" || info.name == "Food")
//            {
//                info.img = "food1.png";
//            }
//            
//            if (info.name == "KID" || info.name == "Kid")
//            {
//                info.img = "kid1.png";
//            }
//            
//            if (info.name == "CHRISTMAS" || info.name == "Christmas")
//            {
//                info.img = "christmas1.png";
//            }
//            
//            if (info.name == "NEW YEAR" || info.name == "New year")
//            {
//                info.img = "newyear1.png";
//            }
            
            vChapter_spare.push_back(info);
        }
        
        
        std::vector<list<std::string>> vList;
        
        for (int i = 0; i < vChapter_spare.size(); ++i)
        {
            list<std::string> lList;
            Chapter_spare info = vChapter_spare.at(i);
            lList.push_back(info.id);
            lList.push_back(info.name);
            lList.push_back(info.frame);
            lList.push_back(info.img);
            
            vList.push_back(lList);
        }
        
        this->saveCSVDataTofile(vList);
        
    }
    
    
    
    UserDefault::getInstance()->setBoolForKey(promotionTo4_0, true);
    
    UserDefault::getInstance()->flush();
}

bool DataPromotionManager::isEverDownloadFinsh()
{
    string lPath = FileUtils::getInstance()->getWritablePath() + "color/chapter.csv";
    string lPath2 = FileUtils::getInstance()->getWritablePath() + "color/chapter4.csv";
    if (FileUtils::getInstance()->isFileExist(lPath) || FileUtils::getInstance()->isFileExist(lPath2))
    {
        return true;
    }
    
    return false;
}

void DataPromotionManager::checkForcedUpdate()
{
    STSystemFunction sf;
    string bundleId = sf.getPlatformBundleID();
    string version = sf.getPlatformVersion();
    
    string url = "http://10.0.18.2:8000/appupdate/?bundle_id=" + bundleId + "&version=" + version;
    
    log("---forced update url = %s",url.c_str());
    xNewDownLoad->getServiceData(url);
}

void DataPromotionManager::saveCSVDataTofile(const vector<list<string>>& lCsvData)
{
    string csvFilePath = FileUtils::getInstance()->getWritablePath() + "color/chapter4.csv";
    
    ofstream _streamToFile(csvFilePath);
    ///    判断打开文件是否成功
    if (!_streamToFile.is_open())
        return;
    /// 本地变量
    static char chQuote = '"';
    static char chComma = ',';
    /// Loop through each list of string in vector
    for (vector<list<string> >::const_iterator vIt = lCsvData.begin(); vIt != lCsvData.end(); vIt ++) {
        /// Loop through each string in list
        for (list<string>::const_iterator lIt = vIt->begin(); lIt != vIt->end(); lIt ++) {
            /// Separate this value from previous
            if (vIt->begin() != lIt)
                _streamToFile.put(chComma);
            /// 考虑string中可能有,或"的情况，这就要特殊包装。
            bool bComma = (lIt->find(chComma) != lIt->npos);
            bool bQuote = (lIt->find(chQuote) != lIt->npos);
            /// 真的含有,或"的情况
            if (bComma || bQuote) {
                _streamToFile.put(chQuote);
                
                
                if (bQuote) {
                    for (string::const_iterator chIt = lIt->begin(); chIt != lIt->end(); chIt ++ ) {
                        // Pairs of quotes interpreted as single quote
                        if (chQuote == *chIt)
                            _streamToFile.put(chQuote);
                        
                        _streamToFile.put(*chIt);
                    }
                }
                else
                    _streamToFile << *lIt;
                
                
                _streamToFile.put(chQuote);
            }
            else
                _streamToFile << *lIt;
        }
        /// 换行
        _streamToFile << endl;
    }
}