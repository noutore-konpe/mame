#pragma once

#include "Enemy.h"

class EnemyGolemResult : public Enemy
{
public: // ’è”
    enum class Animation
    {
        Idle,               // ‘Ò‹@
        Landing,            // ’…’n
        GetUp,              // ‹N‚«ã‚ª‚è
        SwingUpArm,         // ˜r‚ÌU‚èã‚°
        SwingDownArm,       // ˜r‚ÌU‚è‰º‚°
        SwingGetUp,         // ƒXƒCƒ“ƒO‚©‚ç–ß‚è
        Walk,               // •à‚«
        RoarUp,             // ™ôšK€”õ
        RoarDown,           // ™ôšK
        RoarReturn,         // ™ôšK‚©‚ç‚Ì‚à‚Ç‚è
        Attack1Tame,        // UŒ‚‚P‚Ì—­‚ß
        Attack1,            // UŒ‚‚P
        Attack1Return,      // UŒ‚‚©‚ç‚Ì–ß‚è
        ComboAttack1,       // ƒRƒ“ƒ{UŒ‚‚ÌˆêŒ‚–Ú
        ComboAttack2,       // ƒRƒ“ƒ{UŒ‚‚Ì“ñŒ‚–Ú
        ComboAttack2Return, // ƒRƒ“ƒ{‚QŒ‚–Ú–ß‚è
        ComboAttack3Up,     // ƒRƒ“ƒ{‚RŒ‚–ÚU‚èã‚°
        ComboAttack3Down,   // ƒRƒ“ƒ{‚RŒ‚–ÚU‚è‰º‚°
        ComboAttack3Return, // ƒRƒ“ƒ{‚RŒ‚–Ú–ß‚è
        Death0,             // €–S0
        Death1,             // €–S1
        Down,               // ‹¯‚İ“|‚ê
        DownReturn,         // ‹¯‚İ‹N‚«ã‚ª‚è
        Attack2,            // UŒ‚‚Q
    };

public:
    EnemyGolemResult();
    ~EnemyGolemResult() override {};

    void Initialize()                                           override; // ‰Šú‰»
    void Finalize()                                             override {}; // I—¹‰»
    void Begin()                                                override {}; // –ˆƒtƒŒ[ƒ€ˆê”ÔÅ‰‚ÉŒÄ‚Î‚ê‚é
    void Update(const float& elapsedTime)                       override; // XVˆ—
    void End()                                                  override {}; // –ˆƒtƒŒ[ƒ€ˆê”ÔÅŒã‚ÉŒÄ‚Î‚ê‚é
    void Render(const float& scale, ID3D11PixelShader* psShader = nullptr)   override; // •`‰æˆ—

    void Render(const float& scale, bool shadow, ID3D11PixelShader* psShader = nullptr) override {};
    void DrawDebug()                                            override; // ƒfƒoƒbƒO•`‰æ

    void UpdateConstants() override;

private:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> golemPS;
};

