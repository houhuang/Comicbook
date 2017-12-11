//
//  MyArtDataManager.hpp
//  ALABS0010
//
//  Created by houhuang on 16/3/30.
//
//

#ifndef MyArtDataManager_hpp
#define MyArtDataManager_hpp

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;
using namespace std;

#define xMyArtData  MyArtDataManager::getInstance()
#define rateUsCount "rateUsCount"

typedef struct _MyArtData
{
    string name;
    string previewName;
    int serveId = 0;
}MyArtData;

class MyArtDataManager
{
public:
    MyArtDataManager();
    ~MyArtDataManager();

    static MyArtDataManager* getInstance();
    
public:
    //添加删除vector中的数据，程序中使用
    void addImageToVec(MyArtData lArtData);
    void deleteImageFromVec(const string& imageName);
    
    //保存读取文件中的数据，程序结束或开始时使用
    void saveMyArtData();
    void readMyArtData();
    bool isShared(const string& previewName);
    
private:
    void saveMyArtDataTofile(const vector<list<string>>& lCsvData, const string& fileName);
    
public:
    vector<MyArtData> m_vMyArtDatas;
    vector<MyArtData> m_vMyArtPostDatas;
    bool m_bIsChapterEnterMyart;
};

#endif /* MyArtDataManager_hpp */
