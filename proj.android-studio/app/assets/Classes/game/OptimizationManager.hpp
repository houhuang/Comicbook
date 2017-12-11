//
//  OptimizationManager.hpp
//  ColorGarden
//
//  Created by houhuang on 17/3/6.
//
//

#ifndef OptimizationManager_hpp
#define OptimizationManager_hpp

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;
using namespace std;

#define xOptz   OptimizationManager::getInstance()

class OptimizationManager
{
public:
    static OptimizationManager* getInstance();
    OptimizationManager();
    
public:
    //方式1:定期删除文件夹（本APP不适用）
    void clearInspirationResoure_deleteFolder();
    
    //方式2:每次启动删除图片
    void clearInspirationResoure_deleteFile();
protected:
    bool isAlreadyOneWeek(const string& preTime);
    std::vector<std::string> getFilePathAtVec(std::string filePath);
    void deleteInspirationFile();
};

#endif /* OptimizationManager_hpp */
