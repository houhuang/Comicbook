//
//  ChapterManager.cpp
//  ALABS0010
//
//  Created by houhuang on 16/3/21.
//
//

#include "ChapterManager.hpp"
#include "UITools.h"
#include "cocos-ext.h"

static ChapterManager* _instance = NULL;

ChapterManager* ChapterManager::getInstance()
{
    if (!_instance)
    {
        _instance = new ChapterManager();
    }
    
    return _instance;
}

ChapterManager::ChapterManager()
{
    _tableOffset = 10;
//    this->updateChapterData();
}

ChapterManager::~ChapterManager()
{
    
}

void ChapterManager::readChapterCsvInfo()
{
    m_vChapterInfo.clear();
    
//    this->readNewAddCsvDataFromGroup();
    
    string path = FileUtils::getInstance()->getWritablePath() + "color/pub_gte_2.2.json";
    if (!FileUtils::getInstance()->isFileExist(path))
    {
        path = "pub_gte_2.2.json";
    }
    
    CSVParse* lCsv = CSVParse::create("newpicture.csv");
    unsigned int row = lCsv->getRows();
    
    string jsonPath = FileUtils::getInstance()->getStringFromFile(path.c_str());
    rapidjson::Document oDoc;
    oDoc.Parse<0>(jsonPath.c_str());

    
    if (oDoc.HasParseError())
    {
        log("Parse Error: %d\n", oDoc.GetParseError());
    }
    
    if (oDoc.IsNull())
    {
        log("json file is NULL......");
    }
    
    rapidjson::Value& lGroup  = oDoc["groups"];
    rapidjson::Value& lCha    = oDoc["chapters"];
    rapidjson::Value& lCover  = oDoc["covers"];

    for (int i = 0; i < lGroup.Size(); ++i)
    {
        Chapter lChapter;
        rapidjson::Value& lChapterForJson = lGroup[i];
        lChapter.id = stoi(lChapterForJson["id"].GetString());
        lChapter.name = lChapterForJson["name"].GetString();
        
        string coverId = lChapterForJson["cover"].GetString();
        for (int j = 0; j < lCover.Size(); ++j)
        {
            rapidjson::Value& lCoverMem = lCover[j];
            if (lCoverMem["id"].GetString() == coverId)
            {
                lChapter.coverUrl = lCoverMem["url"].GetString();
                lChapter.image = lCoverMem["name"].GetString();
                lChapter.reDownload = lCoverMem["reDownload"].GetString();
                
                string lFrame = lCoverMem["frame"].GetString();
                
                int rIndex = (int)lFrame.find(".");
                int rValue = std::stoi(lFrame.substr(0,rIndex));
                
                int gIndex = (int)lFrame.find(".", rIndex + 1);
                int gValue = std::stoi(lFrame.substr(rIndex + 1, gIndex));
                int bValue = std::stoi(lFrame.substr(gIndex + 1, lFrame.length()));
                
                lChapter.frame = Color3B(rValue, gValue, bValue);
                
                break;
            }
        }
        
        rapidjson::Value& chapterIdArr = lChapterForJson["chapter"];
        for (int j = 0; j < chapterIdArr.Size(); ++j)
        {
            lChapter.vChapterInGroupId.push_back(stoi(chapterIdArr[j].GetString()));

            for (int k = 0; k < lCha.Size(); ++k)
            {
                if (stoi(chapterIdArr[j].GetString()) == stoi(lCha[k]["id"].GetString()))
                {
                    rapidjson::Value& lPicture = lCha[k]["pictures"];
                    
                    log("lPicture size = %d", lPicture.Size());
                    for (int m = 0; m < lPicture.Size(); ++m)
                    {
                        auto graph = xGraph->getData(stoi(lPicture[m].GetString()));
                        graph.belongToCN = lCha[k]["name"].GetString();
                        lChapter.vGraphInfo.push_back(graph);
                    }
                }
            }
            
            for (int m = 1 ; m < row; ++m)
            {
                if (stoi(lCsv->getDatas(m, "GroupID")) == lChapter.id)
                {
                    Graph graph;
                    graph.name = lCsv->getDatas(m, "Name");
                    
                    graph.chapter_id = stoi(lCsv->getDatas(m, "Mark"));
                    graph.url = "";
                    graph.isLock = false;
                    graph.reDownload = "NewResource";
                    graph.belongToCN = "";
                    graph.bgColor = Color3B(248, 248, 248);
                    
                    graph.historyIndex = xUser->getIntegerForKey(string(graph.name + "_hIndex").c_str(), 0);
                    
                    if (lChapter.id == 4082)
                    {
                        
                        int idx = (int)lChapter.vGraphInfo.size() - 1;
                        for (int l = 0; l < lChapter.vGraphInfo.size(); ++l)
                        {
                            if (lChapter.vGraphInfo.at(l).name == "3072_pool_picture")
                            {
                                idx = l;
                                break;
                            }
                        }
                        
                        lChapter.vGraphInfo.insert(lChapter.vGraphInfo.begin() + idx + 1, graph);
                        
                    }else if (lChapter.id == 4086)
                    {
                        int idx = (int)lChapter.vGraphInfo.size() - 1;
                        for (int l = 0; l < lChapter.vGraphInfo.size(); ++l)
                        {
                            if (lChapter.vGraphInfo.at(l).name == "3070_pool_picture")
                            {
                                idx = l;
                                break;
                            }
                        }
                        
                        lChapter.vGraphInfo.insert(lChapter.vGraphInfo.begin() + idx + 1, graph);
                    }else if (lChapter.id == 4080)
                    {
                        int idx = (int)lChapter.vGraphInfo.size() - 1;
                        for (int l = 0; l < lChapter.vGraphInfo.size(); ++l)
                        {
                            if (lChapter.vGraphInfo.at(l).name == "3074_pool_picture")
                            {
                                idx = l;
                                break;
                            }
                        }
                        
                        lChapter.vGraphInfo.insert(lChapter.vGraphInfo.begin() + idx + 1, graph);
                    }else if (lChapter.id == 4084)
                    {
                        int idx = (int)lChapter.vGraphInfo.size() - 1;
                        for (int l = 0; l < lChapter.vGraphInfo.size(); ++l)
                        {
                            if (lChapter.vGraphInfo.at(l).name == "3088_pool_picture")
                            {
                                idx = l;
                                break;
                            }
                        }
                        
                        lChapter.vGraphInfo.insert(lChapter.vGraphInfo.begin() + idx + 1, graph);
                    }
                    else
                    {
                        lChapter.vGraphInfo.push_back(graph);
                    }
                    
                    
                }
            }
        }

        m_vChapterInfo.push_back(lChapter);
    }
}

