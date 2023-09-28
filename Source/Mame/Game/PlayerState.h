#pragma once
#include "State.h"

class Player;
namespace PlayerState
{
    //�ړ��A�ҋ@���܂߂��X�e�[�g
    class NormalState : public State<Player>
    {
        NormalState(Player* player) : State(player, "player") {}
        ~NormalState() {}

        void Initialize() override;
        void Update(const float& elapsedTime) override;
        void Finalize() override;
    };

    //�U��
    class AttackState : public State<Player>
    {
        AttackState(Player* player) : State(player, "player") {}
        ~AttackState() {}

        void Initialize() override;
        void Update(const float& elapsedTime) override;
        void Finalize() override;
    };

    //���
    class AvoidState : public State<Player>
    {
        AvoidState(Player* player) : State(player, "player") {}
        ~AvoidState() {}

        void Initialize() override;
        void Update(const float& elapsedTime) override;
        void Finalize() override;
    };

    //���S
    class DieState : public State<Player>
    {
        DieState(Player* player) : State(player, "player") {}
        ~DieState() {}

        void Initialize() override;
        void Update(const float& elapsedTime) override;
        void Finalize() override;
    };
}