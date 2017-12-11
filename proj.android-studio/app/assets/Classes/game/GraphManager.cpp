//
//  GraphManager.cpp
//  ALABS0010
//
//  Created by houhuang on 16/3/22.
//
//

#include "GraphManager.hpp"
#include "CSVParse.h"
#include "UndoManager.h"
#include "cocos-ext.h"

static GraphManager* _instance = NULL;

GraphManager* GraphManager::getInstance()
{
    if (!_instance)
    {
        _instance = new GraphManager();
    }
    
    return _instance;
}

GraphManager::GraphManager()
{
    _saveImage = nullptr;
    readGraphCsvInfo();
}

GraphManager::~GraphManager()
{

}

void GraphManager::readGraphCsvInfo()
{
    readGraphCsvInfo("graph.csv", m_vGraphInfo);
}

void GraphManager::readGraphCsvInfo(const string& filename, vector<Graph> & graphs)
{
    graphs.clear();
    
    string jsonPath = FileUtils::getInstance()->getStringFromFile("pub_gte_2.2.json");
    rapidjson::Document oDoc;
    oDoc.Parse<0>(jsonPath.c_str());
    
    if (oDoc.HasParseError())
    {
        log("Parse Error: %d\n", oDoc.GetParseError());
    }
    
    if (oDoc.IsNull())
    {
        log("NULL");
    }
    
    rapidjson::Value& lPicture = oDoc["pictures"];
    
//    {
//        Graph graph;
//        graph.name = "animal01";
//        graph.chapter_id = 10000;
//        graph.url = "";
//        graph.isLock = false;
//        graph.reDownload = "NewImage";
//        
//        graph.historyIndex = xUser->getIntegerForKey(string(graph.name + "_hIndex").c_str(), 0);
//        graphs.push_back(graph);
//    }
    
    for (int i = 0; i < lPicture.Size(); ++i)
    {
        rapidjson::Value& lPictureMem = lPicture[i];
        Graph graph;
        graph.name = lPictureMem["name"].GetString();
        graph.chapter_id = stoi(lPictureMem["id"].GetString());
        graph.url = lPictureMem["url"].GetString();
        string isLock = lPictureMem["islock"].GetString();
        graph.reDownload = lPictureMem["reDownload"].GetString();
        
//        if (isLock == "FALSE")
//        {
//            graph.isLock = false;
//        }else
//        {
//            graph.isLock = true;
//        }
        
        graph.isLock = false;
        
        //读取BGColor
        auto color = xUser->getStringForKey(string(graph.name + "_bgColor").c_str(), "255.255.255");           //默认白色
        
        int rIndex = (int)color.find(".");
        int rValue = std::stoi(color.substr(0,rIndex));
        
        int gIndex = (int)color.find(".", rIndex + 1);
        int gValue = std::stoi(color.substr(rIndex + 1, gIndex));
        int bValue = std::stoi(color.substr(gIndex + 1, color.length()));
        
        graph.bgColor = Color3B(rValue, gValue, bValue);
        
        //读取History
        graph.historyIndex = xUser->getIntegerForKey(string(graph.name + "_hIndex").c_str(), 0);
        graph.historyCount = xUser->getIntegerForKey(string(graph.name + "_hCount").c_str(), 1);
        
        graphs.push_back(graph);
    }
    
    
    
//    CSVParse* csv = CSVParse::create(filename.c_str());
//    const unsigned int row = csv->getRows();
//    for (int i = 1; i < row; ++i)
//    {
//        Graph graph;
//        graph.name = csv->getDatas(i, "name");
//        
//        if (graph.name.empty()) break;
//        graph.chapter_id = stoi(csv->getDatas(i, "chapter_id"));
//        
//        string lIslock = csv->getDatas(i, "islock");
//        if (lIslock == "TRUE")
//        {
//            graph.isLock = true;
//        }
//        else
//        {
//            graph.isLock = false;
//        }
//        
//        //读取BGColor
//        auto color = xUser->getStringForKey(string(graph.name + "_bgColor").c_str(), "255.255.255");           //默认白色
//        
//        int rIndex = color.find(".");
//        int rValue = std::stoi(color.substr(0,rIndex));
//        
//        int gIndex = color.find(".", rIndex + 1);
//        int gValue = std::stoi(color.substr(rIndex + 1, gIndex));
//        int bValue = std::stoi(color.substr(gIndex + 1, color.length()));
//        
//        graph.bgColor = Color3B(rValue, gValue, bValue);
//        
//        //读取History
//        graph.historyIndex = xUser->getIntegerForKey(string(graph.name + "_hIndex").c_str(), 0);
//        graph.historyCount = xUser->getIntegerForKey(string(graph.name + "_hCount").c_str(), 1);
//        
//        graphs.push_back(graph);
//    }
}