void ChapterManager::readNewAddCsvDataFromGroup()
{
//    CSVParse* lCsv = CSVParse::create("newgroup.csv");
//    unsigned int row = lCsv->getRows();
    
//    for (int i = 1; i < row; ++i)
//    {
//        Chapter lChapter;
//        lChapter.id = stoi(lCsv->getDatas(i, "GroupID"));
//        lChapter.name = lCsv->getDatas(i, "Name");
//        lChapter.image = lCsv->getDatas(i, "image");
//        lChapter.reDownload = "NewResource";
//        lChapter.coverUrl = "";
//        
//        string lFrame = lCsv->getDatas(i, "frame");
//        int rIndex = (int)lFrame.find(".");
//        int rValue = std::stoi(lFrame.substr(0,rIndex));
//        
//        int gIndex = (int)lFrame.find(".", rIndex + 1);
//        int gValue = std::stoi(lFrame.substr(rIndex + 1, gIndex));
//        int bValue = std::stoi(lFrame.substr(gIndex + 1, lFrame.length()));
//        
//        lChapter.frame = Color3B(rValue, gValue, bValue);
//    
//        m_vChapterInfo.push_back(lChapter);
//    }
    
    CSVParse* lCsv2 = CSVParse::create("newpicture.csv");
    unsigned int row2 = lCsv2->getRows();
    
    for (int i = 0; i < m_vChapterInfo.size(); ++i)
    {
        Chapter* lChapter = &m_vChapterInfo.at(i);
        for (int j = 1; j <row2; ++j)
        {
            int id = stoi(lCsv2->getDatas(j, "GroupID"));
            if (lChapter->id == id)
            {
                Graph graph;
                graph.name = lCsv2->getDatas(j, "Name");

                graph.chapter_id = id;
                graph.url = "";
                graph.isLock = false;
                graph.reDownload = "NewResource";
                graph.belongToCN = lChapter->name;
                graph.bgColor = Color3B(248, 248, 248);
                
                graph.historyIndex = xUser->getIntegerForKey(string(graph.name + "_hIndex").c_str(), 0);
                lChapter->vGraphInfo.push_back(graph);
            }
        }
        
    }
}

