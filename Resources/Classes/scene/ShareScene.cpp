//
//  ShareScene.cpp
//  BabyPlay
//
//  Created by luoxp on 3/18/15.
//
//

#include "ShareScene.h"
#include "UICpp.h"
#include "GraphManager.hpp"
#include "STSystemFunction.h"
#include "ColorAdsManager.h"
#include "ChapterManager.hpp"
#include "STSystemFunction.h"
#include "STInstagramFunction.h"
#include <sys/stat.h>
#include "DownLoadPage.h"
#include "MyArtDataManager.hpp"
#include "RuntimePermissionManager.h"
#include "UIColorManager.hpp"
#include "SSCInternalLibManager.h"
#include "AnalyticsManager.hpp"
#include "BannerManager.hpp"

ShareScene::~ShareScene(){
    NotificationCenter::getInstance()->removeObserver(this, st_myPost_failed);
    NotificationCenter::getInstance()->removeObserver(this, st_myPost_success);
}

ShareScene::ShareScene():_graph(xChapter->getCurrentChapter().getCurrentGraph()){
    
    _rateLayer = nullptr;
    
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(ShareScene::waitPostSuccessful), st_myPost_success, NULL);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(ShareScene::waitPostFaild), st_myPost_failed, NULL);
}

bool ShareScene::init(){
    
    if(BaseScene::init())
    {
//        auto _root = (Layout*)(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("share.json"));
//        addChild(_root);
//        _root->cocos2d::Node::visit();
//        _root->setBackGroundColor(Color3B(255, 255, 255));
        
        this->setName("ShareScene");
        
        auto _root = CSLoader::createNode((V::isIpad()? "ShareScene_ipad.csb" : "ShareScene.csb"));
        _root->setContentSize(this->getContentSize());
        Helper::doLayout(_root);
        this->addChild(_root);
        _root->visit();
        
        auto back = static_cast<Button*>(Helper::seekNodeByName(_root, "btn_back"));
        back->addClickEventListener(CC_CALLBACK_1(ShareScene::onAction, this));
        
        auto home = static_cast<Button*>(Helper::seekNodeByName(_root, "btn_home"));
        home->addClickEventListener(CC_CALLBACK_1(ShareScene::onAction, this));
        
        auto share = static_cast<Button*>(Helper::seekNodeByName(_root, "btn_share"));
        share->addClickEventListener(CC_CALLBACK_1(ShareScene::onAction, this));
        
        auto post = static_cast<Button*>(Helper::seekNodeByName(_root, "btn_post"));
        post->addClickEventListener(CC_CALLBACK_1(ShareScene::onAction, this));
        
        
        if (V::isHighVersion())
        {
            auto titleLayout = static_cast<Text*>(Helper::seekNodeByName(_root, "Text_1"));
            titleLayout->setFontSize(titleLayout->getFontSize()*0.8);
            titleLayout->setScale(titleLayout->getScale()/0.8);
            
            auto titleLayout3 = static_cast<Text*>(Helper::seekNodeByName(_root, "Text_11"));
            titleLayout3->setFontSize(titleLayout3->getFontSize()*0.8);
            titleLayout3->setScale(titleLayout3->getScale()/0.8);
            
            auto titleLayout4 = static_cast<Text*>(Helper::seekNodeByName(_root, "Text_111"));
            titleLayout4->setFontSize(titleLayout4->getFontSize()*0.8);
            titleLayout4->setScale(titleLayout4->getScale()/0.8);

            auto titleLayout2 = static_cast<Text*>(Helper::seekNodeByName(_root, "TEXT_graph"));
            titleLayout2->setFontSize(titleLayout->getFontSize()*0.8);
            titleLayout2->setScale(titleLayout->getScale()/0.8);
        }
        
        
        back->setTouchEnabled(false);
        home->setTouchEnabled(false);
        this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([back, home](){
            back->setTouchEnabled(true);
            home->setTouchEnabled(true);
        }), NULL));
        
        
        auto pic_bg = static_cast<Layout*>(Helper::seekNodeByName(_root, "pic_bg"));
        _preview_bg = pic_bg;
        pic_bg->setScaleZ(V::getRealDesignScale());
        
        m_sPostImage = _graph.getHistoryName();
        
