#pragma once

#include <DirectXMath.h>
#include <string>

#include "../../Taki174/Common.h"
#include "../Scene/SceneGame.h"


// �ԍ��ɑΉ�����Q�[�g�̈ʒu��Ԃ�(��-1�Ń����_���ȃQ�[�g�ʒu��Ԃ�)
inline DirectX::XMFLOAT3 GetGatewayPosition(/*NoConst*/ int gatewayIndex)
{
    // �Q�[�g�ԍ����ߏC��
    if (gatewayIndex > SceneGame::GATEWAY_COUNT_ - 1)
    {
        gatewayIndex = SceneGame::GATEWAY_COUNT_ - 1;
    }

    // 360�x���Q�[�g�����ɓ��������Ƃ��̊p�x
    static constexpr float angle = 360.0f / static_cast<float>(SceneGame::GATEWAY_COUNT_);

    // �ŏ��̈�񂾂�Y��]�l�e�[�u�����쐬����悤�ɂ���
    // �����ׂẴQ�[�g�ʒu���ЂƂЂƂ蓮�Őݒ肷��̂�
    // �@�ʓ|�Ȃ̂�for���Ŋe�Q�[�g����������Y��]�l�̃e�[�u����
    // �@�쐬���ăQ�[�g�ʒu��ݒ肷��
    static float rotationY_table[SceneGame::GATEWAY_COUNT_] = {};    // Y��]�l�e�[�u��(static)
    static bool  createRotationY_TableFlag                  = false; // Y��]�l�e�[�u���쐬�t���O(static)
    if (false == createRotationY_TableFlag)
    {
        // Y��]�l�e�[�u�����쐬
        for (int i = 0; i < SceneGame::GATEWAY_COUNT_; ++i)
        {
            rotationY_table[i] = ::ToRadian(angle * static_cast<float>(i));
        }

        // Y��]�l�e�[�u�����쐬����(�t���OON)
        createRotationY_TableFlag = true;
    }

    // Y��]�l���擾
    const int   rotationY_index = (gatewayIndex > -1) ? gatewayIndex : ::RandInt(0, SceneGame::GATEWAY_COUNT_);
    const float rotationY       = rotationY_table[rotationY_index];

    // �X�e�[�W�̒��S����Q�[�g�̉����炢�܂ł̂��������̔��a(����)
    static constexpr float RADIUS = 35.0f;

    // �Q�[�g�̈ʒu���擾
    DirectX::XMFLOAT3 gatewayPos = {};
    gatewayPos.x = SceneGame::stageCenter.x + ::sinf(rotationY) * RADIUS;
    gatewayPos.z = SceneGame::stageCenter.z + ::cosf(rotationY) * RADIUS;

    // �Q�[�g�̈ʒu��Ԃ�
    return gatewayPos;
}


// �E�F�[�u�֌W�̍\����
#pragma region Wave Struct
// �E�F�[�u�G�l�~�[�\����
struct WaveEnemySet
{
    WaveEnemySet() {}
    WaveEnemySet(
        const float              spawnTime,
        const std::string&       name,
        const DirectX::XMFLOAT3& pos,
        const float              hp,
        const float              atk,
        const int                dropExp)
        : spawnTime_(spawnTime)
        , name_(name)
        , pos_(pos)
        , hp_(hp)
        , atk_(atk)
        , dropExp_(dropExp)
        , isSpawned_(false) // �����t���O�̓f�t�H���g��false
    {}

    public:
    float             spawnTime_ = 0.0f;    // �o������
    std::string       name_      = "";      // ���O(�G�������̓G�N���X�����p)
    DirectX::XMFLOAT3 pos_       = {};      // �o���ʒu
    float             hp_        = 0.0f;    // �̗�
    float             atk_       = 0.0f;    // �U����
    int               dropExp_   = 0;       // ���Ƃ��o���l�̐�
    bool              isSpawned_ = false;   // ����������(�P�񂾂��������邽�߂̃t���O)

};

