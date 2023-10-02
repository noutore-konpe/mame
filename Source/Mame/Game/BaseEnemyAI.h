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
    // 目的地点へ移動
    void MoveToTarget(
        const float elapsedTime,
        const float speedRate
    );

    // ターゲットポジション設定
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
    // 垂直速力処理更新
    void UpdateVerticalVelocity(const float elapsedFrame);
    // 垂直移動更新処理
    void UpdateVerticalMove(const float elapsedTime);
    // 水平速力処理更新
    void UpdateHorizontalVelocity(const float elapsedFrame);
    // 水平移動更新処理
    void UpdateHorizontalMove(const float elapsedTime);

protected:
    std::unique_ptr<BehaviorTree>   behaviorTree_;
    std::unique_ptr<BehaviorData>   behaviorData_; // 主にシーケンスに使う

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

