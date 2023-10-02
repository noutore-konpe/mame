#pragma once

#include "BaseEnemyAI.h"

class EnemyTestAI : public BaseEnemyAI
{
public:
    EnemyTestAI();
    //~EnemyTestAI() override;

    void Initialize() override;                     // 初期化
    //void Finalize() override;                       // 終了化
    void Update(const float& elapsedTime) override;  // 更新処理
    void Render(const float& scale, ID3D11PixelShader* psShader = nullptr) override; // 描画処理
    //void DrawDebug() override;                      // デバッグ描画

private:
    static int nameNum_;

};