// �E�F�[�u�\����
struct Wave
{
    std::string    name_             = "";       // �E�F�[�u�̖��O
    std::string    note_             = "";       // �E�F�[�u�ɂ��Ă̔��l(�E�F�[�u�ɂ��ďڂ��������Ȃǂ����菑���p)
    size_t         spawnEnemyCount_  = 0;        // �o��������G�̑���(�E�F�[�u�G�l�~�[�z��̗v�f��)
    WaveEnemySet*  waveEnemySets_    = nullptr;  // �o��������G�̍\���̃|�C���^(�p�����[�^)

};

// �E�F�[�u�y�A�����g�\����
struct WaveParent
{
    size_t waveCount_    = 0;        // �E�F�[�u�̑���(�E�F�[�u�z��̗v�f��)
    Wave*  children_     = nullptr;  // �E�F�[�u�z��̃|�C���^
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
public:
    struct SpawnGolemPosition
    {
        DirectX::XMFLOAT3 position_;
        bool              isUsed_;
    };

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

    // �S�[���������ʒu�̈ʒu�g�p�ς݃t���O�����Z�b�g����
    void ResetSpawnGolemPositionIsUsedFlag();

public: // Get�ESet Function

    // ���݂̃E�F�[�u�ԍ����擾
    const size_t GetCurrentWaveIndex() const { return currentWaveIndex_; }

    // �E�F�[�u�ԍ��̖������擾
    const size_t GetWaveIndexEnd() const { return (waveParent_.waveCount_ - 1); }

    // ���݂̃E�F�[�u�̖��O���擾
    const std::string& GetCurrentWaveName() const { return waveParent_.children_->name_; }
    // ���݂̃E�F�[�u���o��������G�̑������擾
    const size_t GetCurrentWaveEnemyCount() const { return waveParent_.children_->spawnEnemyCount_; }

    // �S�[���������ʒu�擾(-1�ňʒu�z��̏ォ�珇�ԂɎ擾����)
    // ��Initialize�ƃE�F�[�u�G���h���X�̂Ƃ�����
    // �@�ʒu�g�p�ς݃t���O�̃��Z�b�g�������Ă�ł��Ȃ��̂�
    // �@�E�F�[�u�G���h���X�̂Ƃ��ȊO�ł�-1�͔񐄏�
    const DirectX::XMFLOAT3 GetSpawnGolemPosition(const int index);

public:
    static constexpr float BREAK_TIME_ = 2.5f;              // ���݂̃E�F�[�u���玟�̃E�F�[�u�Ɉڂ�܂ł̋x�e����
    static constexpr float ENDLESS_WAVE_CREATE_TIME_ = 1.0f;

    // �S�[�����𐶐�����ʒu�̐�
    static constexpr int SPAWN_GOLEM_POSITION_COUNT_ = 3;

private:
    WaveParent  waveParent_             = {};       // �E�F�[�u�̐e(�E�F�[�u���Ǘ�)
    float       waveTimer_              = 0.0f;     // �E�F�[�u�^�C�}�[
    int         currentWaveIndex_       = 0;        // ���݂̃E�F�[�u�ԍ�
    int         remainingEnemyCounter_  = 0;        // �c��̓G�̐�
    bool        breakTimeFlag_          = false;    // �x�e�t���O

    float       endlessWaveHp_          = 0.0f;
    float       endlessWaveAtk_         = 0.0f;
    float       endlessWaveExp_         = 0.0f;
    float       endlessGolemHp_         = 0.0f;
    float       endlessGolemExp_         = 0.0f;
    int         endlessWaveCounter_     = 0;        // �G���h���X�E�F�[�u���񂵂���
    int         endlessWaveCreateCount_ = 0;        // �G���h���X�E�F�[�u�̓G�𐶐����鐔
    bool        endlessWaveFlag_        = false;    // �G���h���X�E�F�[�u�t���O

    SpawnGolemPosition spawnGolemPositions_[SPAWN_GOLEM_POSITION_COUNT_] = {
        { SceneGame::stageCenter, false }, // �X�e�[�W�̒��S
        { SceneGame::stageCenter + DirectX::XMFLOAT3(-5.0f, 0.0f, 0.0f), false }, // ��
        { SceneGame::stageCenter + DirectX::XMFLOAT3(+5.0f, 0.0f, 0.0f), false }, // �E
    };
};