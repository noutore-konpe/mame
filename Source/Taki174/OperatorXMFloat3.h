#pragma once

#include <DirectXMath.h>

// éZèpââéZéq
#pragma region Arithmetic Operator

inline const DirectX::XMFLOAT3 operator+(
    const DirectX::XMFLOAT3& float3_1,
    const DirectX::XMFLOAT3& float3_2)
{
    const DirectX::XMFLOAT3 result = {
        float3_1.x + float3_2.x,
        float3_1.y + float3_2.y,
        float3_1.z + float3_2.z,
    };
    DirectX::XMFLOAT3 a = {};
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

#pragma endregion

// ï°çáë„ì¸ââéZéq
#pragma region Compound Assignment Operator

inline const DirectX::XMFLOAT3& operator+=(
    DirectX::XMFLOAT3& float3_1,
    const DirectX::XMFLOAT3& float3_2)
{
    float3_1.x += float3_2.x;
    float3_1.y += float3_2.y;
    float3_1.z += float3_2.z;

    return float3_1;
}
inline const DirectX::XMFLOAT3& operator+=(
    DirectX::XMFLOAT3& float3,
    const float f)
{
    float3.x += f;
    float3.y += f;
    float3.z += f;

    return float3;
}

inline const DirectX::XMFLOAT3& operator-=(
    DirectX::XMFLOAT3& float3_1,
    const DirectX::XMFLOAT3& float3_2)
{
    float3_1.x -= float3_2.x;
    float3_1.y -= float3_2.y;
    float3_1.z -= float3_2.z;

    return float3_1;
}
inline const DirectX::XMFLOAT3& operator-=(
    DirectX::XMFLOAT3& float3,
    const float f)
{
    float3.x -= f;
    float3.y -= f;
    float3.z -= f;

    return float3;
}

inline const DirectX::XMFLOAT3& operator*=(
    DirectX::XMFLOAT3& float3_1,
    const DirectX::XMFLOAT3& float3_2)
{
    float3_1.x *= float3_2.x;
    float3_1.y *= float3_2.y;
    float3_1.z *= float3_2.z;

    return float3_1;
}
inline const DirectX::XMFLOAT3& operator*=(
    DirectX::XMFLOAT3& float3,
    const float f)
{
    float3.x *= f;
    float3.y *= f;
    float3.z *= f;

    return float3;
}

inline const DirectX::XMFLOAT3& operator/=(
    DirectX::XMFLOAT3& float3_1,
    const DirectX::XMFLOAT3& float3_2)
{
    float3_1.x /= float3_2.x;
    float3_1.y /= float3_2.y;
    float3_1.z /= float3_2.z;

    return float3_1;
}
inline const DirectX::XMFLOAT3& operator/=(
    DirectX::XMFLOAT3& float3,
    const float f)
{
    float3.x /= f;
    float3.y /= f;
    float3.z /= f;

    return float3;
}

#pragma endregion

// íPçÄââéZéq
#pragma region Unary Operator

inline const DirectX::XMFLOAT3 operator+(
    const DirectX::XMFLOAT3& float3)
{
    return float3 * (+1.0f);
}
inline const DirectX::XMFLOAT3 operator-(
    const DirectX::XMFLOAT3& float3)
{
    return float3 * (-1.0f);
}

#pragma endregion

// î‰ärââéZéq
#pragma region Comparison operator
inline const bool operator==(
    const DirectX::XMFLOAT3& float3_1,
    const DirectX::XMFLOAT3& float3_2)
{
    const bool result = {
        (float3_1.x == float3_2.x) &&
        (float3_1.y == float3_2.y) &&
        (float3_1.z == float3_2.z)
    };

    return result;
}
inline const bool operator!=(
    const DirectX::XMFLOAT3& float3_1,
    const DirectX::XMFLOAT3& float3_2)
{
    const bool result = {
        (float3_1.x != float3_2.x) ||
        (float3_1.y != float3_2.y) ||
        (float3_1.z != float3_2.z)
    };

    return result;
}
#pragma endregion