#include "EnemyGolemState.h"

#include "../Other/Easing.h"
#include "../Graphics/Camera.h"

#include "../Graphics/Graphics.h"
#include "../Resource/shader.h"

#include "../Other/MathHelper.h"

#include "../Game/PlayerManager.h"
#include "../Game/EnemyManager.h"

#include "../Input/Input.h"

#include "../Resource/AudioManager.h"

// DummyState
namespace EnemyGolemState
{
    void DummyState::Initialize()
    {
        owner->SetCurrentState(static_cast<UINT>(EnemyGolem::StateMachineState::DummyState));

        // �A�j���[�V�����ݒ�
        owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::Idle), true);
    }
}

// IdleState
namespace EnemyGolemState
{
    void IdleState::Initialize()
    {
        owner->SetCurrentState(static_cast<UINT>(EnemyGolem::StateMachineState::IdleState));

        // �A�j���[�V�����ݒ�
        owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::Idle), true);
    }

    void IdleState::Update(const float& elapsedTime)
    {
        owner->GetStateMachine()->ChangeState(static_cast<UINT>(EnemyGolem::StateMachineState::EyeEmissiveDown));
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
        owner->SetCurrentState(static_cast<UINT>(EnemyGolem::StateMachineState::EntryState));

        // �A�j���[�V�����ݒ�
        owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::Landing), false);

        owner->SetEyeEmissiveColor(DirectX::XMFLOAT4(0.2f, 0, 1.0f, 1));
        owner->SetEyeEmissiveIntensity(2.0f);

        landingTimer = 0.0f;

        timer = 0.0f;
        isCameraShake = false;

        AudioManager::Instance().PlaySE(SE::GolemEntry);
    }

    void EntryState::Update(const float& elapsedTime)
    {
        if (landingTimer <= maxTime)
        {
            float posY = Easing::InQuint(landingTimer, maxTime, 0.0f, 15.0f);
            owner->GetTransform()->SetPositionY(posY);

            landingTimer += elapsedTime;
        }
        else
        {
            owner->GetTransform()->SetPositionY(0.0f);
            if (!isCameraShake)
            {   // �J�����U��
                Camera::Instance().ScreenVibrate(0.1f, shakeTime);
                isCameraShake = true;
                // �Q�[���p�b�h�U��
                Input::Instance().GetGamePad().Vibration(shakeTime - 0.5f, gamePadVibPower);
            }

            if (timer >= shakeTime - 1.0f)
            {
                owner->GetStateMachine()->ChangeState(static_cast<UINT>(EnemyGolem::StateMachineState::GetUpState));
                return;
            }

            timer += elapsedTime;
        }
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
        owner->SetCurrentState(static_cast<UINT>(EnemyGolem::StateMachineState::RoarState));

        // �A�j���[�V�����ݒ�
        owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::RoarUp), false, 0.7f);

        Shader* shader = Graphics::Instance().GetShader();
        shader->postEffectConstants.bokehAperture = 0.0f;

        isRoarUp = false;
        isRoarDown = false;
        isBokeh = false;
        isReturn = false;

        animationTimer = 0.0f;
        bokehTimer0 = 0.0f;
        bokehTimer1 = 0.0f;
    }

    void RoarState::Update(const float& elapsedTime)
    {
        Shader* shader = Graphics::Instance().GetShader();

        // ���K�����ł��ĂȂ�������
        if (!isRoarUp)
        {
            // �A�j���[�V�������I�������
            if (!owner->IsPlayAnimation())
            {
                // �A�j���[�V�����ݒ�
                owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::RoarDown), false, 0.6f);

                isRoarUp = true;

                AudioManager::Instance().PlaySE(SE::GolemRoar);
            }
        }
        // ���K���I����ĂȂ�������
        if (!isRoarDown)
        {

            if (!isBokeh)
            {
                if (animationTimer >= 1.1f)
                {
                    // �J�����U��
                    Camera::Instance().ScreenVibrate(0.05f, 1.5f);
                    isBokeh = true;

                    // �Q�[���p�b�h�U��
                    Input::Instance().GetGamePad().Vibration(2.0f, gamePadVibPower);
                }

                animationTimer += elapsedTime;
            }

            // �A�j���[�V�������I�����
            if (!owner->IsPlayAnimation())
            {
                isRoarDown = true;
            }
        }

        if (isBokeh)
        {
            if (bokehTimer0 <= maxBokehTime0)
            {
                shader->postEffectConstants.bokehAperture = Easing::OutCubic(bokehTimer0, maxBokehTime0, 0.03f, 0.0f);

                bokehTimer0 += elapsedTime;
            }
            else
            {
                if (bokehTimer1 <= maxBokehTime1)
                {
                    shader->postEffectConstants.bokehAperture = Easing::InCubic(bokehTimer1, maxBokehTime1, 0.0f, 0.03f);

                    bokehTimer1 += elapsedTime;
                }
                else
                {
                    if (!isReturn)
                    {
                        owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::RoarReturn), false);
                        isReturn = true;
                    }
                    else
                    {
                        if (!owner->IsPlayAnimation())
                        {
                            owner->GetStateMachine()->ChangeState(static_cast<UINT>(EnemyGolem::StateMachineState::IdleState));
                        }
                    }
                }
            }
        }
        else
        {
            shader->postEffectConstants.bokehAperture = 0.0f;
        }
    }

    void RoarState::Finalize()
    {
        Shader* shader = Graphics::Instance().GetShader();
        shader->postEffectConstants.bokehAperture = 0.0f;
    }
}

