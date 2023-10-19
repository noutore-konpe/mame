#pragma once

#include "Enemy.h"

class EnemyGolemResult : public Enemy
{
public: // 定数
    enum class Animation
    {
        Idle,               // 待機
        Landing,            // 着地
        GetUp,              // 起き上がり
        SwingUpArm,         // 腕の振り上げ
        SwingDownArm,       // 腕の振り下げ
        SwingGetUp,         // スイングから戻り
        Walk,               // 歩き
        RoarUp,             // 咆哮準備
        RoarDown,           // 咆哮
        RoarReturn,         // 咆哮からのもどり
        Attack1Tame,        // 攻撃１の溜め
        Attack1,            // 攻撃１
        Attack1Return,      // 攻撃からの戻り
        ComboAttack1,       // コンボ攻撃の一撃目
        ComboAttack2,       // コンボ攻撃の二撃目
        ComboAttack2Return, // コンボ２撃目戻り
        ComboAttack3Up,     // コンボ３撃目振り上げ
        ComboAttack3Down,   // コンボ３撃目振り下げ
        ComboAttack3Return, // コンボ３撃目戻り
        Death0,             // 死亡0
        Death1,             // 死亡1
        Down,               // 怯み倒れ
        DownReturn,         // 怯み起き上がり
        Attack2,            // 攻撃２
    };

public:
    EnemyGolemResult();
    ~EnemyGolemResult() override {};

    void Initialize()                                           override; // 初期化
    void Finalize()                                             override {}; // 終了化
    void Begin()                                                override {}; // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime)                       override; // 更新処理
    void End()                                                  override {}; // 毎フレーム一番最後に呼ばれる
    void Render(const float& scale, ID3D11PixelShader* psShader = nullptr)   override; // 描画処理

    void Render(const float& scale, bool shadow, ID3D11PixelShader* psShader = nullptr) override {};
    void DrawDebug()                                            override; // デバッグ描画

    void UpdateConstants() override;

private:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> golemPS;
};

