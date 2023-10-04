#include "MagicCircleState.h"
#include "../Other/Easing.h"

// AppearState
namespace MagicCircleState
{
    // 初期化
    void AppearState::Initialize()
    {
        for (int i = 0; i < 3; ++i)
        {
            owner->magicCircle[i]->GetTransform()->SetPositionY(0.0f);
            owner->magicCircle[i]->GetTransform()->SetScale(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
            owner->magicCircle[i]->GetTransform()->SetRotation(DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f));
        }

        timer = 0.0f;
    }

    // 更新
    void AppearState::Update(const float& elapsedTime)
    {
        if (timer <= maxTime)
        {
            scale = Easing::InSine(timer, maxTime, 1.0f, 0.0f);

            // タイマー加算
            timer += elapsedTime;
        }
        else
        {
            // 次のステートへ
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(MagicCircleSummon::StateMachineState::ExpandState));
            return;
        }

        for (int i = 0; i < 3; ++i)
        {
            // 回転
            owner->magicCircle[i]->GetTransform()->AddRotationY(elapsedTime);
            // スケール
            owner->magicCircle[i]->GetTransform()->SetScale(DirectX::XMFLOAT3(scale, scale, scale));
        }
    }

    // 終了化
    void AppearState::Finalize()
    {
    }
}

namespace MagicCircleState
{
    // 初期化
    void ExpandState::Initialize()
    {
        timer = 0.0f;
        subTimer = 0.0f;
    }

    // 更新
    void ExpandState::Update(const float& elapsedTime)
    {
        if (timer <= maxTime)
        {
            scale = Easing::OutQuint(timer, maxTime, 2.0f, 1.0f);
            posY = Easing::OutQuint(timer, maxTime, 0.5f, 0.0f);

            for (int i = 0; i < 2; ++i)
            {
                owner->magicCircle[i + 1]->GetTransform()->SetPositionY(posY);
                owner->magicCircle[i + 1]->GetTransform()->AddRotationY(elapsedTime * 2.0f);
                owner->magicCircle[i + 1]->GetTransform()->SetScale(DirectX::XMFLOAT3(scale, scale, scale));
            }            

            timer += elapsedTime;
        }
        else
        {
            if (subTimer <= maxSubTime)
            {
                scale = Easing::OutQuint(subTimer, maxSubTime, 1.5f, 2.0f);
                posY = Easing::OutQuint(subTimer, maxSubTime, 1.0f, 0.5f);

                owner->magicCircle[1]->GetTransform()->AddRotationY(elapsedTime * 2.0f);

                owner->magicCircle[2]->GetTransform()->SetPositionY(posY);
                owner->magicCircle[2]->GetTransform()->AddRotationY(elapsedTime * 2.5f);
                owner->magicCircle[2]->GetTransform()->SetScale(DirectX::XMFLOAT3(scale, scale, scale));
                
                subTimer += elapsedTime;
            }
            else
            {
                owner->magicCircle[1]->GetTransform()->AddRotationY(elapsedTime * 1.5f);

                owner->magicCircle[2]->GetTransform()->AddRotationY(elapsedTime * 2.5f);
            }

        }

        owner->magicCircle[0]->GetTransform()->AddRotationY(elapsedTime);
    }

    // 終了化
    void ExpandState::Finalize()
    {
    }
}