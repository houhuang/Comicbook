/*
 * ST_SystemFunction_Android.h
 *
 *  Created on: 2013-9-2
 *      Author: Steven.Xc.Tian
 */
#ifndef ST_SYSTEMFUNCTION_ANDROID_H_
#define ST_SYSTEMFUNCTION_ANDROID_H_

#include "ST_JNI_Helper.h"
#include <string>
using namespace std;
class SystemFunctionManager
{
public:
	SystemFunctionManager() :
			ClassSF(0),
			stSFJava(0),
			MethodSendMailByIntent(0),
			MethodPopAlertDialog(0),
			MethodMakeToast(0),
			MethodShowMoreGame(0),
			MethodGo2MarketDetail(0),
			MethodRating(0),
			MethodContactUs(0),
			MethodSendEmailAndAssetPic(0),
			MethodSendEmailAndFilePic(0),
			MethodGetSdCardPath(0),
			MethodCheckNetworkAvailable(0),
			MethodIsTabletAvailable(0),
			MethodDensityScale(0),
			MethodEndSession(0),
            MethodRefreshDCIM(0),
            //add by hh
            MethodIsRate(0)
	{
	}
	~SystemFunctionManager();

	static SystemFunctionManager* getInstance();

	static void destroy();

	bool init(JNIEnv * pEnv, jobject pJava);

public:
	void sendEmail(const char* subject, const char* body);
	void popAlertDialog(const char* message);
	void makeToast(const char* message);
	void showMoreGame();
	void go2MarketDetail(std::string packagename);
	void rating();
	void contactUs();
	void sendEmailAndAssetPic(std::string subject, std::string message, std::string assetName);
	void sendEmailAndFilePic(std::string subject, std::string message,std::string fileName);
	std::string getSdCardPath();
	bool checkNetworkAvailable();
	bool isTabletAvailable();
	float densityScale();
	void endSession();
    void refreshDCIM(std::string filename);
    //add by hh
    bool isRate();
    
    void openURL(std::string url);
    void sendFireBase(std::string jsonString);
    void go2PrivacyPage();
private:
	/*>>>>>>>> below declaration is used by jni <<<<<<<<*/
	// Cached Classes.
	jclass ClassSF;
	// Cached java object
	jobject stSFJava;
	// Cached active Methods.
	jmethodID MethodSendMailByIntent;
	jmethodID MethodPopAlertDialog;
	jmethodID MethodMakeToast;
	jmethodID MethodShowMoreGame;
	jmethodID MethodRating;
	jmethodID MethodGo2MarketDetail;
	jmethodID MethodContactUs;
	jmethodID MethodSendEmailAndAssetPic;
	jmethodID MethodSendEmailAndFilePic;
	jmethodID MethodGetSdCardPath;
	jmethodID MethodCheckNetworkAvailable;
	jmethodID MethodIsTabletAvailable;
	jmethodID MethodDensityScale;
	jmethodID MethodEndSession;
    jmethodID MethodRefreshDCIM;
    //add by hh
    jmethodID MethodIsRate;
};

#endif /* ST_SYSTEMFUNCTION_ANDROID_H_ */
