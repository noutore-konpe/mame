#include "ComboAttackStoneState.h"

#include "../Other/MathHelper.h"

// DummyState
namespace ComboAttackStoneState
{
    void DummyState::Initialize()
    {
        owner->stoneBall->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f));
        owner->magicCircle->GetTransform()->SetScale(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
        owner->magicCircle->GetTransform()->SetRotation(DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f));
    }

    void DummyState::Update(const float& elapsedTime)
    {
    }
}

// AppearState
namespace ComboAttackStoneState
{
    // 初期化
    void AppearState::Initialize()
    {
        // 位置設定
        {
            DirectX::XMFLOAT3 ownerPos = owner->GetOwnerTransform()->GetPosition();
            DirectX::XMFLOAT3 ownerFront = owner->GetOwnerTransform()->CalcForward();

            DirectX::XMFLOAT3 setPosition = ownerFront * 2.5f;
            setPosition = ownerPos + setPosition;

            owner->magicCircle->GetTransform()->SetPosition(setPosition);
            
            setPosition.y = -1.0f;
            owner->stoneBall->GetTransform()->SetPosition(setPosition);
        }

        // 変数
        magicCircleTimer = 0.0f;
        stoneUpTimer = 0.0f;
    }

    // 更新
    void AppearState::Update(const float& elapsedTime)
    {
        // 魔法陣展開
        if (magicCircleTimer <= maxMagicCircleTime)
        {
            float scale = Easing::InSine(magicCircleTimer, maxMagicCircleTime, 1.0f, 0.0f);

            owner->magicCircle->GetTransform()->SetScale(DirectX::XMFLOAT3(scale, scale, scale));

            owner->magicCircle->GetTransform()->AddRotationY(elapsedTime * 5);

            magicCircleTimer += elapsedTime;
        }
        else
        {
            if (stoneUpTimer <= maxStoneUpTime)
            {
                float posY = Easing::OutCubic(stoneUpTimer, maxStoneUpTime, 3.0f, -1.0f);

                owner->stoneBall->GetTransform()->SetPositionY(posY);

                stoneUpTimer += elapsedTime;
            }
        }
    }

    // 終了化
    void AppearState::Finalize()
    {

    }
}

namespace ComboAttackStoneState
{

}

namespace ComboAttackStoneState
{

}

namespace ComboAttackStoneState
{

}