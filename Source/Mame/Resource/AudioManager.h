#pragma once

#include <wrl.h>
#include <memory>
#include <xaudio2.h>

#include "Audio.h"


enum class BGM
{
    Title,      // �^�C�g��BGM
    Result,     // ���U���gBGM
    Death,      // ���SBGM
    Enviroment, // ����
    Game,       // Game
    Golem,      // �S�[����
    BGM_Max,
};

enum class SE
{
    GolemEntry,     // �S�[�����o��
    GolemRoar,      // �S�[�������K
    WaveBegin,      // �E�F�[�u�J�n��
    
    CardSelect0,    // �\�͎�������̉�
    CardSelect1,    // �\�͎�������̉�
    CardSelect2,    // �\�͎�������̉�
    CardSelect3,    // �\�͎�������̉�
    
    Enter,          // �I��
    
    SwordSlash1_0,    // �U���P
    SwordSlash1_1,    // �U���P
    SwordSlash1_2,    // �U���P
    SwordSlash1_3,    // �U���P
    SwordSlash1_4,    // �U���P
    SwordSlash1_5,    // �U���P
    SwordSlash1_6,    // �U���P

    SwordSlash2_0,    // �U���Q
    SwordSlash2_1,    // �U���Q

    SwordSlash3,    // �U���R

    SwordSlash4_0,    // �U���S
    SwordSlash4_1,    // �U���S


    GolemWalk0,     // �S�[��������
    GolemWalk1,     // �S�[��������
    GolemWalk2,     // �S�[��������
    GolemWalk3,     // �S�[��������
    GolemWalk4,     // �S�[��������

    Hit_0, // �q�b�g��
    Hit_1, // �q�b�g��
    Hit_2, // �q�b�g��
    Hit_3, // �q�b�g��
    Hit_4, // �q�b�g��
    Hit_5, // �q�b�g��
    Hit_6, // �q�b�g��
    Hit_7, // �q�b�g��
    Hit_8, // �q�b�g��
    Hit_9, // �q�b�g��

    Finishing_0, // �Ƃǂ߂̃q�b�g��
    Finishing_1, // �Ƃǂ߂̃q�b�g��
    Finishing_2, // �Ƃǂ߂̃q�b�g��
    Finishing_3, // �Ƃǂ߂̃q�b�g��
    Finishing_4, // �Ƃǂ߂̃q�b�g��
    Finishing_5, // �Ƃǂ߂̃q�b�g��
    Finishing_6, // �Ƃǂ߂̃q�b�g��
    Finishing_7, // �Ƃǂ߂̃q�b�g��
    Finishing_8, // �Ƃǂ߂̃q�b�g��
    Finishing_9, // �Ƃǂ߂̃q�b�g��
    
    SlowMotion_0, // slowMotion
    SlowMotion_1, // slowMotion

    CounterBegin_0, // �J�E���^�[
    CounterBegin_1, // �J�E���^�[
    CounterBegin_2, // �J�E���^�[
    CounterBegin_3, // �J�E���^�[

    BlackHole_0, // �u���b�N�z�[��
    BlackHole_1, // �u���b�N�z�[��
    BlackHole_2, // �u���b�N�z�[��

    Laser_0,
    Laser_1,
    LaserRest_0,
    LaserRest_1,

    PlayerDash,

    GolemPanchi_0, // �S�[��������
    GolemPanchi_1, // �S�[��������
    GolemPanchi_2, // �S�[��������
    GolemPanchi_3, // �S�[��������
    GolemPanchi_4, // �S�[��������

    GolemEntry_0, // 
    GolemEntry_1, // 
    GolemEntry_2, // 

    GolemFinishing_0,
    GolemFinishing_1,
    GolemFinishing_2,

    GetExp_01,
    GetExp_02,
    GetExp_03,
    GetExp_04,
    GetExp_05,
    GetExp_06,
    GetExp_07,
    GetExp_08,
    GetExp_09,
    GetExp_10,
    GetExp_11,
    GetExp_12,
    GetExp_13,
    GetExp_14,
    GetExp_15,
    GetExp_16,
    GetExp_17,
    GetExp_18,
    GetExp_19,
    GetExp_20,

    BookShot_01,
    BookShot_02,
    BookShot_03,
    BookShot_04,
    BookShot_05,
    BookShot_06,
    BookShot_07,
    BookShot_08,
    BookShot_09,
    BookShot_10,
    BookShot_11,
    BookShot_12,
    BookShot_13,
    BookShot_14,
    BookShot_15,
    BookShot_16,
    BookShot_17,
    BookShot_18,
    BookShot_19,
    BookShot_20,


    LevelUp_0,
    LevelUp_1,
    LevelUp_2,

    GetSkillCard_0,
    GetSkillCard_1,
    GetSkillCard_2,

    SE_Max,
};

enum class SE_NAME
{
    CardSelect,
    GolemWalk,
    SwordSlash1,
    SwordSlash2,
    SwordSlash3,
    SwordSlash4,
    Hit,
    Finishing,//�Ƃǂ߂����������̃q�b�g��
    SlowMotion,
    CounterBegin,
    BlackHole,
    Laser,
    LaserRest,//���[�U�[�̔��ˌ�Đ������SE
    PlayerDash,

    GolemPanchi,
    GolemEntry,
    GolemFinishing,//�S�[�����ɂƂǂ߂��h�������̃q�b�g��


    GetExp,

    BookShot,

    LevelUp,
    GetSkillCard,

    Max,
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

    void LoadAudio();                               // ���y�ǂݍ���

    // BGM�Đ�
    void PlayBGM(
        const BGM& bgm, 
        const bool isLoop = true, 
        const bool isIgnoreQueue = false
    );    

    // SE�Đ�
    void PlaySE(
        const SE& se, 
        const bool isLoop = false, 
        const bool isIgnoreQueue = false
    );

    void StopBGM(const BGM& bgm);                   // BGM��~
    void StopSE(const SE& se);                      // SE��~

    void StopAllBGM();                              // �SBGM��~
    void StopAllSE();                               // �SSE��~
    void StopAllAudio();                            // �S���y��~

public:
    std::unique_ptr<Audio>& GetBGM(const BGM& bgm); //�@BGM�擾
    std::unique_ptr<Audio>& GetSE(const SE& se);    //  SE�擾

public:
    // XAUDIO2
    Microsoft::WRL::ComPtr<IXAudio2> xAudio2     = nullptr;
    IXAudio2MasteringVoice*          masterVoice = nullptr;

private:
    std::unique_ptr<Audio> bgm_[static_cast<int>(BGM::BGM_Max)] = {};
    std::unique_ptr<Audio> se_[static_cast<int>(SE::SE_Max)]    = {};


public:// ������Đ����邽�߂ɂ�����

    void PlaySE(SE_NAME who, SE startIndex, SE endIndex);

private:// ������Đ����邽�߂ɂ�����
    int countIndex[static_cast<UINT>(SE_NAME::Max)] = {};
};

