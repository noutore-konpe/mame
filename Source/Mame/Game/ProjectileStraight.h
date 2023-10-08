#pragma once

#include "Projectile.h"

class ProjectileStraight : public Projectile
{
public:
    ProjectileStraight(ProjectileManager* manager);
    ~ProjectileStraight();

    void Initialize()                                           override;   // 初期化
    void Finalize()                                             override {} // 終了化
    void Begin()                                                override {} // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime)                       override;   // 更新処理
    void End()                                                  override {} // 毎フレーム一番最後に呼ばれる
    void Render(const float& scale, ID3D11PixelShader* psShader = nullptr) override;   // 描画処理
    void DrawDebug()                                            override;   // ImGui用

public:
    // 発射
    void Launch(
        const DirectX::XMFLOAT3& direction,
        const DirectX::XMFLOAT3& position
    );

private:
    static int nameNum_;

private:
    DirectX::XMFLOAT3 direction_    = {};   // 方向
    float speed_                    = 2.0f; // 速度
    float lifeTimer_                = 3.0f; // 寿命

};