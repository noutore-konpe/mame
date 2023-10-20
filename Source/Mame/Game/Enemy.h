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

public:
    const TYPE GetType() const { return type; }
    void SetType(const TYPE t) { type = t; }

    const BLOW_OFF_FORCE_LEVEL& GetBlowOffForceLevel() const { return blowOffForceLevel_; }
    void SetBlowOffForceLevel(const BLOW_OFF_FORCE_LEVEL& blowOffForceLevel) { blowOffForceLevel_ = blowOffForceLevel; }

    const DirectX::XMFLOAT3& GetVelocity() const { return velocity_; }
    void SetVelocity(const DirectX::XMFLOAT3& velocity) { velocity_ = velocity; }
    void AddVelocity(const DirectX::XMFLOAT3& velocity) { velocity_ += velocity; }

    const DirectX::XMFLOAT3& GetBlowOffVec() const { return blowOffVec_; }
    void  SetBlowOffVec(const DirectX::XMFLOAT3& blowOffVec) { blowOffVec_ = blowOffVec; }

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

    const bool GetFlinchStartFlag() const { return flinchStartFlag_; }
    void SetFlinchStartFlag(const bool flinchStartFlag) { flinchStartFlag_ = flinchStartFlag; }

    const bool GetIsFlinch() const { return isFlinch_; }
    void SetIsFlinch(const bool isFlinch) { isFlinch_ = isFlinch; }

    const bool GetIsBlowOff() const { return isBlowOff_; }
    void SetIsBlowOff(const bool isBlowOff) { isBlowOff_ = isBlowOff; }

    const bool GetIsWaveEnemy() const { return isWaveEnemy_; }
    void SetIsWaveEnemy(const bool isWaveEnemy) { isWaveEnemy_ = isWaveEnemy; }

    const float GetBlowOffForceAt(const BLOW_OFF_FORCE_LEVEL& levelIndex) const { return blowOffForce_[static_cast<int>(levelIndex)]; }

public:
    // ���s�^�C�}�[�o��
    void ElapseRunTimer(const float elapsedTime) { runTimer_ = (std::max)(0.0f, runTimer_ - elapsedTime); }

    // �Ђ�܂���
    virtual void Flinch() {}

    /// <summary>
    /// ������΂�
    ///�i���C�ӂŐ�����΂������Ƃ��ɌĂяo���j
    /// </summary>
    /// <param name="blowOffVec�F������΂������x�N�g��(�����K���ŉ�)"></param>
    /// <param name="blowOffForceLevel�F������΂��͂̓x����"></param>
    virtual void BlowOff(
        const DirectX::XMFLOAT3& /*blowOffVec*/,
        const BLOW_OFF_FORCE_LEVEL& /*blowOffForceLevel*/
    ) {}

    // ������΂������x�N�g��(�����K���ŉ�)�Ɨ͂̓x�����̏���ۑ�����
    // �����S�����Ƃ��̐�����тɎg���̂�
    // �@�v���C���[�̍U�����q�b�g���閈�ɌĂяo���Ă���
    void SaveBlowOffInfo(
        const DirectX::XMFLOAT3& blowOffVec,
        const BLOW_OFF_FORCE_LEVEL& blowOffForceLevel
    );

    // ����
    void Destroy();

    // ��������q�m�[�h�����݂��邩��������
    const bool IsExistChildNode(const std::string& findNodeName);

    // �q�m�[�h�̐����擾
    const size_t GetChildNodeCount() const
    {
        if (nullptr == behaviorTree_) return 0;

        return behaviorTree_->GetRoot()->GetChildren()->size();
    }

protected:
    std::unique_ptr<BehaviorTree>   behaviorTree_;
    std::unique_ptr<BehaviorData>   behaviorData_;  // ��ɃV�[�P���X�Ɏg��
    NodeBase*            activeNode_        = nullptr; // BehaviorTree�̃m�[�h���w�������̃|�C���^�Ȃ̂�delete���Ȃ�

    BLOW_OFF_FORCE_LEVEL blowOffForceLevel_ = BLOW_OFF_FORCE_LEVEL::NONE; // ������ԗ͂̓x����

    DirectX::XMFLOAT3    velocity_          = {};
    DirectX::XMFLOAT3    blowOffVec_        = {};      // ������ԕ����x�N�g��(�����K��)
    float                runTimer_          = 0.0f;    // ���s�^�C�}�[
    float                animationSpeed_    = 1.0f;    // �A�j���[�V�������x
    int                  dropExpCount_      = 5;       // �h���b�v����o���l�̐�
    int                  step_              = 0;       // �s���X�e�b�v
    bool                 entryStageFlag_    = false;   // �X�e�[�W�ɓ��������ǂ����̃t���O
    bool                 flinchStartFlag_   = false;   // �Ђ�݊J�n�t���O(�Ђ�ݒ��ɍēx�Ђ�݂��������Ƃ��p�Ɏg��)
    bool                 isFlinch_          = false;   // �Ђ�ݒ��t���O
    bool                 isBlowOff_         = false;   // ������ђ��t���O
    bool                 isWaveEnemy_       = false;   // �E�F�[�u�Ő������ꂽ�G�ł��邩(�f�X�g���N�^�Ŏg�p)

protected:
    // ������΂���
    float blowOffForce_[static_cast<int>(BLOW_OFF_FORCE_LEVEL::COUNT)] = {
        0.0f,   // BLOW_OFF_FORCE_LEVEL::NONE
        1.25f,  // BLOW_OFF_FORCE_LEVEL::VERY_LOW
        2.5f,   // BLOW_OFF_FORCE_LEVEL::LOW
        5.0f,   // BLOW_OFF_FORCE_LEVEL::MIDDLE
        10.0f,  // BLOW_OFF_FORCE_LEVEL::HIGH
        15.0f,  // BLOW_OFF_FORCE_LEVEL::VERY_HIGH
    };

private:
    TYPE type = TYPE::Normal;

};

