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
        //���ݍ��ݏ���
        owner->AttackSteppingUpdate(elapsedTime);

        switch (combo)
        {
        case 0: //1����
            //����������
            if (!initialize)
            {
                owner->ResetSteppingTimer();
                state = UPDATE_FRAME;
                owner->PlayAnimation(Player::Animation::Jab_1, false, owner->GetAttackSpeed());
                initialize = true;
            }

            //�X�V����(���̃R���{�ւ̑J�ڂ����Ă���)
            {
                AttackUpdate(dodgeCanselFrame1,comboCanselFrame1);
            }

            
            break;
        case 1://2����
            //����������
            if (!initialize)
            {
                owner->ResetSteppingTimer();
                state = UPDATE_FRAME;
                owner->PlayAnimation(Player::Animation::Jab_2, false, owner->GetAttackSpeed());
                initialize = true;
            }

            //�X�V����(���̃R���{�ւ̑J�ڂ����Ă���)
            {
                AttackUpdate(dodgeCanselFrame2, comboCanselFrame2);
            }

            break;
        case 2://3����
            //����������
            if (!initialize)
            {
                owner->ResetSteppingTimer();
                state = UPDATE_FRAME;
                owner->PlayAnimation(Player::Animation::Jab_2, false, owner->GetAttackSpeed());
                initialize = true;
            }

            //�X�V����(���̃R���{�ւ̑J�ڂ����Ă���)
            {
                AttackUpdate(dodgeCanselFrame3, comboCanselFrame3);
            }

            break;
        }

        //�I�������@�ʏ�s���֑J��
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
            if (owner->InputAvoid())//����L�����Z��
            {
                owner->ChangeState(Player::STATE::AVOID);
            }

            if (keyframeIndex > comboCanselFrame)
            {
                state++;
            }
            break;
        case COMBO_AND_DODGE_CANSEL_FRAME:
            if (owner->InputAvoid())//����L�����Z��
            {
                owner->ChangeState(Player::STATE::AVOID);
            }

            else if (owner->InputJabAttack())//�R���{���s
            {
                combo++;
                initialize = false;

                //�R���{�͂R�A���ȍ~�Ȃ��̂Ő���
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