#pragma once
#include "State.h"

class Player;
namespace PlayerState
{
    //�ړ��A�ҋ@���܂߂��X�e�[�g
    class NormalState : public State<Player>
    {
    public:
        NormalState(Player* player) : State(player, "Normal") {}
        ~NormalState() {}

        void Initialize() override;
        void Update(const float& elapsedTime) override;
        void Finalize() override;
    };

    //�U��
    class JabAttackState : public State<Player>
    {
    public:
        JabAttackState(Player* player) : State(player, "Jab Attack") {}
        ~JabAttackState() {}

        void Initialize() override;
        void Update(const float& elapsedTime) override;
        void Finalize() override;

    private:
        void HitCollisionUpdate();//���菈����Player�ł����������������...
        void AttackUpdate(int dodgeCanselFrame,int comboCanselFrame);

    private:
        enum ATTACK_STATE
        {
            UPDATE_FRAME,//�L�����Z���s��
            DODGE_CANSEL_FRAME,//����L�����Z���\
            COMBO_AND_DODGE_CANSEL_FRAME,//�A���A����L�����Z���\
        };
        int state;

        int combo;//���݂̘A����
        bool initialize;//�R���{���̏����������t���O

        bool collisionOn;//���菈�������邩

        const float dodgeCanselFrame1 = 10.0f;
        const float dodgeCanselFrame2 = 10.0f;
        const float dodgeCanselFrame3 = 10.0f;
        const float comboCanselFrame1 = 20.0f;
        const float comboCanselFrame2 = 20.0f;
        const float comboCanselFrame3 = 20.0f;
    };

    //���
    class AvoidState : public State<Player>
    {
    public:
        AvoidState(Player* player) : State(player, "Avoid") {}
        ~AvoidState() {}

        void Initialize() override;
        void Update(const float& elapsedTime) override;
        void Finalize() override;

    private:
        float dodgeSpeed  = 20.0f;
    };

    //���S
    class DieState : public State<Player>
    {
    public:
        DieState(Player* player) : State(player, "Die") {}
        ~DieState() {}

        void Initialize() override;
        void Update(const float& elapsedTime) override;
        void Finalize() override;
    };
}