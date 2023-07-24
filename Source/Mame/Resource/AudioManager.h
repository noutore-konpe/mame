#pragma once

#include <wrl.h>
#include <memory>
#include <xaudio2.h>

#include "Audio.h"


enum class BGM
{
    temp,
    BGM_Max,
};

enum class SE
{
    temp,
    SE_Max,
};

class AudioManager
{
private:
    AudioManager()  = default;
    ~AudioManager() = default;

public:
    static AudioManager& Instance()
    {
        static AudioManager instance;
        return instance;
    }

    void LoadAudio();                               // âπäyì«Ç›çûÇ›

    // BGMçƒê∂
    void PlayBGM(
        const BGM& bgm, 
        const bool isLoop = false, 
        const bool isIgnoreQueue = false
    );    

    // SEçƒê∂
    void PlaySE(
        const SE& se, 
        const bool isLoop = false, 
        const bool isIgnoreQueue = false
    );       

    void StopBGM(const BGM& bgm);                   // BGMí‚é~
    void StopSE(const SE& se);                      // SEí‚é~

    void StopAllBGM();                              // ëSBGMí‚é~
    void StopAllSE();                               // ëSSEí‚é~
    void StopAllAudio();                            // ëSâπäyí‚é~

public:
    std::unique_ptr<Audio>& GetBGM(const BGM& bgm); //Å@BGMéÊìæ
    std::unique_ptr<Audio>& GetSE(const SE& se);    //  SEéÊìæ

public:
    // XAUDIO2
    Microsoft::WRL::ComPtr<IXAudio2> xAudio2     = nullptr;
    IXAudio2MasteringVoice*          masterVoice = nullptr;

private:
    std::unique_ptr<Audio> bgm_[static_cast<int>(BGM::BGM_Max)] = {};
    std::unique_ptr<Audio> se_[static_cast<int>(SE::SE_Max)]    = {};
};

