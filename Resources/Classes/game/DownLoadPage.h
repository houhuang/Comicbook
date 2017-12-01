//
//  DownloadPage.hpp
//  ColorGarden
//
//  Created by lidong on 15/10/15.
//
//

#ifndef DownloadPage_hpp
#define DownloadPage_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "../../cocos2d/cocos/network/HttpRequest.h"
#include "../../cocos2d/cocos/network/HttpClient.h"
//#include "network/HttpRequest.h"
//#include "network/HttpClient.h"

#define st_myPost_failed                        "st_myPost_failed"
#define st_myPost_success                       "st_myPost_success"
#define st_FavoriteSprite_getLikeNumber_suc     "st_FavoriteSprite_getLikeNumber_suc"
#define st_inspirationLayer_downLoadJson_fai    "st_inspirationLayer_downLoadJson_fai"
#define st_inspirationLayer_firstDownLoadJson_suc   "st_inspirationLayer_firstDownLoadJson_suc"
#define st_inspirationLayer_downLoadJson_suc    "st_inspirationLayer_downLoadJson_suc"
#define st_inspiration_downloadSuc              "st_inspiration_downloadSuc"
#define st_Inspiration_length                   "st_Inspiration_length"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace network;

typedef struct _versionCsvInfo
{
    std::string versionStr;
    std::string imageSizeStr;
    std::string packFullName;
    
}VersionCsvInfo;

typedef struct _InspirationInfo
{
    std::string imageUrl;
    long int    ID;
    int         likeNumber;

}InspirationInfo;

typedef struct _DownloadImageInfo
{
    std::string imageUrl;
    int imageID;
    
}DownloadImageInfo;

typedef struct _FavoriteInfo
{
    long int imageID;
    int likeNumber;
}FavoriteInfo;

class DownLoadPage  : public Layer, cocos2d::extension::AssetsManagerDelegateProtocol{
    
public:
    static DownLoadPage* getInstance();
    
public:
    DownLoadPage();
    ~DownLoadPage();

public:
    //post
    void postImage(const string& image);
    void postImageFromHttp(const string& image);
    static size_t reponsePostImage(uint8_t *dataBack, size_t size, size_t nmemb, void *user_p);
    
    //inspiration
    void downLoadInspirationInfo(size_t pIdx);
    void firstDownLoadInspirationInfo();
    bool createDirectory(const char *path);
    void downLoadImage(string url, int imageName);
    
    void startLoadImage();
    
    void postClickLike(int imageID);
    void postClickCancelLike(int imageID);
    void getLikeNumber(int imageID);
    
    void updateNeedGetLikeImage(int imageID);
    
    void saveInspirationDatas();
    void readInspitationDatas();
    void removeNullFile();
    
    void readClickLikeInfo();
    void saveClickLikeInfo();
    void saveMyArtDataTofile(const vector<list<string>>& lCsvData);

    void addNeedDownloadImage(DownloadImageInfo info);
    
    inline double bilinear(double a, double b, int uv, int u1v, int uv1, int u1v1)
    {
        return (double) (uv*(1-a)*(1-b)+u1v*a*(1-b)+uv1*b*(1-a)+u1v1*a*b);
    }
    void zoomdata_bilinear(unsigned char* pdst, int dw, int dh, unsigned char* psrc, int sw, int sh, int number);
private:
    std::string _pathToSave;
    std::vector<VersionCsvInfo>  _downloadPackageInfoVec;
    std::vector<VersionCsvInfo> _needDownVector;
    
    int _downloadCount;
    
    std::vector<AssetsManager*> _assertManagers;
    
    std::string m_sPostImageName;
    
public:
    string inspirationSavePath;
    static string m_sInspirationInfo;
    std::vector<InspirationInfo> m_vInspirationInfo;
    int m_iImageID;
    
    std::vector<int> m_vInsImageNameInfo;
    std::vector<int> m_vInsImageLikeNum;
    
    static bool m_bCurlIspostFaild;
    int _lastDownRequestID;
    bool _isRequestingAsk;
    
    std::vector<DownloadImageInfo> m_mNeedDownloadImage;
    bool  m_bIsFinishDownloadImage;
    
//    std::map<int, int> m_mFavoriteInfo;
    std::vector<FavoriteInfo> m_vFavoriteInfo;
    std::vector<int> m_vNeedGetLikeImage;
    bool m_bIsDownloadLike;
    int m_sGetLikeOfImageName;
    
    std::vector<int> m_vPostImageId;
    static int m_iCurrentPostImageId;
    
    int m_iCurrentDownloadIndex;
    
    static int m_iNextPageImageNumber;
    
    std::vector<int> m_vAlreadyClickLick;

    static string m_sCurrentPostImageName;
    static bool m_bIsShareByInsp;
    
    static string _postFailNeedNewOneName;
};

#endif /* DownloadPage_hpp */
