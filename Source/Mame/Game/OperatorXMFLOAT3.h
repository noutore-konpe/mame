#pragma once

#include <DirectXMath.h>

// çáéZån

inline const DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& F1, const DirectX::XMFLOAT3& F2)
{
    const DirectX::XMFLOAT3 F = {
        F1.x + F2.x,
        F1.y + F2.y,
        F1.z + F2.z,
    };

    return F;
}
inline const DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& F, const float& f)
{
    const DirectX::XMFLOAT3 result = {
        F.x + f,
        F.y + f,
        F.z + f,
    };

    return result;
}

inline const DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& F1, const DirectX::XMFLOAT3& F2)
{
    const DirectX::XMFLOAT3 F = {
        F1.x - F2.x,
        F1.y - F2.y,
        F1.z - F2.z,
    };

    return F;
}
inline const DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& F, const float& f)
{
    const DirectX::XMFLOAT3 result = {
        F.x - f,
        F.y - f,
        F.z - f,
    };

    return result;
}
                                               
inline const DirectX::XMFLOAT3 operator*(const DirectX::XMFLOAT3& F1, const DirectX::XMFLOAT3& F2)
{
    const DirectX::XMFLOAT3 F = {
        F1.x * F2.x,
        F1.y * F2.y,
        F1.z * F2.z,
    };

    return F;
}
inline const DirectX::XMFLOAT3 operator*(const DirectX::XMFLOAT3& F, const float& f)
{
    const DirectX::XMFLOAT3 result = {
        F.x * f,
        F.y * f,
        F.z * f,
    };

    return result;
}
                                               
inline const DirectX::XMFLOAT3 operator/(const DirectX::XMFLOAT3& F1, const DirectX::XMFLOAT3& F2)
{
    const DirectX::XMFLOAT3 F = {
        F1.x / F2.x,
        F1.y / F2.y,
        F1.z / F2.z,
    };

    return F;
}
inline const DirectX::XMFLOAT3 operator/(const DirectX::XMFLOAT3& F, const float& f)
{
    const DirectX::XMFLOAT3 result = {
        F.x / f,
        F.y / f,
        F.z / f,
    };

    return result;
}


// çáéZë„ì¸ån

inline const DirectX::XMFLOAT3& operator+=(DirectX::XMFLOAT3& F1, const DirectX::XMFLOAT3& F2)
{
    F1.x += F2.x;
    F1.y += F2.y;
    F1.z += F2.z;

    return F1;
}
inline const DirectX::XMFLOAT3& operator+=(DirectX::XMFLOAT3& F, const float& f)
{
    F.x += f;
    F.y += f;
    F.z += f;

    return F;
}
                                         
inline const DirectX::XMFLOAT3& operator-=(DirectX::XMFLOAT3& F1, const DirectX::XMFLOAT3& F2)
{
    F1.x -= F2.x;
    F1.y -= F2.y;
    F1.z -= F2.z;

    return F1;
}
inline const DirectX::XMFLOAT3& operator-=(DirectX::XMFLOAT3& F, const float& f)
{
    F.x -= f;
    F.y -= f;
    F.z -= f;

    return F;
}
             
inline const DirectX::XMFLOAT3& operator*=(DirectX::XMFLOAT3& F1, const DirectX::XMFLOAT3& F2)
{
    F1.x *= F2.x;
    F1.y *= F2.y;
    F1.z *= F2.z;

    return F1;
}
inline const DirectX::XMFLOAT3& operator*=(DirectX::XMFLOAT3& F, const float& f)
{
    F.x *= f;
    F.y *= f;
    F.z *= f;

    return F;
}

inline const DirectX::XMFLOAT3& operator/=(DirectX::XMFLOAT3& F1, const DirectX::XMFLOAT3& F2)
{
    F1.x /= F2.x;
    F1.y /= F2.y;
    F1.z /= F2.z;

    return F1;
}
inline const DirectX::XMFLOAT3& operator/=(DirectX::XMFLOAT3& F, const float& f)
{
    F.x /= f;
    F.y /= f;
    F.z /= f;

    return F;
}
