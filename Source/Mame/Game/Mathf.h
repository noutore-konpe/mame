#pragma once

// 浮動小数算術
class Mathf
{
public:
    // 線形補完(a：変動値　b：目標値　t：補完値（回転速度）(0.0f～1.0f))
    static float Lerp(const float& a, const float& b, const float& t);

    // 指定範囲のランダム値を計算する
    static float RandomRange(const float& min, const float& max);
};

