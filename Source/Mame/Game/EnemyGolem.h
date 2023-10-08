#pragma once

#include "Enemy.h"

#include "MagicCircleGolem.h"
#include "MagicCircleEnemySummon.h"

#include "StateMachine.h"

class EnemyGolem : public Enemy
{
public: // 定数
    enum class Animation
    {
        Idle,           // 待機
        Landing,        // 着地
        GetUp,          // 起き上がり
        SwingUpArm,     // 腕の振り上げ
        SwingDownArm,   // 腕の振り下げ
        SwingGetUp,     // スイングから戻り
        Walk,           // 歩き
        RoarUp,         // 咆哮準備
        RoarDown,       // 咆哮
        RoarReturn,     // 咆哮からのもどり
        Attack1_tame,   // 攻撃１の溜め
        Attack1,        // 攻撃１
    };

    enum class StateMachineState
    {
        IdleState,      // 待機
        EntryState,     // 登場
        RoarState,      // 咆哮
        SummonState,    // 召喚
        GetUpState,     // 起き上がり
        Attack1State,   // 攻撃１
    };

    const DirectX::XMFLOAT4 magicCircleColor[10] =
    {
        { 0.80f, 0.44f, 0.24f, 1.0f },
        { 0.54f, 0.27f, 0.07f, 1.0f },   // saddleBrown
        { 0.44f, 0.36f, 0.12f, 1.0f },   // 鶯茶
        { 0.34f, 0.21f, 0.13f, 1.0f },   // 黒茶
        { 0.44f, 0.36f, 0.12f, 1.0f },   // 鶯茶
    };

#ifdef _DEBUG
    const char* stateName[5] =
    {
        "Entry",
        "Summon",
        "Roar",
        "Attack1State",
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
    void Render(const float& scale, bool shadow, ID3D11PixelShader* psShader = nullptr);
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

public:
    std::unique_ptr<MagicCircleGolem> magicCircleGolem;
    std::unique_ptr<MagicCircleEnemySummon> magicCircleEnemySummon;

private:
    // ステートマシン
    std::unique_ptr<StateMachine<State<EnemyGolem>>> stateMachine = nullptr;

    Microsoft::WRL::ComPtr<ID3D11PixelShader> golemPS;

    int currentState = 0;

#ifdef _DEBUG
    int currentStateDebug = 0;
#endif // _DEBUG
};

