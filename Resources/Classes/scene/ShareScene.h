//
//  ShareScene.h
//
//
//  Created by luoxp on 3/18/15.
//
//

#pragma once

#include "UITools.h"
#include "PostingLayer.hpp"
#include "BaseScene.hpp"
#include "RateUsLayer.hpp"

class Graph;

class  ShareScene : public BaseScene
{
public :
    virtual ~ShareScene();
    ShareScene();
    
    CREATE_FUNC(ShareScene);
    
    virtual bool init();
    void onButton(Ref* pSender, Widget::TouchEventType type);
    void onAction(Ref* pSender);
    
    void onDialog(const string& name);
    
    void onEnterTransitionDidFinish();
    
    //post
    void waitPostSuccessful(Ref* ref);
    void waitPostFaild(Ref* ref);
    void onPermissionGrantedResult(int requestCode,bool bGranted);
    
    void onPermissionGrantedResultForShare(int requestCode,bool bGranted);
    //override BaseScene function
    void addBackListener();
private:
    Node* _canvas;
    std::string m_sPostImage;
    PostingLayer*   m_pPostingLayer;
    Graph &     _graph;
    Layout *    _back;
    Text *      _inspiration;

    Layout*     _preview_bg;
    RateUsLayer*    _rateLayer;
};