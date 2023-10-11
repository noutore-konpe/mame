#pragma once
#include "State.h"
#include "ComboAttackStone.h"

namespace ComboAttackStoneState
{
    class DummyState : public State<ComboAttackStone>
    {
    public:
        DummyState(ComboAttackStone* comboAttackStone) : State(comboAttackStone, "DummyState") {}
        ~DummyState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override {};
    };

    class AppearState : public State<ComboAttackStone>
    {
    public:
        AppearState(ComboAttackStone* comboAttackStone) : State(comboAttackStone, "AppearState") {}
        ~AppearState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

        void SetModelPosition();

    private:
        float magicCircleTimer = 0.0f;
        float maxMagicCircleTime = 0.5f;

        float stoneUpTimer = 0.0f;
        float maxStoneUpTime = 0.4f;
    };

    class ThrowState : public State<ComboAttackStone>
    {
    public:
        ThrowState(ComboAttackStone* comboAttackStone) : State(comboAttackStone, "ThrowState") {}
        ~ThrowState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

    private:
        DirectX::XMFLOAT3 targetVec{};
        float length = 0.0f;

        float magicCircleTimer = 0.0f;
        float maxMagicCircleTime = 0.5f;

        float stoneFallTimer = 0.0f;
        float maxStoneFallTime = 0.5f;

        bool cameraShake = false;

        float delayTimer = 0.0f;
        float maxDelayTime = 1.5f;
    };
}

