#include "MagicCircleGolemAttack2State.h"

#include "../Other/Easing.h"
#include "../Other/MathHelper.h"

#include "../Game/PlayerManager.h"

// DummyState
namespace MagicCircleGolemAttack2State
{
    void DummyState::Initialize()
    {
        for (int i = 0; i < static_cast<UINT>(MagicCircleGolemAttack2::MAGIC_CIRCLE::Max); ++i)
        {
            owner->baseMagicCircle[i]->GetTransform()->SetScale(DirectX::XMFLOAT3(0, 0, 0));
            owner->baseMagicCircle[i]->GetTransform()->SetRotation(DirectX::XMFLOAT4(0, 0, 0, 0));
        }
    }
}

// AppearState
namespace MagicCircleGolemAttack2State
{
    void AppearState::Initialize()
    {
        for (int i = 0; i < static_cast<UINT>(MAGIC_CIRCLE::Max); ++i)
        {
            owner->baseMagicCircle[i]->GetTransform()->SetScale(DirectX::XMFLOAT3(0, 0, 0));
            owner->baseMagicCircle[i]->GetTransform()->SetRotation(DirectX::XMFLOAT4(0, 0, 0, 0));

            owner->baseMagicCircle[i]->model->PlayAnimation(0, false, 1.5f);

            // 変数初期化
            appearTimer[i] = 0.0f;
            rotationSpeed[i] = 1.5f;
            isAnimationEnd[i] = false;
        }
        delayTimer = 0.0f;
    }

    void AppearState::Update(const float& elapsedTime)
    {
        // アニメーション関連
        for (int i = 0; i < static_cast<UINT>(MAGIC_CIRCLE::Max); ++i)
        {
            if (!owner->baseMagicCircle[i]->model->IsPlayAnimation())
            {
                isAnimationEnd[i] = true;
                owner->baseMagicCircle[i]->model->PlayAnimation(0, false, rotationSpeed[i]);
            }
        }

        // 魔法陣生成処理
        UpdateAppear(elapsedTime);

        // アニメーション更新。（回転）
        for (int i = 0; i < static_cast<UINT>(MAGIC_CIRCLE::Max); ++i)
        {
            owner->baseMagicCircle[i]->model->UpdateAnimation(elapsedTime);
        }
    }

    void AppearState::Finalize()
    {

    }

    // 魔法陣生成
    void AppearState::UpdateAppear(const float& elapsedTime)
    {
        delayTimer += elapsedTime;

        if (delayTimer >= maxDelayTimer[0])
        {
            if (appearTimer[static_cast<UINT>(MAGIC_CIRCLE::Left)] <= maxAppearTime[static_cast<UINT>(MAGIC_CIRCLE::Left)])
            {
                float scale = Easing::InSine(appearTimer[static_cast<UINT>(MAGIC_CIRCLE::Left)], maxAppearTime[static_cast<UINT>(MAGIC_CIRCLE::Left)], 0.7f, 0.0f);

                owner->baseMagicCircle[static_cast<UINT>(MAGIC_CIRCLE::Left)]->GetTransform()->SetScale(DirectX::XMFLOAT3(scale, scale, scale));

                appearTimer[static_cast<UINT>(MAGIC_CIRCLE::Left)] += elapsedTime;
            }
        }
        if (delayTimer >= maxDelayTimer[1])
        {
            if (appearTimer[static_cast<UINT>(MAGIC_CIRCLE::Center)] <= maxAppearTime[static_cast<UINT>(MAGIC_CIRCLE::Center)])
            {
                float scale = Easing::InSine(appearTimer[static_cast<UINT>(MAGIC_CIRCLE::Center)], maxAppearTime[static_cast<UINT>(MAGIC_CIRCLE::Center)], 0.7f, 0.0f);

                owner->baseMagicCircle[static_cast<UINT>(MAGIC_CIRCLE::Center)]->GetTransform()->SetScale(DirectX::XMFLOAT3(scale, scale, scale));

                appearTimer[static_cast<UINT>(MAGIC_CIRCLE::Center)] += elapsedTime;
            }
        }
        if (delayTimer >= maxDelayTimer[2])
        {
            if (appearTimer[static_cast<UINT>(MAGIC_CIRCLE::Right)] <= maxAppearTime[static_cast<UINT>(MAGIC_CIRCLE::Right)])
            {
                float scale = Easing::InSine(appearTimer[static_cast<UINT>(MAGIC_CIRCLE::Right)], maxAppearTime[static_cast<UINT>(MAGIC_CIRCLE::Right)], 0.7f, 0.0f);

                owner->baseMagicCircle[static_cast<UINT>(MAGIC_CIRCLE::Right)]->GetTransform()->SetScale(DirectX::XMFLOAT3(scale, scale, scale));

                appearTimer[static_cast<UINT>(MAGIC_CIRCLE::Right)] += elapsedTime;
            }
            else
            {
                owner->GetStateMachine()->ChangeState(static_cast<UINT>(MagicCircleGolemAttack2::StateMachineState::ThrowState));
            }
        }
    }
}

