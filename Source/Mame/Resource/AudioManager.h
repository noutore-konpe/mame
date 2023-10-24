#pragma once

#include <wrl.h>
#include <memory>
#include <xaudio2.h>

#include "Audio.h"


enum class BGM
{
    Title,      // タイトルBGM
    Result,     // リザルトBGM
    Death,      // 死亡BGM
    Game,       // 環境音
    BGM_Max,
};

enum class SE
{
    GolemEntry,     // ゴーレム登場
    GolemRoar,      // ゴーレム咆哮
    WaveBegin,      // ウェーブ開始音
    CardSelect0,     // 能力取った時の音
    CardSelect1,     // 能力取った時の音
    CardSelect2,     // 能力取った時の音
    Enter,          // 選択
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

    void LoadAudio();                               // 音楽読み込み

    // BGM再生
    void PlayBGM(
        const BGM& bgm, 
        const bool isLoop = true, 
        const bool isIgnoreQueue = false
    );    

    // SE再生
    void PlaySE(
        const SE& se, 
        const bool isLoop = false, 
        const bool isIgnoreQueue = false
    );       

    void StopBGM(const BGM& bgm);                   // BGM停止
    void StopSE(const SE& se);                      // SE停止

    void StopAllBGM();                              // 全BGM停止
    void StopAllSE();                               // 全SE停止
    void StopAllAudio();                            // 全音楽停止

public:
    std::unique_ptr<Audio>& GetBGM(const BGM& bgm); //　BGM取得
    std::unique_ptr<Audio>& GetSE(const SE& se);    //  SE取得

public:
    // XAUDIO2
    Microsoft::WRL::ComPtr<IXAudio2> xAudio2     = nullptr;
    IXAudio2MasteringVoice*          masterVoice = nullptr;

private:
    std::unique_ptr<Audio> bgm_[static_cast<int>(BGM::BGM_Max)] = {};
    std::unique_ptr<Audio> se_[static_cast<int>(SE::SE_Max)]    = {};


public:// 何回も再生するためにいるやつ
    void PlayCardSelectSENum();

private:// 何回も再生するためにいるやつ
    void ResetCardSelectSENum() { cardSelectSENum = 0; }
    void AddCardSelectSENum() { ++cardSelectSENum; }
    int GetCardSelectSENum() { return cardSelectSENum; }
    int cardSelectSENum = 0;
};