// SummonState
namespace EnemyGolemState
{
    void SummonState::Initialize()
    {
        owner->SetCurrentState(static_cast<UINT>(EnemyGolem::StateMachineState::SummonState));

        owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::SwingUpArm), false);

        isSwingUp = false;
        isSwingDown = false;

        delayTimer = 0.0f;
    }

    void SummonState::Update(const float& elapsedTime)
    {
        owner->AttackCollisionVsPlayer(EnemyGolem::ColliderName::R_HAND, owner->GetAttackDamage());
        owner->AttackCollisionVsPlayer(EnemyGolem::ColliderName::R_LEG, owner->GetAttackDamage());
        owner->AttackCollisionVsPlayer(EnemyGolem::ColliderName::R_LEG_END, owner->GetAttackDamage());
        owner->AttackCollisionVsPlayer(EnemyGolem::ColliderName::R_SHOULDER, owner->GetAttackDamage());
        owner->AttackCollisionVsPlayer(EnemyGolem::ColliderName::L_HAND, owner->GetAttackDamage());
        owner->AttackCollisionVsPlayer(EnemyGolem::ColliderName::L_LEG, owner->GetAttackDamage());
        owner->AttackCollisionVsPlayer(EnemyGolem::ColliderName::L_LEG_END, owner->GetAttackDamage());
        owner->AttackCollisionVsPlayer(EnemyGolem::ColliderName::L_SHOULDER, owner->GetAttackDamage());

        // �r�̐U��グ�I����ĂȂ�������
        if (!isSwingUp)
        {
            // �A�j���[�V�������I�������
            if (!owner->IsPlayAnimation())
            {   // �r�U�艺���A�j���[�V����
                owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::SwingDownArm), false, 4.0f);
                isSwingUp = true;
            }
        }
        // �r�̐U�艺�����I����ĂȂ�������
        if (!isSwingDown)
        {
            // �A�j���[�V�������I�������
            if (!owner->IsPlayAnimation())
            {
                // �������@����̖߂�A�j���[�V����
                owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::SwingGetUp), false, 0.5f);

                // �������@�w����
                owner->magicCircleEnemySummon->GetStateMachine()->ChangeState(static_cast<UINT>(MagicCircleEnemySummon::StateMachineState::AppearState));

                // ��ʐU��
                Camera::Instance().ScreenVibrate(0.1f, 1.5f);

                // �Q�[���p�b�h�U��
                Input::Instance().GetGamePad().Vibration(1.0f, gamePadVibPower);

                isSwingDown = true;
            }
        }
        else
        {
            delayTimer += elapsedTime;
            if (delayTimer >= maxDelayTime)
            {   // �ҋ@�X�e�[�g��
                owner->GetStateMachine()->ChangeState(static_cast<UINT>(EnemyGolem::StateMachineState::IdleState));
            }
        }

        // �������@�w�X�V����
        owner->UpdateSummoningMagicCircle(4.0f, 3.0f, DirectX::XMConvertToRadians(45));
    }

    void SummonState::Finalize()
    {
    }
}

// GetUpState
namespace EnemyGolemState
{
    void GetUpState::Initialize()
    {
        owner->SetCurrentState(static_cast<UINT>(EnemyGolem::StateMachineState::GetUpState));

        // �A�j���[�V�����ݒ�
        owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::GetUp), false, 0.7f);
    }

    void GetUpState::Update(const float& elapsedTime)
    {
        // �A�j���[�V�������I�������
        if(!owner->IsPlayAnimation())
        {   // �ҋ@�X�e�[�g��
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(EnemyGolem::StateMachineState::ChoseState));
        }
    }

    void GetUpState::Finalize()
    {
    }
}

