#pragma once

#include <memory>

#include "Enemy.h"

#include "BehaviorTree.h"
#include "BehaviorData.h"
#include "NodeBase.h"

class ProjectileManager;

class BaseEnemyAI : public Enemy
{
public:
    BaseEnemyAI();
    ~BaseEnemyAI() override {}

    void Initialize() override;
    void Finalize()   override {}
    void Begin()      override {}
    void Update(const float& elapsedTime) override;
    void End()        override {}
    void Render(const float& scale, ID3D11PixelShader* psShader = nullptr) override;
    void DrawDebug()  override;

public:
    // 目的地点へ移動
    void MoveToTarget(
        const float elapsedTime,
        const float speedRate,
        const bool isLookAtTarget = true
    );

public: // 取得・設定
    Model* GetSword() const { return sword_.get(); }

    NodeBase* GetActiveNode() const { return activeNode_; }

    ProjectileManager* GetProjectileManager() const { return projectileManager_; }

    const DirectX::XMFLOAT3& GetTargetPosition() const { return targetPosition_; }
    void SetTargetPosition(const DirectX::XMFLOAT3& position) { targetPosition_ = position; }

    const float GetTurnSpeed() const { return turnSpeed_; }

    const float GetAttackLength() const { return attackLength_; }

    const float GetRunTimer() const { return runTimer_; }
    void SetRunTimer(const float runTimer) { runTimer_ = runTimer; }
    void ElapseRunTimer(const float elapsedTime) { runTimer_ = (std::max)(0.0f, runTimer_ - elapsedTime); }

    const float GetAnimationSpeed() const { return animationSpeed_; }
    void SetAnimationSpeed(const float animationSpeed) { animationSpeed_ = animationSpeed; }

    const bool GetIsFlinch() const { return isFlinch_; }
    void SetIsFlinch(const bool isFlinch) { isFlinch_ = isFlinch; }

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

    std::unique_ptr<Model>          sword_;

    NodeBase* activeNode_ = nullptr; // BehaviorTreeのノードを指すだけのポインタなのでdeleteしない

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

    float attackLength_     = 3.0f;     // 攻撃距離
    float runTimer_         = 0.0f;

    float animationSpeed_   = 1.0f;     // アニメーション速度

    bool isGround_          = false;
    bool isFlinch_          = false;    // ひるみフラグ

};

