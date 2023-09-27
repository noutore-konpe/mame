#pragma once

#include <DirectXMath.h>
#include <stdlib.h>

// マクロ定数
#pragma region Macro

#define NoConst // const不要の目印

#pragma endregion

// ラジアン変換関数ポインタ
extern float (*const ToRadian)(float);

// ポインタ消去関数
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

// float版rand関数(※事前にsrand処理が必要)
inline const float RandFloat(
    const float max,
    const float min)
{
    const     float randF = static_cast<float>(rand());
    constexpr float randMax = 0x7fff;
    return randF / (randMax / ((max + 1.0f) - min)) + min;
}