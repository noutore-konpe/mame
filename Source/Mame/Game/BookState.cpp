#include "BookState.h"

#include "../../Taki174/FunctionXMFloat3.h"
#include "Book.h"
#include "PlayerManager.h"
#include "EnemyManager.h"

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

        // �ҋ@���Ԃ��I�������
        if (GetTimer() < 0.0f)
        {
            // �����Ă���G��������Ȃ�������return
            if (false == SearchAliveEnemy()) return;

            // Open�X�e�[�g��
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(Book::StateMachineState::Open));
        }
    }

    // �I����
    void IdleState::Finalize()
    {
    }

    // �����Ă���G��T��
    const bool IdleState::SearchAliveEnemy()
    {
        using DirectX::XMFLOAT3;

        // �����Ă���G��T��
        EnemyManager& enemyManager = EnemyManager::Instance();
        const size_t  enemyCount   = enemyManager.GetEnemyCount();
        for (size_t i = 0; i < enemyCount; ++i)
        {
            // �G�擾
            Enemy* enemy = enemyManager.GetEnemy(i);

            // ����ł�����continue;
            if (true == enemy->GetIsDead()) continue;

            const XMFLOAT3& bookPos  = owner->GetTransform()->GetPosition();
            const XMFLOAT3& enemyPos = enemy->GetPosition();
            const float     lengthSq = ::XMFloat3LengthSq(enemyPos - bookPos);

            // �{�̍��G�͈͊O�Ȃ�continue
            const float searchEnemyRangeSq = owner->GetSearchEnemyRange() * owner->GetSearchEnemyRange();
            if (lengthSq > searchEnemyRangeSq) continue;

            // ��������
            return true;
        }

        // ������Ȃ�����
        return false;
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

            //// �A�j���[�V�����Z���ĕςȊ��������班���Ԋu�����
            //SetTimer(1.0f);
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
    void JabAttackState::Initialize()
    {
        // �A�j���[�V������ݒ�(�y�[�W�؂�؂�)
        owner->PlayAnimation(static_cast<UINT>(Book::Animation::TurnThePage), true);

        // ���ː����Z�b�g
        launchNum = 0;

        // ���˂ł��邩
        isLaunch = true;

        // �ŏ��̔��˂��X���[�Y�ɂ����邽�߂̐ݒ�
        //owner->SetLaunchTimer(0.0f);
        owner->SetLaunchTimer(0.5f); // �{���G�Ɍ�������܂ł̎��Ԃ�����
    }

    // �X�V����
    void JabAttackState::Update(const float& elapsedTime)
    {
        using DirectX::XMFLOAT3;

        // �e�������؂�����Close�X�e�[�g�֑J�ڂ���
        if (false == isLaunch)
        {
            // �^�C�}�[�X�V
            SubtractTime(elapsedTime);

            // �^�C�}�[���I�����Ă�����Close�X�e�[�g�֑J�ڂ���
            if (GetTimer() <= 0.0f)
            {
                owner->GetStateMachine()->ChangeState(static_cast<UINT>(Book::StateMachineState::Close));
                return;
            }

            // �ȍ~�̏������΂�
            return;
        }

        // ��ԋ߂��G��T��
        Enemy* enemy = nullptr;
        const bool isFoundEnemy = SearchNearEnemy(&enemy);

        // �G��������Ȃ�������
        if (false == isFoundEnemy)
        {
            // �^�C�}�[�X�V
            SubtractTime(elapsedTime);

            // �^�C�}�[���I�����Ă�����Close�X�e�[�g�֑J�ڂ���
            if (GetTimer() <= 0.0f)
            {
                owner->GetStateMachine()->ChangeState(static_cast<UINT>(Book::StateMachineState::Close));
                return;
            }

            // �ȍ~�̏������΂�
            return;
        }
        // �G�������Ă�����
        else
        {
            // �����Ȃ��������ɏ����Ԃ��J����Close�X�e�[�g�֑J�ڂ���悤�ɂ���
            SetTimer(0.25f);
        }

        // �G�Ɍ������x�N�g�����擾
        const XMFLOAT3& pos         = owner->GetTransform()->GetPosition();
              XMFLOAT3  enemyPos    = enemy->GetPosition();
                        enemyPos.y += enemy->GetHeight() * 0.5f; // �G�̒��S�ɂȂ�悤�ɍ����𒲐�
        const XMFLOAT3  vec         = enemyPos - pos;

        // ��]�l
        {
            // XZ��]�l��ݒ�
            DirectX::XMFLOAT4 bookRot = PlayerManager::Instance().GetPlayer()->GetTransform()->GetRotation();
            bookRot.x += DirectX::XMConvertToRadians(320.0f); // X��]�l����
            owner->GetTransform()->SetRotationX(bookRot.x);
            owner->GetTransform()->SetRotationZ(bookRot.z);

            // �G�̕�������(Y��]�l�X�V)
            const float turnSpeed = 630.0f; // ��]���x�͑��߂ɐݒ�
            owner->Turn(elapsedTime, vec.x, vec.z, turnSpeed); // Degree
        }

        // �e���ˏ���
        if (true == owner->LaunchProjectile(elapsedTime, vec))
        {
            ++launchNum;
        }

        // ���Ŕ��˂ł���ő吔�ɒB�����甭�˃t���O��OFF�ɂ���
        if (launchNum >= owner->GetMaxLaunchNum())
        {
            isLaunch = false;

            // �����Ԃ��J���Ă���Close�X�e�[�g�֑J�ڂ���悤�ɂ���
            SetTimer(0.25f);
        }

    }

    // �I����
    void JabAttackState::Finalize()
    {
    }

    // ��ԋ߂��G��T��
    const bool JabAttackState::SearchNearEnemy(Enemy** enemy)
    {
        using DirectX::XMFLOAT3;

        // �߂�l�~�X�h�~�̂��߂ɃN���A���Ă���
        (*enemy) = nullptr;

        // �O��̖{�ƓG�Ƃ̋���(���)
        float oldLengthSq = FLT_MAX;

        // �����Ă����ԋ߂��G��T��
        EnemyManager& enemyManager = EnemyManager::Instance();
        const size_t  enemyCount   = enemyManager.GetEnemyCount();
        for (size_t i = 0; i < enemyCount; ++i)
        {
            // �G�擾
            Enemy* otherEnemy = enemyManager.GetEnemy(i);

            // ����ł�����continue;
            if (true == otherEnemy->GetIsDead()) continue;

            const XMFLOAT3& bookPos  = owner->GetTransform()->GetPosition();
            const XMFLOAT3& otherPos = otherEnemy->GetPosition();
            const float     lengthSq = ::XMFloat3LengthSq(otherPos - bookPos);

            // �{�̍��G�͈͊O�Ȃ�continue
            const float searchEnemyRangeSq = owner->GetSearchEnemyRange() * owner->GetSearchEnemyRange();
            if (lengthSq > searchEnemyRangeSq) continue;

            // �O��̖{�ƓG�Ƃ̋���(���)���߂���΂��̓G��������
            if (lengthSq < oldLengthSq)
            {
                (*enemy)    = otherEnemy;
                oldLengthSq = lengthSq;
            }

        }

        if ((*enemy) != nullptr) { return true;  } // �G����������
        else                     { return false; } // �G��������Ȃ�����
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
        PlayerManager& plManager = PlayerManager::Instance();

        SubtractTime(elapsedTime);

        // ��]�l
        {
            // XZ��]�l�ݒ�
            DirectX::XMFLOAT4 bookRot = plManager.GetPlayer()->GetTransform()->GetRotation();
            bookRot.x += DirectX::XMConvertToRadians(320.0f); // X��]�l����
            owner->GetTransform()->SetRotationX(bookRot.x);
            owner->GetTransform()->SetRotationZ(bookRot.z);

            // �v���C���[�������Ă�����ɖ߂�(Y��]�l�X�V)
            const float vecX = ::sinf(plManager.GetPlayer()->GetTransform()->GetRotationY());
            const float vecZ = ::cosf(plManager.GetPlayer()->GetTransform()->GetRotationY());
            const float turnSpeed = 135.0f; // �߂�Ƃ��͂������߂ɐݒ�
            owner->Turn(elapsedTime, vecX, vecZ, turnSpeed); // Degree
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