#include "AudioManager.h"

void AudioManager::LoadAudio()
{
    // ���y�ǂݍ���
    {
        // BGM�ǂݍ���
        {            
            bgm_[static_cast<int>(BGM::Title)       ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/BGM/title.wav");
            bgm_[static_cast<int>(BGM::Result)      ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/BGM/Result.wav");
            bgm_[static_cast<int>(BGM::Death)       ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/BGM/Death.wav");
            bgm_[static_cast<int>(BGM::Enviroment)  ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/BGM/EnviromentalSound.wav");
            bgm_[static_cast<int>(BGM::Game)        ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/BGM/Game.wav");
            bgm_[static_cast<int>(BGM::Golem)       ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/BGM/Golem.wav");
        }

        // SE�ǂݍ���
        {
            se_[static_cast<int>(SE::GolemEntry)    ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/GolemEntry.wav");
            se_[static_cast<int>(SE::GolemRoar)     ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/GolemRoar.wav");
            se_[static_cast<int>(SE::WaveBegin)     ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/WaveBegin.wav");
            se_[static_cast<int>(SE::CardSelect0)   ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/CardSelect.wav");
            se_[static_cast<int>(SE::CardSelect1)   ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/CardSelect.wav");
            se_[static_cast<int>(SE::CardSelect2)   ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/CardSelect.wav");
            se_[static_cast<int>(SE::CardSelect3)   ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/CardSelect.wav");
            se_[static_cast<int>(SE::Enter)         ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/Enter.wav");
            se_[static_cast<int>(SE::SwordSlash1_0) ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/SwordSlash1.wav");
            se_[static_cast<int>(SE::SwordSlash1_1) ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/SwordSlash1.wav");
            se_[static_cast<int>(SE::SwordSlash1_2) ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/SwordSlash1.wav");
            se_[static_cast<int>(SE::SwordSlash1_3) ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/SwordSlash1.wav");
            se_[static_cast<int>(SE::SwordSlash1_4) ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/SwordSlash1.wav");
            se_[static_cast<int>(SE::SwordSlash1_5) ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/SwordSlash1.wav");
            se_[static_cast<int>(SE::SwordSlash1_6) ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/SwordSlash1.wav");
            se_[static_cast<int>(SE::SwordSlash2_0) ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/SwordSlash2.wav");
            se_[static_cast<int>(SE::SwordSlash2_1) ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/SwordSlash2.wav");
            se_[static_cast<int>(SE::SwordSlash3)   ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/SwordSlash3.wav");
            se_[static_cast<int>(SE::SwordSlash4_0) ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/SwordSlash4.wav");
            se_[static_cast<int>(SE::SwordSlash4_1) ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/SwordSlash4.wav");
            se_[static_cast<int>(SE::GolemWalk0)    ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/GolemWalk.wav");
            se_[static_cast<int>(SE::GolemWalk1)    ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/GolemWalk.wav");
            se_[static_cast<int>(SE::GolemWalk2)    ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/GolemWalk.wav");
            se_[static_cast<int>(SE::GolemWalk3)    ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/GolemWalk.wav");
            se_[static_cast<int>(SE::GolemWalk4)    ] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/GolemWalk.wav");
            
            se_[static_cast<int>(SE::Hit_0)] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/Hit.wav");
            se_[static_cast<int>(SE::Hit_1)] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/Hit.wav");
            se_[static_cast<int>(SE::Hit_2)] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/Hit.wav");
            se_[static_cast<int>(SE::Hit_3)] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/Hit.wav");
            se_[static_cast<int>(SE::Hit_4)] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/Hit.wav");
            se_[static_cast<int>(SE::Hit_5)] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/Hit.wav");
            se_[static_cast<int>(SE::Hit_6)] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/Hit.wav");
            se_[static_cast<int>(SE::Hit_7)] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/Hit.wav");
            se_[static_cast<int>(SE::Hit_8)] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/Hit.wav");
            se_[static_cast<int>(SE::Hit_9)] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/Hit.wav");
            
            se_[static_cast<int>(SE::SlowMotion_0)] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/SlowMotion.wav");
            se_[static_cast<int>(SE::SlowMotion_1)] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/SlowMotion.wav");
            
            se_[static_cast<int>(SE::CounterBegin_0)] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/CounterBegin.wav");
            se_[static_cast<int>(SE::CounterBegin_1)] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/CounterBegin.wav");
            se_[static_cast<int>(SE::CounterBegin_2)] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/CounterBegin.wav");
            se_[static_cast<int>(SE::CounterBegin_3)] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/CounterBegin.wav");
            
            se_[static_cast<int>(SE::BlackHole_0)] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/BlackHole.wav");
            se_[static_cast<int>(SE::BlackHole_1)] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/BlackHole.wav");
            se_[static_cast<int>(SE::BlackHole_2)] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/BlackHole.wav");
            
            se_[static_cast<int>(SE::Laser_0)] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/laser.wav");
            se_[static_cast<int>(SE::Laser_1)] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/laser.wav");

            se_[static_cast<int>(SE::PlayerDash)] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/PlayerDash.wav");
            
            se_[static_cast<int>(SE::GolemPanchi_0)] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/GolemPanchi.wav");
            se_[static_cast<int>(SE::GolemPanchi_1)] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/GolemPanchi.wav");
            se_[static_cast<int>(SE::GolemPanchi_2)] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/GolemPanchi.wav");
            se_[static_cast<int>(SE::GolemPanchi_3)] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/GolemPanchi.wav");
            se_[static_cast<int>(SE::GolemPanchi_4)] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/GolemPanchi.wav");
            
            se_[static_cast<int>(SE::GolemEntry_0)] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/GolemPanchi.wav");
            se_[static_cast<int>(SE::GolemEntry_1)] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/GolemPanchi.wav");
            se_[static_cast<int>(SE::GolemEntry_2)] = std::make_unique<Audio>(xAudio2.Get(), L"./resources/audio/SE/GolemPanchi.wav");
        }
    }

    // ���ʒ���
    {    
        // BGM���ʒ���
        {
            bgm_[static_cast<int>(BGM::Title)       ]->Volume(0.35f);
            bgm_[static_cast<int>(BGM::Game)        ]->Volume(0.3f);
            bgm_[static_cast<int>(BGM::Golem)       ]->Volume(0.35f);
        }

        // SE���ʒ���
        {
            se_[static_cast<int>(SE::Enter)         ]->Volume(1.5f);
            se_[static_cast<int>(SE::GolemWalk0)    ]->Volume(2.0f);
            se_[static_cast<int>(SE::GolemWalk1)    ]->Volume(2.0f);
            se_[static_cast<int>(SE::GolemWalk2)    ]->Volume(2.0f);
            se_[static_cast<int>(SE::GolemWalk3)    ]->Volume(2.0f);
            se_[static_cast<int>(SE::GolemWalk4)    ]->Volume(2.0f);

            se_[static_cast<int>(SE::SwordSlash1_0) ]->Volume(0.4f);
            se_[static_cast<int>(SE::SwordSlash1_1) ]->Volume(0.4f);
            se_[static_cast<int>(SE::SwordSlash1_2) ]->Volume(0.4f);
            se_[static_cast<int>(SE::SwordSlash1_3) ]->Volume(0.4f);
            se_[static_cast<int>(SE::SwordSlash1_4) ]->Volume(0.4f);
            se_[static_cast<int>(SE::SwordSlash1_5) ]->Volume(0.4f);
            se_[static_cast<int>(SE::SwordSlash1_6) ]->Volume(0.4f);
                                                                
            se_[static_cast<int>(SE::SwordSlash2_0) ]->Volume(0.4f);
            se_[static_cast<int>(SE::SwordSlash2_1) ]->Volume(0.4f);

            se_[static_cast<int>(SE::SwordSlash3) ]->Volume(0.4f);

            se_[static_cast<int>(SE::SwordSlash4_0) ]->Volume(0.2f);
            se_[static_cast<int>(SE::SwordSlash4_1) ]->Volume(0.2f);

            se_[static_cast<int>(SE::SlowMotion_0) ]->Volume(1.0f);
            se_[static_cast<int>(SE::SlowMotion_1) ]->Volume(1.0f);

            se_[static_cast<int>(SE::CounterBegin_0) ]->Volume(0.1f);
            se_[static_cast<int>(SE::CounterBegin_1) ]->Volume(0.1f);
            se_[static_cast<int>(SE::CounterBegin_2) ]->Volume(0.1f);
            se_[static_cast<int>(SE::CounterBegin_3) ]->Volume(0.1f);

            se_[static_cast<int>(SE::BlackHole_0) ]->Volume(1.3f);
            se_[static_cast<int>(SE::BlackHole_1) ]->Volume(1.3f);
            se_[static_cast<int>(SE::BlackHole_2) ]->Volume(1.3f);

            se_[static_cast<int>(SE::Laser_0) ]->Volume(1.0f);
            se_[static_cast<int>(SE::Laser_1) ]->Volume(1.0f);
            
            se_[static_cast<int>(SE::Hit_0) ]->Volume(0.1f);
            se_[static_cast<int>(SE::Hit_1) ]->Volume(0.1f);
            se_[static_cast<int>(SE::Hit_2) ]->Volume(0.1f);
            se_[static_cast<int>(SE::Hit_3) ]->Volume(0.1f);
            se_[static_cast<int>(SE::Hit_4) ]->Volume(0.1f);
            se_[static_cast<int>(SE::Hit_5) ]->Volume(0.1f);
            se_[static_cast<int>(SE::Hit_6) ]->Volume(0.1f);
            se_[static_cast<int>(SE::Hit_7) ]->Volume(0.1f);
            se_[static_cast<int>(SE::Hit_8) ]->Volume(0.1f);
            se_[static_cast<int>(SE::Hit_9) ]->Volume(0.1f);

            se_[static_cast<int>(SE::PlayerDash) ]->Volume(0.5f);

            se_[static_cast<int>(SE::GolemPanchi_0)]->Volume(1.0f);
            se_[static_cast<int>(SE::GolemPanchi_1)]->Volume(1.0f);
            se_[static_cast<int>(SE::GolemPanchi_2)]->Volume(1.0f);
            se_[static_cast<int>(SE::GolemPanchi_3)]->Volume(1.0f);
            se_[static_cast<int>(SE::GolemPanchi_4)]->Volume(1.0f);

            se_[static_cast<int>(SE::GolemEntry_0)]->Volume(1.0f);
            se_[static_cast<int>(SE::GolemEntry_1)]->Volume(1.0f);
            se_[static_cast<int>(SE::GolemEntry_2)]->Volume(1.0f);
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


void AudioManager::PlaySE(SE_NAME who, SE startIndex, SE endIndex)
{
    // �Đ�������SE�̎��
    int currentIndex = countIndex[static_cast<UINT>(who)];

    // �Đ�����SE�̔z��擪
    int StartIndex = static_cast<UINT>(startIndex);

    // ������Đ�����SE�̏ꏊ���o��
    int playIndex = StartIndex + currentIndex;

    // SE�Đ�
    se_[playIndex]->Play(false);

    // ������SE�z��̍Ō��
    int EndIndex = static_cast<UINT>(endIndex);

    // ������SE�̐����擾
    int IndexCount = EndIndex - StartIndex;

    // ���ɍĐ�����ԍ��ɐݒ肷��
    if (currentIndex >= IndexCount)
    {// �ő�܂ōs������ŏ��ɖ߂�
        countIndex[static_cast<UINT>(who)] = 0;
    }
    else
    {// �܂���������ꍇ�͎���ݒ肷��
        ++currentIndex;
        countIndex[static_cast<UINT>(who)] = currentIndex;
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
