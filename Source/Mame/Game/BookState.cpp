#include "BookState.h"
#include "Book.h"
#include "PlayerManager.h"

// IdleState
namespace BookState
{
    // ������
    void IdleState::Initialize()
    {
        // �A�j���[�V������ݒ�(�������)
        owner->PlayAnimation(static_cast<UINT>(Book::Animation::Closed), true);

        SetTimer(3.0f);
    }

    // �X�V����
    void IdleState::Update(const float& elapsedTime)
    {
        SubtractTime(elapsedTime);

        // ��]�l
        {
            DirectX::XMFLOAT4 bookRot = PlayerManager::Instance().GetPlayer()->GetTransform()->GetRotation();
            bookRot.x += DirectX::XMConvertToRadians(320.0f);

            // ��]�l��ݒ�
            owner->GetTransform()->SetRotation(bookRot);
        }

        // �ҋ@���Ԃ��I�������Open�X�e�[�g��
        if (GetTimer() < 0.0f)
        {
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(Book::StateMachineState::Open));
        }
    }

    // �I����
    void IdleState::Finalize()
    {
    }
}

// OpenState
namespace BookState
{
    // ������
    void OpenState::Initialize()
    {
        // �A�j���[�V������ݒ�(�J��)
        owner->PlayAnimation(static_cast<UINT>(Book::Animation::Open), false);

        isOpen = false;

        // �����_���ŉ�]���邩���߂�
        // case RotateState::ContenuOpenState �ɂ͓���Ȃ��悤�ɂ�����
        //rotateToOpen = rand() % 4; 
        ++rotateToOpen;
        if (rotateToOpen > RotateState::DiagonalRot)rotateToOpen = 0;
        addBookRot = {};
    }

    // �X�V����
    void OpenState::Update(const float& elapsedTime)
    {
        SubtractTime(elapsedTime);

        // ��]�l
        {
            DirectX::XMFLOAT4 bookRot = PlayerManager::Instance().GetPlayer()->GetTransform()->GetRotation();
            bookRot.x += DirectX::XMConvertToRadians(320.0f);

            // �m���ŉ�]����
            switch (rotateToOpen)
            {
            case RotateState::No_Rot: break; // ��]�Ȃ�
            case RotateState::VerticalRot: // �c��]
                if (addBookRot.x <= DirectX::XMConvertToRadians(720))
                {
                    //owner->PlayAnimation(static_cast<UINT>(Book::Animation::Closed), false);
                    addBookRot.x += DirectX::XMConvertToRadians(720) * elapsedTime;
                    bookRot.x += addBookRot.x;
                }
                else
                {
                    //owner->PlayAnimation(static_cast<UINT>(Book::Animation::Open), false, 2.0f);
                    //rotateToOpen = RotateState::ContenuOpenState;
                }
                break;
#if 0   // �����̓N�r
            case RotateState::HorizontalRot: // ����]
                if (addBookRot.y <= DirectX::XMConvertToRadians(720))
                {
                    addBookRot.y += (DirectX::XMConvertToRadians(720) * elapsedTime) * 2;
                    bookRot.y += addBookRot.y;
                }
                break;
#endif
            case RotateState::HorizontalRot:
                if (addBookRot.y <= DirectX::XMConvertToRadians(720))
                {
                    owner->PlayAnimation(static_cast<UINT>(Book::Animation::Closed), false);
                    addBookRot.y += (DirectX::XMConvertToRadians(720) * elapsedTime) * 2;
                    bookRot.y += addBookRot.y;
                }
                else
                {
                    owner->PlayAnimation(static_cast<UINT>(Book::Animation::Open), false, 2.0f);
                    rotateToOpen = RotateState::ContenuOpenState;
                }
                break;
            case RotateState::DiagonalRot:
                if (addBookRot.x <= DirectX::XMConvertToRadians(720))
                {
                    addBookRot.x += (DirectX::XMConvertToRadians(720) * elapsedTime);
                    bookRot.x += addBookRot.x;
                }
                if (addBookRot.y <= DirectX::XMConvertToRadians(720))
                {
                    addBookRot.y += (DirectX::XMConvertToRadians(720) * elapsedTime);
                    bookRot.y += addBookRot.y;
                }

                break;
            case RotateState::ContenuOpenState:
                if (!owner->IsPlayAnimation())
                {
                    owner->GetStateMachine()->ChangeState(static_cast<UINT>(Book::StateMachineState::Attack));
                }
                break;
            }

            // ��]�l��ݒ�
            owner->GetTransform()->SetRotation(bookRot);
        }

        // �J���A�j���[�V�������I������� ���������ɂ��鏈��
        if (!owner->IsPlayAnimation())
        {
            owner->PlayAnimation(static_cast<UINT>(Book::Animation::Opened), true);

            // �A�j���[�V�����Z���ĕςȊ��������班���Ԋu�����
            SetTimer(1.0f);
            isOpen = true;
        }
        
        // Attack�X�e�[�g��
        if (GetTimer() < 0.0f && isOpen)
        {
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(Book::StateMachineState::Attack));
        }
    }

    // �I����
    void OpenState::Finalize()
    {
    }
}

