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

        // アニメーション設定
        owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::Idle), true);
    }
}

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

        // アニメーション設定
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
            {   // カメラ振動
                Camera::Instance().ScreenVibrate(0.1f, shakeTime);
                isCameraShake = true;
                // ゲームパッド振動
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

        // アニメーション設定
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

        // 咆哮準備できてなかったら
        if (!isRoarUp)
        {
            // アニメーションが終わったら
            if (!owner->IsPlayAnimation())
            {
                // アニメーション設定
                owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::RoarDown), false, 0.6f);

                isRoarUp = true;

                AudioManager::Instance().PlaySE(SE::GolemRoar);
            }
        }
        // 咆哮が終わってなかったら
        if (!isRoarDown)
        {

            if (!isBokeh)
            {
                if (animationTimer >= 1.1f)
                {
                    // カメラ振動
                    Camera::Instance().ScreenVibrate(0.05f, 1.5f);
                    isBokeh = true;

                    // ゲームパッド振動
                    Input::Instance().GetGamePad().Vibration(2.0f, gamePadVibPower);
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

                // ゲームパッド振動
                Input::Instance().GetGamePad().Vibration(1.0f, gamePadVibPower);

                isSwingDown = true;
            }
        }
        else
        {
            delayTimer += elapsedTime;
            if (delayTimer >= maxDelayTime)
            {   // 待機ステートへ
                owner->GetStateMachine()->ChangeState(static_cast<UINT>(EnemyGolem::StateMachineState::IdleState));
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
        // アニメーションが終わったら
        if(!owner->IsPlayAnimation())
        {   // 待機ステートへ
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
        owner->AttackCollisionVsPlayer(EnemyGolem::ColliderName::R_HAND, owner->GetAttackDamage());
        owner->AttackCollisionVsPlayer(EnemyGolem::ColliderName::R_LEG, owner->GetAttackDamage());
        owner->AttackCollisionVsPlayer(EnemyGolem::ColliderName::R_LEG_END, owner->GetAttackDamage());
        owner->AttackCollisionVsPlayer(EnemyGolem::ColliderName::R_SHOULDER, owner->GetAttackDamage());

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

                // ゲームパッド振動
                Input::Instance().GetGamePad().Vibration(0.5f, gamePadVibPower);
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

    // 更新
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

        ComboAttack1(elapsedTime);  // コンボ１撃目
        ComboAttack2(elapsedTime);  // コンボ２撃目
        ComboAttack3(elapsedTime);  // コンボ３撃目

        Turn(elapsedTime); // 回転処理

        // 待機ステートへ
        if (owner->comboAttackStone->isChangeState)
        {
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(EnemyGolem::StateMachineState::IdleState));
            owner->comboAttackStone->isChangeState = false;
        }
    }

    // 終了
    void ComboAttack1State::Finalize()
    {
        // 石のスケールを０にする
        owner->comboAttackStone->stoneBall->GetTransform()->SetScale(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
        // 魔法陣のスケールを０にする
        owner->comboAttackStone->magicCircle->GetTransform()->SetScale(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
    }

    // コンボ１撃目
    void ComboAttack1State::ComboAttack1(const float& elapsedTime)
    {
        // コンボ攻撃１が終わってない
        if (!isComboAttack1)
        {
            moveTimer += elapsedTime;

            if (moveTimer >= maxMoveTime1)
            {   // 前進する
                isMoveFrontAttack1 = true;
            }
            if (moveTimer >= maxMoveTime1 + 0.2f)
            {   // カメラ振動
                Camera::Instance().ScreenVibrate(0.04f, 0.5f);
                // ゲームパッド振動
                Input::Instance().GetGamePad().Vibration(0.15f, gamePadVibPower);
            }

            // アニメーションが終わったら
            if (!owner->IsPlayAnimation())
            {
                owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::ComboAttack2), false, 1.5f);

                isComboAttack1 = true;
            }
        }

        // 前進する
        if (isMoveFrontAttack1)
        {
            if (MoveFront(elapsedTime, 0.07f, 0.4f))
            {
                // 動いてるときは何もしない
            }
            else
            {   // 止まったら
                isMoveFrontAttack1 = false;
                moveTimer = 0.0f;
                moveFrontTimer = 0.0f;
            }
        }
    }

    // コンボ２撃目
    void ComboAttack1State::ComboAttack2(const float& elapsedTime)
    {
        // コンボ２撃目が終わったか
        if (!isComboAttack2 && isComboAttack1)
        {
            moveTimer += elapsedTime;

            if (moveTimer >= maxMoveTime2)
            {
                isMoveFrontAttack2 = true;
            }
            if (moveTimer >= maxMoveTime2 + 0.3f)
            {   // カメラ振動
                Camera::Instance().ScreenVibrate(0.05f, 0.8f);
                // ゲームパッド振動
                Input::Instance().GetGamePad().Vibration(0.3f, gamePadVibPower);
            }

            // アニメーションが終わったら
            if (!owner->IsPlayAnimation())
            {   // 起き上がりアニメーション
                owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::ComboAttack2Return), false);
                isComboAttack2 = true;
            }
        }

        // 全身する
        if (isMoveFrontAttack2)
        {
            if (MoveFront(elapsedTime, 0.05f, 0.3f))
            {
                // 動いてるときは何もしない
            }
            else
            {   // 止まったら
                isMoveFrontAttack2 = false;
                moveTimer = 0.0f;
                moveFrontTimer = 0.0f;
            }
        }

        // コンボ２撃目起き上がりが終わったか
        if (!isComboAttack2Return && isComboAttack2)
        {
            stoneTimer += elapsedTime;
            if (stoneTimer >= 0.4f)
            {
                isStoneCreate = true;
            }

            // アニメーションが終わったら
            if (!owner->IsPlayAnimation())
            {   // ３撃目振り上げ
                owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::ComboAttack3Up), false);
                isComboAttack2Return = true;
            }
        }

        if (isStoneCreate && !isStoneCreated)
        {
            // 石のステート設定
            owner->comboAttackStone->GetStateMachine()->ChangeState(static_cast<UINT>(ComboAttackStone::StateMachineState::AppearState));
            isStoneCreated = true;
        }
    }

    // コンボ３撃目
    void ComboAttack1State::ComboAttack3(const float& elapsedTime)
    {
        // コンボ3撃目振り上げ終わったか
        if (!isComboAttack3Up && isComboAttack2Return)
        {
            // アニメーションが終わったら
            if (!owner->IsPlayAnimation())
            {   // ３撃目振り下げ
                owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::ComboAttack3Down), false);
                isComboAttack3Up = true;
            }
        }

        // コンボ３撃目振り下げ終わったか
        if (!isComboAttack3Down && isComboAttack3Up)
        {
            // アニメーションが終わったら
            if (!owner->IsPlayAnimation())
            {   // ３撃目戻り
                owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::ComboAttack3Return), false);
                isComboAttack3Down = true;
            }
        }

        // コンボ３撃目戻り終わったか
        if (!isComboAttack3Return && isComboAttack3Down)
        {
            // アニメーションが終わったら
            if (!owner->IsPlayAnimation())
            {
            }
        }
    }

    // 前方向に進む
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

    // 回転処理
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
    // 初期化
    void DownState::Initialize()
    {
        owner->SetCurrentState(static_cast<UINT>(EnemyGolem::StateMachineState::DownState));

        // アニメーションセット
        owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::Down), false);

        // 変数初期化
        isDown = false;
        isReturn = false;

        getUpTimer = 0.0f;
    }

    // 更新
    void DownState::Update(const float& elapsedTime)
    {
        // down0アニメーションが終わってない
        if (!isDown)
        {
            // アニメーションが終わったら
            if (!owner->IsPlayAnimation())
            {
                isDown = true;
            }
        }

        if (!isReturn && isDown)
        {
            getUpTimer += elapsedTime;
            // 怯み終わり
            if (getUpTimer >= maxGetUpTimer)
            {
                owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::DownReturn), false);
                isReturn = true;
            }
        }
        else
        {
            // アニメーションが終わったら
            if (!owner->IsPlayAnimation())
            {
                owner->GetStateMachine()->ChangeState(static_cast<UINT>(EnemyGolem::StateMachineState::IdleState));
            }
        }

    }

    // 終了化
    void DownState::Finalize()
    {
    }
}

