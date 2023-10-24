#include "Collision.h"
#include "../../Taki174/FunctionXMFloat3.h"


// ‹…‚Æ‹…‚ÌŒğ·”»’è
const bool Collision::IntersectSphereVsSphere(
    const DirectX::XMFLOAT3& positionA, const float radiusA,
    const DirectX::XMFLOAT3& positionB, const float radiusB,
    DirectX::XMFLOAT3* outPosition)
{
#if 0
    // B ¨ A‚Ì’PˆÊƒxƒNƒgƒ‹‚ğZo
    DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);
    DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);
    DirectX::XMVECTOR vec = DirectX::XMVectorSubtract(PositionB, PositionA);
    DirectX::XMVECTOR LengthSq = DirectX::XMVector3Length(vec);
    float lengthSq;
    DirectX::XMStoreFloat(&lengthSq, LengthSq);

    // ‹——£”»’è
    float range = radiusA + radiusB;
    if (lengthSq > range)
    {
        return false;
    }

    // A ‚ª B ‚ğ‰Ÿ‚µo‚·
    float dist = range - lengthSq;                      // ‚ß‚è‚ñ‚Å‚¢‚é‹——£‚ğZo
    vec = DirectX::XMVectorScale(vec, dist);            // ƒxƒNƒgƒ‹‚É•ÏŠ·
    PositionB = DirectX::XMVectorAdd(PositionB, vec);   // ‰Ÿ‚µo‚µˆÊ’u
    DirectX::XMStoreFloat3(&outPositionB, PositionB);

    return true;

#else
    using DirectX::XMFLOAT3;

    const XMFLOAT3 vec      = positionB - positionA;
    const float    lengthSq = ::XMFloat3LengthSq(vec);
    const float    range    = radiusA + radiusB;

    if (lengthSq > (range * range)) return false;

    const XMFLOAT3 vecN = ::XMFloat3Normalize(vec);
    (*outPosition) = (positionA + vecN * range);

    return true;

#endif
}

const bool Collision::IntersectSphereVsSphere(
    const DirectX::XMFLOAT3& positionA, const float radiusA,
    const DirectX::XMFLOAT3& positionB, const float radiusB)
{
    using DirectX::XMFLOAT3;

    const XMFLOAT3 vec      = positionB - positionA;
    const float    lengthSq = ::XMFloat3LengthSq(vec);
    const float    range    = radiusA + radiusB;

    if (lengthSq > (range * range)) return false;

    return true;
}

const bool Collision::IntersectShrereVsRay(const DirectX::XMFLOAT3& spherePos, const float radius, const DirectX::XMFLOAT3& rayStart, const DirectX::XMFLOAT3& rayDirection, const float length)
{
    DirectX::XMFLOAT3 vec = spherePos - rayStart;

    //Œõü‚Ì•ûŒü‚Æ‹t•ûŒü‚É‹…‚ª‚ ‚Á‚½ê‡‚Íˆ—‚µ‚È‚¢
    if (XMFloat3Dot(XMFloat3Normalize(vec), rayDirection) < 0)return false;

    //Œõü‚Ì’·‚³‚æ‚è‚à‹…‚Ü‚Å‚Ì‹——£‚ª’·‚©‚Á‚½ê‡ˆ—‚µ‚È‚¢
    if (XMFloat3Length(vec) > length)return false;

    //Œõü‚©‚ç‹…‚Ü‚Å‚ÌÅ’Z‹——£
    float projectionLength = XMFloat3Dot(vec, rayDirection);

    const DirectX::XMFLOAT3 p1 = rayStart + rayDirection * projectionLength;
    float nearestLength = XMFloat3Length(p1 - spherePos);

    //‚±‚Ì‹——£‚ª‹…‚Ì”¼Œa‚æ‚è‚à’Z‚¢‚È‚çŒğ·‚µ‚Ä‚¢‚é
    if (nearestLength <= radius)
    {
        return true;
    }

    return false;
}

const bool Collision::IntersectShrereVsCapsule(const DirectX::XMFLOAT3& spherePos, const float sphereRadius, const DirectX::XMFLOAT3& capsuleStart, const DirectX::XMFLOAT3& capsuleDirection, const float length, const float capsuleRadius)
{
    DirectX::XMFLOAT3 vec = spherePos - capsuleStart;

    //Œõü‚Ì•ûŒü‚Æ‹t•ûŒü‚É‹…‚ª‚ ‚Á‚½ê‡‚Íˆ—‚µ‚È‚¢
    if (XMFloat3Dot(XMFloat3Normalize(vec), capsuleDirection) < 0)return false;

    //Œõü‚Ì’·‚³‚æ‚è‚à‹…‚Ü‚Å‚Ì‹——£‚ª’·‚©‚Á‚½ê‡ˆ—‚µ‚È‚¢
    if (XMFloat3Length(vec) > length + capsuleRadius)return false;

    //Œõü‚©‚ç‹…‚Ü‚Å‚ÌÅ’Z‹——£
    float projectionLength = XMFloat3Dot(vec, capsuleDirection);

    const DirectX::XMFLOAT3 p1 = capsuleStart + capsuleDirection * projectionLength;
    float nearestLength = XMFloat3Length(p1 - spherePos);

    //‚±‚Ì‹——£‚ª‹…‚Ì”¼Œa‚æ‚è‚à’Z‚¢‚È‚çŒğ·‚µ‚Ä‚¢‚é
    if (nearestLength <= sphereRadius + capsuleRadius)
    {
        return true;
    }

    return false;
}
