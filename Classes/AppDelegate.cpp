#include "AppDelegate.h"
#include "DownloadManager.h"
#include "HomeScene.h"
#include "SearchPathManager.h"
#include "STVisibleRect.h"
#include "ReadScene.h"
#include "CartoonManager.h"
#include "STSystemFunction.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

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

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {

        glview = GLViewImpl::create("ComicBook");
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    V::setupVisibleRect(director->getOpenGLView()->getFrameSize(), Size(640, 1136));
    SearchPathManager::getInstance()->updateSearchPath();
    
    register_all_packages();
    log("%s",FileUtils::getInstance()->getWritablePath().c_str());
    
    
    xDownload->downloadCarttonCsv();


    xCartoon->setFirstInGame();
    
    xCartoon->readCategoryCsv();
    
    director->runWithScene(HomeScene::create());

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
    
    Scene* lScene = dynamic_cast<Scene*>(Director::getInstance()->getRunningScene());
    if (lScene)
    {
        if (lScene->getName() == "ReadScene")
        {
            ReadScene* scene = (ReadScene*)Director::getInstance()->getRunningScene();
            if (scene)
            {
                scene->saveCurrentPage();
            }
        }
    }

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