//        Texture2D* texture = new Texture2D();
//        texture->initWithImage(xGraph->getSaveImage());
        
//        auto preview = Sprite::createWithTexture(texture);

        auto preview = Sprite::create(m_sPostImage);
        preview->setPosition(pic_bg->getContentSize()/2);
        pic_bg->addChild(preview);
        preview->setScale(pic_bg->getContentSize().width/preview->getContentSize().width);
        
        xAnalytics->sendScreenEvent("ShareScene");
        
        ATMInstance->_bannerEventParas = "share_page";
        xBanner->show();
        
        DownLoadPage::m_bIsShareByInsp = true;

        return true;
    }

    return false;
}

void ShareScene::onEnterTransitionDidFinish()
{
    BaseScene::onEnterTransitionDidFinish();
    
    this->runAction(Sequence::create(DelayTime::create(0.2f), CallFunc::create([this](){
        SceneManager::Tag tag = xScene->getLastSceneTag();
        if (tag == SceneManager::Tag::GAME)
        {
            
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            STSystemFunction sf;
            
            sf.isShowSystemRateUs();
            
            string appVerison = sf.getPlatformVersion();
            string firstVerionNum = appVerison.substr(0, appVerison.find("."));
            
            string userName = "isFirstShowRateUs" + firstVerionNum;
            
            bool firstShowRateUs = UserDefault::getInstance()->getBoolForKey( userName.c_str(), true);
            if (firstShowRateUs && sf.isShowSystemRateUs())
            {
                UserDefault::getInstance()->setBoolForKey(userName.c_str(), false);
                sf.showSystemRateUs();
            }else
            {
                string xxxName = "isCloseRateUs" + appVerison;
                bool alreadyCloseRt = UserDefault::getInstance()->getBoolForKey(xxxName.c_str(), false);
                if (!alreadyCloseRt)
                {
                    if (!SSCInternalLibManager::getInstance()->hasRated())
                    {
                        RateUsLayer* layer = RateUsLayer::create();
                        this->addChild(layer, 102);
                        _rateLayer = layer;
                    }
                }

            }
            
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            if (!SSCInternalLibManager::getInstance()->hasRated())
            {
                RateUsLayer* layer = RateUsLayer::create();
                this->addChild(layer, 102);
                _rateLayer = layer;
            }
#endif
        }
    }), NULL));

}