// Attack1State
namespace EnemyGolemState
{
    // ������
    void Attack1State::Initialize()
    {


        owner->SetCurrentState(static_cast<UINT>(EnemyGolem::StateMachineState::Attack1State));

        // �A�j���[�V�����X�V
        owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::Attack1Tame), false);

        isAttack1_tame = false;
        isAttack1 = false;
        isShakeCamera = false;
        isReturn = false;

        animationTimer = 0.0f;
        returnTimer = 0.0f;
        returnTimer1 = 0.0f;
    }

    // �X�V
    void Attack1State::Update(const float& elapsedTime)
    {
        owner->AttackCollisionVsPlayer(EnemyGolem::ColliderName::R_HAND, owner->GetAttackDamage());
        owner->AttackCollisionVsPlayer(EnemyGolem::ColliderName::R_LEG, owner->GetAttackDamage());
        owner->AttackCollisionVsPlayer(EnemyGolem::ColliderName::R_LEG_END, owner->GetAttackDamage());
        owner->AttackCollisionVsPlayer(EnemyGolem::ColliderName::R_SHOULDER, owner->GetAttackDamage());

        // �r�Ђ�
        if (!isAttack1_tame)
        {
            // �A�j���[�V�������I�������
            if (!owner->IsPlayAnimation())
            {
                owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::Attack1), false);

                isAttack1_tame = true;
            }
        }

        // ����
        if (!isAttack1)
        {
            if (animationTimer >= 0.3f)
            {
                // �J�����U��
                Camera::Instance().ScreenVibrate(0.1f, 1.0f);
                isShakeCamera = true;

                // �Q�[���p�b�h�U��
                Input::Instance().GetGamePad().Vibration(0.5f, gamePadVibPower);
            }
            else
            {
                // �U���ƃA�j���[�V�����̃^�C�~���O�����킹�Ă�
                if (isAttack1_tame) animationTimer += elapsedTime;
            }

            // �A�j���[�V�������I�������
            if (!owner->IsPlayAnimation())
            {
                isAttack1 = true;
            }
        }

        // �U��������
        if (isShakeCamera)
        {
            // �U���I�������߂�A�j���[�V�����ɂȂ���p
            returnTimer += elapsedTime;

            if (returnTimer >= 1.0f)
            {
                owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::Attack1Return), false);
                isShakeCamera = false;
                isReturn = true;
            }
        }


        if (isReturn)
        {
            returnTimer1 += elapsedTime;

            if (returnTimer1 >= 1.0f)
            {
                owner->GetStateMachine()->ChangeState(static_cast<UINT>(EnemyGolem::StateMachineState::IdleState));
            }
        }
    }

    // �I����
    void Attack1State::Finalize()
    {
    }
}

