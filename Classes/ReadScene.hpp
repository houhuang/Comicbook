//
//  ReadScene.hpp
//  ComicBook
//
//  Created by houhuang on 17/12/6.
//
//

#ifndef ReadScene_hpp
#define ReadScene_hpp

#include <stdio.h>
#include "CartoonManager.hpp"
#include "ContentLayer.hpp"
USING_NS_CC;
using namespace std;

class ReadScene : public Scene
{
public:
//    CREATE_FUNC(ReadScene);
    static ReadScene* create(int page, string sceneName);
    virtual bool init(int page, string sceneName);
    
    ReadScene();
    void initUILayer();
public:
    void addListener();
    void onButton(Ref* ref);
    
public:
    void towardLeftMove();
    void towardRightMove();
    
    void resetLayerPointer(bool towardLeftMove);
    
    void resetCenterLayer();

    void updateLeftContent();
    void updateRightContent();
    
    void saveCurrentPage();
private:
    ContentLayer* _leftLayer;
    ContentLayer* _centerLayer;
    ContentLayer* _rightLayer;
    
    bool    _isMoving;
    int     _currentPage;
    
    string  _folder;
    vector<Picture> _currentPic;
    
    string  _preSceneName;
};

#endif /* ReadScene_hpp */
