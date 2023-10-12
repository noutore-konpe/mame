#include "ComboAttackStoneState.h"

#include "../Other/MathHelper.h"

#include "PlayerManager.h"

#include "../Input/Input.h"

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
        // �΂̃T�C�Y�ݒ�
        owner->stoneBall->GetTransform()->SetScale(DirectX::XMFLOAT3(4.0f, 4.0f, 4.0f));
        // �΂̐F��������
        owner->stoneBall->model->SetModelColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

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

        // �ϐ�������
        magicCircleTimer = 0.0f;
        stoneUpTimer = 0.0f;
    }

    // �X�V
    void AppearState::Update(const float& elapsedTime)
    {
        // ���f���ʒu�ݒ�
        SetModelPosition();

        // ���@�w�W�J
        if (magicCircleTimer <= maxMagicCircleTime)
        {
            float scale = Easing::InSine(magicCircleTimer, maxMagicCircleTime, 2.0f, 0.0f);

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
            else
            {   // �X�e�[�g�i�߂�
                owner->GetStateMachine()->ChangeState(static_cast<UINT>(ComboAttackStone::StateMachineState::ThrowState));
                return;
            }
        }
    }

    // �I����
    void AppearState::Finalize()
    {

    }

    // ���f���ʒu�ݒ�
    void AppearState::SetModelPosition()
    {
        DirectX::XMFLOAT3 ownerPos = owner->GetOwnerTransform()->GetPosition();
        DirectX::XMFLOAT3 ownerFront = owner->GetOwnerTransform()->CalcForward();
        DirectX::XMFLOAT3 setPosition = ownerFront * 2.5f;
        setPosition = ownerPos + setPosition;

        setPosition.y = 0.05f;

        // ���@�w�̈ʒu�ݒ�
        owner->magicCircle->GetTransform()->SetPosition(setPosition);

        DirectX::XMFLOAT3 stonePos = owner->stoneBall->GetTransform()->GetPosition();
        setPosition.y = stonePos.y;

        // ���@�w�̈ʒu�ݒ�
        owner->stoneBall->GetTransform()->SetPosition(setPosition);
    }
}

// ThrowState
namespace ComboAttackStoneState
{
    // ������
    void ThrowState::Initialize()
    {
        DirectX::XMFLOAT3 playerPos = PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition();
        DirectX::XMFLOAT3 ownerPos = owner->GetOwnerTransform()->GetPosition();
        DirectX::XMFLOAT3 vec = playerPos - ownerPos;
        targetVec = Normalize(vec);
        length = Length(vec);

        // �ϐ�������
        magicCircleTimer = 0.0f;
        stoneFallTimer = 0.0f;

        cameraShake = false;

        delayTimer = 0.0f;
    }

    // �X�V
    void ThrowState::Update(const float& elapsedTime)
    {
        // ���@�w�Ђ����߂�
        if (magicCircleTimer <= maxMagicCircleTime)
        {
            float scale = Easing::OutCubic(magicCircleTimer, maxMagicCircleTime, 0.0f, 2.0f);

            owner->magicCircle->GetTransform()->SetScale(DirectX::XMFLOAT3(scale, scale, scale));

            magicCircleTimer += elapsedTime;
        }

        // �΂𓊂��鏈��
        if (stoneFallTimer <= maxStoneFallTime)
        {
            float posY = Easing::InSine(stoneFallTimer, maxStoneFallTime, 0.0f, 3.0f);
            owner->stoneBall->GetTransform()->SetPositionY(posY);

            DirectX::XMFLOAT3 addPos = targetVec * length * 1.7f * elapsedTime;
            addPos.y = 0.0f;
            owner->stoneBall->GetTransform()->AddPosition(addPos);
            

            stoneFallTimer += elapsedTime;
        }
        else
        {
            if (!cameraShake)
            {   // �J�����U��
                Camera::Instance().ScreenVibrate(0.05f, 0.8f);
                cameraShake = true;

                // �Q�[���p�b�h�U��
                Input::Instance().GetGamePad().Vibration(0.2f, gamePadVibPower);
            }

            // �f�B���C
            if (delayTimer <= maxDelayTime)
            {
                
                if (delayTimer >= 0.3f)
                {
                    float colorW = Easing::InQuint(delayTimer, maxDelayTime, 0.0f, 1.0f);

                    DirectX::XMFLOAT4 color = owner->stoneBall->model->GetModelColor();
                    color.w = colorW;
                    owner->stoneBall->model->SetModelColor(color);
                }

                delayTimer += elapsedTime;
            }
            else
            {
                owner->isChangeState = true;
                owner->GetStateMachine()->ChangeState(static_cast<UINT>(ComboAttackStone::StateMachineState::DummyState));
                return;
            }
        }
    }
    
    // �I����
    void ThrowState::Finalize()
    {
    }
}