void ChapterManager::readWeeklyData()
{
    m_vWeeklyInfo.clear();
    
    string jsonPath = FileUtils::getInstance()->getStringFromFile("pub_gte_2.2.json");
    rapidjson::Document oDoc;
    oDoc.Parse<0>(jsonPath.c_str());
    
    if (oDoc.HasParseError())
    {
        log("Parse Error: %d\n", oDoc.GetParseError());
    }
    
    if (oDoc.IsNull())
    {
        log("json file is NULL......");
    }
    
    rapidjson::Value& lWeekly = oDoc["weekly"];
    rapidjson::Value& lCover  = oDoc["covers"];
    rapidjson::Value& lPicture= oDoc["pictures"];
    
    for (int i = 0; i < lWeekly.Size(); ++i)
    {
        rapidjson::Value& lWeeklyInfo = lWeekly[i];
        Chapter lChapter;
        lChapter.name = lWeeklyInfo["name"].GetString();
        lChapter.id = stoi(lWeeklyInfo["id"].GetString());
        lChapter.rule = lWeeklyInfo["rule"].GetString();
        
        for (int j = 0; j < lCover.Size(); ++j)
        {
            rapidjson::Value& lCoverInfo = lCover[j];
            if (strcmp(lCoverInfo["id"].GetString(), lWeeklyInfo["cover"].GetString()) == 0)
            {
                lChapter.reDownload = lCoverInfo["reDownload"].GetString();
                
                lChapter.coverUrl = lCoverInfo["url"].GetString();
                lChapter.image = lCoverInfo["name"].GetString();
                
                string lFrame = lCoverInfo["frame"].GetString();
                
                int rIndex = (int)lFrame.find(".");
                int rValue = std::stoi(lFrame.substr(0,rIndex));
                
                int gIndex = (int)lFrame.find(".", rIndex + 1);
                int gValue = std::stoi(lFrame.substr(rIndex + 1, gIndex));
                int bValue = std::stoi(lFrame.substr(gIndex + 1, lFrame.length()));
                lChapter.frame = Color3B(rValue, gValue, bValue);
                
                break;
            }
        }
        
        rapidjson::Value& lPictureInfo = lWeeklyInfo["pictures"];
        for (int k = 0; k < lPictureInfo.Size(); ++k)
        {
            string picID = lPictureInfo[k].GetString();
            
            for (int m = 0; m < lPicture.Size(); ++m)
            {
                if (picID == lPicture[m]["id"].GetString())
                {
                    auto graph = xGraph->getData(stoi(picID));
                    lChapter.vGraphInfo.push_back(graph);
                    break;
                }
            }
        }
        
        if (lChapter.vGraphInfo.size() <= 0) return;
        
        m_vChapterInfo.push_back(lChapter);
    }
    
    log("");
}

void ChapterManager::readChapterCsvInfoDownload()
{
    CSVParse* lCsv = CSVParse::create("_chapter4.csv");
    const unsigned int row = lCsv->getRows();
    
    for (int i = 1; i < row; ++i)
    {
        string lID = lCsv->getDatas(i, "id");
        if (lID.empty()) break;
        string lName = lCsv->getDatas(i, "name");
        
        bool found = false;
        for (auto & chapter : m_vChapterInfo) {
            if (chapter.id == stoi(lID)) {
                found = true;
                createChapter(lCsv, i, false, chapter);
                break;
            }
        }

        if (!found) {
            Chapter lChapterInfo;
            createChapter(lCsv, i, true, lChapterInfo);
            m_vChapterInfo.push_back(lChapterInfo);
        }
    }
    
    
    vector<Chapter>::iterator iter = m_vChapterInfo.begin();
    while(iter != m_vChapterInfo.end())
    {
        log("%d", (*iter).id);
        bool found = false;
        for (int i = 1; i < row; ++i) {
            string lID = lCsv->getDatas(i, "id");
            if (lID.empty()) break;
            
            if ((*iter).id == stoi(lID)) {
                found = true;
                break;
            }
        }
        
        if (!found) {
            iter = m_vChapterInfo.erase(iter);
        } else {
            iter++;
        }
    }
}

void ChapterManager::createChapter(CSVParse* lCsv, int i, bool isNewChapter, Chapter &lChapterInfo)
{
    string lID = lCsv->getDatas(i, "id");
    string lName = lCsv->getDatas(i, "name");
    string lFrame = lCsv->getDatas(i, "frame");
    string lReDownload = lCsv->getDatas(i, "reDownload");
    
    lChapterInfo.id = std::stoi(lID);
    lChapterInfo.name = lName;
    lChapterInfo.image = lCsv->getDatas(i, "img");
    lChapterInfo.isNewChapter = true;
    
    if (lReDownload == "1")
        lChapterInfo.isReDownloadCover = true;
    
    lChapterInfo.isNewChapter = isNewChapter;
    
    int rIndex = lFrame.find(".");
    int rValue = std::stoi(lFrame.substr(0,rIndex));
    
    int gIndex = lFrame.find(".", rIndex + 1);
    int gValue = std::stoi(lFrame.substr(rIndex + 1, gIndex));
    int bValue = std::stoi(lFrame.substr(gIndex + 1, lFrame.length()));
    
    lChapterInfo.frame = Color3B(rValue, gValue, bValue);
}

