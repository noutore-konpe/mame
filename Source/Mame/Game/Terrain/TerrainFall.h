#pragma once

#include "Terrain.h"

// 落下する地形
class TerrainFall : public Terrain
{
public:
    TerrainFall(const char* const filename);
    ~TerrainFall() override {};

    void Initialize()                       override;   // 初期化
    void Update(const float& elapsedTime)   override;   // 更新処理
    void DrawDebug()                        override;   // デバッグ描画

    void OnRiding()                         override;   // 乗られたときに呼ばれる

private:
    float   fallAcceleration_   = -0.025f;  // 落下加速力
    float   fallVelocityMax_    = -25.0f;   // 最大落下速度
    float   defaultBlinkTime_   = 1.0f;     // 点滅する時間
    float   blinkTimer_         = 0.0f;     // 点滅タイマー
    bool    isFall_             = false;    // 落下するかどうか

};
