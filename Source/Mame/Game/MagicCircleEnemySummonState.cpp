#include "MagicCircleEnemySummonState.h"

#include "EnemyManager.h"
#include "EnemyAura.h"

// dummyState
namespace MagicCircleEnemySummonState
{
    void DummyState::Initialize()
    {
        for (int i = 0; i < 2; ++i)
        {
            owner->magicCircle[i]->GetTransform()->SetScale(DirectX::XMFLOAT3(0, 0, 0));
            owner->magicCircle[i]->GetTransform()->SetRotation(DirectX::XMFLOAT4(0, 0, 0, 0));
            
            for (int j = 0; j < 3; ++j)
            {
                owner->magicCircleMove[i][j]->GetTransform()->SetScale(DirectX::XMFLOAT3(0, 0, 0));
                owner->magicCircleMove[i][j]->GetTransform()->SetRotation(DirectX::XMFLOAT4(0, 0, 0, 0));
            }
        }
    }
}

// AppearState
namespace MagicCircleEnemySummonState
{
    // 初期化
    void AppearState::Initialize()
    {
        for (int i = 0; i < 2; ++i)
        {
            owner->magicCircle[i]->GetTransform()->SetPositionY(0.0f);
            owner->magicCircle[i]->GetTransform()->SetScale(DirectX::XMFLOAT3(0, 0, 0));
            owner->magicCircle[i]->GetTransform()->SetRotation(DirectX::XMFLOAT4(0, 0, 0, 0));

            for (int j = 0; j < 3; ++j)
            {
                owner->magicCircleMove[i][j]->GetTransform()->SetPositionY(0.0f);
                owner->magicCircleMove[i][j]->GetTransform()->SetScale(DirectX::XMFLOAT3(0, 0, 0));
                owner->magicCircleMove[i][j]->GetTransform()->SetRotation(DirectX::XMFLOAT4(0, 0, 0, 0));
            }
        }

        scale = 0.0f;
        posY = 0.0f;

        timer = 0.0f;
        subTimer1 = 0.0f;
        subTimer2 = 0.0f;
        subTimer3 = 0.0f;
    }

    // 更新処理
    void AppearState::Update(const float& elapsedTime)
    {
        const float speed = 3.0f;

        if (timer <= maxTime)
        {   // 展開
            scale = Easing::InCubic(timer, maxTime, 1.0f, 0.0f);

            // トランスフォーム
            for (int i = 0; i < 2; ++i)
            {
                owner->magicCircle[i]->GetTransform()->SetScale(DirectX::XMFLOAT3(scale, scale, scale));

                for (int j = 0; j < 3; ++j)
                {
                    owner->magicCircleMove[i][j]->GetTransform()->SetScale(DirectX::XMFLOAT3(scale, scale, scale));
                }
            }

            // タイマー加算
            timer += elapsedTime;
        }
        else
        {   // 移動
            if (subTimer1 <= maxSubTime1)
            {   // 一つ目
                scale = Easing::OutQuint(subTimer1, maxSubTime1, 1.5f, 1.0f);
                posY = Easing::OutQuint(subTimer1, maxSubTime1, 0.3f, 0.0f);

                for (int i = 0; i < 2; ++i)
                {
                    owner->magicCircle[i]->GetTransform()->AddRotationY(elapsedTime * speed);

                    for (int j = 0; j < 3; ++j)
                    {
                        owner->magicCircleMove[i][j]->GetTransform()->SetPositionY(posY);
                        owner->magicCircleMove[i][j]->GetTransform()->SetScale(DirectX::XMFLOAT3(scale, scale, scale));
                        owner->magicCircleMove[i][j]->GetTransform()->AddRotationY(elapsedTime * speed);
                    }
                }

                subTimer1 += elapsedTime;
            }
            else
            {
                if (subTimer2 <= maxSubTime2)
                {   // 二つ目
                    scale = Easing::OutQuint(subTimer2, maxSubTime2, 1.7f, 1.5f);
                    posY = Easing::OutQuint(subTimer2, maxSubTime2, 0.7f, 0.3f);

                    for (int i = 0; i < 2; ++i)
                    {
                        owner->magicCircle[i]->GetTransform()->AddRotationY(elapsedTime * speed);

                        for (int j = 0; j < 2; ++j)
                        {
                            owner->magicCircleMove[i][j + 1]->GetTransform()->SetPositionY(posY);
                            owner->magicCircleMove[i][j + 1]->GetTransform()->SetScale(DirectX::XMFLOAT3(scale, scale, scale));
                            owner->magicCircleMove[i][j + 1]->GetTransform()->AddRotationY(elapsedTime * speed);
                        }
                    }

                    subTimer2 += elapsedTime;
                }
                else
                {
                    if (subTimer3 <= maxSubTime3)
                    {   // 三つ目
                        scale = Easing::OutQuint(subTimer3, maxSubTime3, 1.9f, 1.7f);
                        posY = Easing::OutQuint(subTimer3, maxSubTime3, 1.2f, 0.7f);

                        owner->magicCircle[0]->GetTransform()->AddRotationY(elapsedTime * speed);
                        owner->magicCircle[1]->GetTransform()->AddRotationY(elapsedTime * speed);

                        for (int i = 0; i < 2; ++i)
                        {
                            owner->magicCircleMove[i][2]->GetTransform()->SetPositionY(posY);
                            owner->magicCircleMove[i][2]->GetTransform()->SetScale(DirectX::XMFLOAT3(scale, scale, scale));
                            owner->magicCircleMove[i][2]->GetTransform()->AddRotationY(elapsedTime * speed);
                        }

                        subTimer3 += elapsedTime;
                    }
                    else
                    {
                        // エネミー生成
                        for (int i = 0; i < 2; ++i)
                        {
                            EnemyAura* enemyAura = new EnemyAura;
                            const DirectX::XMFLOAT3 setPosition = owner->magicCircle[i]->GetTransform()->GetPosition();
                            enemyAura->SetPosition(setPosition);
                            EnemyManager::Instance().Register(enemyAura);
                        }

                        owner->GetStateMachine()->ChangeState(static_cast<UINT>(MagicCircleEnemySummon::StateMachineState::CreateState));
                    }
                }
            }
        }

        // 回転(基本)
        for (int i = 0; i < 2; ++i)
        {
            owner->magicCircle[i]->GetTransform()->AddRotationY(elapsedTime);

            for (int j = 0; j < 3; ++j)
            {
                owner->magicCircleMove[i][j]->GetTransform()->AddRotationY(elapsedTime);
            }
        }
    }