void GraphManager::destorySaveImage()
{
    CC_SAFE_DELETE(_saveImage);
    xUndo->reset();
}

vector<Graph> & GraphManager::getData()
{
    return m_vGraphInfo;
}

Graph& GraphManager::getData(const int id)
{
    for (int i = 0; i < m_vGraphInfo.size(); ++i)
    {
        Graph& lGraph = m_vGraphInfo.at(i);
        if (id == lGraph.chapter_id)
        {
            return lGraph;
        }
    }
    return m_vGraphInfo.at(0);
}

vector<Graph> & GraphManager::getDataDownload()
{
    if (m_vGrapInfoDownload.empty()) {
        readGraphCsvInfo("_graph.csv", m_vGrapInfoDownload);
    }
    
    return m_vGrapInfoDownload;
}

string Graph::getHistoryName()
{
    return std::move(string(name + "_" + std::to_string(historyIndex) + "_save.png"));
}

string Graph::getPreviewName()
{
    return std::move(string(name + "_" + std::to_string(historyIndex) + "_preview.png"));
}

string Graph::getThumbsName()
{
    return std::move(string(name + "_thumb.png"));
}

string Graph::getPngName()
{
    return std::move(string(name + ".png"));
}

string Graph::getMaskName()
{
    return std::move(string(name + "_mask.png"));
}

void Graph::setBGColor(const Color3B& color)
{
    if (bgColor != color) {
        bgColor = color;
        
        auto value = std::to_string(bgColor.r) + "." + std::to_string(bgColor.g) + "." + std::to_string(bgColor.b);
        xUser->setStringForKey(string(name + "_bgColor").c_str(), value);
        xUser->flush();
    }
}

void Graph::deleteHistory(const string& previewName)
{
    auto end = previewName.find_last_of("_") - 1;
    auto start = previewName.find_last_of("_", end);
    auto number = previewName.substr(start + 1, end - start);
    
    int index = xUser->getIntegerForKey(string(name + "_hIndex").c_str(), 0);
    int backupData = index;
    if (index == stoi(number))
    {
        for (int i = index - 1; i >=0 ; --i)
        {

            stringstream ss;
            ss<<FileUtils::getInstance()->getWritablePath();
            ss<<name;
            ss<<"_";
            ss<<i;
            ss<<"_save.png";
            
            log("%s",ss.str().c_str());
            if (FileUtils::getInstance()->isFileExist(ss.str()))
            {
                backupData = i;
                break;
            }
            
        }
    }

    historyIndex = stoi(number);
    remove((FileUtils::getInstance()->getWritablePath() + getPreviewName()).c_str());
    remove((FileUtils::getInstance()->getWritablePath() + getHistoryName()).c_str());
    Director::getInstance()->getTextureCache()->removeTextureForKey(getPreviewName());
    Director::getInstance()->getTextureCache()->removeTextureForKey(getHistoryName());

    historyIndex = backupData;
    xUser->setIntegerForKey(string(name + "_hIndex").c_str(), historyIndex);
    xUser->flush();

    setBGColor(Color3B::WHITE);
}

void Graph::setNew(bool iNew)
{
    isNew = iNew;
    historyName.clear();
    
    if (isNew)
    {
//        historyCount++;
//        historyIndex = historyCount - 1;
        historyIndex = xUser->getIntegerForKey(string(name + "_hIndex").c_str(), 0) + 1;

        xUser->setIntegerForKey(string(name + "_hIndex").c_str(), historyIndex);
        xUser->setIntegerForKey(string(name + "_hCount").c_str(), historyCount);
        xUser->flush();
        
        setBGColor(Color3B(248, 248, 248));
    }else
    {
        historyIndex = xUser->getIntegerForKey(string(name + "_hIndex").c_str(), 0);
    }
}

void Graph::setDuplicate(const string& previewName, const Color3B& bgColor)
{
    setNew(true);
    setBGColor(bgColor);
    auto underline = previewName.find_last_of("_");
    auto historyName = previewName.substr(0, underline) + "_save.png";
    this->historyName = historyName;
}

void Graph::setIndex(const string& previewName)
{
    auto end = previewName.find_last_of("_") - 1;
    auto start = previewName.find_last_of("_", end);
    auto number = previewName.substr(start + 1, end - start);
    
    isNew = false;
    historyIndex = stoi(number);
    xUser->setIntegerForKey(string(name + "_hIndex").c_str(), historyIndex);
    xUser->flush();
}