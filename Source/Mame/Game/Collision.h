#pragma once

#include <DirectXMath.h>

// �R���W����
class Collision
{
public:
    // ���Ƌ��̌�������
    static const bool IntersectSphereVsSphere(
        const DirectX::XMFLOAT3& positionA, const float radiusA,
        const DirectX::XMFLOAT3& positionB, const float radiusB,
        DirectX::XMFLOAT3* outPosition
    );
    // �����o�������Ȃ��o�[�W����
    static const bool IntersectSphereVsSphere(
        const DirectX::XMFLOAT3& positionA, const float radiusA,
        const DirectX::XMFLOAT3& positionB, const float radiusB
    );
};

