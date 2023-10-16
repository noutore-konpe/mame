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
        enum class MAGIC_CIRCLE
        {
            Left,
            Center,
            Right,

            Max,
        };

    public:
        AppearState(MagicCircleGolemAttack2* magicCircleGolemAttack2) : State(magicCircleGolemAttack2, "AppearState") {}
        ~AppearState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

        void UpdateAppear(const float& elapsedTime);

    private:
        float appearTimer[3] = {};
        float maxAppearTime[3] = { 0.5f, 0.4f, 0.3f };

        float delayTimer = {};
        float maxDelayTimer[3] = { 0.4f, 0.5f, 0.7f };

        float rotationSpeed[3] = {};
        bool isAnimationEnd[3] = {};
    };

    class ThrowState : public State<MagicCircleGolemAttack2>
    {
    public:
        enum class MAGIC_CIRCLE
        {
            Left,
            Center,
            Right,

            Max,
        };

    public:
        ThrowState(MagicCircleGolemAttack2* magicCircleGolemAttack2) : State(magicCircleGolemAttack2, "ThrowState") {}
        ~ThrowState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

        void UpdateAlpha(const float& elapsedTime);

    private:
        float length[3] = {};
        DirectX::XMFLOAT3 targetVec[3] = {};

        float stoneFallTimer[3] = {};
        float maxStoneFallTime[3] = { 0.3f,0.3f,0.3f };

        bool isHoming[3] = {};
        
        bool isLunch[3] = {};
        int stoneNum[3] = {};

        bool isOnGround[3][3] = {};
        float alphaTimer[3][3] = {};

        // ÉQÅ[ÉÄÉpÉbÉhêUìÆã≠Ç≥
        float gamePadVibPower = 0.3f;
    };
};

