#include "EnemyGolemState.h"

#include "../Other/Easing.h"
#include "../Graphics/Camera.h"

#include "../Graphics/Graphics.h"
#include "../Resource/shader.h"

#include "../Other/MathHelper.h"

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

        landingTimer = 0.0f;
        
        timer = 0.0f;
        isCameraShake = false;
    }

    void EntryState::Update(const float& elapsedTime)
    {
        if (landingTimer <= maxTime)
        {
            float posY = Easing::InQuint(landingTimer, maxTime, 0.0f, 7.0f);
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
        shader->postEffectConstants.bokehFocus = 0.0f;

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
            }
        }
        // ���K���I����ĂȂ�������
        if (!isRoarDown)
        {

            if (!isBokeh)
            {
                if (animationTimer >= 1.1f)
                {
                    Camera::Instance().ScreenVibrate(0.05f, 1.5f);
                    isBokeh = true;
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
                shader->postEffectConstants.bokehFocus = Easing::OutCubic(bokehTimer0, maxBokehTime0, 0.03f, 0.0f);

                bokehTimer0 += elapsedTime;
            }
            else
            {
                if (bokehTimer1 <= maxBokehTime1)
                {
                    shader->postEffectConstants.bokehFocus = Easing::InCubic(bokehTimer1, maxBokehTime1, 0.0f, 0.03f);

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
                            owner->GetStateMachine()->ChangeState(static_cast<UINT>(EnemyGolem::StateMachineState::IdleState));
                    }
                }
            }
        }
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
        owner->SetCurrentState(static_cast<UINT>(EnemyGolem::StateMachineState::SummonState));

        owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::SwingUpArm), false);

        isSwingUp = false;
        isSwingDown = false;
    }

    void SummonState::Update(const float& elapsedTime)
    {
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

                isSwingDown = true;
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
    }

    // �X�V
    void ComboAttack1State::Update(const float& elapsedTime)
    {
        ComboAttack1(elapsedTime);  // �R���{�P����
        ComboAttack2(elapsedTime);  // �R���{�Q����
        ComboAttack3(elapsedTime);  // �R���{�R����
    }

    // �I��
    void ComboAttack1State::Finalize()
    {
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
            // �A�j���[�V�������I�������
            if (!owner->IsPlayAnimation())
            {   // �R���ڐU��グ
                owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::ComboAttack3Up), false);
                isComboAttack2Return = true;
            }
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
            {   // �ҋ@�X�e�[�g��
                owner->GetStateMachine()->ChangeState(static_cast<UINT>(EnemyGolem::StateMachineState::IdleState));
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
}

namespace EnemyGolemState
{

}