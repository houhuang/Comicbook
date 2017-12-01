//
//  ChapterManager.hpp
//  ALABS0010
//
//  Created by houhuang on 16/3/21.
//
//

#ifndef ChapterManager_hpp
#define ChapterManager_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "GraphManager.hpp"
#include "CSVParse.h"

USING_NS_CC;
using namespace std;

#define st_updateProgress_message    "st_updateProgress_message"

#define CptM_getInstance    ChapterManager::getInstance()
#define xChapter            ChapterManager::getInstance()


class Chapter{
public:
    void enterGraph(int index);
    Graph& getCurrentGraph();
    bool isDownloading();
    int getPercent() const;
    void setCurrentGraph(Graph graph);
public:
    int id;
    string name;
    Color3B frame;
    string image;
    vector<Graph> vGraphInfo;
    int downloaded = 0;
    int total = 0;
    bool isNewChapter = false;
    float tableOffset = 1.0f;
    bool isReDownloadCover = false;
    string coverUrl;
    string rule;
    string reDownload;
    
    vector<int> vChapterInGroupId;
    Graph _graph;
    
private:
    int _current = 0;
};

class ChapterManager
{
public:
    ChapterManager();
    ~ChapterManager();
    
    static ChapterManager* getInstance();
    
    vector<Chapter> & getData();
    Chapter & getChapterData(int id);
    
    void enterChapter(int index);
    void enterChapter(const string& name);
    bool enterGraph(const string &name);
    Chapter & getCurrentChapter();
    void readChapterCsvInfo();
    void readChapterCsvInfoDownload();
    void updateProgress(int id);
    void addTotal(int id);
    void addCoverTotal(int id);
    Graph & getGraph(const string &name);
    
    bool test = false;
    
    //weekly data
    vector<Chapter>& getWeeklyData();
    void readWeeklyData();

    void updateChapterData();
    
    void readNewAddCsvDataFromGroup();
private:
    void updateChapterInfo(int id);
    void createChapter(CSVParse* lCsv, int i, bool isNewChapter, Chapter &lChapterInfo);
    vector<Chapter> m_vChapterInfo;
    int _current = 0;
    CC_SYNTHESIZE(float, _tableOffset, TableOffset);
    CC_SYNTHESIZE(string, _currentFinishImage, CurrentFinishImage);
    
    vector<Chapter> m_vWeeklyInfo;
};

#endif /* ChapterManager_hpp */
