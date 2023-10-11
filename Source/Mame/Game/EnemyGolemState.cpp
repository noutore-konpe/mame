#include "EnemyGolemState.h"

#include "../Other/Easing.h"
#include "../Graphics/Camera.h"

#include "../Graphics/Graphics.h"
#include "../Resource/shader.h"

#include "../Other/MathHelper.h"

#include "../Game/PlayerManager.h"

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
        owner->Turn(elapsedTime, vec.x, vec.z, 50.0f);
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
        owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::Down0), false, 0.7f);

        // 変数初期化
        isDown0 = false;
        isDown1 = false;
        isReturn = false;

        getUpTimer = 0.0f;
    }

    // 更新
    void DownState::Update(const float& elapsedTime)
    {
        // down0アニメーションが終わってない
        if (!isDown0)
        {
            // アニメーションが終わったら
            if (!owner->IsPlayAnimation())
            {
                owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::Down1), false, 0.6f);
                isDown0 = true;
            }
        }

        // down1アニメーションが終わっていない
        if (!isDown1 && isDown0)
        {
            // アニメーションが終わったら
            if (!owner->IsPlayAnimation())
            {
                isDown1 = true;
            }
        }

        // 倒れきったら
        if (isDown1&& !isReturn)
        {
            getUpTimer += elapsedTime;
            
            // 怯み終わり
            if (getUpTimer >= maxGetUpTimer)
            {
                owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::DownReturn), false, 0.5f);
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
        owner->Turn(elapsedTime, vec.x, vec.z, 50.0f);
    }
}

// ChoseState
namespace EnemyGolemState
{
    void ChoseState::Initialize()
    {
    }
    void ChoseState::Update(const float& elapsedTime)
    {
    }
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
        // アニメーション設定
        owner->PlayAnimation(static_cast<UINT>(EnemyGolem::Animation::Death), false);
    }

    // 更新
    void DeathState::Update(const float& elapsedTime)
    {
    }

    // 終了化
    void DeathState::Finalize()
    {
    }
}