void ChapterManager::updateChapterInfo(int id)
{
    Chapter & chapter = getChapterData(id);
    chapter.vGraphInfo.clear();
    
    auto graphs = xGraph->getDataDownload();
    for (auto graph : graphs) {
        if (graph.chapter_id == id) {
            chapter.vGraphInfo.push_back(graph);
        }
    }
}

void ChapterManager::updateProgress(int id)
{
    //downloaded++;
//    for (auto chaper : m_vChapterInfo)
//    {
//        if (id == chaper.id)
//        {
//            chaper.downloaded ++;
//        }
//    }
    
    for (int i = 0 ; i < m_vChapterInfo.size(); ++i)
    {
        Chapter *lInfo = &m_vChapterInfo.at(i);
        if (lInfo->id == id)
        {
            lInfo->downloaded ++;
            if (lInfo->downloaded >= lInfo->total)
            {
                this->updateChapterInfo(lInfo->id);
            }
        }
    }
    
    
    NotificationCenter::getInstance()->postNotification(st_updateProgress_message);
    
    
}

void ChapterManager::addTotal(int id)
{
    getChapterData(id).total += 2;
}

void ChapterManager::addCoverTotal(int id)
{
    getChapterData(id).total += 1;
}

vector<Chapter> & ChapterManager::getData()
{
    return m_vChapterInfo;
}

vector<Chapter>& ChapterManager::getWeeklyData()
{
    return m_vWeeklyInfo;
}

Chapter & ChapterManager::getChapterData(int id)
{
    for (int i = 0; i < m_vChapterInfo.size(); i++)
    {
        if (m_vChapterInfo.at(i).id == id)
        {
            return m_vChapterInfo.at(i);
        }
    }
    
    CCASSERT(false, "error");
    return m_vChapterInfo.at(0);
}

void ChapterManager::enterChapter(int index)
{
    _current = index;
    quickFlurry("chapter_open_" + m_vChapterInfo.at(index).name);
}

void ChapterManager::enterChapter(const string& name)
{
    for (int i = 0; i < m_vChapterInfo.size(); ++i)
    {
        if (name == m_vChapterInfo.at(i).name)
        {
            _current = i;
            return;
        }
    }
    _current = 0;
}

bool ChapterManager::enterGraph(const string &name)
{
    for (int chapteri = 0; chapteri < m_vChapterInfo.size(); chapteri++) {
        
        auto & graphs = m_vChapterInfo.at(chapteri).vGraphInfo;
        for (int graphi = 0; graphi < graphs.size(); graphi++) {
            
            if (graphs.at(graphi).getPngName() == name) {
                _current = chapteri;
                getCurrentChapter().enterGraph(graphi);
                return true;
            }
        }
    }
    
    return false;
    CCASSERT(false, "error");
}

Chapter & ChapterManager::getCurrentChapter()
{
    return m_vChapterInfo.at(_current);
}

void Chapter::enterGraph(int index)
{
    _current = index;
    _graph = vGraphInfo.at(_current);
    quickFlurry("img_open_" + vGraphInfo.at(index).name);
}

bool Chapter::isDownloading()
{
    if (getPercent() == 100)
        return false;
    else
        return true;
}

Graph & Chapter::getCurrentGraph()
{
    return _graph;
//    return vGraphInfo.at(_current);
}

void Chapter::setCurrentGraph(Graph graph)
{
    _graph = graph;
}

int Chapter::getPercent() const
{
    if (total == 0) {
        return 100;
    }
    else
    {
        return downloaded * 100 / total;
    }
}

Graph & ChapterManager::getGraph(const string &name)
{
    for (int chapteri = 0; chapteri < m_vChapterInfo.size(); chapteri++) {
        vector<Graph> & graphs = m_vChapterInfo.at(chapteri).vGraphInfo;
        for (int graphi = 0; graphi < graphs.size(); graphi++) {
            if (graphs.at(graphi).getPngName() == name) {
                return graphs.at(graphi);
            }
        }
    }
    
    CCASSERT(false, "error");
    return m_vChapterInfo.at(0).vGraphInfo.at(0);
}

void ChapterManager::updateChapterData()
{
    this->readWeeklyData();
    this->readChapterCsvInfo();
}