// ThrowState
namespace MagicCircleGolemAttack2State
{
    // 初期化
    void ThrowState::Initialize()
    {

        for (int i = 0; i < static_cast<UINT>(MAGIC_CIRCLE::Max); ++i)
        {
            isHoming[i] = true;
            isLunch[i] = false;
            stoneNum[i] = 0;

            for (int j = 0; j < static_cast<UINT>(MAGIC_CIRCLE::Max); ++j)
            {
                owner->stoneBalls[i][j]->GetTransform()->
                    SetPosition(owner->baseMagicCircle[i]->GetTransform()->GetPosition());
            }
        }

        stoneFallTimer[0] = 0.2f;
        stoneFallTimer[1] = -0.1f;
        stoneFallTimer[2] = -0.4f;
    }

    // 更新処理
    void ThrowState::Update(const float& elapsedTime)
    {
        DirectX::XMFLOAT3 playerPos = PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition();
        playerPos.y += 0.8f;
        DirectX::XMFLOAT3 magicCirclePos[3] = {};
        DirectX::XMFLOAT3 vec[3] = {};

        for (int i = 0; i < static_cast<UINT>(MAGIC_CIRCLE::Max); ++i)
        {
            magicCirclePos[i] = owner->baseMagicCircle[i]->GetTransform()->GetPosition();
            vec[i] = playerPos - magicCirclePos[i];
        }

        for (int i = 0; i < static_cast<UINT>(MAGIC_CIRCLE::Max); ++i)
        {
            if (isLunch[i])
            {
                //if (stoneFallTimer[i] > maxStoneFallTime[i] / 2.0f)
                //{
                //    isHoming[i] = false;
                //}

                if (isHoming[i])
                {
                    targetVec[i] = Normalize(vec[i]);
                    length[i] = Length(vec[i]);
                }

                DirectX::XMFLOAT3 addPos = targetVec[i] * length[i] * 1.7f * elapsedTime;
                owner->stoneBalls[i][stoneNum[i]]->GetTransform()->AddPosition(addPos);

                // 地面より下に行ったら
                if (owner->stoneBalls[i][stoneNum[i]]->GetTransform()->GetPosition().y <= 0.0f)
                {
                    ++stoneNum[i];
                    isLunch[i] = false;
                    stoneFallTimer[i] = 0.0f;
                }
            }
            else
            {
                if (stoneNum[i] < 3)
                {
                    if (stoneFallTimer[i] <= maxStoneFallTime[i])
                    {
                        stoneFallTimer[i] += elapsedTime;
                    }
                    else
                    {
                        isLunch[i] = true;
                    }
                }
            }
        }

    }

    // 終了化
    void ThrowState::Finalize()
    {

    }
}