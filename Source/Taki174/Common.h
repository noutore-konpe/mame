#pragma once

#include <DirectXMath.h>
#include <stdlib.h>

// ���W�A���ϊ��֐��|�C���^
extern float (*const ToRadian)(float);

// �|�C���^�����֐�
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


// �����_��float�֐�(�����O��srand�������K�v)
inline const float RandFloat(const float min, const float max)
{
    // 0.0�`1.0�̊Ԃ܂ł̃����_���l
    static constexpr float randMaxf = 0x7fff;
    const float value = static_cast<float>(::rand()) / randMaxf;

    // min�`max�܂ł̃����_���l�ɕϊ�
    return value * (max - min) + min;
}

// �����_��int�֐�(�����O��srand�������K�v)
inline const int RandInt(const int min, const int max)
{
    return ::rand() % ((max + 1) - min) + min;
}