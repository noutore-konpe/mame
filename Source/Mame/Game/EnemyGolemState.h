#pragma once
#include "State.h"
#include "EnemyGolem.h"

namespace EnemyGolemState
{
    // ƒ_ƒ~[
    class DummyState : public State<EnemyGolem>
    {
    public:
        DummyState(EnemyGolem* enemyGolem) : State(enemyGolem, "DummyState") {}
        ~DummyState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override {};
        void Finalize()                         override {};
    };

    // ‘Ò‹@
    class IdleState : public State<EnemyGolem>
    {
    public:
        IdleState(EnemyGolem* enemyGolem) : State(enemyGolem, "IdleState") {}
        ~IdleState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

    private:
        float timer = 0.0f;
    };

    // “oê
    class EntryState : public State<EnemyGolem>
    {
    public:
        EntryState(EnemyGolem* enemyGolem) : State(enemyGolem, "EntryState") {}
        ~EntryState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

    private:
        float landingTimer = 0.0f;
        float maxTime = 2.0f;
        //float maxTime = 0.7f;

        float timer = 0.0f;
        float shakeTime = 2.0f;
        bool isCameraShake = false;

        // ƒQ[ƒ€ƒpƒbƒhU“®‹­‚³
        float gamePadVibPower = 1.0f;
    };

    // ™ôšK
    class RoarState : public State<EnemyGolem>
    {
    public:
        RoarState(EnemyGolem* enemyGolem) : State(enemyGolem, "RoarState") {}
        ~RoarState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

    private:
        bool isRoarUp = false;
        bool isRoarDown = false;

        bool isBokeh = false;

        bool isReturn = false;

        float animationTimer = 0.0f;

        float bokehTimer0 = 0.0f;
        float bokehTimer1 = 0.0f;

        float maxBokehTime0 = 1.0f;
        float maxBokehTime1 = 1.0f;

        // ƒQ[ƒ€ƒpƒbƒhU“®‹­‚³
        float gamePadVibPower = 0.5f;
    };

    // ¢Š«
    class SummonState : public State<EnemyGolem>
    {
    public:
        SummonState(EnemyGolem* enemyGolem) : State(enemyGolem, "SummonState") {}
        ~SummonState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

    private:
        bool isSwingUp = false;     // ˜r‚ÌU‚èã‚°Š®—¹‚µ‚½‚©
        bool isSwingDown = false;   // ˜r‚ÌU‚è‰º‚°Š®—¹‚µ‚½‚©

        float delayTimer = 0.0f;
        float maxDelayTime = 2.5f;

        // ƒQ[ƒ€ƒpƒbƒhU“®‹­‚³
        float gamePadVibPower = 0.5f;
    };

    // ‹N‚«ã‚ª‚è
    class GetUpState : public State<EnemyGolem>
    {
    public:
        GetUpState(EnemyGolem* enemyGolem) : State(enemyGolem, "GetUpState") {}
        ~GetUpState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;
    };

    class Attack1State : public State<EnemyGolem>
    {
    public:
        Attack1State(EnemyGolem* enemyGolem) : State(enemyGolem, "Attack1State") {}
        ~Attack1State() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

    private:
        bool isAttack1_tame = false;
        bool isAttack1 = false;
        bool isShakeCamera = false;
        bool isReturn = false;

        float animationTimer = 0.0f;
        float returnTimer = 0.0f;
        float returnTimer1 = 0.0f;

        // ƒQ[ƒ€ƒpƒbƒhU“®‹­‚³
        float gamePadVibPower = 0.3f;
    };

    class ComboAttack1State : public State<EnemyGolem>
    {
    public:
        ComboAttack1State(EnemyGolem* enemyGolem) : State(enemyGolem, "ComboAttack1State") {}
        ~ComboAttack1State() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

        void ComboAttack1(const float& elapsedTime);
        void ComboAttack2(const float& elapsedTime);
        void ComboAttack3(const float& elapsedTime);

        bool MoveFront(const float& elapsedTime, const float& addLength, const float& maxMoveFrontTime);
        void Turn(const float& elapsedTime);

    private:
        bool isComboAttack1         = false;
        bool isMoveFrontAttack1     = false;
        bool isMoveFrontAttack2     = false;
        bool isComboAttack2         = false;
        bool isComboAttack2Return   = false;
        bool isComboAttack3Up       = false;
        bool isComboAttack3Down     = false;
        bool isComboAttack3Return   = false;

        bool isStoneCreate = false;
        bool isStoneCreated = false;
        float stoneTimer = 0.0f;

        float moveTimer         = 0.0f;
        float maxMoveTime1      = 0.5f;
        float maxMoveTime2      = 0.2f;

        float moveFrontTimer    = 0.0f;

        float myTrunSpeed = DirectX::XMConvertToRadians(180);

        // ƒQ[ƒ€ƒpƒbƒhU“®‹­‚³
        float gamePadVibPower = 0.3f;

