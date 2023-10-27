#pragma once

#include "Projectile.h"

// �X�y���~�X
class ProjectileHorming : public Projectile
{
public:
    ProjectileHorming(
        ProjectileManager* manager,
        Character* owner,
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
    // ����
    void Launch(
        const DirectX::XMFLOAT3& position,
        const DirectX::XMFLOAT3& direciton
    );

private:
    static int nameNum_;

private:
    float               speed_     = 3.0f; // ���x
    float               lifeTimer_ = 3.0f; // ����

};