// ComboAttack1State
namespace EnemyGolemState
{
    // ������
    void ComboAttack1State::Initialize()
    {
        owner->SetCurrentState(static_cast<UINT>(EnemyGolem::StateMachineState::ComboAttack1State));

        // �A�j���[�V�����ݒ�
        owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::ComboAttack1), false);

        isComboAttack1          = false;
        isMoveFrontAttack1      = false;
        isMoveFrontAttack2      = false;
        isComboAttack2          = false;
        isComboAttack2Return    = false;
        isComboAttack3Up        = false;
        isComboAttack3Down      = false;
        isComboAttack3Return    = false;

        moveTimer = 0.0f;
        moveFrontTimer = 0.0f;

        isStoneCreate = false;
        isStoneCreated = false;
        stoneTimer = 0.0f;
    }

    // �X�V
    void ComboAttack1State::Update(const float& elapsedTime)
    {
        owner->AttackCollisionVsPlayer(EnemyGolem::ColliderName::R_HAND, owner->GetAttackDamage());
        owner->AttackCollisionVsPlayer(EnemyGolem::ColliderName::R_LEG, owner->GetAttackDamage());
        owner->AttackCollisionVsPlayer(EnemyGolem::ColliderName::R_LEG_END, owner->GetAttackDamage());
        owner->AttackCollisionVsPlayer(EnemyGolem::ColliderName::R_SHOULDER, owner->GetAttackDamage());
        owner->AttackCollisionVsPlayer(EnemyGolem::ColliderName::L_HAND, owner->GetAttackDamage());
        owner->AttackCollisionVsPlayer(EnemyGolem::ColliderName::L_LEG, owner->GetAttackDamage());
        owner->AttackCollisionVsPlayer(EnemyGolem::ColliderName::L_LEG_END, owner->GetAttackDamage());
        owner->AttackCollisionVsPlayer(EnemyGolem::ColliderName::L_SHOULDER, owner->GetAttackDamage());

        ComboAttack1(elapsedTime);  // �R���{�P����
        ComboAttack2(elapsedTime);  // �R���{�Q����
        ComboAttack3(elapsedTime);  // �R���{�R����

        Turn(elapsedTime); // ��]����

        // �ҋ@�X�e�[�g��
        if (owner->comboAttackStone->isChangeState)
        {
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(EnemyGolem::StateMachineState::IdleState));
            owner->comboAttackStone->isChangeState = false;
        }
    }

    // �I��
    void ComboAttack1State::Finalize()
    {
        // �΂̃X�P�[�����O�ɂ���
        owner->comboAttackStone->stoneBall->GetTransform()->SetScale(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
        // ���@�w�̃X�P�[�����O�ɂ���
        owner->comboAttackStone->magicCircle->GetTransform()->SetScale(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
    }

    // �R���{�P����
    void ComboAttack1State::ComboAttack1(const float& elapsedTime)
    {
        // �R���{�U���P���I����ĂȂ�
        if (!isComboAttack1)
        {
            moveTimer += elapsedTime;

            if (moveTimer >= maxMoveTime1)
            {   // �O�i����
                isMoveFrontAttack1 = true;
            }
            if (moveTimer >= maxMoveTime1 + 0.2f)
            {   // �J�����U��
                Camera::Instance().ScreenVibrate(0.04f, 0.5f);
                // �Q�[���p�b�h�U��
                Input::Instance().GetGamePad().Vibration(0.15f, gamePadVibPower);
            }

            // �A�j���[�V�������I�������
            if (!owner->IsPlayAnimation())
            {
                owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::ComboAttack2), false, 1.5f);

                isComboAttack1 = true;
            }
        }

        // �O�i����
        if (isMoveFrontAttack1)
        {
            if (MoveFront(elapsedTime, 0.07f, 0.4f))
            {
                // �����Ă�Ƃ��͉������Ȃ�
            }
            else
            {   // �~�܂�����
                isMoveFrontAttack1 = false;
                moveTimer = 0.0f;
                moveFrontTimer = 0.0f;
            }
        }
    }

    // �R���{�Q����
    void ComboAttack1State::ComboAttack2(const float& elapsedTime)
    {
        // �R���{�Q���ڂ��I�������
        if (!isComboAttack2 && isComboAttack1)
        {
            moveTimer += elapsedTime;

            if (moveTimer >= maxMoveTime2)
            {
                isMoveFrontAttack2 = true;
            }
            if (moveTimer >= maxMoveTime2 + 0.3f)
            {   // �J�����U��
                Camera::Instance().ScreenVibrate(0.05f, 0.8f);
                // �Q�[���p�b�h�U��
                Input::Instance().GetGamePad().Vibration(0.3f, gamePadVibPower);
            }

            // �A�j���[�V�������I�������
            if (!owner->IsPlayAnimation())
            {   // �N���オ��A�j���[�V����
                owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::ComboAttack2Return), false);
                isComboAttack2 = true;
            }
        }

        // �S�g����
        if (isMoveFrontAttack2)
        {
            if (MoveFront(elapsedTime, 0.05f, 0.3f))
            {
                // �����Ă�Ƃ��͉������Ȃ�
            }
            else
            {   // �~�܂�����
                isMoveFrontAttack2 = false;
                moveTimer = 0.0f;
                moveFrontTimer = 0.0f;
            }
        }

        // �R���{�Q���ڋN���オ�肪�I�������
        if (!isComboAttack2Return && isComboAttack2)
        {
            stoneTimer += elapsedTime;
            if (stoneTimer >= 0.4f)
            {
                isStoneCreate = true;
            }

            // �A�j���[�V�������I�������
            if (!owner->IsPlayAnimation())
            {   // �R���ڐU��グ
                owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::ComboAttack3Up), false);
                isComboAttack2Return = true;
            }
        }

        if (isStoneCreate && !isStoneCreated)
        {
            // �΂̃X�e�[�g�ݒ�
            owner->comboAttackStone->GetStateMachine()->ChangeState(static_cast<UINT>(ComboAttackStone::StateMachineState::AppearState));
            isStoneCreated = true;
        }
    }

    // �R���{�R����
    void ComboAttack1State::ComboAttack3(const float& elapsedTime)
    {
        // �R���{3���ڐU��グ�I�������
        if (!isComboAttack3Up && isComboAttack2Return)
        {
            // �A�j���[�V�������I�������
            if (!owner->IsPlayAnimation())
            {   // �R���ڐU�艺��
                owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::ComboAttack3Down), false);
                isComboAttack3Up = true;
            }
        }

        // �R���{�R���ڐU�艺���I�������
        if (!isComboAttack3Down && isComboAttack3Up)
        {
            // �A�j���[�V�������I�������
            if (!owner->IsPlayAnimation())
            {   // �R���ږ߂�
                owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::ComboAttack3Return), false);
                isComboAttack3Down = true;
            }
        }

        // �R���{�R���ږ߂�I�������
        if (!isComboAttack3Return && isComboAttack3Down)
        {
            // �A�j���[�V�������I�������
            if (!owner->IsPlayAnimation())
            {
            }
        }
    }

    // �O�����ɐi��
    bool ComboAttack1State::MoveFront(const float& elapsedTime, const float& addLength, const float& maxMoveFrontTime)
    {
        if (moveFrontTimer <= maxMoveFrontTime)
        {
            float addPos = Easing::InSine(moveFrontTimer, maxMoveFrontTime, addLength, 0.0f);

            DirectX::XMFLOAT3 frontVec = owner->GetTransform()->CalcForward();
            frontVec = frontVec * addPos;
            owner->GetTransform()->AddPosition(frontVec);

            moveFrontTimer += elapsedTime;
            return true;
        }

        return false;
    }

    // ��]����
    void ComboAttack1State::Turn(const float& elapsedTime)
    {
        DirectX::XMFLOAT3 playerPos = PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition();
        DirectX::XMFLOAT3 ownerPos = owner->GetTransform()->GetPosition();
        DirectX::XMFLOAT3 vec = Normalize(playerPos - ownerPos);
        owner->Turn(elapsedTime, vec.x, vec.z, rotateSpeed);
    }
}