void ShareScene::addBackListener()
{
    auto listener = EventListenerKeyboard::create();
    listener->onKeyReleased = [this](EventKeyboard::KeyCode code, Event* event)
    {
        if(code == EventKeyboard::KeyCode::KEY_BACK)
        {
            if (_rateLayer)
            {
                _rateLayer->removeFromParentAndCleanup(true);
                _rateLayer = nullptr;
                ATMInstance->sendEvent("rate_us", "rate_us_selection", "close");
            }else
            {
                _graph.setNew(false);
                SceneManager::Tag tag = xScene->getLastSceneTag();
                if (tag == SceneManager::Tag::GAME)
                {
                    xColorAds->setIsEnableShowInterstitialAds(false);
                    xScene->back(SceneManager::SceneAnimation::OUTLEFT);
                }else
                {
                    xScene->back(SceneManager::SceneAnimation::NONE);
                }
            }

        }
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void ShareScene::onAction(Ref* pSender)
{
    string name = ((Node*)pSender)->getName();
    
    if (name == "btn_back")
    {
        _graph.setNew(false);
        SceneManager::Tag tag = xScene->getLastSceneTag();
        if (tag == SceneManager::Tag::GAME)
        {
            xColorAds->setIsEnableShowInterstitialAds(false);
            xScene->back(SceneManager::SceneAnimation::OUTLEFT);
        }else
        {
            xScene->back(SceneManager::SceneAnimation::NONE);
        }
        
        
        ATMInstance->_interstitialEventParas = "back_button";
//        xColorAds->changeScene();
        
    }else if (name == "btn_home")
    {
//        xGraph->destorySaveImage();
//        xScene->back(SceneManager::Tag::MY_ARTS);
        xScene->forward(SceneManager::Tag::MY_ARTS);
        ATMInstance->_interstitialEventParas = "home_button";
//        xColorAds->changeScene();
        quickFlurry("btn_backtohome");
    
    }else if (name == "btn_share")
    {

//        auto saveImage = xGraph->getSaveImage();
//        auto texture = new Texture2D();
//        texture->initWithImage(saveImage);
//        texture->autorelease();
//        auto image = Sprite::createWithTexture(texture);
        auto image = Sprite::create(m_sPostImage);
        image->setPosition(512, 512);
        
        Vec2 lOrginPos = _preview_bg->getPosition();
        RenderTexture* lRenderTexture = RenderTexture::create(1024, 1024);
        lRenderTexture->begin();
        image->visit();
        lRenderTexture->end();
        
        Director::getInstance()->getRenderer()->render();
        
        STSystemFunction sf;
        Image* lImage = lRenderTexture->newImage();
        string lPath;
//        xGraph->setSaveImage(lImage);
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        lPath = FileUtils::getInstance()->getWritablePath() + "share_alabs0006.png";
        lImage->saveToFile(lPath, false);
        lImage->autorelease();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//        lPath = SSCInternalLibManager::getInstance()->getSDCardPath()+"/"+"share_alabs0006.png";
#endif
//        lImage->saveToFile(lPath, false);
//        lImage->autorelease();
        
        
        _preview_bg->setPosition(lOrginPos);
        
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        sf.sendPictureToMore(lPath.c_str());
        Director::getInstance()->getTextureCache()->removeTextureForKey(lPath.c_str());
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        //        STInstagramFunction st;
        //        st.sendImageToInstagram(lPath.c_str());
        
        RuntimePermissionManager::getInstance()->onPermissionGrantedResult = [&](int requestcode,bool bgranted){
            onPermissionGrantedResultForShare(requestcode, bgranted);
        };
        RuntimePermissionManager::getInstance()->requestRuntimePermissions(1, PERMISSION::kWriteExternalStorage);
        
#endif
        
        xColorAds->share();
        quickFlurry("share_share");
        
        map<string, string> map;
        map.insert(pair<string, string>("image_id", xChapter->getCurrentChapter().getCurrentGraph().name));
        map.insert(pair<string, string>("share_location", "share_page"));
        ATMInstance->sendEvent("image_share", map);
        
    }else if (name == "btn_save")
    {
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        STSystemFunction function = STSystemFunction();
        function.saveLocalImage(xGraph->getSaveImage());
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        RuntimePermissionManager::getInstance()->onPermissionGrantedResult = [&](int requestcode,bool bgranted){
            onPermissionGrantedResult(requestcode, bgranted);
        };
        RuntimePermissionManager::getInstance()->requestRuntimePermissions(1, PERMISSION::kWriteExternalStorage);
#endif
        
        xColorAds->share();
        quickFlurry("share_save");
        
    }else if (name == "btn_post")
    {

        if (!SSCInternalLibManager::getInstance()->checkNetworkAvailable())
        {
            NewDialog* dialog = NewDialog::create("network_err", "", "ok");
            dialog->addButtonListener(CC_CALLBACK_1(ShareScene::onDialog, this));
            this->addChild(dialog, 101);
            
            return;
        }
        
        PostingLayer* lSwallowLayer = PostingLayer::create(Color4B(0, 0, 0, 200));
        this->addChild(lSwallowLayer, 100);
        lSwallowLayer->setTouchEnable(true);
        m_pPostingLayer = lSwallowLayer;
        
        runAction(Sequence::create(DelayTime::create(0.1), CallFunc::create([&](){
            DownLoadPage::m_sCurrentPostImageName = _graph.getPreviewName();
            DownLoadPage::getInstance()->postImage(m_sPostImage);
            
        }), nullptr));
        
        map<string, string> map;
        map.insert(pair<string, string>("image_id", xChapter->getCurrentChapter().getCurrentGraph().name));
        map.insert(pair<string, string>("published_location", "share_page"));
        ATMInstance->sendEvent("image_publish", map);
    }
}

void ShareScene::onButton(Ref* pSender, Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED) return;
    
    string name = ((Node*)pSender)->getName();
    
    if(name == "back")
    {
        _graph.setNew(false);
        xScene->back(SceneManager::SceneAnimation::OUTLEFT);
        xColorAds->changeScene();
    }
    else if(name == "home")
    {
        xGraph->destorySaveImage();
        xScene->back(SceneManager::Tag::CHAPTER);
        xColorAds->changeScene();
        quickFlurry("btn_backtohome");
    }
    else if(name == "save")
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        STSystemFunction function = STSystemFunction();
        function.saveLocalImage(xGraph->getSaveImage());
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        RuntimePermissionManager::getInstance()->onPermissionGrantedResult = [&](int requestcode,bool bgranted){
            onPermissionGrantedResult(requestcode, bgranted);
        };
        RuntimePermissionManager::getInstance()->requestRuntimePermissions(1, PERMISSION::kWriteExternalStorage);
#endif
        
        xColorAds->share();
        quickFlurry("share_save");
    }
    else if (name == "share")
    {
        auto saveImage = xGraph->getSaveImage();
        auto texture = new Texture2D();
        texture->initWithImage(saveImage);
        texture->autorelease();
        auto image = Sprite::createWithTexture(texture);
        image->setPosition(512, 512);
        
        Vec2 lOrginPos = _preview_bg->getPosition();
        RenderTexture* lRenderTexture = RenderTexture::create(1024, 1024);
        lRenderTexture->begin();
        image->visit();
        lRenderTexture->end();
        
        Director::getInstance()->getRenderer()->render();
        
        STSystemFunction sf;
        Image* lImage = lRenderTexture->newImage();
        string lPath;
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        lPath = FileUtils::getInstance()->getWritablePath() + "share_alabs0006.png";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        lPath = SSCInternalLibManager::getInstance()->getSDCardPath()+"/"+"share_alabs0006.png";
#endif
        lImage->saveToFile(lPath, false);
        lImage->autorelease();
        
        Director::getInstance()->getTextureCache()->removeTextureForKey(lPath.c_str());
        _preview_bg->setPosition(lOrginPos);

        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        sf.sendPictureToMore(lPath.c_str());
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//        STInstagramFunction st;
//        st.sendImageToInstagram(lPath.c_str());

        SSCInternalLibManager::getInstance()->sendEmailAndFilePic("", "Just painted this myself. So cool and relaxing! Come join me! ", lPath.c_str());
#endif
        
        xColorAds->share();
        quickFlurry("share_share");
    }else if (name == "TXT_post_inspiration")
    {
        
        NewDialog* dialog = NewDialog::create("post to insprision", "later", "post");
        dialog->addButtonListener(CC_CALLBACK_1(ShareScene::onDialog, this));
        this->addChild(dialog, 101);
    }
}

