//
//  AudioHelper.h
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-9-25.
//
//

#ifndef __LVUP002__AudioHelper__
#define __LVUP002__AudioHelper__

#include "cocos2d.h"

#define xAudio          (AudioHelper::getInstance())

using namespace cocos2d;
using std::string;

class AudioHelper : public cocos2d::Ref
{
    AudioHelper();
    
    float mMusicVolume;
    float mSoundVolume;
    
    std::string preMusicName;
public:
    ~AudioHelper();
    
    static AudioHelper* getInstance();
    static void destroy();
    
public:
    //
    void changeMusicVolume(float volume);
    //
    void changeSoundVolume(float volume);
    //
    float getMusicVolume()
    {
        return mMusicVolume;
    };
    
    //
    float getSoundVolume()
    {
        return mSoundVolume;
    };
    
    //
    unsigned playDefaultSound();
    unsigned playSound(const string& sound, bool bLoop = false);
    void preloadSound(const char* pszFilePath);
    void unloadSound(const char* pszFilePath);
    void stopSound(unsigned int nSoundId);
    void stopAllSounds();
    void pauseSound(unsigned int nSoundId);
    void resumeSound(unsigned int nSoundId);
    //
    void playMusic(const char* music, bool bLoop = true);
    void stopMusic();
    void preloadMusic(const char* pszFilePath);
    //
    void reset();
    
    void switchMusic(bool on);
    void switchSound(bool on);
};

#endif /* defined(__LVUP002__AudioHelper__) */