// DownState
namespace EnemyGolemState
{
    // ������
    void DownState::Initialize()
    {
        owner->SetCurrentState(static_cast<UINT>(EnemyGolem::StateMachineState::DownState));

        // �A�j���[�V�����Z�b�g
        owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::Down), false);

        // �ϐ�������
        isDown = false;
        isReturn = false;

        getUpTimer = 0.0f;
    }

    // �X�V
    void DownState::Update(const float& elapsedTime)
    {
        // down0�A�j���[�V�������I����ĂȂ�
        if (!isDown)
        {
            // �A�j���[�V�������I�������
            if (!owner->IsPlayAnimation())
            {
                isDown = true;
            }
        }

        if (!isReturn && isDown)
        {
            getUpTimer += elapsedTime;
            // ���ݏI���
            if (getUpTimer >= maxGetUpTimer)
            {
                owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::DownReturn), false);
                isReturn = true;
            }
        }
        else
        {
            // �A�j���[�V�������I�������
            if (!owner->IsPlayAnimation())
            {
                owner->GetStateMachine()->ChangeState(static_cast<UINT>(EnemyGolem::StateMachineState::IdleState));
            }
        }

    }

    // �I����
    void DownState::Finalize()
    {
    }
}

// ComboAttack2State
namespace EnemyGolemState
{
    // ������
    void ComboAttack2State::Initialize()
    {
        owner->SetCurrentState(static_cast<UINT>(EnemyGolem::StateMachineState::ComboAttack2State));

        // �A�j���[�V�����ݒ�
        owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::ComboAttack3Up), false);

        // �ΐ���
        owner->comboAttackStones[0]->GetStateMachine()->ChangeState(static_cast<UINT>(ComboAttackStone::StateMachineState::AppearState));

        // �ϐ�������
        isComboAttackUp = false;
        isComboAttackDown = false;
        isComboAttackReturn = false;

        num = 0;
        delayTimer = 0.0f;
    }

    // �X�V
    void ComboAttack2State::Update(const float& elapsedTime)
    {
        if (num < maxNum)
        {   // �O��U������
            if (isComboAttackReturn)
            {
                AttackInitialize();
            }
            AttackUpdate(elapsedTime);
        }
        else
        {
            if (delayTimer >= maxDelayTime)
            {
                // �ҋ@�X�e�[�g
                owner->GetStateMachine()->ChangeState(static_cast<UINT>(EnemyGolem::StateMachineState::IdleState));
            }
            delayTimer += elapsedTime;
        }

        Turn(elapsedTime);
    }

    // �I����
    void ComboAttack2State::Finalize()
    {
    }

    // �U��������
    void ComboAttack2State::AttackInitialize()
    {
        owner->comboAttackStones[num]->GetStateMachine()->ChangeState(static_cast<UINT>(ComboAttackStone::StateMachineState::AppearState));
        owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::ComboAttack3Up), false);
        isComboAttackUp = false;
        isComboAttackDown = false;
        isComboAttackReturn = false;
    }

    // �U��
    void ComboAttack2State::AttackUpdate(const float& elapsedTime)
    {
        if (!isComboAttackUp)
        {
            if (!owner->IsPlayAnimation())
            {
                owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::ComboAttack3Down), false);
                isComboAttackUp = true;
            }
        }

        if (!isComboAttackDown && isComboAttackUp)
        {
            if (!owner->IsPlayAnimation())
            {
                owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::ComboAttack3Return), false);
                isComboAttackDown = true;
            }
        }

        if (!isComboAttackReturn && isComboAttackDown)
        {
            if (!owner->IsPlayAnimation())
            {
                ++num;
                isComboAttackReturn = true;
            }
        }
    }

    // ��]����
    void ComboAttack2State::Turn(const float& elapsedTime)
    {
        DirectX::XMFLOAT3 playerPos = PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition();
        DirectX::XMFLOAT3 ownerPos = owner->GetTransform()->GetPosition();
        DirectX::XMFLOAT3 vec = Normalize(playerPos - ownerPos);
        owner->Turn(elapsedTime, vec.x, vec.z, rotateSpeed);
    }
}

