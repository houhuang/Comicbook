//
//  MyArtDataManager.cpp
//  ALABS0010
//
//  Created by houhuang on 16/3/30.
//
//

#include "MyArtDataManager.hpp"
#include "CSVParse.h"
#include "GraphManager.hpp"

#define st_SaveMyArtDataFile    "myArtFile.csv"

static MyArtDataManager* _instance = NULL;

MyArtDataManager::MyArtDataManager()
{
//    for (int i = 0; i < xGraph->getData().size(); ++i)
//    {
//        MyArtData lM;
//        lM.name = xGraph->getData().at(i).getThumbsName();
//        m_vMyArtDatas.push_back(lM);
//    }
    
    //readMyArtData();
    
    m_bIsChapterEnterMyart = false;
}

MyArtDataManager::~MyArtDataManager()
{
    
}

MyArtDataManager* MyArtDataManager::getInstance()
{
    if (!_instance)
    {
        _instance = new MyArtDataManager();
    }
    
    return _instance;
}

void MyArtDataManager::addImageToVec(MyArtData lArtData)
{
    this->deleteImageFromVec(lArtData.previewName);
    m_vMyArtDatas.push_back(lArtData);
}

void MyArtDataManager::deleteImageFromVec(const string& imageName)
{
    for (int i = 0; i < m_vMyArtDatas.size(); ++i)
    {
        MyArtData lMyArtData = m_vMyArtDatas.at(i);
        if (lMyArtData.previewName == imageName)
        {
            vector<MyArtData>::iterator iter = m_vMyArtDatas.begin() + i;
            m_vMyArtDatas.erase(iter);
        }
    }
}

void MyArtDataManager::saveMyArtData()
{
    vector<list<string>> vMyArtData;
    for (int i = 0; i < m_vMyArtDatas.size(); ++i)
    {
        MyArtData lMyArtData = m_vMyArtDatas.at(i);
        list<string>lList;
        lList.push_back(lMyArtData.name);
        
        stringstream ss;
        ss<<"";
        ss<<lMyArtData.serveId;
        lList.push_back(ss.str());
        
        lList.push_back(lMyArtData.previewName);
        
        vMyArtData.push_back(lList);
    }
    this->saveMyArtDataTofile(vMyArtData, "myArtDataAll.csv");
    
    vector<list<string>> vMyArtDataPost;
    for (int i = 0; i < m_vMyArtPostDatas.size(); ++i)
    {
        MyArtData lMyArtData = m_vMyArtPostDatas.at(i);
        list<string>lList;
        lList.push_back(lMyArtData.name);
        
        stringstream ss;
        ss<<"";
        ss<<lMyArtData.serveId;
        lList.push_back(ss.str());
        
        lList.push_back(lMyArtData.previewName);
        
        vMyArtDataPost.push_back(lList);
    }
    this->saveMyArtDataTofile(vMyArtDataPost, "myArtDataPublished.csv");
}

void MyArtDataManager::readMyArtData()
{
    bool isFirstRead = UserDefault::getInstance()->getBoolForKey("isFirstReadMyartDatas", false);
    if (!isFirstRead)
    {
        UserDefault::getInstance()->setBoolForKey("isFirstReadMyartDatas", true);
        
        if (!m_vMyArtDatas.empty())
        {
            m_vMyArtDatas.clear();
        }
        
        if (!FileUtils::getInstance()->isFileExist(FileUtils::getInstance()->getWritablePath() + st_SaveMyArtDataFile))
        {
            return;
        }
        
        CSVParse* lCsv = CSVParse::create((FileUtils::getInstance()->getWritablePath() + st_SaveMyArtDataFile).c_str());
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
            
            m_vMyArtDatas.push_back(lMyArtData);
            
            if (lMyArtData.serveId != 0)
            {
                Data data = FileUtils::getInstance()->getDataFromFile(FileUtils::getInstance()->getWritablePath()+ lPreName);
                string newName = lPreName.substr(0, lPreName.find(".png"));
                string newPath = FileUtils::getInstance()->getWritablePath() + newName + "_p.png";
                FileUtils::getInstance()->writeDataToFile(data, newPath);
                
                lMyArtData.previewName = newName + "_p.png";
                m_vMyArtPostDatas.push_back(lMyArtData);
            }
        }
    
        return;
    }
    
    if (!m_vMyArtDatas.empty())
    {
        m_vMyArtDatas.clear();
    }
    
    if (!FileUtils::getInstance()->isFileExist(FileUtils::getInstance()->getWritablePath() + "myArtDataAll.csv"))
    {
        return;
    }
    
    CSVParse* lCsv = CSVParse::create((FileUtils::getInstance()->getWritablePath() + "myArtDataAll.csv").c_str());
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
        
        m_vMyArtDatas.push_back(lMyArtData);
    }
  
    
    {
        if (!m_vMyArtPostDatas.empty())
        {
            m_vMyArtPostDatas.clear();
        }
        
        if (!FileUtils::getInstance()->isFileExist(FileUtils::getInstance()->getWritablePath() + "myArtDataPublished.csv"))
        {
            return;
        }
        
        CSVParse* lCsv = CSVParse::create((FileUtils::getInstance()->getWritablePath() + "myArtDataPublished.csv").c_str());
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
            
            m_vMyArtPostDatas.push_back(lMyArtData);
        }
    }
    
    
}

bool MyArtDataManager::isShared(const string& previewName)
{
    for (int i = 0; i < m_vMyArtDatas.size(); ++ i)
    {
        if (previewName == m_vMyArtDatas.at(i).previewName)
        {
            if (m_vMyArtDatas.at(i).serveId == 0)
                return false;
            else
                return true;
        }
    }
    return false;
}

void MyArtDataManager::saveMyArtDataTofile(const vector<list<string>>& lCsvData, const string& fileName)
{
    string csvFilePath = FileUtils::getInstance()->getWritablePath() + fileName;
    
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