#pragma once

#include "Enemy.h"

enum class BossAnimation
{
    Attack,         // 攻撃
    Recoil,         // 壁に当たる
    Confusion,      // 混乱
    DeConfusion,    // 混乱が解ける
    Idle,           // 待機
    AttackBefore,   // 予備動作
    GetAttack,      // くらい
    Dead,           // やられ
    Fall,           // 尻もち（倒れる）
    Cry,            // なく
    Walk,           // 歩き
};

class Boss : public Enemy
{
public:
    Boss();
    Boss(const char* filename); // 仮です

    ~Boss() override;

    void Initialize()                     override; // 初期化
    void Finalize()                       override; // 終了化
    void Begin()                          override; // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime) override; // 更新処理
    void End()                            override; // 毎フレーム一番最後に呼ばれる
    void Render(const float& elapsedTime) override; // 描画処理
    void DrawDebug()                      override; // デバッグ描画

    void OnDead()                         override; // 死亡したときに呼ばれる
    void OnHitWall()                      override; // 壁に当たった時に呼ばれる

    void OnDamaged()override;

public:
    // Imgui名前被り防止のためのやつ //
    static int nameNum;
    const char* GetName() const { return name.c_str(); }
    void SetName(const char* n) { name = n; }
    // Imgui名前被り防止のためのやつ //    


private:
    std::string name;   // Imgui用
};