// ChoseState
namespace EnemyGolemState
{
    // ������
    void ChoseState::Initialize()
    {
        // �A�j���[�V�����ݒ�
        owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::Idle), true);

        // �A�����ē����X�e�[�g���o�Ȃ��悤�ɐ���
        currentNum = 0;
        for (int i = 0; i < static_cast<UINT>(STATE::Max); ++i)
        {
            if (isState[i])currentNum++;
        }
        if (currentNum >= resetNum)
        {
            for (int i = 0; i < static_cast<UINT>(STATE::Max); ++i)
            {
                isState[i] = false;
            }
        }
    }

    // �X�V
    void ChoseState::Update(const float& elapsedTime)
    {
        while (1)
        {
            // �����_���ŃX�e�[�g�����肷��
            setState = rand() % static_cast<UINT>(STATE::Max);

            // �G�����̂��o�Ă�Ƃ���summonState�͒ʂ�Ȃ�
            if (setState == static_cast<UINT>(STATE::Summon))
            {
                std::vector<Enemy*> enemy;
                EnemyManager::Instance().GetSpecifyEnemy(Enemy::TYPE::Normal, enemy);

                if (enemy.size() > 0)
                {
                    continue;
                }
            }

            // �O�̃X�e�[�g�Ɠ����Ȃ�ʂ�Ȃ�
            if (oldState == setState)
            {
                continue;
            }

            // ���̃X�e�[�g���g���Ă��Ȃ������疳�����[�v�𔲂���
            if (!isState[setState])
            {
                oldState = setState;
                break;
            }
        }

        switch (setState)
        {
        case static_cast<UINT>(STATE::Summon):
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(EnemyGolem::StateMachineState::SummonState));
            break;
        case static_cast<UINT>(STATE::Attack):
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(EnemyGolem::StateMachineState::Attack1State));
            break;
        case static_cast<UINT>(STATE::ComboAttack1):
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(EnemyGolem::StateMachineState::ComboAttack1State));
            break;
        case static_cast<UINT>(STATE::ComboAttack2):
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(EnemyGolem::StateMachineState::ComboAttack2State));
            break;
        case static_cast<UINT>(STATE::Roar):
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(EnemyGolem::StateMachineState::RoarState));
            break;
        case static_cast<UINT>(STATE::Attack2):
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(EnemyGolem::StateMachineState::Attack2State));
            break;
        }

        isState[setState] = true;
    }

    // �I����
    void ChoseState::Finalize()
    {
    }
}

