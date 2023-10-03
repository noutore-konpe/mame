#pragma once

#include "State.h"
#include "MagicCircleSummon.h"

namespace MagicCircleState
{
    class DummyState : public State<MagicCircleSummon>
    {
    public:
        DummyState(MagicCircleSummon* magicCircleSummon) : State(magicCircleSummon, "DummyState") {}
        ~DummyState() {}

        void Initialize()                       override {};
        void Update(const float& elapsedTime)   override {};
        void Finalize()                         override {};
    };

    class AppearState : public State<MagicCircleSummon>
    {
    public:
        AppearState(MagicCircleSummon* magicCircleSummon) : State(magicCircleSummon, "AppearState") {}
        ~AppearState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

    private:
        float timer = 0.0f; // easing—p

        float maxTime = 1.0f;
        float scale = 0.0f;
    };

    class ExpandState : public State<MagicCircleSummon>
    {
    public:
        ExpandState(MagicCircleSummon* magicCircleSummon) : State(magicCircleSummon, "ExpandState") {}
        ~ExpandState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

    private:
        float timer = 0.0f;     // easing—p
        float subTimer = 0.0f;  // easing—p

        float maxTime = 0.5f;
        float maxSubTime = 0.5f;
        float scale = 0.0f;
        float posY = 0.0f;
    };

}