// ComboAttack2State
namespace EnemyGolemState
{
    // 初期化
    void ComboAttack2State::Initialize()
    {
        owner->SetCurrentState(static_cast<UINT>(EnemyGolem::StateMachineState::ComboAttack2State));

        // アニメーション設定
        owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::ComboAttack3Up), false);

        // 石生成
        owner->comboAttackStones[0]->GetStateMachine()->ChangeState(static_cast<UINT>(ComboAttackStone::StateMachineState::AppearState));

        // 変数初期化
        isComboAttackUp = false;
        isComboAttackDown = false;
        isComboAttackReturn = false;

        num = 0;
        delayTimer = 0.0f;
    }

    // 更新
    void ComboAttack2State::Update(const float& elapsedTime)
    {
        if (num < maxNum)
        {   // 三回攻撃する
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
                // 待機ステート
                owner->GetStateMachine()->ChangeState(static_cast<UINT>(EnemyGolem::StateMachineState::IdleState));
            }
            delayTimer += elapsedTime;
        }

        Turn(elapsedTime);
    }

    // 終了化
    void ComboAttack2State::Finalize()
    {
    }

    // 攻撃初期化
    void ComboAttack2State::AttackInitialize()
    {
        owner->comboAttackStones[num]->GetStateMachine()->ChangeState(static_cast<UINT>(ComboAttackStone::StateMachineState::AppearState));
        owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::ComboAttack3Up), false);
        isComboAttackUp = false;
        isComboAttackDown = false;
        isComboAttackReturn = false;
    }

    // 攻撃
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

    // 回転処理
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
    // 初期化
    void ChoseState::Initialize()
    {
        // アニメーション設定
        owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::Idle), true);

        // 連続して同じステートが出ないように制御
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

    // 更新
    void ChoseState::Update(const float& elapsedTime)
    {
        while (1)
        {
            // ランダムでステートを決定する
            setState = rand() % static_cast<UINT>(STATE::Max);

            // 敵が何体か出てるときはsummonStateは通らない
            if (setState == static_cast<UINT>(STATE::Summon))
            {
                std::vector<Enemy*> enemy;
                EnemyManager::Instance().GetSpecifyEnemy(Enemy::TYPE::Normal, enemy);

                if (enemy.size() > 0)
                {
                    continue;
                }
            }

            // 前のステートと同じなら通らない
            if (oldState == setState)
            {
                continue;
            }

            // そのステートが使われていなかったら無限ループを抜ける
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

    // 終了化
    void ChoseState::Finalize()
    {
    }
}

// DeathState
namespace EnemyGolemState
{
    // 初期化
    void DeathState::Initialize()
    {
        owner->SetCurrentState(static_cast<UINT>(EnemyGolem::StateMachineState::DeathState));

        // アニメーション設定
        owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::Death0), false ,0.7f);

        // 変数初期化
        isDeath0 = false;
        isDeath1 = false;

        delayTimer = 0.0f;
        isDelay = false;

        cameraShakeTimer = 0.0f;
        isCameraShake = false;

        deathTimer = 0.0f;
        isDestroy = false;
    }

    // 更新
    void DeathState::Update(const float& elapsedTime)
    {

        if (!isDeath0)
        {
            if (!owner->IsPlayAnimation())
            {
                // アニメーション設定
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
                    // カメラ振動
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

                // 徐々にBGMを消す
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

    // 終了化
    void DeathState::Finalize()
    {
    }
}

// WalkState
namespace EnemyGolemState
{
    // 初期化
    void WalkState::Initialize()
    {
        owner->SetCurrentState(static_cast<UINT>(EnemyGolem::StateMachineState::WalkState));

        // アニメーション設定
        owner->PlayBlendAnimation(
            static_cast<UINT>(EnemyGolem::Animation::Idle),
            static_cast<UINT>(EnemyGolem::Animation::Walk),
            true);

        owner->model->weight = 0.0f;

        owner->SetEyeEmissiveColor(DirectX::XMFLOAT4(0.2, 0, 1.0, 0));
        owner->SetEyeEmissiveIntensity(2.0f);

        // 変数初期化
        isChangeState = false;
        moveSpeed = 0.0f;
    }

    // 更新
    void WalkState::Update(const float& elapsedTime)
    {
        float maxWalk = 1.0f;
        if (walkTimer > maxWalk)
        {
            AudioManager::Instance().PlaySE(SE_NAME::GolemWalk, SE::GolemWalk0, SE::GolemWalk4);
            walkTimer = 0.0;
        }
        walkTimer += elapsedTime;

        // モーションブレンド
        {
            if (!isChangeState)
            {// 徐々に速度上げていく
                owner->model->weight += elapsedTime;
                owner->model->weight = min(1.0f, owner->model->weight);
            }
            else
            {// 範囲に入ったら、徐々に速度を下げる
                owner->model->weight -= elapsedTime * 2.0f;
                if (owner->model->weight <= 0.0f)
                {// 選択ステートに行く
                    owner->GetStateMachine()->ChangeState(static_cast<UINT>(EnemyGolem::StateMachineState::ChoseState));
                }
            }
        }

        // 判定
        JudgeChangeState();

        // 移動
        Move(elapsedTime);

        // 回転
        Turn(elapsedTime);
    }

    // 終了化
    void WalkState::Finalize()
    {

    }

    // 範囲に入ればステート変更
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

    // 移動
    void WalkState::Move(const float& elapsedTime)
    {
        moveSpeed = owner->model->weight * maxMoveSpeed;

        DirectX::XMFLOAT3 playerPos = PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition();
        DirectX::XMFLOAT3 ownerPos = owner->GetTransform()->GetPosition();
        DirectX::XMFLOAT3 vec = playerPos - ownerPos;
        vec = Normalize(vec);
        vec = vec * moveSpeed * elapsedTime;
        vec.y = 0.0f;   // Yは移動なし。

        owner->GetTransform()->AddPosition(vec);
    }

    // 回転処理
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
    // 初期化
    void Attack2State::Initialize()
    {
        owner->SetCurrentState(static_cast<UINT>(EnemyGolem::StateMachineState::Attack2State));

        // アニメーション設定
        owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::Attack2), false);

        // 魔法陣生成
        owner->magicCircleGolemAttack2->GetStateMachine()->ChangeState(static_cast<UINT>(MagicCircleGolemAttack2::StateMachineState::AppearState));
    }

    // 更新
    void Attack2State::Update(const float& elapsedTime)
    {
        if (!owner->IsPlayAnimation())
        {
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(EnemyGolem::StateMachineState::IdleState));
        }

        // 回転処理
        Turn(elapsedTime);
    }

    // 終了化
    void Attack2State::Finalize()
    {
    }

    // 回転
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
    // 初期化
    void DelayState::Initialize()
    {
        delayTimer = 0.0f;

        maxDelay = rand() % 3 + 0.5f;

        owner->SetEyeEmissiveColor(DirectX::XMFLOAT4(0.2, 0, 0, 0));
        owner->SetEyeEmissiveIntensity(1.0f);
    }

    // 更新
    void DelayState::Update(const float& elapsedTime)
    {
        if (delayTimer >= maxDelay)
        {
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(EnemyGolem::StateMachineState::EyeEmissiveUp));
        }

        delayTimer += elapsedTime;
    }

    // 終了
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