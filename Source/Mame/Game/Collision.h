#pragma once

#include <DirectXMath.h>

// ƒRƒŠƒWƒ‡ƒ“
class Collision
{
public:
    // ‹…‚Æ‹…‚ÌŒğ·”»’è
    static bool IntersectSphereVsSphere(
        const DirectX::XMFLOAT3& positionA,
        const float& radiusA,
        const DirectX::XMFLOAT3& positionB,
        const float& radiusB,
        DirectX::XMFLOAT3& outPositionB
    );
};

