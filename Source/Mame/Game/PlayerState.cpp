#include "PlayerState.h"
#include "Player.h"
#include "PlayerManager.h"

namespace PlayerState
{
    void NormalState::Initialize()
    {
        enableInputButton = false;
        owner->PlayWalkAnimation();
        owner->PlaySwordWalkAnimation();
    }
    void NormalState::Update(const float& elapsedTime)
    {
        GamePad& gamePad = Input::Instance().GetGamePad();

        float ax = gamePad.GetAxisLX();
        float ay = gamePad.GetAxisLY();

        owner->MoveUpdate(elapsedTime,ax,ay);

        //��x����{�^���𗣂�����
        if (owner->InputAvoid() == 0)
        {
            enableInputButton = true;
        }

        if (owner->InputAvoid() && enableInputButton)
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
        owner->isActiveAttackFrame = true;

        hit.clear();
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
                owner->GetSword()->PlayAnimation(Player::Animation::Jab_1, false, owner->GetAttackSpeed());
                hit.clear();//�q�b�g�����G�̏�����
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
                owner->GetSword()->PlayAnimation(Player::Animation::Jab_2, false, owner->GetAttackSpeed());
                hit.clear();//�q�b�g�����G�̏�����
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
                owner->GetSword()->PlayAnimation(Player::Animation::Jab_2, false, owner->GetAttackSpeed());
                hit.clear();//�q�b�g�����G�̏�����
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

        HitCollisionUpdate();
    }

    void JabAttackState::Finalize()
    {
        //owner->model->weight = 0.0f;
        owner->SetVelocity(DirectX::XMFLOAT3(0, 0, 0));
        owner->isActiveAttackFrame = false;
    }

    void JabAttackState::HitCollisionUpdate()
    {
        std::vector<Enemy*> hitEnemies;
        if (PlayerManager::Instance().AttackCollisionPlayerToEnemy(hitEnemies))
        {
            for (auto& enemy : hitEnemies)
            {
                for (auto& h : hit)
                {
                    if (h == enemy)goto skip;//���łɈ�x�q�b�g���Ă���G�Ȃ珈�����Ȃ�
                }
                //��x���q�b�g���Ă��Ȃ��G�Ȃ̂œo�^����
                hit.emplace_back(enemy);

                enemy->ApplyDamage(owner->jabMotionAtkMuls[combo],owner);
                enemy->Flinch();
            skip:;
            }
        }
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

        /*if (state < COMBO_AND_DODGE_CANSEL_FRAME)
        {
            owner->isActiveAttackFrame = true;
        }
        else
        {
            owner->isActiveAttackFrame = false;
        }*/
    }

    void AvoidState::Initialize()
    {
        owner->PlayAnimation(Player::Animation::Avoid,false);
        owner->GetSword()->PlayAnimation(Player::Animation::Avoid,false);
        owner->SetAcceleration(dodgeSpeed);
    }

    void AvoidState::Update(const float& elapsedTime)
    {
        //owner->MoveUpdate(elapsedTime);

        if (!owner->IsPlayAnimation()/* ||
            Input::Instance().GetGamePad().GetTriggerR() == 0*/)
        {
            owner->GetStateMachine()->ChangeState(Player::NORMAL);
            return;
        }

        owner->AvoidUpdate(elapsedTime);
        owner->ModelRotZUpdate(elapsedTime);
    }

    void AvoidState::Finalize()
    {
        owner->SetAcceleration(Player::InitAcceleration);
        owner->GetTransform()->SetRotationZ(0);
    }

    void DieState::Initialize()
    {
        owner->SetVelocity(DirectX::XMFLOAT3(0, 0, 0));
        owner->PlayAnimation(Player::Animation::HardStagger,false);
    }
    void DieState::Update(const float& elapsedTime)
    {
        owner->BlownUpdate(elapsedTime);
    }
    void DieState::Finalize()
    {
    }

    void HardAttackState::Initialize()
    {

    }
    void HardAttackState::Update(const float& elapsedTime)
    {

    }
    void HardAttackState::Finalize()
    {

    }

    void SoftStaggerState::Initialize()
    {
        owner->PlayAnimation(Player::Animation::SoftStagger, false,1.5f);
        //owner->isInvincible = true;//���G
        owner->SetVelocity(DirectX::XMFLOAT3(0, 0, 0));
    }
    void SoftStaggerState::Update(const float& elapsedTime)
    {
        if (!owner->IsPlayAnimation())
        {
            owner->ChangeState(Player::STATE::NORMAL);
        }
    }
    void SoftStaggerState::Finalize()
    {
        //owner->isInvincible = false;
    }

    void HardStaggerState::Initialize()
    {
    }
    void HardStaggerState::Update(const float& elapsedTime)
    {
    }
    void HardStaggerState::Finalize()
    {
    }

    void CounterState::Initialize()
    {
        counterCompleted = false;
        owner->PlayAnimation(Player::Animation::Counter, false);
        timer = 0;
    }
    void CounterState::Update(const float& elapsedTime)
    {
        switch (state)
        {
        case 0://�O�����[�V����
            if (owner->model->GetCurrentKeyframeIndex() > startUpFrame)
            {
                state++;
            }
            break;

        case 1://�J�E���^�[��t���ԁA�㌄
            if (timer < receptionTime)
            {
                //���菈��
            }

            if (counterCompleted)
            {
                owner->PlayAnimation(Player::Animation::CounterAttack,false);
                state++;
            }

            timer += elapsedTime;
            break;
        case 2:
            if (!owner->IsPlayAnimation())
            {
                owner->ChangeState(Player::STATE::NORMAL);
            }
            break;
        }
    }
    void CounterState::Finalize()
    {
    }
}