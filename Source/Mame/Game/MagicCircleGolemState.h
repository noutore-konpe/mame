#pragma once
#include "State.h"
#include "MagicCircleGolem.h"

namespace MagicCircleGolemState
{
    class DummyState : public State<MagicCircleGolem>
    {
    public:
        DummyState(MagicCircleGolem* magicCircleGolem) : State(magicCircleGolem, "DummyState") {}
        ~DummyState() {}

        void Initialize()                       override {};
        void Update(const float& elapsedTime)   override {};
        void Finalize()                         override {};
    };

    class AppearState : public State<MagicCircleGolem>
    {
    public:
        AppearState(MagicCircleGolem* magicCircleGolem) : State(magicCircleGolem, "AppearState") {}
        ~AppearState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

    private:
        float timer = 0.0f; // easing—p

        float maxTime = 1.0f;
        float scale = 0.0f;
    };
}

