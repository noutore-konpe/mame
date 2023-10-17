#pragma once

#include <DirectXMath.h>
#include <stdlib.h>

// ラジアン変換関数ポインタ
extern float (*const ToRadian)(float);

// ポインタ消去関数
template <class T>
inline void SafeDeletePtr(T*& p)
{
    if (nullptr == p) return;

    delete (p);
    p = nullptr;
}


inline void rotate(
    float* x, float* y,
    const float cx, const float cy,
    const float angle)
{
    (*x) -= cx;
    (*y) -= cy;

    const float cos = ::cosf(ToRadian(angle));
    const float sin = ::sinf(ToRadian(angle));
    const float tx = (*x);
    const float ty = (*y);

    (*x) = (cos * tx) + (-sin * ty);
    (*y) = (sin * tx) + (cos * ty);

    (*x) += cx;
    (*y) += cy;
};


// ランダムfloat関数(※事前にsrand処理が必要)
inline const float RandFloat(const float min, const float max)
{
    // 0.0～1.0の間までのランダム値
    static constexpr float randMaxf = 0x7fff;
    const float value = static_cast<float>(::rand()) / randMaxf;

    // min～maxまでのランダム値に変換
    return value * (max - min) + min;
}

// ランダムint関数(※事前にsrand処理が必要)
inline const int RandInt(const int min, const int max)
{
    return ::rand() % ((max + 1) - min) + min;
}