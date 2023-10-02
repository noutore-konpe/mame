#pragma once

#include <DirectXMath.h>
#include "Common.h"

// çáéZån

inline const DirectX::XMFLOAT4 operator+(
    const DirectX::XMFLOAT4& float4_1,
    const DirectX::XMFLOAT4& float4_2)
{
    const DirectX::XMFLOAT4 result = {
        float4_1.x + float4_2.x,
        float4_1.y + float4_2.y,
        float4_1.z + float4_2.z,
        float4_1.w + float4_2.w,
    };

    return result;
}
inline const DirectX::XMFLOAT4 operator+(
    const DirectX::XMFLOAT4& float4,
    const float f)
{
    const DirectX::XMFLOAT4 result = {
        float4.x + f,
        float4.y + f,
        float4.z + f,
        float4.w + f,
    };

    return result;
}

inline const DirectX::XMFLOAT4 operator-(
    const DirectX::XMFLOAT4& float4_1,
    const DirectX::XMFLOAT4& float4_2)
{
    const DirectX::XMFLOAT4 result = {
        float4_1.x - float4_2.x,
        float4_1.y - float4_2.y,
        float4_1.z - float4_2.z,
        float4_1.w - float4_2.w,
    };

    return result;
}
inline const DirectX::XMFLOAT4 operator-(
    const DirectX::XMFLOAT4& float4,
    const float f)
{
    const DirectX::XMFLOAT4 result = {
        float4.x - f,
        float4.y - f,
        float4.z - f,
        float4.w - f,
    };

    return result;
}

inline const DirectX::XMFLOAT4 operator*(
    const DirectX::XMFLOAT4& float4_1,
    const DirectX::XMFLOAT4& float4_2)
{
    const DirectX::XMFLOAT4 result = {
        float4_1.x * float4_2.x,
        float4_1.y * float4_2.y,
        float4_1.z * float4_2.z,
        float4_1.w * float4_2.w,
    };

    return result;
}
inline const DirectX::XMFLOAT4 operator*(
    const DirectX::XMFLOAT4& float4,
    const float f)
{
    const DirectX::XMFLOAT4 result = {
        float4.x * f,
        float4.y * f,
        float4.z * f,
        float4.w * f,
    };

    return result;
}

inline const DirectX::XMFLOAT4 operator/(
    const DirectX::XMFLOAT4& float4_1,
    const DirectX::XMFLOAT4& float4_2)
{
    const DirectX::XMFLOAT4 result = {
        float4_1.x / float4_2.x,
        float4_1.y / float4_2.y,
        float4_1.z / float4_2.z,
        float4_1.w / float4_2.w,
    };

    return result;
}
inline const DirectX::XMFLOAT4 operator/(
    const DirectX::XMFLOAT4& float4,
    const float f)
{
    const DirectX::XMFLOAT4 result = {
        float4.x / f,
        float4.y / f,
        float4.z / f,
        float4.w / f,
    };

    return result;
}


// çáéZë„ì¸ån

inline const DirectX::XMFLOAT4& operator+=(
    NoConst DirectX::XMFLOAT4& float4_1,
    const   DirectX::XMFLOAT4& float4_2)
{
    float4_1.x += float4_2.x;
    float4_1.y += float4_2.y;
    float4_1.z += float4_2.z;
    float4_1.w += float4_2.w;

    return float4_1;
}
inline const DirectX::XMFLOAT4& operator+=(
    NoConst DirectX::XMFLOAT4& float4,
    const float f)
{
    float4.x += f;
    float4.y += f;
    float4.z += f;
    float4.w += f;

    return float4;
}

inline const DirectX::XMFLOAT4& operator-=(
    NoConst DirectX::XMFLOAT4& float4_1,
    const   DirectX::XMFLOAT4& float4_2)
{
    float4_1.x -= float4_2.x;
    float4_1.y -= float4_2.y;
    float4_1.z -= float4_2.z;
    float4_1.w -= float4_2.w;

    return float4_1;
}
inline const DirectX::XMFLOAT4& operator-=(
    NoConst DirectX::XMFLOAT4& float4,
    const float f)
{
    float4.x -= f;
    float4.y -= f;
    float4.z -= f;
    float4.w -= f;

    return float4;
}

inline const DirectX::XMFLOAT4& operator*=(
    NoConst DirectX::XMFLOAT4& float4_1,
    const   DirectX::XMFLOAT4& float4_2)
{
    float4_1.x *= float4_2.x;
    float4_1.y *= float4_2.y;
    float4_1.z *= float4_2.z;
    float4_1.w *= float4_2.w;

    return float4_1;
}
inline const DirectX::XMFLOAT4& operator*=(
    NoConst DirectX::XMFLOAT4& float4,
    const float f)
{
    float4.x *= f;
    float4.y *= f;
    float4.z *= f;
    float4.w *= f;

    return float4;
}

inline const DirectX::XMFLOAT4& operator/=(
    NoConst DirectX::XMFLOAT4& float4_1,
    const   DirectX::XMFLOAT4& float4_2)
{
    float4_1.x /= float4_2.x;
    float4_1.y /= float4_2.y;
    float4_1.z /= float4_2.z;
    float4_1.w /= float4_2.w;

    return float4_1;
}
inline const DirectX::XMFLOAT4& operator/=(
    NoConst DirectX::XMFLOAT4& float4,
    const float f)
{
    float4.x /= f;
    float4.y /= f;
    float4.z /= f;
    float4.w /= f;

    return float4;
}
