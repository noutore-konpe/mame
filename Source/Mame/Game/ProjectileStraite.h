#pragma once

#include "Projectile.h"

class ProjectileStraite : public Projectile
{
public:
    ProjectileStraite(ProjectileManager* manager);
    ~ProjectileStraite() {}

    void Initialize()                                           override;   // 初期化
    void Finalize()                                             override {} // 終了化
    void Begin()                                                override {} // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime)                       override;   // 更新処理
    void End()                                                  override {} // 毎フレーム一番最後に呼ばれる
    void Render(const float& elapsedTime, const float& scale)   override;   // 描画処理
    void DrawDebug()                                            override;   // ImGui用

public:
    // 発射
    void Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position);

private:
    DirectX::XMFLOAT3 direction;    // 方向
    float speed = 1.0f;            // 速度
    float lifeTimer = 3.0f;         // 寿命
};