void ShareScene::onDialog(const string& name)
{
    if (name == "post"){
//        _back->setVisible(false);
//        _inspiration->setVisible(false);
//        ((Node*)_inspiration->getUserData())->setVisible(false);
        
//        PostingLayer* lSwallowLayer = PostingLayer::create(Color4B(0, 0, 0, 200));
//        this->addChild(lSwallowLayer, 100);
//        lSwallowLayer->setTouchEnable(true);
//        m_pPostingLayer = lSwallowLayer;
//        
//        Label* label = Label::createWithTTF("Posting...", "fonts/Seravek.ttf", 50);
//        label->setPosition(Vec2(lSwallowLayer->getContentSize().width/2, lSwallowLayer->getContentSize().height/2));
//        lSwallowLayer->addChild(label);
//        
//        runAction(Sequence::create(DelayTime::create(0.1), CallFunc::create([&](){
//            DownLoadPage::m_sCurrentPostImageName = _graph.getPngName();
//            DownLoadPage::getInstance()->postImage(m_sPostImage);
//            
//        }), nullptr));
    }else if (name == "later")
    {
        if (m_pPostingLayer)
        {
            m_pPostingLayer->removeFromParentAndCleanup(true);
            m_pPostingLayer = NULL;
        }
    }else if (name == "tryAgain")
    {
        PostingLayer* lSwallowLayer = PostingLayer::create(Color4B(0, 0, 0, 200));
        this->addChild(lSwallowLayer, 100);
        lSwallowLayer->setTouchEnable(true);
        m_pPostingLayer = lSwallowLayer;
        
//        Label* label = Label::createWithTTF("POSTING...", "res/kalinga.ttf", 50);
//        label->setPosition(Vec2(lSwallowLayer->getContentSize().width/2, lSwallowLayer->getContentSize().height/2));
//        lSwallowLayer->addChild(label);
        
        runAction(Sequence::create(DelayTime::create(0.1), CallFunc::create([&](){
            
            DownLoadPage::m_sCurrentPostImageName = _graph.getPreviewName();
            DownLoadPage::getInstance()->postImage(m_sPostImage);
            
        }), nullptr));
    }else if (name == "go_ins")
    {
        xScene->forward(SceneManager::Tag::INSPIRATION);
    }else if (name == "postfail_ok")
    {
        Graph & graph = xChapter->getCurrentChapter().getCurrentGraph();
        graph.setNew(true);
        xScene->forward(SceneManager::Tag::GAME);
    }
}

