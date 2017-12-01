#include "AppDelegate.h"
#include "AppConfigs.h"
#include "AudioHelper.h"
#include "UICpp.h"
#include "audio/include/SimpleAudioEngine.h"
#include "GraphManager.hpp"
#include "MyArtDataManager.hpp"
#include "AdsLoadingLayer.h"
#include "AdsLoadingManager.h"
#include "ColorAdsManager.h"
#include "GameScene.h"
#include "DownLoadPage.h"
#include "DataPromotionManager.hpp"
#include "NewDownloadManager.hpp"
#include "OptimizationManager.hpp"
#include "SearchPathManager.hpp"
#include "Analytics.h"
#include "AnalyticsManager.hpp"

USING_NS_CC;
using namespace std;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
    
    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = cocos2d::GLViewImpl::create("ColorBook");
        director->setOpenGLView(glview);
    }
    

    log("%s",FileUtils::getInstance()->getWritablePath().c_str());
    
    OptimizationManager::getInstance()->clearInspirationResoure_deleteFile();
    //强制更新 API方式
//    DataPromotionManager::getInstance()->checkForcedUpdate();
    
    xDPM_getInstance->promotionToV2_0();
    xDPM_getInstance->promotionToV4_0();
    namespaceST::STVisibleRect::setupVisibleRect(director->getOpenGLView()->getFrameSize(), Size(640, 1136));
    
    SearchPathManager::getInstance()->updateSearchPath();
    
    Director::getInstance()->setProjection(Director::Projection::_2D);
    
    xMyArtData->readMyArtData();
    ATMInstance->sendOldDataToFireBase();
 
    xAdLoading->init(AdsLoadingLayer::create(false));

//    xScene->forward(SceneManager::Tag::LOADING);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    xScene->forward(SceneManager::Tag::CHAPTER);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    xScene->forward(SceneManager::Tag::LOADING);
#endif
    
    
    
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    
    xMyArtData->saveMyArtData();
    DownLoadPage::getInstance()->saveClickLikeInfo();
    
    auto scene = Director::getInstance()->getRunningScene();
    if (scene && scene->getName() == "GameScene") {
        
        if (!xColorAds->getshowCrossed())
        {
            ((GameScene*)scene)->applicationDidEnterBackground();
        }
    }
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();
    CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    
    xColorAds->setShowed(false);
}
