/*
 * STSystemFunction.cpp
 *
 *  Created on: 2013-9-2
 *      Author: Steven.Xc.Tian
 */
#include "STSystemFunction.h"
#include "ST_SystemFunction_Android.h"

#include "platform/android/jni/JniHelper.h"
#define AppActivityPath "com.relaxedandrelaxed.comicbook/AppActivity"

using namespace cocos2d;

STSystemFunction::STSystemFunction()
{
}
STSystemFunction::~STSystemFunction()
{
}

void STSystemFunction::sendEmail(const char* subject, const char* body)
{
	SystemFunctionManager::getInstance()->sendEmail(subject, body);
}

void STSystemFunction::popAlertDialog(const char* message)
{
	SystemFunctionManager::getInstance()->popAlertDialog(message);
}

void STSystemFunction::makeToast(const char* message)
{
	SystemFunctionManager::getInstance()->makeToast(message);
}

void STSystemFunction::showMoreGame()
{
	SystemFunctionManager::getInstance()->showMoreGame();
}

bool STSystemFunction::isRate()
{
	return false;
}

void STSystemFunction::rating()
{
    if (!SystemFunctionManager::getInstance()->isRate()) {
        SystemFunctionManager::getInstance()->rating();
    }
}

void STSystemFunction::go2MarketDetail(std::string packagename)
{
	SystemFunctionManager::getInstance()->go2MarketDetail(packagename);
}

void STSystemFunction::contactUs()
{
	SystemFunctionManager::getInstance()->contactUs();
}

void STSystemFunction::sendEmailAndAssetPic(std::string subject, std::string message, std::string assetName)
{
	SystemFunctionManager::getInstance()->sendEmailAndAssetPic(subject, message, assetName);
}

void STSystemFunction::sendEmailAndFilePic(std::string subject, std::string message, std::string fileName)
{
	SystemFunctionManager::getInstance()->sendEmailAndFilePic(subject, message, fileName);

}

std::string STSystemFunction::getSdCardPath()
{
	return SystemFunctionManager::getInstance()->getSdCardPath();
}

bool STSystemFunction::checkNetworkAvailable()
{
	return SystemFunctionManager::getInstance()->checkNetworkAvailable();

}

bool STSystemFunction::isTabletAvailable()
{
	return SystemFunctionManager::getInstance()->isTabletAvailable();

}
float STSystemFunction::densityScale()
{
	return SystemFunctionManager::getInstance()->densityScale();
}
void STSystemFunction::rating(const char* appleID, const char* message)
{
	// just for iOS platform.
}

void STSystemFunction::go2PrivacyPage()
{
	// NOT implement yet.
    openURL("https://www.applabsinc.net/privacy/");
}

void STSystemFunction::openURL(std::string url)
{
	// NOT implement yet.
    JniMethodInfo methodInfo;
    
    bool isHave = JniHelper::getStaticMethodInfo(methodInfo,
                                                 AppActivityPath, "openURL", "(Ljava/lang/String;)V");
    
    if (isHave)
    {
        jstring jurl = methodInfo.env->NewStringUTF(url.c_str());
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jurl);
    }
}

void STSystemFunction::sendFireBase(std::string jsonString)
{

    JniMethodInfo methodInfo;
    
    bool isHave = JniHelper::getStaticMethodInfo(methodInfo,
                                                 AppActivityPath, "sendFireBase", "(Ljava/lang/String;)V");
    
    if (isHave)
    {
        jstring jurl = methodInfo.env->NewStringUTF(jsonString.c_str());
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jurl);
    }
}

void STSystemFunction::openGooglePlayStore()
{
    // NOT implement yet.
    JniMethodInfo info;
    
    bool ret = JniHelper::getStaticMethodInfo(info,AppActivityPath,"getInstance","()Ljava/lang/Object;");
    
    //先获得类的对象，然后用这个对象去调用它的非静态函数
    
    jobject jobj;

    if(ret)
        
    {
        jobj = info.env->CallStaticObjectMethod(info.classID,info.methodID);
    }
    //getMethodInfo判断java定义的类非静态函数是否存在，返回bool
    
    bool re = JniHelper::getMethodInfo(info,AppActivityPath,"openGooglePlayStore","()V");
    
    if(re)
    {
        info.env->CallVoidMethod(jobj,info.methodID);
    }
}

