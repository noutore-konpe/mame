#pragma once

#include <DirectXMath.h>
#include <stdlib.h>

// �}�N���萔
#pragma region Macro

#define NoConst // const�s�v�̖ڈ�

#pragma endregion

// ���W�A���ϊ��֐��|�C���^
extern float (*const ToRadian)(float);

// �|�C���^�����֐�
template <typename T>
inline void SafeDelete(NoConst T*& p)
{
    if (!p) return;

    delete (p);
    p = nullptr;
}

inline void rotate(
    float& x, float& y,
    const float& cx, const float& cy,
    const float& angle)
{
    x -= cx;
    y -= cy;

    const float cos = cosf(ToRadian(angle));
    const float sin = sinf(ToRadian(angle));
    const float tx = x;
    const float ty = y;

    x = (cos * tx) + (-sin * ty);
    y = (sin * tx) + (cos * ty);

    x += cx;
    y += cy;
};

// float��rand�֐�(�����O��srand�������K�v)
inline const float RandFloat(
    const float max,
    const float min)
{
    const     float randF = static_cast<float>(rand());
    constexpr float randMax = 0x7fff;
    return randF / (randMax / ((max + 1.0f) - min)) + min;
}