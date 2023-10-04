#include "PlayerState.h"
#include "Player.h"

namespace PlayerState
{
    void NormalState::Initialize()
    {
        owner->PlayWalkAnimation();
    }
    void NormalState::Update(const float& elapsedTime)
    {
        owner->MoveUpdate(elapsedTime);
        if (owner->inputAvoid())
        {
            owner->GetStateMachine()->ChangeState(Player::STATE::AVOID);
        }
    }
    void NormalState::Finalize()
    {
    }

    void AttackState::Initialize()
    {
    }
    void AttackState::Update(const float& elapsedTime)
    {
    }
    void AttackState::Finalize()
    {
    }

    void AvoidState::Initialize()
    {
        owner->PlayAnimation(Player::Animation::Avoid,false);
        owner->SetAcceleration(dodgeSpeed);
    }

    void AvoidState::Update(const float& elapsedTime)
    {
        //owner->MoveUpdate(elapsedTime);
        owner->AvoidUpdate(elapsedTime);
        if (!owner->IsPlayAnimation())
        {
            owner->GetStateMachine()->ChangeState(Player::NORMAL);
        }
    }

    void AvoidState::Finalize()
    {
        owner->SetAcceleration(Player::InitAcceleration);
    }

    void DieState::Initialize()
    {
    }
    void DieState::Update(const float& elapsedTime)
    {
    }
    void DieState::Finalize()
    {
    }
}