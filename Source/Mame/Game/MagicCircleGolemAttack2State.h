#pragma once
#include "State.h"
#include "MagicCircleGolemAttack2.h"

namespace MagicCircleGolemAttack2State
{
    class DummyState : public State <MagicCircleGolemAttack2>
    {
    public:
        DummyState(MagicCircleGolemAttack2* magicCircleGolemAttack2) : State(magicCircleGolemAttack2, "DummyState") {}
        ~DummyState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override {};
        void Finalize()                         override {};
    };

    class AppearState : public State<MagicCircleGolemAttack2>
    {
    public:
        AppearState(MagicCircleGolemAttack2* magicCircleGolemAttack2) : State(magicCircleGolemAttack2, "AppearState") {}
        ~AppearState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;
    };
};

