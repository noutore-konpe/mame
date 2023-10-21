#include "BookState.h"

#include "../../Taki174/FunctionXMFloat3.h"
#include "Book.h"
#include "PlayerManager.h"
#include "EnemyManager.h"

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

        // 待機時間が終わったら
        if (GetTimer() < 0.0f)
        {
            // 生きている敵が見つからなかったらreturn
            if (false == SearchAliveEnemy()) return;

            // Openステートへ
            owner->GetStateMachine()->ChangeState(static_cast<UINT>(Book::StateMachineState::Open));
        }
    }

    // 終了化
    void IdleState::Finalize()
    {
    }

    // 生きている敵を探す
    const bool IdleState::SearchAliveEnemy()
    {
        using DirectX::XMFLOAT3;

        // 生きている敵を探す
        EnemyManager& enemyManager = EnemyManager::Instance();
        const size_t  enemyCount   = enemyManager.GetEnemyCount();
        for (size_t i = 0; i < enemyCount; ++i)
        {
            // 敵取得
            Enemy* enemy = enemyManager.GetEnemy(i);

            // 死んでいたらcontinue;
            if (true == enemy->GetIsDead()) continue;

            const XMFLOAT3& bookPos  = owner->GetTransform()->GetPosition();
            const XMFLOAT3& enemyPos = enemy->GetPosition();
            const float     lengthSq = ::XMFloat3LengthSq(enemyPos - bookPos);

            // 本の索敵範囲外ならcontinue
            const float searchEnemyRangeSq = owner->GetSearchEnemyRange() * owner->GetSearchEnemyRange();
            if (lengthSq > searchEnemyRangeSq) continue;

            // 見つかった
            return true;
        }

        // 見つからなかった
        return false;
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

            //// アニメーション短くて変な感じだから少し間隔を取る
            //SetTimer(1.0f);
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
    void JabAttackState::Initialize()
    {
        // アニメーションを設定(ページぺらぺら)
        owner->PlayAnimation(static_cast<UINT>(Book::Animation::TurnThePage), true);

        // 発射数リセット
        launchNum = 0;

        // 発射できるか
        isLaunch = true;

        // 最初の発射をスムーズにさせるための設定
        //owner->SetLaunchTimer(0.0f);
        owner->SetLaunchTimer(0.5f); // 本が敵に向ききるまでの時間をつくる
    }

    // 更新処理
    void JabAttackState::Update(const float& elapsedTime)
    {
        using DirectX::XMFLOAT3;

        // 弾を撃ち切ったらCloseステートへ遷移する
        if (false == isLaunch)
        {
            // タイマー更新
            SubtractTime(elapsedTime);

            // タイマーが終了していたらCloseステートへ遷移する
            if (GetTimer() <= 0.0f)
            {
                owner->GetStateMachine()->ChangeState(static_cast<UINT>(Book::StateMachineState::Close));
                return;
            }

            // 以降の処理を飛ばす
            return;
        }

        // 一番近い敵を探す
        Enemy* enemy = nullptr;
        const bool isFoundEnemy = SearchNearEnemy(&enemy);

        // 敵が見つからなかったら
        if (false == isFoundEnemy)
        {
            // タイマー更新
            SubtractTime(elapsedTime);

            // タイマーが終了していたらCloseステートへ遷移する
            if (GetTimer() <= 0.0f)
            {
                owner->GetStateMachine()->ChangeState(static_cast<UINT>(Book::StateMachineState::Close));
                return;
            }

            // 以降の処理を飛ばす
            return;
        }
        // 敵を見つけていたら
        else
        {
            // 見つかなかった時に少し間を開けてCloseステートへ遷移するようにする
            SetTimer(0.25f);
        }

        // 敵に向かうベクトルを取得
        const XMFLOAT3& pos         = owner->GetTransform()->GetPosition();
              XMFLOAT3  enemyPos    = enemy->GetPosition();
                        enemyPos.y += enemy->GetHeight() * 0.5f; // 敵の中心になるように高さを調整
        const XMFLOAT3  vec         = enemyPos - pos;

        // 回転値
        {
            // XZ回転値を設定
            DirectX::XMFLOAT4 bookRot = PlayerManager::Instance().GetPlayer()->GetTransform()->GetRotation();
            bookRot.x += DirectX::XMConvertToRadians(320.0f); // X回転値調整
            owner->GetTransform()->SetRotationX(bookRot.x);
            owner->GetTransform()->SetRotationZ(bookRot.z);

            // 敵の方を向く(Y回転値更新)
            const float turnSpeed = 630.0f; // 回転速度は速めに設定
            owner->Turn(elapsedTime, vec.x, vec.z, turnSpeed); // Degree
        }

        // 弾発射処理
        if (true == owner->LaunchProjectile(elapsedTime, vec))
        {
            ++launchNum;
        }

        // 一回で発射できる最大数に達したら発射フラグをOFFにする
        if (launchNum >= owner->GetMaxLaunchNum())
        {
            isLaunch = false;

            // 少し間を開けてからCloseステートへ遷移するようにする
            SetTimer(0.25f);
        }

    }

    // 終了化
    void JabAttackState::Finalize()
    {
    }

    // 一番近い敵を探す
    const bool JabAttackState::SearchNearEnemy(Enemy** enemy)
    {
        using DirectX::XMFLOAT3;

        // 戻り値ミス防止のためにクリアしておく
        (*enemy) = nullptr;

        // 前回の本と敵との距離(二乗)
        float oldLengthSq = FLT_MAX;

        // 生きている一番近い敵を探す
        EnemyManager& enemyManager = EnemyManager::Instance();
        const size_t  enemyCount   = enemyManager.GetEnemyCount();
        for (size_t i = 0; i < enemyCount; ++i)
        {
            // 敵取得
            Enemy* otherEnemy = enemyManager.GetEnemy(i);

            // 死んでいたらcontinue;
            if (true == otherEnemy->GetIsDead()) continue;

            const XMFLOAT3& bookPos  = owner->GetTransform()->GetPosition();
            const XMFLOAT3& otherPos = otherEnemy->GetPosition();
            const float     lengthSq = ::XMFloat3LengthSq(otherPos - bookPos);

            // 本の索敵範囲外ならcontinue
            const float searchEnemyRangeSq = owner->GetSearchEnemyRange() * owner->GetSearchEnemyRange();
            if (lengthSq > searchEnemyRangeSq) continue;

            // 前回の本と敵との距離(二乗)より近ければこの敵を代入する
            if (lengthSq < oldLengthSq)
            {
                (*enemy)    = otherEnemy;
                oldLengthSq = lengthSq;
            }

        }

        if ((*enemy) != nullptr) { return true;  } // 敵が見つかった
        else                     { return false; } // 敵が見つからなかった
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
        PlayerManager& plManager = PlayerManager::Instance();

        SubtractTime(elapsedTime);

        // 回転値
        {
            // XZ回転値設定
            DirectX::XMFLOAT4 bookRot = plManager.GetPlayer()->GetTransform()->GetRotation();
            bookRot.x += DirectX::XMConvertToRadians(320.0f); // X回転値調整
            owner->GetTransform()->SetRotationX(bookRot.x);
            owner->GetTransform()->SetRotationZ(bookRot.z);

            // プレイヤーが向いてる方向に戻す(Y回転値更新)
            const float vecX = ::sinf(plManager.GetPlayer()->GetTransform()->GetRotationY());
            const float vecZ = ::cosf(plManager.GetPlayer()->GetTransform()->GetRotationY());
            const float turnSpeed = 135.0f; // 戻るときはゆっくりめに設定
            owner->Turn(elapsedTime, vecX, vecZ, turnSpeed); // Degree
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