#pragma once

#include <memory>

#include "Enemy.h"

class ProjectileManager;

class BaseEnemyAI : public Enemy
{
public:
    enum class HitColName
    {
        NECK,
        HIP,
        R_LEG,
        L_LEG,
        R_LEG_END,
        L_LEG_END,
        R_ELBOW,
        L_ELBOW,
        R_HAND,
        L_HAND,
        END
    };

public:
    BaseEnemyAI();
    ~BaseEnemyAI() override {}

    void Initialize()   override;
    void Finalize()     override {}
    void Begin()        override {}
    void Update(const float& elapsedTime) override;
    void End()          override {}
    void Render(const float& scale, ID3D11PixelShader* psShader = nullptr) override;
    void DrawDebug()    override;

    void OnDamaged() override;
    void OnDead(DamageResult result)    override;

public:
    // �ړI�n�_�ֈړ�
    void MoveToTarget(
        const float elapsedTime,
        const float speedRate,
        const bool isLookAtTarget = true
    );

public: // �擾�E�ݒ�
    Model* GetSword() const { return sword_.get(); }

    NodeBase* GetActiveNode() const { return activeNode_; }

    ProjectileManager* GetProjectileManager() const { return projectileManager_; }

    const DirectX::XMFLOAT3& GetTargetPosition() const { return targetPosition_; }
    void SetTargetPosition(const DirectX::XMFLOAT3& position) { targetPosition_ = position; }

    const float GetTurnSpeed() const { return turnSpeed_; }

    const float GetAttackLength() const { return attackLength_; }

    //�����蔻��ݒ�
    void ColliderInitialize();

    //�����蔻����{�[���ɂ�������
    void ColliderPosUpdate(const float& scale);

public:
    void Move(
        const float vx,
        const float vz,
        const float moveSpeed
    );

    void Turn(
        const float elapsedTime,
        const float vx,
        const float vz,
        const float turnSpeed /*Degree*/
    );

protected:
    void UpdateNode(const float elapsedTime);
    void UpdateVelocity(const float elapsedTime);

    void UpdateSword(const float elapsedTime);

protected:
    virtual void OnLanding() {}

private:
    void Flinch() override;

    void BlowOff(
        const DirectX::XMFLOAT3& blowOffVec,
        const BLOW_OFF_FORCE_LEVEL& blowOffForceLevel
    ) override;

private:
    // �������͏����X�V
    void UpdateVerticalVelocity(const float elapsedFrame);
    // �����ړ��X�V����
    void UpdateVerticalMove(const float elapsedTime);
    // �������͏����X�V
    void UpdateHorizontalVelocity(const float elapsedFrame);
    // �����ړ��X�V����
    void UpdateHorizontalMove(const float elapsedTime);

protected:
    std::unique_ptr<Model>          sword_;

    ProjectileManager* projectileManager_ = nullptr;

    DirectX::XMFLOAT3 targetPosition_ = {};
    DirectX::XMFLOAT3 moveVec_  = {};

    float moveSpeed_        = 3.5f;
    float maxMoveSpeed_     = 0.0f;
    float turnSpeed_        = 360.0f;
    float gravity_          = -1.0f;
    float friction_         = 0.2f;
    float airControl_       = 0.3f;
    float acceleration_     = 0.3f;

    float attackLength_     = 2.0f;     // �U������

    bool isGround_          = false;

    float swordColliderNum = 5;
    float swordColliderRadius = 0.07f;
};

