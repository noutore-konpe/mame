#pragma once
#include "Enemy.h"
#include "CannonBallManager.h"

// 大砲
class EnemyCannon : public Enemy
{
public:
    EnemyCannon();
    ~EnemyCannon() override;

    void Initialize()                       override; // 初期化
    void Finalize()                         override; // 終了化
    void Begin()                            override; // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime)   override; // 更新処理
    void End()                              override; // 毎フレーム一番最後に呼ばれる
    void Render(const float& elapsedTime)   override; // 描画処理
    void DrawDebug()                        override;

private:
    void CollisionCannonBallVsPlayer(); // エネミー弾丸とプレイヤーの衝突判定

public:
    // Imgui名前被り防止のためのやつ //
    static int nameNum;
    const char* GetName() const { return name.c_str(); }
    void SetName(const char* n) { name = n; }
    // Imgui名前被り防止のためのやつ //

private:
    std::string name = {};
};

