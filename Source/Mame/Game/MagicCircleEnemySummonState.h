#pragma once
#include "State.h"
#include "MagicCircleEnemySummon.h"

namespace MagicCircleEnemySummonState
{
    class DummyState : public State<MagicCircleEnemySummon>
    {
    public:
        DummyState(MagicCircleEnemySummon* magicCircleEnemySummon) : State(magicCircleEnemySummon, "DummyState") {}
        ~DummyState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override {};
        void Finalize()                         override {};
    };

    class AppearState :public State<MagicCircleEnemySummon>
    {
    public:
        AppearState(MagicCircleEnemySummon* magicCircleEnemySummon) : State(magicCircleEnemySummon, "AppearState") {}
        ~AppearState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

    private:
        float timer = 0.0f;     // easing用
        float subTimer1 = 0.0f;  // easing用
        float subTimer2 = 0.0f; // easing用
        float subTimer3 = 0.0f; // easing用

        float maxTime = 0.7f;
        float maxSubTime1 = 0.2f;
        float maxSubTime2 = 0.2f;
        float maxSubTime3 = 0.2f;

        float scale = 0.0f;
        float posY = 0.0f;
    };

    class CreateState : public State<MagicCircleEnemySummon>
    {
    public:
        CreateState(MagicCircleEnemySummon* magicCircleEnemySummon) : State(magicCircleEnemySummon, "AppearState") {}
        ~CreateState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

    private:
        float timer = 0.0f;     // easing用
        float subTimer = 0.0f;  // easing用

        float maxTime = 0.5f;
        float maxSubTime = 0.3f;

        float posY[3] = {};
        float scale[3] = {};

        float baseScale = 0.0f; // 魔法陣の一番下のbase
        float scaleTimer = 0.0f;
        float maxScaleTime = 0.4f;
    };

};

