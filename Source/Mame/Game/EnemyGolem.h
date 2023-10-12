#pragma once

#include "Enemy.h"

#include "MagicCircleGolem.h"
#include "MagicCircleEnemySummon.h"

#include "ComboAttackStone.h"

#include "StateMachine.h"

class EnemyGolem : public Enemy
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
    };

    enum class StateMachineState
    {
        IdleState,          // 待機
        EntryState,         // 登場
        RoarState,          // 咆哮
        SummonState,        // 召喚
        GetUpState,         // 起き上がり
        Attack1State,       // 攻撃１
        ComboAttack1State,  // コンボ攻撃１
        DownState,          // 怯み
        ComboAttack2State,  // コンボ攻撃２
        ChoseState,         // 選択ステート
        DeathState,         // 死亡
        WalkState,          // 歩き
    };

    const DirectX::XMFLOAT4 magicCircleColor[10] =
    {
        { 0.0f, 0.0f, 1.0f , 1.0f },    // 青
        { 0.80f, 0.44f, 0.24f, 1.0f },
        { 0.54f, 0.27f, 0.07f, 1.0f },   // saddleBrown
        { 0.44f, 0.36f, 0.12f, 1.0f },   // 鶯茶
        { 0.34f, 0.21f, 0.13f, 1.0f },   // 黒茶
        { 0.44f, 0.36f, 0.12f, 1.0f },   // 鶯茶
    };

#ifdef _DEBUG
    const char* stateName[9] =
    {
        "Entry",
        "Summon",
        "Roar",
        "Attack1State",
        "ComboAttack1State",
        "DownState",
        "ComboAttack2State",
        "DeathState",
        "WalkState"
    };
#endif // _DEBUG

public:
    EnemyGolem();
    ~EnemyGolem() override;

    void Initialize()                                           override; // 初期化
    void Finalize()                                             override; // 終了化
    void Begin()                                                override; // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime)                       override; // 更新処理
    void End()                                                  override; // 毎フレーム一番最後に呼ばれる
    void Render(const float& scale, ID3D11PixelShader* psShader = nullptr)   override; // 描画処理
    
    void Render(const float& scale, bool shadow, ID3D11PixelShader* psShader = nullptr) override;
    void DrawDebug()                                            override; // デバッグ描画

    void UpdateConstants() override;

public:// 取得・設定
    // ステートマシン
    StateMachine<State<EnemyGolem>>* GetStateMachine() const { return stateMachine.get(); }

    void SetCurrentState(int state) { currentState = state; }
    int GetCurrentState() { return currentState; }

public:
    // 召喚魔法陣更新処理
    void UpdateSummoningMagicCircle(const float& lengthX, const float& lengthZ, const float& angle);

private:
    static int nameNum_;

public:
    std::unique_ptr<MagicCircleGolem> magicCircleGolem;
    std::unique_ptr<MagicCircleEnemySummon> magicCircleEnemySummon;
    std::unique_ptr<ComboAttackStone> comboAttackStone;
    std::unique_ptr<ComboAttackStone> comboAttackStones[3];

private:
    // ステートマシン
    std::unique_ptr<StateMachine<State<EnemyGolem>>> stateMachine = nullptr;

    Microsoft::WRL::ComPtr<ID3D11PixelShader> golemPS;

    int currentState = 0;

#ifdef _DEBUG
    int currentStateDebug = 0;
#endif // _DEBUG
};

