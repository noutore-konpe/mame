#pragma once
#include "State.h"
#include "EnemyGolem.h"

namespace EnemyGolemState
{
    // ë“ã@
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

    // ìoèÍ
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
        float maxTime = 0.7f;

        float timer = 0.0f;
        float shakeTime = 2.0f;
        bool isCameraShake = false;
    };

    // ôÙöK
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

        float maxBokehTime0 = 0.5f;
        float maxBokehTime1 = 0.5f;
    };

    // è¢ä´
    class SummonState : public State<EnemyGolem>
    {
    public:
        SummonState(EnemyGolem* enemyGolem) : State(enemyGolem, "SummonState") {}
        ~SummonState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

    private:
        bool isSwingUp = false;     // òrÇÃêUÇËè„Ç∞äÆóπÇµÇΩÇ©
        bool isSwingDown = false;   // òrÇÃêUÇËâ∫Ç∞äÆóπÇµÇΩÇ©

    };

    // ãNÇ´è„Ç™ÇË
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
    };

    class ComboAttack1State : public State<EnemyGolem>
    {
    public:
        ComboAttack1State(EnemyGolem* enemyGolem) : State(enemyGolem, "ComboAttack1State") {}
        ~ComboAttack1State() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

    private:
        bool isComboAttack1 = false;
        bool isMoveFront = false;

        float moveTimer = 0.0f;
        float maxMoveTime = 0.5f;

        float moveFrontTimer = 0.0f;
        float maxMoveFrontTime = 0.4f;
    };
};

