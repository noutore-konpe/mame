#pragma once
#include "State.h"
#include "EnemyGolem.h"

namespace EnemyGolemState
{
    // ダミー
    class DummyState : public State<EnemyGolem>
    {
    public:
        DummyState(EnemyGolem* enemyGolem) : State(enemyGolem, "DummyState") {}
        ~DummyState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override {};
        void Finalize()                         override {};
    };

    // 待機
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

    // 登場
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

        // ゲームパッド振動強さ
        float gamePadVibPower = 1.0f;
    };

    // 咆哮
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

        // ゲームパッド振動強さ
        float gamePadVibPower = 0.5f;
    };

    // 召喚
    class SummonState : public State<EnemyGolem>
    {
    public:
        SummonState(EnemyGolem* enemyGolem) : State(enemyGolem, "SummonState") {}
        ~SummonState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

    private:
        bool isSwingUp = false;     // 腕の振り上げ完了したか
        bool isSwingDown = false;   // 腕の振り下げ完了したか

        float delayTimer = 0.0f;
        float maxDelayTime = 2.0f;

        // ゲームパッド振動強さ
        float gamePadVibPower = 0.5f;
    };

    // 起き上がり
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

        // ゲームパッド振動強さ
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

        // ゲームパッド振動強さ
        float gamePadVibPower = 0.3f;
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
        float maxGetUpTimer = 2.5f; // 起き上がるまでの時間
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
    };

    class ChoseState : public State<EnemyGolem>
    {
    public:
        ChoseState(EnemyGolem* enemyGolem) : State(enemyGolem, "ChoseState") {}
        ~ChoseState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

    private:// 定数
        enum class STATE
        {
            Summon,         // 召喚
            Attack,         // 攻撃
            ComboAttack1,   // コンボ攻撃１
            ComboAttack2,   // コンボ攻撃２
            Roar,           // 咆哮
            
            Max,
        };

    private:
        bool isState[static_cast<UINT>(STATE::Max)] = {};

        int currentNum = 0;
        int resetNum = 3; // 上限回数

        int setState = 0;
    };

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
    };

    class WalkState : public State<EnemyGolem>
    {
    public:
        WalkState(EnemyGolem* enemyGolem) : State(enemyGolem, "WalkState") {}
        ~WalkState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

        void JudgeChangeState();                // ステート判定
        void Move(const float& elapsedTime);    // 移動
        void Turn(const float& elapsedTime);    // 回転

    private:
        bool isChangeState = false;

        float changeStateLength = 7.0f; // ステート切り替えのためのプレイヤーとの距離

        float moveSpeed = 0.0f;
        float maxMoveSpeed = 3.0f;
    };
};