    // 終了化
    void AppearState::Finalize()
    {

    }
}

// CreateState
namespace MagicCircleEnemySummonState
{
    // 初期化
    void CreateState::Initialize()
    {
        timer = 0.0f;
        subTimer = 0.0f;
        scaleTimer = 0.0f;
    }

    // 更新
    void CreateState::Update(const float& elapsedTime)
    {
        if (timer <= maxTime)
        {
            posY[0] = Easing::OutCubic(timer, maxTime, 0.0f, 0.3f);
            posY[1] = Easing::OutCubic(timer, maxTime, 0.01f, 0.7f);
            posY[2] = Easing::OutCubic(timer, maxTime, 0.02f, 1.2f);

            for (int i = 0; i < 3; ++i)
            {
                owner->magicCircleMove[0][i]->GetTransform()->SetPositionY(posY[i]);
                owner->magicCircleMove[1][i]->GetTransform()->SetPositionY(posY[i]);

                owner->magicCircleMove[0][i]->GetTransform()->AddRotationY(elapsedTime * (i + 1) * 2);
                owner->magicCircleMove[1][i]->GetTransform()->AddRotationY(elapsedTime * (i + 1) * 2);
            }

            float rot = Easing::OutCubic(subTimer, maxScaleTime, 5.0f, 2.0f);
            owner->magicCircle[0]->GetTransform()->AddRotationY(elapsedTime * rot);
            owner->magicCircle[1]->GetTransform()->AddRotationY(elapsedTime * rot);

            timer += elapsedTime;
        }
        else
        {
            if (subTimer <= maxSubTime)
            {
                scale[0] = Easing::OutCubic(subTimer, maxSubTime, 0.0f, 1.5f);
                scale[1] = Easing::OutCubic(subTimer, maxSubTime, 0.0f, 1.7f);
                scale[2] = Easing::OutCubic(subTimer, maxSubTime, 0.0f, 1.9f);

                for (int i = 0; i < 3; ++i)
                {
                    owner->magicCircleMove[0][i]->GetTransform()->SetScale(DirectX::XMFLOAT3(scale[i], scale[i], scale[i]));
                    owner->magicCircleMove[1][i]->GetTransform()->SetScale(DirectX::XMFLOAT3(scale[i], scale[i], scale[i]));

                    owner->magicCircleMove[0][i]->GetTransform()->AddRotationY(elapsedTime * 3);
                    owner->magicCircleMove[1][i]->GetTransform()->AddRotationY(elapsedTime * 3);

                }

                float rot = Easing::OutCubic(subTimer, maxScaleTime, 8.0f, 4.0f);
                owner->magicCircle[0]->GetTransform()->AddRotationY(elapsedTime * rot);
                owner->magicCircle[1]->GetTransform()->AddRotationY(elapsedTime * rot);

                subTimer += elapsedTime;
            }
            else
            {
                if (scaleTimer <= maxScaleTime)
                {
                    baseScale = Easing::OutCubic(scaleTimer, maxScaleTime, 0.0f, 1.0f);

                    for (int i = 0; i < 2; ++i)
                    {
                        owner->magicCircle[i]->GetTransform()->SetScale(DirectX::XMFLOAT3(baseScale, baseScale, baseScale));
                        owner->magicCircle[i]->GetTransform()->AddRotationY(elapsedTime * 5);
                    }

                    scaleTimer += elapsedTime;
                }
                else
                {
                    // 次のステートへ(終了だから、待機のdummy)
                    owner->GetStateMachine()->ChangeState(static_cast<UINT>(MagicCircleEnemySummon::StateMachineState::DummyState));
                }
            }
        }

        // 回転値
        owner->magicCircle[0]->GetTransform()->AddRotationY(elapsedTime);
        owner->magicCircle[1]->GetTransform()->AddRotationY(elapsedTime);
    }

    // 終了化
    void CreateState::Finalize()
    {
    }
}