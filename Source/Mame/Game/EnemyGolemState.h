#pragma once
#include "State.h"
#include "EnemyGolem.h"

namespace EnemyGolemState
{
    // ë“ã@
    class IdleState : public State<EnemyGolem>
    {
    public:
        IdleState(EnemyGolem* enemyGolem) : State(enemyGolem, "IdleState") {}
        ~IdleState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;
    };

    // ìoèÍ
    class EntryState : public State<EnemyGolem>
    {
    public:
        EntryState(EnemyGolem* enemyGolem) : State(enemyGolem, "EntryState") {}
        ~EntryState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;
    };

    // ôÙöK
    class RoarState : public State<EnemyGolem>
    {
    public:
        RoarState(EnemyGolem* enemyGolem) : State(enemyGolem, "RoarState") {}
        ~RoarState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;
    };

    // è¢ä´
    class SummonState : public State<EnemyGolem>
    {
    public:
        SummonState(EnemyGolem* enemyGolem) : State(enemyGolem, "SummonState") {}
        ~SummonState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;
    };
};

