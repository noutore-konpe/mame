#pragma once

#include <DirectXMath.h>
#include "Common.h"

// çáéZån

inline const DirectX::XMFLOAT3 operator+(
    const DirectX::XMFLOAT3& float3_1,
    const DirectX::XMFLOAT3& float3_2)
{
    const DirectX::XMFLOAT3 result = {
        float3_1.x + float3_2.x,
        float3_1.y + float3_2.y,
        float3_1.z + float3_2.z,
    };

    return result;
}
inline const DirectX::XMFLOAT3 operator+(
    const DirectX::XMFLOAT3& float3,
    const float f)
{
    const DirectX::XMFLOAT3 result = {
        float3.x + f,
        float3.y + f,
        float3.z + f,
    };

    return result;
}

inline const DirectX::XMFLOAT3 operator-(
    const DirectX::XMFLOAT3& float3_1,
    const DirectX::XMFLOAT3& float3_2)
{
    const DirectX::XMFLOAT3 result = {
        float3_1.x - float3_2.x,
        float3_1.y - float3_2.y,
        float3_1.z - float3_2.z,
    };

    return result;
}
inline const DirectX::XMFLOAT3 operator-(
    const DirectX::XMFLOAT3& float3,
    const float f)
{
    const DirectX::XMFLOAT3 result = {
        float3.x - f,
        float3.y - f,
        float3.z - f,
    };

    return result;
}

inline const DirectX::XMFLOAT3 operator*(
    const DirectX::XMFLOAT3& float3_1,
    const DirectX::XMFLOAT3& float3_2)
{
    const DirectX::XMFLOAT3 result = {
        float3_1.x * float3_2.x,
        float3_1.y * float3_2.y,
        float3_1.z * float3_2.z,
    };

    return result;
}
inline const DirectX::XMFLOAT3 operator*(
    const DirectX::XMFLOAT3& float3,
    const float f)
{
    const DirectX::XMFLOAT3 result = {
        float3.x * f,
        float3.y * f,
        float3.z * f,
    };

    return result;
}

inline const DirectX::XMFLOAT3 operator/(
    const DirectX::XMFLOAT3& float3_1,
    const DirectX::XMFLOAT3& float3_2)
{
    const DirectX::XMFLOAT3 result = {
        float3_1.x / float3_2.x,
        float3_1.y / float3_2.y,
        float3_1.z / float3_2.z,
    };

    return result;
}
inline const DirectX::XMFLOAT3 operator/(
    const DirectX::XMFLOAT3& float3,
    const float f)
{
    const DirectX::XMFLOAT3 result = {
        float3.x / f,
        float3.y / f,
        float3.z / f,
    };

    return result;
}


// çáéZë„ì¸ån

inline const DirectX::XMFLOAT3& operator+=(
    NoConst DirectX::XMFLOAT3& float3_1,
    const   DirectX::XMFLOAT3& float3_2)
{
    float3_1.x += float3_2.x;
    float3_1.y += float3_2.y;
    float3_1.z += float3_2.z;

    return float3_1;
}
inline const DirectX::XMFLOAT3& operator+=(
    NoConst DirectX::XMFLOAT3& float3,
    const float f)
{
    float3.x += f;
    float3.y += f;
    float3.z += f;

    return float3;
}

inline const DirectX::XMFLOAT3& operator-=(
    NoConst DirectX::XMFLOAT3& float3_1,
    const   DirectX::XMFLOAT3& float3_2)
{
    float3_1.x -= float3_2.x;
    float3_1.y -= float3_2.y;
    float3_1.z -= float3_2.z;

    return float3_1;
}
inline const DirectX::XMFLOAT3& operator-=(
    NoConst DirectX::XMFLOAT3& float3,
    const float f)
{
    float3.x -= f;
    float3.y -= f;
    float3.z -= f;

    return float3;
}

inline const DirectX::XMFLOAT3& operator*=(
    NoConst DirectX::XMFLOAT3& float3_1,
    const   DirectX::XMFLOAT3& float3_2)
{
    float3_1.x *= float3_2.x;
    float3_1.y *= float3_2.y;
    float3_1.z *= float3_2.z;

    return float3_1;
}
inline const DirectX::XMFLOAT3& operator*=(
    NoConst DirectX::XMFLOAT3& float3,
    const float f)
{
    float3.x *= f;
    float3.y *= f;
    float3.z *= f;

    return float3;
}

inline const DirectX::XMFLOAT3& operator/=(
    NoConst DirectX::XMFLOAT3& float3_1,
    const   DirectX::XMFLOAT3& float3_2)
{
    float3_1.x /= float3_2.x;
    float3_1.y /= float3_2.y;
    float3_1.z /= float3_2.z;

    return float3_1;
}
inline const DirectX::XMFLOAT3& operator/=(
    NoConst DirectX::XMFLOAT3& float3,
    const float f)
{
    float3.x /= f;
    float3.y /= f;
    float3.z /= f;

    return float3;
}
