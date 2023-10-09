#pragma once

#include <DirectXMath.h>

// ÉRÉäÉWÉáÉì
class Collision
{
public:
    // ãÖÇ∆ãÖÇÃåç∑îªíË
    static bool IntersectSphereVsSphere(
        const DirectX::XMFLOAT3& positionA, const float radiusA,
        const DirectX::XMFLOAT3& positionB, const float radiusB,
        DirectX::XMFLOAT3* outPosition
    );

    static bool IntersectSphereVsSphere(
        const DirectX::XMFLOAT3& positionA, const float radiusA,
        const DirectX::XMFLOAT3& positionB, const float radiusB
    )
    {
        DirectX::XMFLOAT3 outPosition = {};
        return IntersectSphereVsSphere(
            positionA, radiusA, positionB, radiusB, &outPosition
        );
    }
};

