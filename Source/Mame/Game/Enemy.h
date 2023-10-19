#pragma once

#include "../../Taki174/OperatorXMFloat3.h"
#include "Character.h"
#include "BehaviorTree.h"
#include "BehaviorData.h"
#include "NodeBase.h"

class Enemy : public Character
{
public:
    enum class TYPE
    {
        Normal,
        Golem,
    };

public:
    Enemy() {}
    virtual ~Enemy();

    virtual void Initialize();                                          // ������
    virtual void Finalize() = 0;                                        // �I����
    virtual void Begin()    = 0;                                           // ���t���[����ԍŏ��ɌĂ΂��
    virtual void Update(const float& elapsedTime);                      // �X�V����
    virtual void End()      = 0;                                             // ���t���[����ԍŌ�ɌĂ΂��
    //virtual void Render(const float& elapsedTime, const float& scale);  // �`�揈��
    virtual void Render(const float& scale, ID3D11PixelShader* psShader);  // �`�揈��
    virtual void DrawDebug() = 0;                                       // �f�o�b�O�`��

    virtual void Render(const float& scale, bool shadow, ID3D11PixelShader* psShader = nullptr) {}

    virtual int GetCurrentState() { return 0; }

    //virtual void UpdateConstants() = 0;

public:
    const TYPE GetType() const { return type; }
    void SetType(const TYPE t) { type = t; }

    const DirectX::XMFLOAT3& GetVelocity() const { return velocity_; }
    void SetVelocity(const DirectX::XMFLOAT3& velocity) { velocity_ = velocity; }
    void AddVelocity(const DirectX::XMFLOAT3& velocity) { velocity_ += velocity; }

    const float GetRunTimer() const { return runTimer_; }
    void SetRunTimer(const float runTimer) { runTimer_ = runTimer; }

    const float GetAnimationSpeed() const { return animationSpeed_; }
    void SetAnimationSpeed(const float animationSpeed) { animationSpeed_ = animationSpeed; }

    const int GetDropExpCount() const { return dropExpCount_; }
    void SetDropExpCount(const int dropExpCount) { dropExpCount_ = dropExpCount; }

    const int GetStep() const { return step_; }
    void SetStep(const int step) { step_ = step; }

    const bool GetEntryStageFlag() const { return entryStageFlag_; }
    void SetEntryStageFlag(const bool entryStageFlag) { entryStageFlag_ = entryStageFlag; }

    const bool GetIsFlinch() const { return isFlinch_; }
    void SetIsFlinch(const bool isFlinch) { isFlinch_ = isFlinch; }

    const bool GetIsWaveEnemy() const { return isWaveEnemy_; }
    void SetIsWaveEnemy(const bool isWaveEnemy) { isWaveEnemy_ = isWaveEnemy; }

public:
    // ���s�^�C�}�[�o��
    void ElapseRunTimer(const float elapsedTime) { runTimer_ = (std::max)(0.0f, runTimer_ - elapsedTime); }

    // �Ђ�܂���
    void Flinch();

protected:
    std::unique_ptr<BehaviorTree>   behaviorTree_;
    std::unique_ptr<BehaviorData>   behaviorData_;  // ��ɃV�[�P���X�Ɏg��
    NodeBase*           activeNode_     = nullptr;  // BehaviorTree�̃m�[�h���w�������̃|�C���^�Ȃ̂�delete���Ȃ�

    DirectX::XMFLOAT3   velocity_       = {};
    float               runTimer_       = 0.0f;     // ���s�^�C�}�[
    float               animationSpeed_ = 1.0f;     // �A�j���[�V�������x
    int                 dropExpCount_   = 5;        // �h���b�v����o���l�̐�
    int                 step_           = 0;        // �s���X�e�b�v
    bool                entryStageFlag_ = false;    // �X�e�[�W�ɓ��������ǂ����̃t���O
    bool                isFlinch_       = false;    // �Ђ�݃t���O
    bool                isWaveEnemy_    = false;    // �E�F�[�u�Ő������ꂽ�G�ł��邩(�f�X�g���N�^�Ŏg�p)

private:
    TYPE type = TYPE::Normal;

};

