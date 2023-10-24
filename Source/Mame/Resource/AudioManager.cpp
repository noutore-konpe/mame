#include "AudioManager.h"

void AudioManager::LoadAudio()
{
    // 音楽読み込み
    {
        // BGM読み込み
        {            
            bgm_[static_cast<int>(BGM::Title)       ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/BGM/title.wav");
            bgm_[static_cast<int>(BGM::Result)      ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/BGM/Result.wav");
            bgm_[static_cast<int>(BGM::Death)       ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/BGM/Death.wav");
            bgm_[static_cast<int>(BGM::Enviroment)  ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/BGM/EnviromentalSound.wav");
            bgm_[static_cast<int>(BGM::Game)        ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/BGM/Game.wav");
            bgm_[static_cast<int>(BGM::Golem)       ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/BGM/Golem.wav");
        }

        // SE読み込み
        {
            se_[static_cast<int>(SE::GolemEntry)    ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/GolemEntry.wav");
            se_[static_cast<int>(SE::GolemRoar)     ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/GolemRoar.wav");
            se_[static_cast<int>(SE::WaveBegin)     ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/WaveBegin.wav");
            se_[static_cast<int>(SE::CardSelect0)   ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/CardSelect.wav");
            se_[static_cast<int>(SE::CardSelect1)   ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/CardSelect.wav");
            se_[static_cast<int>(SE::CardSelect2)   ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/CardSelect.wav");
            se_[static_cast<int>(SE::Enter)         ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/Enter.wav");
            se_[static_cast<int>(SE::SwordSlash1)   ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/SwordSlash1.wav");
            se_[static_cast<int>(SE::SwordSlash2)   ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/SwordSlash2.wav");
            se_[static_cast<int>(SE::SwordSlash3)   ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/SwordSlash3.wav");
            se_[static_cast<int>(SE::SwordSlash4)   ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/SwordSlash4.wav");
        }
    }

    // 音量調整
    {    
        // BGM音量調整
        {
            //bgm_[static_cast<int>(BGM::Title)           ]->Volume(0.75f);
        }

        // SE音量調整
        {
            se_[static_cast<int>(SE::Enter)         ]->Volume(1.5f);
        }
    }

}


void AudioManager::PlayBGM(const BGM& bgm, const bool isLoop, const bool isIgnoreQueue)
{
    bgm_[static_cast<int>(bgm)]->Play(isLoop, isIgnoreQueue);
}

void AudioManager::PlaySE(const SE& se, const bool isLoop, const bool isIgnoreQueue)
{
    se_[static_cast<int>(se)]->Play(isLoop, isIgnoreQueue);
}


void AudioManager::StopBGM(const BGM& bgm)
{
    bgm_[static_cast<int>(bgm)]->Stop();
}

void AudioManager::StopSE(const SE& se)
{
    se_[static_cast<int>(se)]->Stop();
}


std::unique_ptr<Audio>& AudioManager::GetBGM(const BGM& bgmIndex)
{
    return bgm_[static_cast<int>(bgmIndex)];
}

std::unique_ptr<Audio>& AudioManager::GetSE(const SE& seIndex)
{
    return se_[static_cast<int>(seIndex)];
}

// カード選択音
void AudioManager::PlayCardSelectSENum()
{
    switch (GetCardSelectSENum())
    {
    case 0:
        PlaySE(SE::CardSelect0);
        AddCardSelectSENum();
        break;
    case 1:
        PlaySE(SE::CardSelect1);
        AddCardSelectSENum();
        break;
    case 2:
        PlaySE(SE::CardSelect2);
        ResetCardSelectSENum();
        break;
    }
}


void AudioManager::StopAllBGM()
{
    for (std::unique_ptr<Audio>& bgm :bgm_)
    {
        bgm->Stop();
    }
}

void AudioManager::StopAllSE()
{
    for (std::unique_ptr<Audio>& se : se_)
    {
        se->Stop();
    }
}

void AudioManager::StopAllAudio()
{
    for (std::unique_ptr<Audio>& bgm : bgm_)
    {
        bgm->Stop();
    }
    for (std::unique_ptr<Audio>& se : se_)
    {
        se->Stop();
    }
}
