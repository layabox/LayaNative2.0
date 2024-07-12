#include "../conch/JCConch.h"
#include <hilog/log.h>

using namespace laya;

class NapiHelper{
public:
    ~NapiHelper(){}
    static NapiHelper *GetInstance(){return &NapiHelper::help_;}
    std::string getDeviceInfo();
    int getNetworkType();
    void startVibration(float duration);
    void playBackgroundMusic(const char *p_sUrl,int p_nTimes,float nCurrentTime);
    void pauseBackgroundMusic();
    void stopBackgroundMusic();
    void setBackgroundMusicVolume(float p_nVolume);
    void resumeBackgroundMusic();
    void setCurrentTime(double nCurrentTime);
    double getCurrentTime();
    double getDuration();
    std::string getAppVersion();
    std::string getAppLocalVersion();

    void createVideoPlayer(int m_tag);
    void removeVideoPlayer(int m_tag);
    void setURL(int m_tag,std::string path,int isurl);
    void play(int m_tag);
    void pause(int m_tag);
    void stop(int m_tag);
    void setLooping(int m_tag,bool value);
    void setAutoplay(int m_tag,bool value);
    void setX(int m_tag,double value);
    void setY(int m_tag,double value);
    void setWidth(int m_tag,double value);
    void setHeight(int m_tag,double value);
    void setVideoCurrentTime(int m_tag,double value);
    void setVolume(int m_tag,double value);
    int getVolume(int m_tag);
    void showDialog(const char *p_sBuffer);
    void setKeepScreenOn(bool value);
    void setPreferredOrientation(int orientation);
    float getScreenInch();
    int getAvalidMem();
    int getUsedMem();
    void exitGame();
    void enableAccelerometer();
    void disableAccelerometer();
    void enableOrientation();
    void disableOrientation();
    std::string postMessageToUIThread(std::string eventName, std::string data);
    std::string postSyncMessageToUIThread(std::string eventName, std::string data);

private:
    std::string __getDeviceInfo();
    int __getNetworkType();
    void __startVibration(float duration);
    void __playBackgroundMusic(const char *p_sUrl,int p_nTimes,float nCurrentTime);
    void __pauseBackgroundMusic();
    void __stopBackgroundMusic();
    void __setBackgroundMusicVolume(float p_nVolume);
    void __resumeBackgroundMusic();
    void __setCurrentTime(double nCurrentTime);
    double __getCurrentTime();
    double __getDuration();
    std::string __getAppVersion();
    std::string __getAppLocalVersion();

    void __createVideoPlayer(int m_tag);
    void __removeVideoPlayer(int m_tag);
    void __setURL(int m_tag,std::string path,int isurl);
    void __play(int m_tag);
    void __pause(int m_tag);
    void __stop(int m_tag);
    void __setLooping(int m_tag,bool value);
    void __setAutoplay(int m_tag,bool value);
    void __setX(int m_tag,double value);
    void __setY(int m_tag,double value);
    void __setWidth(int m_tag,double value);
    void __setHeight(int m_tag,double value);
    void __setVideoCurrentTime(int m_tag,double value);
    void __setVolume(int m_tag,double value);
    int __getVolume(int m_tag);
    void __showDialog(const char *p_sBuffer);
    void __setKeepScreenOn(bool value);
    void __setPreferredOrientation(int orientation);
    float __getScreenInch();
    int __getAvalidMem();
    int __getUsedMem();
    void __exitGame();
    void __enableAccelerometer();
    void __disableAccelerometer();
    void __enableOrientation();
    void __disableOrientation();
    std::string __postMessageToUIThread(std::string eventName, std::string data);
    std::string __postSyncMessageToUIThread(std::string eventName, std::string data);

    static NapiHelper help_;

private:
    std::string deviceInfo;
    std::string appVersion;
    std::string appLocalVersion;
    std::string eventResult;
    std::string syncEventResult;
    int networkType;
    int volume;
    double currentTime;
    double duration;
    float screenInch;
    int avalidMem;
    int usedMem;

};