#pragma once
#include "Character.h"

class Enemy : public Character
{
public:
    Enemy() {}
    virtual ~Enemy() {};

    virtual void Initialize();                                          // 初期化
    virtual void Finalize() = 0;                                        // 終了化
    virtual void Begin() = 0;                                           // 毎フレーム一番最初に呼ばれる
    virtual void Update(const float& elapsedTime);                      // 更新処理
    virtual void End() = 0;                                             // 毎フレーム一番最後に呼ばれる
    virtual void Render(const float& elapsedTime, const float& scale);  // 描画処理
    virtual void DrawDebug() = 0;                                       // デバッグ描画


private:

};