void ShareScene::waitPostFaild(Ref* ref)
{
    
    if (m_pPostingLayer)
    {
        m_pPostingLayer->removeFromParentAndCleanup(true);
        m_pPostingLayer = NULL;
    }
    
    
    NewDialog* dialog = NewDialog::create("postFail", "later", "postfail_ok");
    dialog->addButtonListener(CC_CALLBACK_1(ShareScene::onDialog, this));
    this->addChild(dialog, 101);
    
}

void ShareScene::waitPostSuccessful(Ref* ref)
{
    if (m_pPostingLayer)
    {
        m_pPostingLayer->setPushSuccessed();
        m_pPostingLayer = NULL;
    }
    
//    xDialog->show("post to insprision", "later", "go_ins", CC_CALLBACK_1(ShareScene::onDialog, this));
//    xScene->forward(SceneManager::Tag::INSPIRATION);
    
    quickFlurry(string("insprision_") + _graph.name);
}


void ShareScene::onPermissionGrantedResult(int requestCode,bool bGranted) {
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
    if (requestCode == 1) {
        if (bGranted) {
            //add your code....
            log("-------->anroid runtime permisson was granted,requestcode = %d",requestCode);
            auto writePath = SSCInternalLibManager::getInstance()->getSDCardPath()+"/Color Book/";
            
            if(!FileUtils::sharedFileUtils()->isFileExist(writePath))
            {
                mode_t processMask = umask(0);
                int ret = mkdir(writePath.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
                umask(processMask);
                if (ret != 0 && (errno != EEXIST))
                    return;
            }
            
            struct timeval now;
            gettimeofday(&now, nullptr);
            std::string _time = StringUtils::format("%ld", (now.tv_sec * 1000 + now.tv_usec / 1000));
            std::string fullPath = writePath + _time + ".jpg";
            
            bool isSaveSuccess = xGraph->getSaveImage()->saveToFile(fullPath.c_str());
            if(isSaveSuccess)
                SSCInternalLibManager::getInstance()->makeToast("Saved to Camera Roll!");
            else
                SSCInternalLibManager::getInstance()->makeToast("Allow access to your album in \"Setting\"->\"Privacy\"->\"Photos\"");
            SSCInternalLibManager::getInstance()->refreshDCIM(fullPath);
        }else{
            //add your code....
            log("-------->anroid runtime permisson was not  granted,requestcode = %d",requestCode);
        }
    }
#endif
}

void ShareScene::onPermissionGrantedResultForShare(int requestCode,bool bGranted) {
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
    if (requestCode == 1) {
        if (bGranted) {
            //add your code....
            log("-------->anroid runtime permisson was granted,requestcode = %d",requestCode);
            auto writePath = SSCInternalLibManager::getInstance()->getSDCardPath()+ "/share_alabs0006.png";
            xGraph->getSaveImage()->saveToFile(writePath.c_str());
  
            SSCInternalLibManager::getInstance()->refreshDCIM(writePath);
            
            SSCInternalLibManager::getInstance()->sendEmailAndFilePic("", "Just painted this myself. So cool and relaxing! Come join me! ", writePath.c_str());
            Director::getInstance()->getTextureCache()->removeTextureForKey(writePath.c_str());
        }else{
            //add your code....
            log("-------->anroid runtime permisson was not  granted,requestcode = %d",requestCode);
        }
    }
#endif
}


