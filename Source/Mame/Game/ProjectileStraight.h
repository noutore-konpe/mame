#pragma once

#include "Projectile.h"

class ProjectileStraight : public Projectile
{
public:
    ProjectileStraight(
        ProjectileManager* manager,
        Character* owner,
        const bool isPoison = false
    );
    ~ProjectileStraight() override;

    void Initialize()                    override;
    void Finalize()                      override {}
    void Begin()                         override {}
    void Update(const float elapsedTime) override;
    void End()                           override {}
    void Render(const float scale, ID3D11PixelShader* psShader = nullptr) override;
    void DrawDebug()                     override;

public:
    // ”­ŽË
    void Launch(
        const DirectX::XMFLOAT3& direction,
        const DirectX::XMFLOAT3& position
    );

private:
    static int nameNum_;

private:
    float               speed_      = 5.0f; // ‘¬“x
    float               lifeTimer_  = 3.0f; // Žõ–½

};