// DeathState
namespace EnemyGolemState
{
    // ������
    void DeathState::Initialize()
    {
        owner->SetCurrentState(static_cast<UINT>(EnemyGolem::StateMachineState::DeathState));

        // �A�j���[�V�����ݒ�
        owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::Death0), false ,0.7f);

        // �ϐ�������
        isDeath0 = false;
        isDeath1 = false;

        delayTimer = 0.0f;
        isDelay = false;

        cameraShakeTimer = 0.0f;
        isCameraShake = false;

        deathTimer = 0.0f;
        isDestroy = false;
    }

    // �X�V
    void DeathState::Update(const float& elapsedTime)
    {

        if (!isDeath0)
        {
            if (!owner->IsPlayAnimation())
            {
                // �A�j���[�V�����ݒ�
                owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::Death1), false, 0.6f);
                isDeath0 = true;
            }
        }
        else
        {
            if (!isDelay)
            {
                delayTimer += elapsedTime;
                if (delayTimer >= maxDelay)
                {
                    // �J�����U��
                    //Camera::Instance().ScreenVibrate(0.04f, 0.3f);
                    isDelay = true;
                }
            }
        }

        if (!isCameraShake && isDelay)
        {
            cameraShakeTimer += elapsedTime;
            if (cameraShakeTimer >= maxTimer)
            {
                isCameraShake = true;
                Camera::Instance().ScreenVibrate(0.10f, 1.5f);

                isDestroy = true;
            }
        }


        if (isDestroy)
        {
            float maxTime = 2.0f;
            if (deathTimer <= maxTime)
            {
                float alpha = Easing::InSine(deathTimer, maxTime, 0.0f, 1.0f);

                DirectX::XMFLOAT4 c = owner->model->GetModelColor();
                c.w = alpha;
                owner->model->SetModelColor(c);

                // ���X��BGM������
                float volume = Easing::InSine(deathTimer, maxTime, 0.0f, 0.35f);
                AudioManager::Instance().GetBGM(BGM::Golem)->Volume(volume);

                deathTimer += elapsedTime;
            }
            else
            {
                owner->Destroy();
            }
        }
    }

    // �I����
    void DeathState::Finalize()
    {
    }
}

// WalkState
namespace EnemyGolemState
{
    // ������
    void WalkState::Initialize()
    {
        owner->SetCurrentState(static_cast<UINT>(EnemyGolem::StateMachineState::WalkState));

        // �A�j���[�V�����ݒ�
        owner->PlayBlendAnimation(
            static_cast<UINT>(EnemyGolem::Animation::Idle),
            static_cast<UINT>(EnemyGolem::Animation::Walk),
            true);

        owner->model->weight = 0.0f;

        owner->SetEyeEmissiveColor(DirectX::XMFLOAT4(0.2, 0, 1.0, 0));
        owner->SetEyeEmissiveIntensity(2.0f);

        // �ϐ�������
        isChangeState = false;
        moveSpeed = 0.0f;
    }

    // �X�V
    void WalkState::Update(const float& elapsedTime)
    {
        float maxWalk = 1.0f;
        if (walkTimer > maxWalk)
        {
            AudioManager::Instance().PlaySE(SE_NAME::GolemWalk, SE::GolemWalk0, SE::GolemWalk4);
            walkTimer = 0.0;
        }
        walkTimer += elapsedTime;

        // ���[�V�����u�����h
        {
            if (!isChangeState)
            {// ���X�ɑ��x�グ�Ă���
                owner->model->weight += elapsedTime;
                owner->model->weight = min(1.0f, owner->model->weight);
            }
            else
            {// �͈͂ɓ�������A���X�ɑ��x��������
                owner->model->weight -= elapsedTime * 2.0f;
                if (owner->model->weight <= 0.0f)
                {// �I���X�e�[�g�ɍs��
                    owner->GetStateMachine()->ChangeState(static_cast<UINT>(EnemyGolem::StateMachineState::ChoseState));
                }
            }
        }

        // ����
        JudgeChangeState();

        // �ړ�
        Move(elapsedTime);

        // ��]
        Turn(elapsedTime);
    }

    // �I����
    void WalkState::Finalize()
    {

    }