// AttackState
namespace BookState
{
    // ������
    void AttackState::Initialize()
    {
        // �A�j���[�V������ݒ�(�y�[�W�؂�؂�)
        owner->PlayAnimation(static_cast<UINT>(Book::Animation::TurnThePage), true);

        // ���ː����Z�b�g
        launchNum = 0;

        // ���˂ł��邩
        isLaunch = true;

        // �ŏ��̔��˂��X���[�Y�ɂ����邽�߂̐ݒ�
        owner->SetLaunchTimer(0.0f);
    }

    // �X�V����
    void AttackState::Update(const float& elapsedTime)
    {
        // ��]�l
        {
            DirectX::XMFLOAT4 bookRot = PlayerManager::Instance().GetPlayer()->GetTransform()->GetRotation();
            bookRot.x += DirectX::XMConvertToRadians(320.0f);

            // ��]�l��ݒ�
            owner->GetTransform()->SetRotation(bookRot);
        }

        // ���˂ł��邩
        if (isLaunch)
        {
            // �e���ˏ���
            if (owner->LaunchProjectile(elapsedTime))
            {
                ++launchNum;
            }

            // �o�O���N���Ȃ��悤�ɂ����ŉ��ɐݒ�
            SetTimer(10.0f);
        }
        else
        {
            SubtractTime(elapsedTime);
        }

        // ���Ŕ��˂ł���ő吔�ɒB����
        if (owner->GetMaxLaunchNum() <= launchNum && isLaunch)
        {
            isLaunch = false;
            SetTimer(1.0f);
        }

        // Close�X�e�[�g��
        if (GetTimer() < 0.0f && !isLaunch)
        {
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(Book::StateMachineState::Close));
        }
    }

    // �I����
    void AttackState::Finalize()
    {
    }
}

// CloseState
namespace BookState
{
    // ������
    void CloseState::Initialize()
    {
        // �A�j���[�V������ݒ�
        owner->PlayAnimation(static_cast<UINT>(Book::Animation::Opened), true);

        // �^�C�}�[��ݒ�
        SetTimer(1.0f);

        isClose = false;
    }

    // �X�V����
    void CloseState::Update(const float& elapsedTime)
    {
        SubtractTime(elapsedTime);

        // ��]�l
        {
            DirectX::XMFLOAT4 bookRot = PlayerManager::Instance().GetPlayer()->GetTransform()->GetRotation();
            bookRot.x += DirectX::XMConvertToRadians(320.0f);

            // ��]�l��ݒ�
            owner->GetTransform()->SetRotation(bookRot);
        }

        // ����܂ł̃f�B���C�����
        if (GetTimer() < 0.0f && !isClose)
        {
            owner->PlayAnimation(static_cast<UINT>(Book::Animation::Close), false);
            isClose = true;
        }


        // ����A�j���[�V�������I������� Idle�X�e�[�g��
        if (!owner->IsPlayAnimation())
        {
            owner->PlayAnimation(static_cast<UINT>(Book::Animation::Closed), true);
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(Book::StateMachineState::Idle));
        }
    }

    // �I����
    void CloseState::Finalize()
    {
    }
}