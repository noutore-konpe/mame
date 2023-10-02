#pragma once

#include <memory>

#include "Character.h"

#include "BehaviorTree.h"
#include "BehaviorData.h"
#include "NodeBase.h"

class BaseEnemyAI : public Character
{
public:
    BaseEnemyAI() {}
    ~BaseEnemyAI() override;

    void Initialize() override;
    void Update(const float& elapsedTime) override;
    void Render(const float& scale, ID3D11PixelShader* psShader = nullptr) override;
    void DrawDebug() override;

public:
    // �ړI�n�_�ֈړ�
    void MoveToTarget(
        const float elapsedTime,
        const float speedRate
    );

    // �^�[�Q�b�g�|�W�V�����ݒ�
    void SetTargetPosition(const DirectX::XMFLOAT3& position) { targetPosition_ = position; }


protected:
    void Move(
        const float vx,
        const float vz,
        const float moveSpeed
    );

    void Turn(
        const float elapsedTime,
        const float vx,
        const float vz,
        const float turnSpeed
    );

protected:
    void UpdateNode(const float elapsedTime);
    void UpdateVelocity(const float elapsedTime);

protected:
    void OnLanding() {}

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
    std::unique_ptr<BehaviorTree>   behaviorTree_;
    std::unique_ptr<BehaviorData>   behaviorData_; // ��ɃV�[�P���X�Ɏg��

    NodeBase* activeNode_ = nullptr;

    DirectX::XMFLOAT3 targetPosition_ = {};
    DirectX::XMFLOAT3 moveVec_  = {};
    DirectX::XMFLOAT3 velocity_ = {};

    float moveSpeed_    = 0.0f;
    float maxMoveSpeed_ = 0.0f;
    float turnSpeed_    = 0.0f;
    float gravity_      = -1.0f;
    float friction_     = 0.5f;
    float airControl_   = 0.3f;
    float acceleration_ = 1.0f;

    bool isGround_      = false;

};

