#include "PlayerState.h"
#include "Player.h"
#include "PlayerManager.h"
#include "SlowMotionManager.h"

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

        //一度回避ボタンを離させる
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
        
        owner->ActiveCounter();
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
        //踏み込み処理
        owner->AttackSteppingUpdate(elapsedTime);

        switch (combo)
        {
        case 0: //1撃目
            //初期化処理
            if (!initialize)
            {
                // 敵に与える吹っ飛び力レベルの設定(各攻撃の開始前に設定する)
                owner->SetInflictBlowOffForceLevel(BLOW_OFF_FORCE_LEVEL::MIDDLE);

                owner->ResetSteppingTimer();
                state = UPDATE_FRAME;
                owner->PlayAnimation(Player::Animation::Jab_1, false, owner->GetAttackSpeed());
                owner->GetSword()->PlayAnimation(Player::Animation::Jab_1, false, owner->GetAttackSpeed());
                hit.clear();//ヒットした敵の初期化
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
                // 敵に与える吹っ飛び力レベルの設定(各攻撃の開始前に設定する)
                owner->SetInflictBlowOffForceLevel(BLOW_OFF_FORCE_LEVEL::MIDDLE);

                owner->ResetSteppingTimer();
                state = UPDATE_FRAME;
                owner->PlayAnimation(Player::Animation::Jab_2, false, owner->GetAttackSpeed());
                owner->GetSword()->PlayAnimation(Player::Animation::Jab_2, false, owner->GetAttackSpeed());
                hit.clear();//ヒットした敵の初期化
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
                // 敵に与える吹っ飛び力レベルの設定(各攻撃の開始前に設定する)
                owner->SetInflictBlowOffForceLevel(BLOW_OFF_FORCE_LEVEL::MIDDLE);

                owner->ResetSteppingTimer();
                state = UPDATE_FRAME;
                owner->PlayAnimation(Player::Animation::Jab_3, false, owner->GetAttackSpeed());
                owner->GetSword()->PlayAnimation(Player::Animation::Jab_3, false, owner->GetAttackSpeed());
                hit.clear();//ヒットした敵の初期化
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
                Character::DamageResult result;
                for (auto& h : hit)
                {
                    if (h == enemy)goto skip;//すでに一度ヒットしている敵なら処理しない
                }
                //一度もヒットしていない敵なので登録する
                hit.emplace_back(enemy);

                result = enemy->ApplyDamage(owner->jabMotionAtkMuls[combo], owner);

                enemy->Flinch();

                //ダメージ吸収処理
                PlayerManager::Instance().GetDrainSkill()->Assimilate(result.damage);
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
                //コンボは３連撃以降ないので制限
                if (combo < 2)
                {
                    combo++;
                    initialize = false;
                }
            }

            if (combo < 2)
            {
                //カウンター派生
                owner->ActiveCounter();
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
        owner->SetVelocity(DirectX::XMFLOAT3(0, 0, 0));
        owner->PlayAnimation(Player::Animation::HardStagger, false);
        //owner->GetSword()->PlayAnimation(Player::Animation::HardStagger, false);
    }
    void HardAttackState::Update(const float& elapsedTime)
    {
        owner->BlownUpdate(elapsedTime);
    }
    void HardAttackState::Finalize()
    {

    }

    void SoftStaggerState::Initialize()
    {
        owner->PlayAnimation(Player::Animation::SoftStagger, false,1.8f);
        owner->GetSword()->PlayAnimation(Player::Animation::SoftStagger, false,1.8f);
        //owner->isInvincible = true;//無敵
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
        state = 0;
        owner->counterCompleted = false;
        owner->PlayAnimation(Player::Animation::Counter, false);
        owner->GetSword()->PlayAnimation(Player::Animation::Counter, false);
        timer = 0;
    }
    void CounterState::Update(const float& elapsedTime)
    {
        switch (state)
        {
        case 0://前隙モーション
            if (owner->model->GetCurrentKeyframeIndex() > startUpFrame)
            {
                owner->isCounter = true;
                state++;
            }
            break;

        case 1://カウンター受付時間、後隙

            //カウンターの受付時間を越えている場合
            if (owner->isCounter && timer > receptionTime)
            {
                owner->isCounter = false;
            }

            //カウンター成功処理
            if (owner->counterCompleted)
            {
                owner->isInvincible = true;
                owner->PlayAnimation(Player::Animation::CounterAttack,false);
                owner->GetSword()->PlayAnimation(Player::Animation::CounterAttack, false);

                //スローモーション
                SlowMotionManager::Instance().ExecuteSlowMotion(0.1f,0.7f,0.05f,0.3f);

                state++;
            }

            timer += elapsedTime;
            break;
        case 2:
            
            break;
        }

        if (!owner->IsPlayAnimation())
        {
            owner->ChangeState(Player::STATE::NORMAL);
        }
    }
    void CounterState::Finalize()
    {
        owner->isInvincible = false;
    }
}