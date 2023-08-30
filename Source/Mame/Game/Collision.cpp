#include "Collision.h"

// ���Ƌ��̌�������
bool Collision::IntersectSphereVsSphere(const DirectX::XMFLOAT3& positionA, const float& radiusA, const DirectX::XMFLOAT3& positionB, const float& radiusB, DirectX::XMFLOAT3& outPositionB)
{
    // B �� A�̒P�ʃx�N�g�����Z�o
    DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);
    DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);
    DirectX::XMVECTOR vec = DirectX::XMVectorSubtract(PositionB, PositionA);
    DirectX::XMVECTOR LengthSq = DirectX::XMVector3Length(vec);
    float lengthSq;
    DirectX::XMStoreFloat(&lengthSq, LengthSq);

    // ��������
    float range = radiusA + radiusB;
    if (lengthSq > range)
    {
        return false;
    }

    // A �� B �������o��
    float dist = range - lengthSq;                      // �߂荞��ł��鋗�����Z�o
    vec = DirectX::XMVectorScale(vec, dist);            // �x�N�g���ɕϊ�
    PositionB = DirectX::XMVectorAdd(PositionB, vec);   // �����o���ʒu
    DirectX::XMStoreFloat3(&outPositionB, PositionB);

    return true;
}
