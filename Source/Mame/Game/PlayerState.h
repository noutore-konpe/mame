#pragma once
#include "State.h"

class Player;
namespace PlayerState
{
    //移動、待機を含めたステート
    class NormalState : public State<Player>
    {
        NormalState(Player* player) : State(player, "player") {}
        ~NormalState() {}

        void Initialize() override;
        void Update(const float& elapsedTime) override;
        void Finalize() override;
    };

    //攻撃
    class AttackState : public State<Player>
    {
        AttackState(Player* player) : State(player, "player") {}
        ~AttackState() {}

        void Initialize() override;
        void Update(const float& elapsedTime) override;
        void Finalize() override;
    };

    //回避
    class AvoidState : public State<Player>
    {
        AvoidState(Player* player) : State(player, "player") {}
        ~AvoidState() {}

        void Initialize() override;
        void Update(const float& elapsedTime) override;
        void Finalize() override;
    };

    //死亡
    class DieState : public State<Player>
    {
        DieState(Player* player) : State(player, "player") {}
        ~DieState() {}

        void Initialize() override;
        void Update(const float& elapsedTime) override;
        void Finalize() override;
    };
}