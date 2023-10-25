#include "PlayerState.h"
#include "Player.h"
#include "PlayerManager.h"
#include "SlowMotionManager.h"
#include "LightColorManager.h"

#include "EnemyManager.h"

#include "Collision.h"

#include "../Scene/SceneManager.h"
#include "../Scene/SceneResult.h"

#include "../Resource/AudioManager.h"

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
        else if (owner->InputHardAttack())
        {
            owner->ChangeState(Player::STATE::ATTACK_HARD);
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
        //owner->isActiveAttackFrame = true;

        hit.clear();
    }
    void JabAttackState::Update(const float& elapsedTime)
    {
        //踏み込み処理
        owner->AttackSteppingUpdate(elapsedTime);

        const float keyframeIndex = owner->model->GetCurrentKeyframeIndex();
        if (keyframeIndex > activeAttackFrame[combo])owner->isActiveAttackFrame = true;
        else owner->isActiveAttackFrame = false;

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
                AttackUpdate(elapsedTime,dodgeCanselFrame1,comboCanselFrame1);
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
                AttackUpdate(elapsedTime,dodgeCanselFrame2, comboCanselFrame2);
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
                AttackUpdate(elapsedTime,dodgeCanselFrame3, comboCanselFrame3);
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
        DirectX::XMFLOAT3 hitPos;
        std::vector<Enemy*> hitEnemies;
        if (PlayerManager::Instance().AttackCollisionPlayerToEnemy(hitEnemies,hitPos))
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

                result = enemy->ApplyDamage(owner->jabMotionAtkMuls[combo] * owner->GetBasePower(), hitPos, owner);

                enemy->Flinch();

                //ダメージ吸収処理
                PlayerManager::Instance().GetDrainSkill()->Assimilate(result.damage);
            skip:;
            }
        }
    }
    void JabAttackState::AttackUpdate(float elapsedTime,int dodgeCanselFrame,int comboCanselFrame)
    {
        const float keyframeIndex = owner->model->GetCurrentKeyframeIndex();

        

        switch (state)
        {
        case UPDATE_FRAME:
            if (keyframeIndex > dodgeCanselFrame)
            {
                state++;
            }

            //近くの敵へ回転
            owner->TurnNearEnemy(2.0f,elapsedTime);
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
                //三段目のスキルを持っていないならコンボ継続しない
                if (combo == 1)
                {
                    if (!PlayerManager::Instance().GetTripleAttackSkill()->Active())goto skip;
                }

                //コンボは３連撃以降ないので制限
                if (combo < 2)
                {
                    combo++;
                    initialize = false;
                }

            skip:;
            }

            if (combo < 2)
            {
                //カウンター派生
                owner->ActiveCounter();

                if (owner->InputHardAttack())
                {
                    owner->ChangeState(Player::STATE::ATTACK_HARD);
                }
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
        hit.clear();
    }
    void AvoidState::Update(const float& elapsedTime)
    {
        //owner->MoveUpdate(elapsedTime);

        //攻撃判定
        DirectX::XMFLOAT3 hitPos;
        std::vector<Enemy*> hitEnemies;
        if (PlayerManager::Instance().HitCollisionPlayerToEnemy(hitEnemies, hitPos))
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

                result = enemy->ApplyDamage(0.1f * owner->GetBasePower(), hitPos, owner);

                enemy->Flinch();

                //ダメージ吸収処理
                PlayerManager::Instance().GetDrainSkill()->Assimilate(result.damage);
            skip:;
            }
        }

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
        changeSceneTimer = 0;
        isPlayBGM = false;

        owner->SetVelocity(DirectX::XMFLOAT3(0, 0, 0));
        owner->PlayAnimation(Player::Animation::HardStagger,false);
        owner->GetSword()->PlayAnimation(Player::Animation::HardStagger,false);
    }
    void DieState::Update(const float& elapsedTime)
    {
        owner->BlownUpdate(elapsedTime);

        if (!isPlayBGM)
        {// 死亡BGMを流す
            AudioManager::Instance().PlayBGM(BGM::Death);
            isPlayBGM = true;
        }

        //シーン遷移
        changeSceneTimer += elapsedTime;
        if (changeSceneTimer > 4.0f)
        {
            // リザルトに飛ばすフラグ（sceneGameで更新する）
            owner->SetIsResult();
        }
    }
    void DieState::Finalize()
    {
    }

    void HardAttackState::Initialize()
    {
        owner->SetVelocity(DirectX::XMFLOAT3(0, 0, 0));
        owner->PlayAnimation(Player::Animation::HardAttack, false, owner->GetAttackSpeed() + 0.2f);
        owner->GetSword()->PlayAnimation(Player::Animation::HardAttack, false, owner->GetAttackSpeed() + 0.2f);
        owner->ResetSteppingTimer();
        hit.clear();

        owner->isActiveAttackFrame = true;
    }
    void HardAttackState::Update(const float& elapsedTime)
    {
        int keyframeIndex = owner->model->GetCurrentKeyframeIndex();

        if (keyframeIndex < 10)
        {
            owner->TurnNearEnemy(2.0f, elapsedTime);
        }

        //回避キャンセル
        if (keyframeIndex > 55)
        {
            if (owner->InputAvoid())
            {
                owner->ChangeState(Player::STATE::AVOID);
            }
        }

        owner->AttackSteppingUpdate(elapsedTime);

        if (keyframeIndex > 20)
        {
            DirectX::XMFLOAT3 hitPos;
            std::vector<Enemy*> hitEnemies;
            if (PlayerManager::Instance().AttackCollisionPlayerToEnemy(hitEnemies, hitPos))
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

                    result = enemy->ApplyDamage(owner->hardAtkMuls * owner->GetBasePower(), hitPos, owner);

                    enemy->Flinch();

                    //ダメージ吸収処理
                    PlayerManager::Instance().GetDrainSkill()->Assimilate(result.damage);
                skip:;
                }
            }
        }
        if (!owner->IsPlayAnimation())
        {
            owner->ChangeState(Player::STATE::NORMAL);
        }
    }
    void HardAttackState::Finalize()
    {
        owner->isActiveAttackFrame = false;
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
        owner->isInvincible = true;
        state = 0;
        owner->SetVelocity(DirectX::XMFLOAT3(0, 0, 0));
        owner->PlayAnimation(Player::Animation::HardStagger, false);
        owner->GetSword()->PlayAnimation(Player::Animation::HardStagger, false);
    }
    void HardStaggerState::Update(const float& elapsedTime)
    {
        switch (state)
        {
        case 0:
            owner->BlownUpdate(elapsedTime);
            if (!owner->IsPlayAnimation())
            {
                owner->PlayAnimation(Player::Animation::StandUp,false);
                owner->GetSword()->PlayAnimation(Player::Animation::StandUp,false);
                state++;
            }
            break;
        case 1:
            if (!owner->IsPlayAnimation())
            {
                owner->ChangeState(Player::STATE::NORMAL);
            }
            break;
        }
    }
    void HardStaggerState::Finalize()
    {
        owner->isInvincible = false;
    }

    void CounterState::Initialize()
    {
        state = 0;
        owner->counterCompleted = false;
        owner->PlayAnimation(Player::Animation::Counter, false);
        owner->GetSword()->PlayAnimation(Player::Animation::Counter, false);

        owner->SetVelocity(DirectX::XMFLOAT3(0, 0, 0));

        cAttack = false;

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
                owner->isCounter = false;
                owner->isInvincible = true;
                owner->PlayAnimation(Player::Animation::CounterAttack,false);
                owner->GetSword()->PlayAnimation(Player::Animation::CounterAttack, false);

                //スローモーション
                SlowMotionManager::Instance().ExecuteSlowMotion(0.3f,0.3f,0.05f,0.3f);

                //画面演出
                auto& lcManager = LightColorManager::Instance();
                lcManager.GradualChangeColor(LightColorManager::ColorType::GROUND,DirectX::XMFLOAT3(0.2f,0.2f,1), 0.3f);
                lcManager.GradualChangeColor(LightColorManager::ColorType::SKY,DirectX::XMFLOAT3(0.2f,0.2f,1), 0.3f);
                lcManager.ChangeVignetteValue(0.1f, 0.3f);
                Camera::Instance().ScreenVibrate(0.1f, 0.1f);
                Camera::Instance().ChangeFov(DirectX::XMConvertToRadians(60), 0.1f);
                Input::Instance().GetGamePad().Vibration(0.2f,0.3f);

                //エフェクト
                PlayerManager::Instance().GetPlayer()->PlayLaserEffect();

                state++;
            }

            timer += elapsedTime;
            break;
        case 2:
            //攻撃
            if (!cAttack)
            {
                if (owner->model->GetCurrentKeyframeIndex() > 10)
                {
                    auto shotPos = owner->GetTransform()->GetPosition();
                    shotPos += 0.7f;
                    Camera::Instance().ScreenVibrate(0.2f, 0.1f);
                    Input::Instance().GetGamePad().Vibration(0.2f, 0.5f);

                    //攻撃判定
                    for (auto& enemy : EnemyManager::Instance().GetEnemies())
                    {
                        for (auto& collider : enemy->GetHitCollider())
                        {
                            if (Collision::IntersectSphereVsCapsule(
                            collider.position,collider.radius,
                                shotPos,owner->GetTransform()->CalcForward(),100,1
                            ))
                            {
                                enemy->SaveBlowOffInfo(owner->GetTransform()->CalcForward(), BLOW_OFF_FORCE_LEVEL::HIGH);
                                enemy->ApplyDamage(owner->GetBasePower() * 3.5f, collider.position, owner,0,true);
                                break;
                            }
                        }
                    }


                    cAttack = true;
                }
            }

            if (!owner->IsPlayAnimation())
            {
                auto& lcManager = LightColorManager::Instance();
                lcManager.AllRestoreColor(5.0f);
                lcManager.RestoreVignetteValue(5.0f);
                Camera::Instance().RestoreFov(3.0f);
            }


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
        owner->isCounter = false;
    }
}