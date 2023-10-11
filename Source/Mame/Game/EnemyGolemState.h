#pragma once
#include "State.h"
#include "EnemyGolem.h"

namespace EnemyGolemState
{
    // �_�~�[
    class DummyState : public State<EnemyGolem>
    {
    public:
        DummyState(EnemyGolem* enemyGolem) : State(enemyGolem, "DummyState") {}
        ~DummyState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override {};
        void Finalize()                         override {};
    };

    // �ҋ@
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

    // �o��
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

    // ���K
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

    // ����
    class SummonState : public State<EnemyGolem>
    {
    public:
        SummonState(EnemyGolem* enemyGolem) : State(enemyGolem, "SummonState") {}
        ~SummonState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

    private:
        bool isSwingUp = false;     // �r�̐U��グ����������
        bool isSwingDown = false;   // �r�̐U�艺������������

        float delayTimer = 0.0f;
        float maxDelayTime = 2.0f;
    };

    // �N���オ��
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
        bool isDown0 = false;
        bool isDown1 = false;
        bool isReturn = false;
        
        float getUpTimer = 0.0f;
        float maxGetUpTimer = 2.5f; // �N���オ��܂ł̎���
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

    private:// �萔
        enum class STATE
        {
            Summon,         // ����
            Attack,         // �U��
            ComboAttack1,   // �R���{�U���P
            ComboAttack2,   // �R���{�U���Q
            
            Max,
        };

    private:
        bool isState[static_cast<UINT>(STATE::Max)];
    };

    class DeathState : public State<EnemyGolem>
    {
    public:
        DeathState(EnemyGolem* enemyGolem) : State(enemyGolem, "DeathState") {}
        ~DeathState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;
    };

    class WalkState : public State<EnemyGolem>
    {
    public:
        WalkState(EnemyGolem* enemyGolem) : State(enemyGolem, "WalkState") {}
        ~WalkState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

        void JudgeChangeState();                // �X�e�[�g����
        void Move(const float& elapsedTime);    // �ړ�
        void Turn(const float& elapsedTime);    // ��]

    private:
        bool isChangeState = false;

        float changeStateLength = 7.0f; // �X�e�[�g�؂�ւ��̂��߂̃v���C���[�Ƃ̋���

        float moveSpeed = 0.0f;
        float maxMoveSpeed = 3.0f;
    };
};

