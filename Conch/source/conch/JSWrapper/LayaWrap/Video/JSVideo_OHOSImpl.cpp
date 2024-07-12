#ifdef OHOS
#include "JSVideo.h"
#include "aki/jsbind.h"
#include "util/Log.h"
#include "helper/NapiHelper.h"

namespace laya
{
    static int curIndex = 0;
    static std::unordered_map<int, JSVideo *> s_allVideoPlayers;
    static bool _isLooping = false;
    static bool _isAutoplay = false;
    static bool _isPause = false;
    static double valX = 0;
    static double valY = 0;
    static double valW = 0;
    static double valH = 0;
    static int readyState= 0;
    static int _duration= 0;
    static int _currentTime= 0;
    
    std::set<std::string> JSVideo::ms_supportFormat = {
        "video/mp4",
        "video/ogg",
    };
    JSVideo::JSVideo(){
        Init();
        //增加索引
        m_tag = curIndex;
        curIndex++;
        s_allVideoPlayers[m_tag] = this;
        AdjustAmountOfExternalAllocatedMemory(sizeof(JSVideo));
        JCMemorySurvey::GetInstance()->newClass("JSVideo",sizeof(JSVideo),this);
        NapiHelper::GetInstance()->createVideoPlayer(m_tag);
    }
    JSVideo::~JSVideo(){
        Dispose();
        JCMemorySurvey::GetInstance()->releaseClass("JSVideo",this);
        NapiHelper::GetInstance()->removeVideoPlayer(m_tag);
        auto iter = s_allVideoPlayers.find(m_tag);
        if(iter != s_allVideoPlayers.end()){
            s_allVideoPlayers.erase(iter);
        }
    }
    void JSVideo::_releaseHandler(){

    }   
    void JSVideo::LoadInternal(const std::string& path){
        int isurl =1;
        NapiHelper::GetInstance()->setURL(m_tag,path,isurl);
    } 
    void JSVideo::Play(){
        _isPause = false;
        NapiHelper::GetInstance()->play(m_tag);
    }
    void JSVideo::Pause(){
        _isPause = true;
        m_isDownloadWaitPlay = false;
        NapiHelper::GetInstance()->pause(m_tag);
    }
    void JSVideo::Stop(){
        NapiHelper::GetInstance()->stop(m_tag);
    }
    bool JSVideo::GetPaused(){
        return _isPause;
    }
    bool JSVideo::GetLoop(){
        return _isLooping;
    }
    void JSVideo::SetLoop(bool value){
        _isLooping = value;
        NapiHelper::GetInstance()->setLooping(m_tag,value);
    }
    void JSVideo::SetAutoplay(bool value){
        _isAutoplay = value;
        NapiHelper::GetInstance()->setAutoplay(m_tag,value);
    }
    bool JSVideo::GetAutoplay(){
       return _isAutoplay;
    }
    void JSVideo::SetX(double value){
         valX = value;
        NapiHelper::GetInstance()->setX(m_tag,value);
    }
    double JSVideo::GetX(){
        return valX;
    }
    void JSVideo::SetY(double value){
         valY = value;
        NapiHelper::GetInstance()->setY(m_tag,value);
    }
    double JSVideo::GetY(){
        return valY;
    }
    double JSVideo::GetVideoWidth(){
        auto iter = s_allVideoPlayers.find(m_tag);
        if(iter != s_allVideoPlayers.end()){
           return s_allVideoPlayers[m_tag]->GetWidth();
        }
        return 0;
    }
    double JSVideo::GetVideoHeight(){
        auto iter = s_allVideoPlayers.find(m_tag);
        if(iter != s_allVideoPlayers.end()){
           return s_allVideoPlayers[m_tag]->GetHeight();
        }
        return 0;
    }
    double JSVideo::GetWidth(){
        return valW;
    }
    void JSVideo::SetWidth(double value){
         valW = value;
        NapiHelper::GetInstance()->setWidth(m_tag,value);
    }
    double JSVideo::GetHeight(){
        return valH;
    }
    void JSVideo::SetHeight(double value){
         valH = value;
        NapiHelper::GetInstance()->setHeight(m_tag,value);
    }
    double JSVideo::GetCurrentTime(){
        return (double)(_currentTime*0.001);
    }
    void JSVideo::SetCurrentTime(double value){
        NapiHelper::GetInstance()->setVideoCurrentTime(m_tag,value);
    }
    double JSVideo::GetDuration(){
        return (double)(_duration * 0.001);
    }
    double JSVideo::GetVolume(){
        int ret = NapiHelper::GetInstance()->getVolume(m_tag);
        return (double)ret;
    }
    void JSVideo::SetVolume(double value){
        NapiHelper::GetInstance()->setVolume(m_tag,value);
    }
    int32_t JSVideo::GetReadyState(){
        return readyState;
    }
    void JSVideo::executeVideoCallback(int index,int event){
        readyState = event;
    }
    void JSVideo::executeLoadedmetadata(int index,const char* evtName){
        auto iter = s_allVideoPlayers.find(index);
        if(iter != s_allVideoPlayers.end()){
           s_allVideoPlayers[index]->CallHandle(evtName);
        }
    }
    void JSVideo::executeDurationBack(int index, int duration){
        _duration = duration;
    }
    void JSVideo::executeCurrentTimeBack(int index, int currentTime){
        _currentTime = currentTime;
    }
}
#endif //OHOS