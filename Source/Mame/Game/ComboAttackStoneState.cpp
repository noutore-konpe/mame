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
    // ������
    void AppearState::Initialize()
    {
        // �ʒu�ݒ�
        {
            DirectX::XMFLOAT3 ownerPos = owner->GetOwnerTransform()->GetPosition();
            DirectX::XMFLOAT3 ownerFront = owner->GetOwnerTransform()->CalcForward();

            DirectX::XMFLOAT3 setPosition = ownerFront * 2.5f;
            setPosition = ownerPos + setPosition;

            owner->magicCircle->GetTransform()->SetPosition(setPosition);
            
            setPosition.y = -1.0f;
            owner->stoneBall->GetTransform()->SetPosition(setPosition);
        }

        // �ϐ�
        magicCircleTimer = 0.0f;
        stoneUpTimer = 0.0f;
    }

    // �X�V
    void AppearState::Update(const float& elapsedTime)
    {
        // ���@�w�W�J
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

    // �I����
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