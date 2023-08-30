#include "Collision.h"

// 球と球の交差判定
bool Collision::IntersectSphereVsSphere(const DirectX::XMFLOAT3& positionA, const float& radiusA, const DirectX::XMFLOAT3& positionB, const float& radiusB, DirectX::XMFLOAT3& outPositionB)
{
    // B → Aの単位ベクトルを算出
    DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);
    DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);
    DirectX::XMVECTOR vec = DirectX::XMVectorSubtract(PositionB, PositionA);
    DirectX::XMVECTOR LengthSq = DirectX::XMVector3Length(vec);
    float lengthSq;
    DirectX::XMStoreFloat(&lengthSq, LengthSq);

    // 距離判定
    float range = radiusA + radiusB;
    if (lengthSq > range)
    {
        return false;
    }

    // A が B を押し出す
    float dist = range - lengthSq;                      // めり込んでいる距離を算出
    vec = DirectX::XMVectorScale(vec, dist);            // ベクトルに変換
    PositionB = DirectX::XMVectorAdd(PositionB, vec);   // 押し出し位置
    DirectX::XMStoreFloat3(&outPositionB, PositionB);

    return true;
}
