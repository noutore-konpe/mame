#pragma once

#include <DirectXMath.h>
#include <string>

#include "../../Taki174/Common.h"
#include "../Scene/SceneGame.h"


// �ԍ��ɑΉ�����Q�[�g�̈ʒu��Ԃ�
inline DirectX::XMFLOAT3 GetGatewayPosition(/*NoConst*/ int gatewayIndex)
{
    static constexpr int gatewayCount = 10; // �Q�[�g�̐�

    // �Q�[�g�ԍ����ߏC��
    if (gatewayIndex > gatewayCount) gatewayIndex = gatewayCount;

    // �ŏ��̈�񂾂��ʒu�e�[�u�����쐬����悤�ɂ���
    // �����ׂẴQ�[�g�ʒu���ЂƂЂƂ蓮�Őݒ肷��̂�
    // �@�ʓ|�Ȃ̂�for���ŃQ�[�g����������Y��]�l�̃e�[�u����
    // �@�쐬���ăQ�[�g�ʒu��ݒ肷��
    static float rotationY_table[gatewayCount] = {};    // Y��]�l�e�[�u��(static)
    static bool  createRotationY_TableFlag     = false; // Y��]�l�e�[�u���쐬�t���O(static)
    if (false == createRotationY_TableFlag)
    {
        // 360�x���Q�[�g�����ɓ��������Ƃ��̊p�x
        static constexpr float angle = 360.0f / static_cast<float>(gatewayCount);

        // Y��]�l�e�[�u�����쐬
        for (int i = 0; i < gatewayCount; ++i)
        {
            rotationY_table[i] = ::ToRadian(angle * static_cast<float>(i));
        }

        // Y��]�l�e�[�u�����쐬����(�t���OON)
        createRotationY_TableFlag = true;
    }

    // Y��]�l���擾
    const int   rotationY_index = (gatewayIndex > -1) ? gatewayIndex : ::RandInt(0, gatewayCount);
    const float rotationY       = rotationY_table[rotationY_index];

    // �X�e�[�W�̒��S����Q�[�g�̉����炢�܂ł̂��������̔��a(����)
    static constexpr float RADIUS = 35.0f;

    // �Q�[�g��XZ�ʒu�ݒ�
    DirectX::XMFLOAT3 gatewayPos = {};
    gatewayPos.x = SceneGame::stageCenter.x + ::sinf(rotationY) * RADIUS;
    gatewayPos.z = SceneGame::stageCenter.z + ::cosf(rotationY) * RADIUS;

    // �Q�[�g��XZ�ʒu��Ԃ�
    return gatewayPos;
}


// �E�F�[�u�֌W�̍\����
#pragma region Wave Struct
// �E�F�[�u�G�l�~�[�\����
struct WaveEnemySet
{
    // �z������Ƃ��ɂǂ̒l�����̒l���w���Ă���̂�
    // �킩��₷���悤�ɃR���X�g���N�^���쐬���Ă���
    WaveEnemySet(
        const float              spawnTime,
        const std::string&       name,
        const DirectX::XMFLOAT3& pos,
        const float              hp,
        const int                dropExp)
        : spawnTime_(spawnTime)
        , name_(name)
        , pos_(pos)
        , hp_(hp)
        , dropExp_(dropExp)
        , isSpawned_(false) // �����t���O�̓f�t�H���g��false
    {}

    float             spawnTime_ = 0.0f;    // �o������
    std::string       name_      = "";      // ���O(�G�������̓G�N���X�����p)
    DirectX::XMFLOAT3 pos_       = {};      // �o���ʒu
    float             hp_        = 0.0f;    // �̗�
    int               dropExp_   = 0;       // ���Ƃ��o���l�̐�
    bool              isSpawned_ = false;   // ����������(�P�񂾂��������邽�߂̃t���O)

};

// �E�F�[�u�\����
struct Wave
{
    std::string name_            = "";       // �E�F�[�u�̖��O
    std::string note_            = "";       // �E�F�[�u�ɂ��Ă̔��l(�E�F�[�u�ɂ��ďڂ��������Ȃǂ����菑���p)
    int         spawnEnemyCount_ = 0;        // �o��������G�̑���(�E�F�[�u�G�l�~�[�z��̗v�f��)
    WaveEnemySet*  waveEnemy_       = nullptr;  // �o��������G�̍\���̃|�C���^(�p�����[�^)

};

// �E�F�[�u�y�A�����g�\����
struct WaveParent
{
    int   waveCount_    = 0;        // �E�F�[�u�̑���(�E�F�[�u�z��̗v�f��)
    Wave* children_     = nullptr;  // �E�F�[�u�z��̃|�C���^
};
#pragma endregion


// �E�F�[�u�ݒ�
// ���v�f���̎擾��ύX�Ȃǂ��s���Ƃ��ɃN���X���Œ�`���Ă���Ɩʓ|�Ȃ̂ŊO�Œ�`
#pragma region Wave Setting
// WaveEnemySet
extern WaveEnemySet waveEnemy1[];
// Wave
extern Wave wave1_;
// Wave Array
extern Wave waves_[];
#pragma endregion


class WaveManager
{
private:
    WaveManager()  = default;
    ~WaveManager() = default;

public:
    static WaveManager& Instance()
    {
        static WaveManager instance;
        return instance;
    }

    void InitWave(const int waveindex = -1);    // �E�F�[�u������(�����̔ԍ��Ŏw��̃E�F�[�u����n�߂�(���f�o�b�O���̂݋@�\))
    void UpdateWave(const float elapsedTime);   // �E�F�[�u�X�V
    void DrawDebug();

    void ResetWaveEnemySpawnFlag();             // ���ׂẴE�F�[�u�̓G�����t���O�����Z�b�g

    void SpawnEnemy(WaveEnemySet* waveEnemy);      // �G����

    // �c��̓G�J�E���^�[���P���炷
    void ReduceRemainingEnemyCounter() { --remainingEnemyCounter_; }

public: // Get�ESet Function

    // ���݂̃E�F�[�u�ԍ����擾
    const int GetCurrentWaveIndex() const { return currentWaveIndex_; }

    // �E�F�[�u�ԍ��̖������擾
    const int GetWaveIndexEnd() const { return (waveParent_.waveCount_ - 1); }

    // ���݂̃E�F�[�u�̖��O���擾
    const std::string& GetCurrentWaveName() const { return waveParent_.children_->name_; }
    // ���݂̃E�F�[�u���o��������G�̑������擾
    const int GetCurrentWaveEnemyCount() const { return waveParent_.children_->spawnEnemyCount_; }

private:
    static constexpr float BREAK_TIME = 5.0f; // ���݂̃E�F�[�u���玟�̃E�F�[�u�Ɉڂ�܂ł̋x�e����

private:
    WaveParent  waveParent_             = {};       // �E�F�[�u�̐e(�E�F�[�u���Ǘ�)
    float       waveTimer_              = 0.0f;     // �E�F�[�u�^�C�}�[
    int         currentWaveIndex_       = 0;        // ���݂̃E�F�[�u�ԍ�
    int         remainingEnemyCounter_  = 0;        // �c��̓G�̐�
    bool        breakTimeFlag_          = false;    // �x�e�t���O
};