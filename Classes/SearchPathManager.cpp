//
//  SearchPathManager.cpp
//  ColorGarden
//
//  Created by houhuang on 17/3/16.
//
//

#include "SearchPathManager.hpp"

static SearchPathManager* _instance = nullptr;

SearchPathManager* SearchPathManager::getInstance()
{
    if (!_instance)
    {
        _instance = new SearchPathManager();
    }
    
    return _instance;
}

void SearchPathManager::updateSearchPath()
{
    //set resource search path
    vector<string> searchPaths;
    searchPaths.push_back(FileUtils::getInstance()->getWritablePath());
    searchPaths.push_back("cover");
    searchPaths.push_back("data");
    searchPaths.push_back("ui");
    
    FileUtils::getInstance()->setSearchPaths(searchPaths);
    
}