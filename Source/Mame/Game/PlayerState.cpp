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
        GamePad& gamePad = Input::Instance().GetGamePad();

        float ax = gamePad.GetAxisLX();
        float ay = gamePad.GetAxisLY();

        owner->MoveUpdate(elapsedTime,ax,ay);
        if (owner->InputAvoid())
        {
            owner->ChangeState(Player::STATE::AVOID);
        }
        else if (owner->InputJabAttack())
        {
            owner->ChangeState(Player::STATE::ATTACK_JAB);
        }
    }
    void NormalState::Finalize()
    {
    }

    void JabAttackState::Initialize()
    {
        combo = 0;
        initialize = false;
    }
    void JabAttackState::Update(const float& elapsedTime)
    {
        //踏み込み処理
        owner->AttackSteppingUpdate(elapsedTime);

        switch (combo)
        {
        case 0: //1撃目
            //初期化処理
            if (!initialize)
            {
                owner->ResetSteppingTimer();
                state = UPDATE_FRAME;
                owner->PlayAnimation(Player::Animation::Jab_1, false, owner->GetAttackSpeed());
                initialize = true;
            }

            //更新処理(次のコンボへの遷移もしている)
            {
                AttackUpdate(dodgeCanselFrame1,comboCanselFrame1);
            }

            
            break;
        case 1://2撃目
            //初期化処理
            if (!initialize)
            {
                owner->ResetSteppingTimer();
                state = UPDATE_FRAME;
                owner->PlayAnimation(Player::Animation::Jab_2, false, owner->GetAttackSpeed());
                initialize = true;
            }

            //更新処理(次のコンボへの遷移もしている)
            {
                AttackUpdate(dodgeCanselFrame2, comboCanselFrame2);
            }

            break;
        case 2://3撃目
            //初期化処理
            if (!initialize)
            {
                owner->ResetSteppingTimer();
                state = UPDATE_FRAME;
                owner->PlayAnimation(Player::Animation::Jab_2, false, owner->GetAttackSpeed());
                initialize = true;
            }

            //更新処理(次のコンボへの遷移もしている)
            {
                AttackUpdate(dodgeCanselFrame3, comboCanselFrame3);
            }

            break;
        }

        //終了処理　通常行動へ遷移
        if (!owner->IsPlayAnimation())
        {
            owner->ChangeState(Player::STATE::NORMAL);
        }
    }
    void JabAttackState::Finalize()
    {
        owner->SetVelocity(DirectX::XMFLOAT3(0, 0, 0));
    }

    void JabAttackState::HitCollisionUpdate()
    {
        if (!collisionOn)return;

    }

    void JabAttackState::AttackUpdate(int dodgeCanselFrame,int comboCanselFrame)
    {
        const float keyframeIndex = owner->model->GetCurrentKeyframeIndex();

        switch (state)
        {
        case UPDATE_FRAME:
            if (keyframeIndex > dodgeCanselFrame)
            {
                state++;
            }
            break;
        case DODGE_CANSEL_FRAME:
            if (owner->InputAvoid())//回避キャンセル
            {
                owner->ChangeState(Player::STATE::AVOID);
            }

            if (keyframeIndex > comboCanselFrame)
            {
                state++;
            }
            break;
        case COMBO_AND_DODGE_CANSEL_FRAME:
            if (owner->InputAvoid())//回避キャンセル
            {
                owner->ChangeState(Player::STATE::AVOID);
            }

            else if (owner->InputJabAttack())//コンボ続行
            {
                combo++;
                initialize = false;

                //コンボは３連撃以降ないので制限
                if (combo > 2)combo = 2;
            }
            break;
        }
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