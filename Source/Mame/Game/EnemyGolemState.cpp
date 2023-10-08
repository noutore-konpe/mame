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

        // アニメーション設定
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

        // アニメーション設定
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
            {   // カメラ振動
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

        // アニメーション設定
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

        // 咆哮準備できてなかったら
        if (!isRoarUp)
        {
            // アニメーションが終わったら
            if (!owner->IsPlayAnimation())
            {
                // アニメーション設定
                owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::RoarDown), false, 0.6f);

                isRoarUp = true;
            }
        }
        // 咆哮が終わってなかったら
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

            // アニメーションが終わった
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
        // 腕の振り上げ終わってなかったら
        if (!isSwingUp)
        {
            // アニメーションが終わったら
            if (!owner->IsPlayAnimation())
            {   // 腕振り下げアニメーション
                owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::SwingDownArm), false, 4.0f);
                isSwingUp = true;
            }
        }
        // 腕の振り下げが終わってなかったら
        if (!isSwingDown)
        {
            // アニメーションが終わったら
            if (!owner->IsPlayAnimation())
            {
                // 召喚魔法からの戻りアニメーション
                owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::SwingGetUp), false, 0.5f);

                // 召喚魔法陣生成
                owner->magicCircleEnemySummon->GetStateMachine()->ChangeState(static_cast<UINT>(MagicCircleEnemySummon::StateMachineState::AppearState));

                // 画面振動
                Camera::Instance().ScreenVibrate(0.1f, 1.5f);

                isSwingDown = true;
            }
        }

        // 召喚魔法陣更新処理
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

        // アニメーション設定
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
    // 初期化
    void Attack1State::Initialize()
    {
        owner->SetCurrentState(static_cast<UINT>(EnemyGolem::StateMachineState::Attack1State));

        // アニメーション更新
        owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::Attack1Tame), false);

        isAttack1_tame = false;
        isAttack1 = false;
        isShakeCamera = false;
        isReturn = false;

        animationTimer = 0.0f;
        returnTimer = 0.0f;
        returnTimer1 = 0.0f;
    }

    // 更新
    void Attack1State::Update(const float& elapsedTime)
    {        
        // 腕ひき
        if (!isAttack1_tame)
        {
            // アニメーションが終わったら
            if (!owner->IsPlayAnimation())
            {
                owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::Attack1), false);

                isAttack1_tame = true;
            }
        }

        // 殴り
        if (!isAttack1)
        {
            if (animationTimer >= 0.3f)
            {
                // カメラ振動
                Camera::Instance().ScreenVibrate(0.1f, 1.0f);
                isShakeCamera = true;
            }
            else
            {
                // 振動とアニメーションのタイミングを合わせてる
                if (isAttack1_tame) animationTimer += elapsedTime;
            }

            // アニメーションが終わったら
            if (!owner->IsPlayAnimation())
            {
                isAttack1 = true;
            }
        }

        // 振動したら
        if (isShakeCamera)
        {
            // 振動終わったら戻るアニメーションにつなげる用
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

    // 終了化
    void Attack1State::Finalize()
    {
    }
}

// ComboAttack1State
namespace EnemyGolemState
{
    // 初期化
    void ComboAttack1State::Initialize()
    {
        owner->SetCurrentState(static_cast<UINT>(EnemyGolem::StateMachineState::ComboAttack1State));

        // アニメーション設定
        owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::ComboAttack1), false);

        isComboAttack1 = false;
        isMoveFront = false;
        
        moveTimer = 0.0f;
        moveFrontTimer = 0.0f;
    }

    // 更新
    void ComboAttack1State::Update(const float& elapsedTime)
    {
        // コンボ攻撃１が終わってない
        if (!isComboAttack1)
        {
            moveTimer += elapsedTime;

            if (moveTimer >= maxMoveTime)
                isMoveFront = true;

            // アニメーションが終わったら
            if (!owner->IsPlayAnimation())
            {
                owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::ComboAttack2), false, 1.5f);

                isComboAttack1 = true;
            }
        }

        // 前進する
        if (isMoveFront)
        {
            if (moveFrontTimer <= maxMoveFrontTime)
            {
                float addPos = Easing::InSine(moveFrontTimer, maxMoveFrontTime, 0.07f, 0.0f);

                DirectX::XMFLOAT3 frontVec = owner->GetTransform()->CalcForward();
                frontVec = frontVec * addPos;
                owner->GetTransform()->AddPosition(frontVec);

                moveFrontTimer += elapsedTime;
            }
            else
            {
                isMoveFront = false;
                moveTimer = 0.0f;
            }
        }
    }

    // 終了
    void ComboAttack1State::Finalize()
    {
    }
}

namespace EnemyGolemState
{

}