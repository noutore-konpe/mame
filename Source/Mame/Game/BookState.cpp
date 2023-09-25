#include "BookState.h"
#include "Book.h"
#include "PlayerManager.h"

// IdleState
namespace BookState
{
    // 初期化
    void IdleState::Initialize()
    {
        // アニメーションを設定(閉じた状態)
        owner->PlayAnimation(static_cast<UINT>(Book::Animation::Closed), true);

        SetTimer(3.0f);
    }

    // 更新処理
    void IdleState::Update(const float& elapsedTime)
    {
        SubtractTime(elapsedTime);

        // 回転値
        {
            DirectX::XMFLOAT4 bookRot = PlayerManager::Instance().GetPlayer()->GetTransform()->GetRotation();
            bookRot.x += DirectX::XMConvertToRadians(320.0f);

            // 回転値を設定
            owner->GetTransform()->SetRotation(bookRot);
        }

        // 待機時間が終わったらOpenステートへ
        if (GetTimer() < 0.0f)
        {
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(Book::StateMachineState::Open));
        }
    }

    // 終了化
    void IdleState::Finalize()
    {
    }
}

// OpenState
namespace BookState
{
    // 初期化
    void OpenState::Initialize()
    {
        // アニメーションを設定(開く)
        owner->PlayAnimation(static_cast<UINT>(Book::Animation::Open), false);

        isOpen = false;

        // ランダムで回転するか決める
        // case RotateState::ContenuOpenState には入らないようにしたい
        //rotateToOpen = rand() % 4; 
        ++rotateToOpen;
        if (rotateToOpen > RotateState::DiagonalRot)rotateToOpen = 0;
        addBookRot = {};
    }

    // 更新処理
    void OpenState::Update(const float& elapsedTime)
    {
        SubtractTime(elapsedTime);

        // 回転値
        {
            DirectX::XMFLOAT4 bookRot = PlayerManager::Instance().GetPlayer()->GetTransform()->GetRotation();
            bookRot.x += DirectX::XMConvertToRadians(320.0f);

            // 確率で回転する
            switch (rotateToOpen)
            {
            case RotateState::No_Rot: break; // 回転なし
            case RotateState::VerticalRot: // 縦回転
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
#if 0   // こいつはクビ
            case RotateState::HorizontalRot: // 横回転
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

            // 回転値を設定
            owner->GetTransform()->SetRotation(bookRot);
        }

        // 開くアニメーションが終わったら いい感じにする処理
        if (!owner->IsPlayAnimation())
        {
            owner->PlayAnimation(static_cast<UINT>(Book::Animation::Opened), true);

            // アニメーション短くて変な感じだから少し間隔を取る
            SetTimer(1.0f);
            isOpen = true;
        }
        
        // Attackステートへ
        if (GetTimer() < 0.0f && isOpen)
        {
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(Book::StateMachineState::Attack));
        }
    }

    // 終了化
    void OpenState::Finalize()
    {
    }
}

// AttackState
namespace BookState
{
    // 初期化
    void AttackState::Initialize()
    {
        // アニメーションを設定(ページぺらぺら)
        owner->PlayAnimation(static_cast<UINT>(Book::Animation::TurnThePage), true);

        // 発射数リセット
        launchNum = 0;

        // 発射できるか
        isLaunch = true;

        // 最初の発射をスムーズにさせるための設定
        owner->SetLaunchTimer(0.0f);
    }

    // 更新処理
    void AttackState::Update(const float& elapsedTime)
    {
        // 回転値
        {
            DirectX::XMFLOAT4 bookRot = PlayerManager::Instance().GetPlayer()->GetTransform()->GetRotation();
            bookRot.x += DirectX::XMConvertToRadians(320.0f);

            // 回転値を設定
            owner->GetTransform()->SetRotation(bookRot);
        }

        // 発射できるか
        if (isLaunch)
        {
            // 弾発射処理
            if (owner->LaunchProjectile(elapsedTime))
            {
                ++launchNum;
            }

            // バグが起きないようにここで仮に設定
            SetTimer(10.0f);
        }
        else
        {
            SubtractTime(elapsedTime);
        }

        // 一回で発射できる最大数に達した
        if (owner->GetMaxLaunchNum() <= launchNum && isLaunch)
        {
            isLaunch = false;
            SetTimer(1.0f);
        }

        // Closeステートへ
        if (GetTimer() < 0.0f && !isLaunch)
        {
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(Book::StateMachineState::Close));
        }
    }

    // 終了化
    void AttackState::Finalize()
    {
    }
}

// CloseState
namespace BookState
{
    // 初期化
    void CloseState::Initialize()
    {
        // アニメーションを設定
        owner->PlayAnimation(static_cast<UINT>(Book::Animation::Opened), true);

        // タイマーを設定
        SetTimer(1.0f);

        isClose = false;
    }

    // 更新処理
    void CloseState::Update(const float& elapsedTime)
    {
        SubtractTime(elapsedTime);

        // 回転値
        {
            DirectX::XMFLOAT4 bookRot = PlayerManager::Instance().GetPlayer()->GetTransform()->GetRotation();
            bookRot.x += DirectX::XMConvertToRadians(320.0f);

            // 回転値を設定
            owner->GetTransform()->SetRotation(bookRot);
        }

        // 閉じるまでのディレイを作る
        if (GetTimer() < 0.0f && !isClose)
        {
            owner->PlayAnimation(static_cast<UINT>(Book::Animation::Close), false);
            isClose = true;
        }


        // 閉じるアニメーションが終わったら Idleステートへ
        if (!owner->IsPlayAnimation())
        {
            owner->PlayAnimation(static_cast<UINT>(Book::Animation::Closed), true);
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(Book::StateMachineState::Idle));
        }
    }

    // 終了化
    void CloseState::Finalize()
    {
    }
}