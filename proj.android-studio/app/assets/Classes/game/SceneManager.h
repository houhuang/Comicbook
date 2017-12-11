//
//  SceneManager.h
//  SingleEye
//
//  Created by luoxiaopeng on 1/21/14.
//
//

#pragma once

#include "UIHeader.h"

#define xScene      (SceneManager::getInstance())

class SceneManager : public Node{
    
public:
    enum class Tag{
        NONE,
        LOADING,
        CHAPTER,
        GRAPH,
        GAME,
        MY_ARTS,
        SHARE,
        SETTING,
        INSPIRATION,
    };
    
    enum class SceneAnimation
    {
        NONE,
        INUP,
        INRIGHT,
        INLEFT,
        OUTLEFT,
    };
    
    struct SceneData {
        Tag aTag;
        int iData;
        void* pData;
        bool isForward;
        SceneAnimation sceneAnimation;
    };
    
public:
    static SceneManager* getInstance();
    
public:
    ~SceneManager();    
    void forward(Tag aTag, SceneAnimation animate = SceneAnimation::NONE);
    void forward(Tag aTag, SceneAnimation animate, int iData, void* pData);
    
    void back(SceneAnimation animate = SceneAnimation::NONE);
    void back(Tag aTag);
    Tag getRunningSceneTag();
    Tag getLastSceneTag();
    void updateCSV();
    
    void dumpAll();
    
    CC_SYNTHESIZE(bool, _isResponseBack, IsResponseBack);
    
    static bool _isEnableClick;
private:
    SceneManager();    
    bool isOldScene();
    void replaceScene(SceneData &data);
    
    Tag                     m_eLastScene;
    Scene*                  m_pOldScene;
    SceneData               m_eBackup;
    std::vector<SceneData>  m_vData;
    
    bool _updateCSV = false;
};
