#pragma once

#include "Projectile.h"

class ProjectileHorming : public Projectile
{
public:
    ProjectileHorming(
        ProjectileManager* manager,
        const bool isPoison = false
    );
    ~ProjectileHorming() override;

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
        const DirectX::XMFLOAT3& position,
        const DirectX::XMFLOAT3& direciton
    );

private:
    static int nameNum_;

private:
    DirectX::XMFLOAT3   direction_ = {};   // •ûŒü
    float               speed_     = 3.0f; // ‘¬“x
    float               lifeTimer_ = 3.0f; // Žõ–½

};
