#include "Mathf.h"
#include <stdlib.h>

float Mathf::Lerp(const float& a, const float& b, const float& t)
{
    return a * (1.0f - t) + (b * t);
}

float Mathf::RandomRange(const float& min, const float& max)
{
    // 0.0～1.0までのランダム値
    const float randValue = static_cast<float>(rand()) / RAND_MAX;

    // min～maxまでのランダム値に変換
    return min + (max - min) * randValue;
}
