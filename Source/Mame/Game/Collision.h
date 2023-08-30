#pragma once

#include <DirectXMath.h>

// �R���W����
class Collision
{
public:
    // ���Ƌ��̌�������
    static bool IntersectSphereVsSphere(
        const DirectX::XMFLOAT3& positionA,
        const float& radiusA,
        const DirectX::XMFLOAT3& positionB,
        const float& radiusB,
        DirectX::XMFLOAT3& outPositionB
    );
};

