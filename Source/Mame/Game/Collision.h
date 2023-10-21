#pragma once

#include <DirectXMath.h>

// コリジョン
class Collision
{
public:
    // 球と球の交差判定
    static const bool IntersectSphereVsSphere(
        const DirectX::XMFLOAT3& positionA, const float radiusA,
        const DirectX::XMFLOAT3& positionB, const float radiusB,
        DirectX::XMFLOAT3* outPosition
    );
    // 押し出し処理なしバージョン
    static const bool IntersectSphereVsSphere(
        const DirectX::XMFLOAT3& positionA, const float radiusA,
        const DirectX::XMFLOAT3& positionB, const float radiusB
    );
};

