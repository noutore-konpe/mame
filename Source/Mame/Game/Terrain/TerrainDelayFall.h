#pragma once

#include "Terrain.h"

class TerrainDelayFall : public Terrain
{
public:
    TerrainDelayFall(const char* const filename);
    ~TerrainDelayFall() override {};

    void Initialize()                       override;   // 初期化
    void Update(const float& elapsedTime)   override;   // 更新処理
    void DrawDebug()                        override;   // デバッグ描画

    void OnRiding()                         override;   // 乗られたときに呼ばれる

private:
    float   fallAcceleration_ = -0.025f;  // 落下加速力
    float   fallVelocityMax_  = -25.0f;   // 最大落下速度
    float   blinkTimer_       = 0.0f;     // 点滅タイマー
    float   fallTimer_        = 0.0f;     // 落下するまでの時間

    bool    isReadyFall_      = false;    // 落下する準備をするかどうか
    bool    isFall_           = false;    // 落下するかどうか

};