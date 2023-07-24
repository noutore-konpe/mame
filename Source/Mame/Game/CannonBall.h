#pragma once

#include "Character.h"

// 前方宣言（相互インクルードしないようにする）
class CannonBallManager;

class CannonBall : public Character
{
public:
    CannonBall(
        NO_CONST DirectX::XMFLOAT3 newPosition, 
        const float& directionZ, 
        NO_CONST CannonBallManager* manager);
    ~CannonBall();

    void Initialize();                      // 初期化
    void Finalize();                        // 終了化
    void Begin();                           // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime);  // 更新処理
    void End();                             // 毎フレーム一番最後に呼ばれる
    void Render(const float& elapsedTime);  // 描画処理

    void DrawDebug() override;              // デバッグ描画

public:
    // Imgui名前被り防止のためのやつ //
    static int nameNum;
    const char* GetName() const { return name.c_str(); }
    void SetName(const char* n) { name = n; }
    // Imgui名前被り防止のためのやつ //    

    void Destroy();

    static float renderLengthXLimit_;   // 弾丸の描画距離制限

private:
    CannonBallManager* cannonBallManager = nullptr;

    std::string name    = {};   // Imgui用

    float directionZ    = 0.0f;
    float offsetZ       = 3.0f;

    float lifeTimer     = 3.0f; // 寿命タイマー
};

