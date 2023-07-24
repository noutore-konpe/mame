#pragma once
#include "Enemy.h"

// 豆腐アニメーション
enum TofuAnimation
{
    Walk,
    Turn,
};

// 豆腐君
class EnemyTofu : public Enemy
{
public:
    EnemyTofu();
    ~EnemyTofu() override;

    void Initialize()                      override; // 初期化
    void Finalize()                        override; // 終了化
    void Begin()                           override; // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime)  override; // 更新処理
    void End()                             override; // 毎フレーム一番最後に呼ばれる
    void Render(const float& elapsedTime)  override; // 描画処理
    void DrawDebug()                       override; // デバッグ描画

    void OnLanding()                       override; // 着地時に呼ばれる
    void OnDamaged()                       override; // ダメージを受けたときに呼ばれる
    void OnDead()                          override; // 死亡したときに呼ばれる
    void OnFallDead()                      override; // 落下ミスしたときに呼ばれる

    void OnHitWall()                       override; // 壁に当たった時に呼ばれる
    void OnAttacked()                      override; // 攻撃したときに呼ばれる
    void OnHitFriend()                     override; // 味方に当たった時に呼ばれる

public:
    // Imgui名前被り防止のためのやつ //
    static int nameNum;
    const char* GetName() const { return name.c_str(); }
    void SetName(const char* n) { name = n; }
    // Imgui名前被り防止のためのやつ //

private:
    std::string name = {};

};