void STSystemFunction::showBanner()
{
    // NOT implement yet.
    JniMethodInfo info;
    
    bool ret = JniHelper::getStaticMethodInfo(info,AppActivityPath,"getInstance","()Ljava/lang/Object;");
    
    //先获得类的对象，然后用这个对象去调用它的非静态函数
    
    jobject jobj;
    
    if(ret)
        
    {
        jobj = info.env->CallStaticObjectMethod(info.classID,info.methodID);
    }
    //getMethodInfo判断java定义的类非静态函数是否存在，返回bool
    
    bool re = JniHelper::getMethodInfo(info,AppActivityPath,"showBanner","()V");
    
    if(re)
    {
        info.env->CallVoidMethod(jobj,info.methodID);
    }
}

void STSystemFunction::showFullScreen()
{
    // NOT implement yet.
    JniMethodInfo info;
    
    bool ret = JniHelper::getStaticMethodInfo(info,AppActivityPath,"getInstance","()Ljava/lang/Object;");
    
    //先获得类的对象，然后用这个对象去调用它的非静态函数
    
    jobject jobj;
    
    if(ret)
        
    {
        jobj = info.env->CallStaticObjectMethod(info.classID,info.methodID);
    }
    //getMethodInfo判断java定义的类非静态函数是否存在，返回bool
    
    bool re = JniHelper::getMethodInfo(info,AppActivityPath,"showFullScreen","()V");
    
    if(re)
    {
        info.env->CallVoidMethod(jobj,info.methodID);
    }
}

string STSystemFunction::getPlatformBundleID()
{
    // NOT implement yet.
    JniMethodInfo info;
    
    bool ret = JniHelper::getStaticMethodInfo(info,AppActivityPath,"getInstance","()Ljava/lang/Object;");
    
    //先获得类的对象，然后用这个对象去调用它的非静态函数
    
    jobject jobj;
    
    if(ret)
        
    {
        
        jobj = info.env->CallStaticObjectMethod(info.classID,info.methodID);
    }
    //getMethodInfo判断java定义的类非静态函数是否存在，返回bool
    
    bool re = JniHelper::getMethodInfo(info,AppActivityPath,"getBundleId","()Ljava/lang/String;");
    
    jstring str = (jstring)info.env->CallObjectMethod(jobj,info.methodID);

    return JniHelper::jstring2string(str);

}

string STSystemFunction::getPlatformVersion()
{
    JniMethodInfo info;
    
    bool ret = JniHelper::getStaticMethodInfo(info,AppActivityPath,"getInstance","()Ljava/lang/Object;");
    
    //先获得类的对象，然后用这个对象去调用它的非静态函数
    
    jobject jobj;
    
    if(ret)
        
    {
        
        jobj = info.env->CallStaticObjectMethod(info.classID,info.methodID);
    }
    //getMethodInfo判断java定义的类非静态函数是否存在，返回bool
    
    bool re = JniHelper::getMethodInfo(info,AppActivityPath,"getVersionName","()Ljava/lang/String;");
    
    jstring str = (jstring)info.env->CallObjectMethod(jobj,info.methodID);
    
    return JniHelper::jstring2string(str);
}

//void STSystemFunction::sendImageToInstagram(std::string fileName)
//{
//    
//}

void STSystemFunction::sendPictureToMore(std::string fileName)
{
    
}

void STSystemFunction::endSession()
{
	SystemFunctionManager::getInstance()->endSession();
}

void STSystemFunction::saveLocalImage(cocos2d::CCImage* m_image)
{

}

void STSystemFunction::refreshDCIM(std::string filename)
{
    SystemFunctionManager::getInstance()->refreshDCIM(filename);
}