    // �͈͂ɓ���΃X�e�[�g�ύX
    void WalkState::JudgeChangeState()
    {
        DirectX::XMFLOAT3 playerPos = PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition();
        DirectX::XMFLOAT3 ownerPos = owner->GetTransform()->GetPosition();
        DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&playerPos), DirectX::XMLoadFloat3(&ownerPos));
        float length = DirectX::XMVectorGetX(DirectX::XMVector3Length(Vec));

        if (length < changeStateLength)
        {
            isChangeState = true;
        }
    }

    // �ړ�
    void WalkState::Move(const float& elapsedTime)
    {
        moveSpeed = owner->model->weight * maxMoveSpeed;

        DirectX::XMFLOAT3 playerPos = PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition();
        DirectX::XMFLOAT3 ownerPos = owner->GetTransform()->GetPosition();
        DirectX::XMFLOAT3 vec = playerPos - ownerPos;
        vec = Normalize(vec);
        vec = vec * moveSpeed * elapsedTime;
        vec.y = 0.0f;   // Y�͈ړ��Ȃ��B

        owner->GetTransform()->AddPosition(vec);
    }

    // ��]����
    void WalkState::Turn(const float& elapsedTime)
    {
        DirectX::XMFLOAT3 playerPos = PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition();
        DirectX::XMFLOAT3 ownerPos = owner->GetTransform()->GetPosition();
        DirectX::XMFLOAT3 vec = Normalize(playerPos - ownerPos);
        owner->Turn(elapsedTime, vec.x, vec.z, rotateSpeed);
    }
}

// Attack2State
namespace EnemyGolemState
{
    // ������
    void Attack2State::Initialize()
    {
        owner->SetCurrentState(static_cast<UINT>(EnemyGolem::StateMachineState::Attack2State));

        // �A�j���[�V�����ݒ�
        owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::Attack2), false);

        // ���@�w����
        owner->magicCircleGolemAttack2->GetStateMachine()->ChangeState(static_cast<UINT>(MagicCircleGolemAttack2::StateMachineState::AppearState));
    }

    // �X�V
    void Attack2State::Update(const float& elapsedTime)
    {
        if (!owner->IsPlayAnimation())
        {
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(EnemyGolem::StateMachineState::IdleState));
        }

        // ��]����
        Turn(elapsedTime);
    }

    // �I����
    void Attack2State::Finalize()
    {
    }

    // ��]
    void Attack2State::Turn(const float& elapsedTime)
    {
        DirectX::XMFLOAT3 playerPos = PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition();
        DirectX::XMFLOAT3 ownerPos = owner->GetTransform()->GetPosition();
        DirectX::XMFLOAT3 vec = Normalize(playerPos - ownerPos);
        owner->Turn(elapsedTime, vec.x, vec.z, rotateSpeed);
    }
}

// DelayState
namespace EnemyGolemState
{
    // ������
    void DelayState::Initialize()
    {
        delayTimer = 0.0f;

        maxDelay = rand() % 3 + 0.5f;

        owner->SetEyeEmissiveColor(DirectX::XMFLOAT4(0.2, 0, 0, 0));
        owner->SetEyeEmissiveIntensity(1.0f);
    }

    // �X�V
    void DelayState::Update(const float& elapsedTime)
    {
        if (delayTimer >= maxDelay)
        {
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(EnemyGolem::StateMachineState::EyeEmissiveUp));
        }

        delayTimer += elapsedTime;
    }

    // �I��
    void DelayState::Finalize()
    {
    }
}

// EyeEmissiveDownState
namespace EnemyGolemState
{
    void EyeEmissiveDownState::Initialize()
    {
        timer = 0.0f;
    }
    void EyeEmissiveDownState::Update(const float& elapsedTime)
    {
        float maxTime = 0.3f;
        if (timer <= maxTime)
        {

            float b = Easing::InSine(timer, maxTime, 0.0f, 1.0f);

            DirectX::XMFLOAT4 color = owner->GetEyeEmissiveColor();

            color.z = b;
            owner->SetEyeEmissiveColor(color);

            timer += elapsedTime;
        }
        else
        {
            owner->SetEyeEmissiveColor(DirectX::XMFLOAT4(0.2f, 0, 0, 1));
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(EnemyGolem::StateMachineState::DelayState));
        }
    }
    void EyeEmissiveDownState::Finalize()
    {
    }
}

// EyeEmissiveUpState
namespace EnemyGolemState
{
    void EyeEmissiveUpState::Initialize()
    {
        timer = 0.0f;
    }
    void EyeEmissiveUpState::Update(const float& elapsedTime)
    {
        float maxTime = 0.3f;
        if (timer <= maxTime)
        {
            
            float b = Easing::InSine(timer, maxTime, 1.0f, 0.0f);

            DirectX::XMFLOAT4 color = owner->GetEyeEmissiveColor();
            
            color.z = b;
            owner->SetEyeEmissiveColor(color);

            timer += elapsedTime;
        }
        else
        {
            owner->SetEyeEmissiveColor(DirectX::XMFLOAT4(0.2, 0, 1.0f, 1));
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(EnemyGolem::StateMachineState::WalkState));
        }
    }
    void EyeEmissiveUpState::Finalize()
    {
    }
}