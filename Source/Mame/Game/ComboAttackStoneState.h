#pragma once
#include "State.h"
#include "ComboAttackStone.h"

namespace ComboAttackStoneState
{
    class DummyState : public State<ComboAttackStone>
    {
    public:
        DummyState(ComboAttackStone* ComboAttackStone) : State(ComboAttackStone, "DummyState") {}
        ~DummyState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override {};
    };

    class AppearState : public State<ComboAttackStone>
    {
    public:
        AppearState(ComboAttackStone* ComboAttackStone) : State(ComboAttackStone, "AppearState") {}
        ~AppearState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

    private:
        float magicCircleTimer = 0.0f;
        float maxMagicCircleTime = 0.5f;

        float stoneUpTimer = 0.0f;
        float maxStoneUpTime = 0.2f;
    };
}

