#pragma once

#include "Enemy.h"

#include "MagicCircleGolem.h"
#include "MagicCircleEnemySummon.h"

#include "ComboAttackStone.h"

#include "StateMachine.h"

class EnemyGolem : public Enemy
{
public: // ’θ”
    enum class Animation
    {
        Idle,               // ‘Ò‹@
        Landing,            // ’…’n
        GetUp,              // ‹N‚«γ‚ª‚θ
        SwingUpArm,         // r‚ΜU‚θγ‚°
        SwingDownArm,       // r‚ΜU‚θ‰Ί‚°
        SwingGetUp,         // ƒXƒCƒ“ƒO‚©‚η–ί‚θ
        Walk,               // •ΰ‚«
        RoarUp,             // ™τK€”υ
        RoarDown,           // ™τK
        RoarReturn,         // ™τK‚©‚η‚Μ‚ΰ‚Η‚θ
        Attack1Tame,        // U‚‚P‚Μ—­‚ί
        Attack1,            // U‚‚P
        Attack1Return,      // U‚‚©‚η‚Μ–ί‚θ
        ComboAttack1,       // ƒRƒ“ƒ{U‚‚Μκ‚–Ϊ
        ComboAttack2,       // ƒRƒ“ƒ{U‚‚Μ“ρ‚–Ϊ
        ComboAttack2Return, // ƒRƒ“ƒ{‚Q‚–Ϊ–ί‚θ
        ComboAttack3Up,     // ƒRƒ“ƒ{‚R‚–ΪU‚θγ‚°
        ComboAttack3Down,   // ƒRƒ“ƒ{‚R‚–ΪU‚θ‰Ί‚°
        ComboAttack3Return, // ƒRƒ“ƒ{‚R‚–Ϊ–ί‚θ
        Down0,              // ‹―‚έ“|‚κ‚O
        Down1,              // ‹―‚έ“|‚κ‚P
        DownReturn,         // ‹―‚έ‚©‚η–ί‚θ
        Death,              // €–S
    };

    enum class StateMachineState
    {
        IdleState,          // ‘Ò‹@
        EntryState,         // “oκ
        RoarState,          // ™τK
        SummonState,        // Ά«
        GetUpState,         // ‹N‚«γ‚ª‚θ
        Attack1State,       // U‚‚P
        ComboAttack1State,  // ƒRƒ“ƒ{U‚‚P
        DownState,          // ‹―‚έ
        ComboAttack2State,  // ƒRƒ“ƒ{U‚‚Q
        ChoseState,         // ‘I‘πƒXƒe[ƒg
        DeathState,         // €–S
    };

    const DirectX::XMFLOAT4 magicCircleColor[10] =
    {
        { 0.0f, 0.0f, 1.0f , 1.0f },    // Β
        { 0.80f, 0.44f, 0.24f, 1.0f },
        { 0.54f, 0.27f, 0.07f, 1.0f },   // saddleBrown
        { 0.44f, 0.36f, 0.12f, 1.0f },   // ις’ƒ
        { 0.34f, 0.21f, 0.13f, 1.0f },   // •’ƒ
        { 0.44f, 0.36f, 0.12f, 1.0f },   // ις’ƒ
    };

#ifdef _DEBUG
    const char* stateName[8] =
    {
        "Entry",
        "Summon",
        "Roar",
        "Attack1State",
        "ComboAttack1State",
        "DownState",
        "ComboAttack2State",
        "DeathState"
    };
#endif // _DEBUG

public:
    EnemyGolem();
    ~EnemyGolem() override;

    void Initialize()                                           override; // ‰ϊ‰»
    void Finalize()                                             override; // I—Ή‰»
    void Begin()                                                override; // –ƒtƒ[ƒ€κ”ΤΕ‰‚ΙΔ‚Ξ‚κ‚ι
    void Update(const float& elapsedTime)                       override; // XV—
    void End()                                                  override; // –ƒtƒ[ƒ€κ”ΤΕγ‚ΙΔ‚Ξ‚κ‚ι
    void Render(const float& scale, ID3D11PixelShader* psShader = nullptr)   override; // •`‰ζ—
    void Render(const float& scale, bool shadow, ID3D11PixelShader* psShader = nullptr);
    void DrawDebug()                                            override; // ƒfƒoƒbƒO•`‰ζ

    void UpdateConstants() override;

public:// ζ“ΎEέ’θ
    // ƒXƒe[ƒgƒ}ƒVƒ“
    StateMachine<State<EnemyGolem>>* GetStateMachine() const { return stateMachine.get(); }

    void SetCurrentState(int state) { currentState = state; }
    int GetCurrentState() { return currentState; }

public:
    // Ά«–‚–@wXV—
    void UpdateSummoningMagicCircle(const float& lengthX, const float& lengthZ, const float& angle);

public:
    std::unique_ptr<MagicCircleGolem> magicCircleGolem;
    std::unique_ptr<MagicCircleEnemySummon> magicCircleEnemySummon;
    std::unique_ptr<ComboAttackStone> comboAttackStone;
    std::unique_ptr<ComboAttackStone> comboAttackStones[3];

private:
    // ƒXƒe[ƒgƒ}ƒVƒ“
    std::unique_ptr<StateMachine<State<EnemyGolem>>> stateMachine = nullptr;

    Microsoft::WRL::ComPtr<ID3D11PixelShader> golemPS;

    int currentState = 0;

#ifdef _DEBUG
    int currentStateDebug = 0;
#endif // _DEBUG
};

