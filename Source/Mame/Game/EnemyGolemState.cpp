#include "EnemyGolemState.h"

// IdleState
namespace EnemyGolemState
{
    void IdleState::Initialize()
    {

    }

    void IdleState::Update(const float& elapsedTime)
    {

    }

    void IdleState::Finalize()
    {

    }
}

// EntryState
namespace EnemyGolemState
{
    void EntryState::Initialize()
    {

    }

    void EntryState::Update(const float& elapsedTime)
    {

    }

    void EntryState::Finalize()
    {

    }
}

// RoarState
namespace EnemyGolemState
{
    void RoarState::Initialize()
    {

    }

    void RoarState::Update(const float& elapsedTime)
    {

    }

    void RoarState::Finalize()
    {

    }
}

// SummonState
namespace EnemyGolemState
{
    void SummonState::Initialize()
    {
    }

    void SummonState::Update(const float& elapsedTime)
    {
        // �������@�w�X�V����
        owner->UpdateSummoningMagicCircle(4.0f, 3.0f, DirectX::XMConvertToRadians(45));

#if 0 // ���@�w�����i��񂾂��Ăяo���j
        owner->magicCircleEnemySummon->GetStateMachine()->ChangeState(static_cast<UINT>(MagicCircleEnemySummon::StateMachineState::AppearState));
#endif
    }

    void SummonState::Finalize()
    {
    }
}

namespace EnemyGolemState
{

}

namespace EnemyGolemState
{

}