        float rotateSpeed = 50.0f;  // ‰ñ“]‘¬“x
    };

    class DownState : public State<EnemyGolem>
    {
    public:
        DownState(EnemyGolem* enemyGolem) : State(enemyGolem, "DownState") {}
        ~DownState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

    private:
        bool isDown = false;
        bool isReturn = false;
        
        float getUpTimer = 0.0f;
        float maxGetUpTimer = 2.5f; // ‹N‚«ã‚ª‚é‚Ü‚Å‚ÌŠÔ
    };

    class ComboAttack2State : public State<EnemyGolem>
    {
    public:
        ComboAttack2State(EnemyGolem* enemyGolem) : State(enemyGolem, "ComboAttack2State") {}
        ~ComboAttack2State() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

        void AttackInitialize();
        void AttackUpdate(const float& elapsedTime);
        void Turn(const float& elapsedTime);

    private:
        bool isComboAttackUp = false;
        bool isComboAttackDown = false;
        bool isComboAttackReturn = false;

        int num = 0;
        int maxNum = 3;

        float delayTimer = 0.0f;
        float maxDelayTime = 1.0f;

        float rotateSpeed = 50.0f;
    };

    // ‘I‘ğƒXƒe[ƒg
    class ChoseState : public State<EnemyGolem>
    {
    public:
        ChoseState(EnemyGolem* enemyGolem) : State(enemyGolem, "ChoseState") {}
        ~ChoseState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

    private:// ’è”
        enum class STATE
        {
            Summon,         // ¢Š«
            Attack,         // UŒ‚
            ComboAttack1,   // ƒRƒ“ƒ{UŒ‚‚P
            ComboAttack2,   // ƒRƒ“ƒ{UŒ‚‚Q
            Roar,           // ™ôšK
            Attack2,        // UŒ‚‚Q

            Max,
        };

    private:
        bool isState[static_cast<UINT>(STATE::Max)] = {};

        int currentNum = 0;
        int resetNum = 3; // ãŒÀ‰ñ”

        int setState = 0;

        int oldState = 0;
    };

    // €–S
    class DeathState : public State<EnemyGolem>
    {
    public:
        DeathState(EnemyGolem* enemyGolem) : State(enemyGolem, "DeathState") {}
        ~DeathState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

    private:
        bool isDeath0 = false;
        bool isDeath1 = false;

        float delayTimer = 0.0f;
        float maxDelay = 0.2f;
        bool isDelay = false;

        float cameraShakeTimer = 0;
        bool isCameraShake = false;
        float maxTimer = 0.2f;

        float deathTimer = 0.0f;
        bool isDestroy = false;
    };

    // •à‚«
    class WalkState : public State<EnemyGolem>
    {
    public:
        WalkState(EnemyGolem* enemyGolem) : State(enemyGolem, "WalkState") {}
        ~WalkState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

        void JudgeChangeState();                // ƒXƒe[ƒg”»’è
        void Move(const float& elapsedTime);    // ˆÚ“®
        void Turn(const float& elapsedTime);    // ‰ñ“]

    private:
        bool isChangeState = false;

        float changeStateLength = 7.0f; // ƒXƒe[ƒgØ‚è‘Ö‚¦‚Ì‚½‚ß‚ÌƒvƒŒƒCƒ„[‚Æ‚Ì‹——£

        float moveSpeed = 0.0f;
        float maxMoveSpeed = 3.0f;

        float rotateSpeed = 100.0f; // ‰ñ“]‘¬“x

        float walkTimer = 0.0f;
    };

    // UŒ‚‚Q
    class Attack2State : public State<EnemyGolem>
    {
    public:
        Attack2State(EnemyGolem* enemyGolem) : State(enemyGolem, "Attack2State") {}
        ~Attack2State() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

        void Turn(const float& elapsedTime);    // ‰ñ“]

    private:
        float rotateSpeed = 100.0f; // ‰ñ“]‘¬“x
    };

    // UŒ‚ƒfƒBƒŒƒC
    class DelayState : public State<EnemyGolem>
    {
    public:
        DelayState(EnemyGolem* enemyGolem) : State(enemyGolem, "DelayState") {}
        ~DelayState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;
    private:
        float delayTimer = 0.0f;
        float maxDelay = 2.0f;
    };

    class EyeEmissiveDownState : public State<EnemyGolem>
    {
    public:
        EyeEmissiveDownState(EnemyGolem* enemyGolem) :State(enemyGolem, "EyeEmissiveDownState") {}
        ~EyeEmissiveDownState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;
        
    private:
        float timer = 0.0f;
    };

    class EyeEmissiveUpState : public State<EnemyGolem>
    {
    public:
        EyeEmissiveUpState(EnemyGolem* enemyGolem) :State(enemyGolem, "EyeEmissiveUpState") {}
        ~EyeEmissiveUpState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

    private:
        float timer = 0.0f;
    };
};