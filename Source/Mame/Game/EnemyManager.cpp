#include "EnemyManager.h"
#include "../Game/Common.h"
#include "../Game/BossStateDerived.h"
#include "../Mame/AudioManager.h"


void EnemyManager::Initialize()
{
    for (Enemy*& enemy : enemies)
    {
        enemy->Initialize();
    }
}


void EnemyManager::Finalize()
{
    for (Enemy*& enemy : enemies)
    {
        enemy->Finalize();
    }
}


void EnemyManager::Begin()
{
    for (Enemy*& enemy : enemies)
    {
        enemy->Begin();
    }
}


// 更新処理
void EnemyManager::Update(const float& elapsedTime)
{
    // 更新
    for (Enemy*& enemy : enemies)
    {
        enemy->Update(elapsedTime);
    }

    // 破棄処理
    {
        for (Enemy* enemy : removes)
        {
            // vectorから要素を削除する場合はイテレーターで削除
            std::vector<Enemy*>::iterator it = std::find(enemies.begin(), enemies.end(), enemy);

            // std::vectorで管理されている要素を削除するにはerase()関数を使用する
            // (破棄リストのポインタからイテレーターを検索し、erase関数に渡す)
            if (it != enemies.end())
            {
                enemies.erase(it);
            }

            // 敵の破棄処理
            SafeDelete(enemy);
        }
        // 破棄リストをクリア
        removes.clear();
    }


    // エネミー同士の衝突判定処理
    CollisionEnemyVsEnemy();
}


void EnemyManager::End()
{
    for (Enemy*& enemy : enemies)
    {
        enemy->End();
    }
}


// 描画処理
void EnemyManager::Render(const float& elapsedTime)
{
    for (Enemy*& enemy : enemies)
    {
        enemy->Render(elapsedTime);
    }
}


// エネミー登録
void EnemyManager::Register(Enemy* enemy)
{
    enemies.emplace_back(enemy);
}


// エネミー削除
void EnemyManager::Remove(Enemy* enemy)
{
    // 破棄リストに追加
    removes.insert(enemy);
}


// エネミー全削除
void EnemyManager::Clear()
{
    for (Enemy*& enemy : enemies)
    {
        SafeDelete(enemy);
    }
    enemies.clear();
    enemies.shrink_to_fit();    // vectorの余分なメモリを解放する関数(C++11)
}


// デバッグ
void EnemyManager::DrawDebug()
{
    for (Enemy*& enemy : enemies)
    {
        enemy->DrawDebug();
    }
}

void EnemyManager::AllKill()
{
    for (Enemy*& enemy : enemies)
    {
        enemy->ApplyDamage(999, 1.0f);

        // ヒットSEを再生
        if (enemy->IsInLengthPlayer(Enemy::playSELengthXLimit_))
        {
            AudioManager& audioManager = AudioManager::Instance();
            audioManager.StopSE(SE::PL_BounceHit);
            audioManager.PlaySE(SE::PL_BounceHit, false, true);
        }
    }
}


// エネミー同士の衝突判定処理
void EnemyManager::CollisionEnemyVsEnemy()
{
    NO_CONST bool isHitX_A = false;
    NO_CONST bool isHitX_B = false;

    NO_CONST bool isHitY_A = false;
    NO_CONST bool isHitY_B = false;

    EnemyManager& enemyManager = EnemyManager::Instance();
    const int enemyCount       = enemyManager.GetEnemyCount();
    for (int a = 0; a < enemyCount; ++a)
    {
        Enemy* enemyA = enemyManager.GetEnemy(a);
        if (enemyA->GetHealth() <= 0) continue;

        //// Aの味方の上に乗っかっているかフラグを最初にリセットする
        //enemyA->SetIsOnFriend(false);

        // a以降の敵と判定を行う（a以前はすでに判定済みのため）
        for (int b = a + 1; b < enemyCount; ++b)
        {
            Enemy* enemyB = enemyManager.GetEnemy(b);
            if (enemyB->GetHealth() <= 0) continue;

            //// Bの味方の上に乗っかっているかフラグを最初にリセットする
            //enemyB->SetIsOnFriend(false);

            if (Collision::IntersectAABBVsAABB(enemyA->aabb_, enemyB->aabb_))
            {
                // エネミーAが赤豆腐で追跡中ならエネミーBを吹き飛ばす
                if (enemyA->enemyType_ == EnemyType::RedTofu &&
                    enemyB->enemyType_ == EnemyType::Tofu)
                {
                    if (enemyA->GetStateMachine()->GetStateIndex() == static_cast<int>(RED_TOFU::STATE::Track))
                    {
                        enemyB->SetMoveDirectionX(enemyA->GetMoveDirectionX());
                        enemyB->ApplyDamage(1, 1.0f);

                        // ヒットSEを再生
                        if (enemyB->IsInLengthPlayer(Enemy::playSELengthXLimit_))
                        {
                            AudioManager& audioManager = AudioManager::Instance();
                            audioManager.StopSE(SE::PL_BounceHit);              // バウンスヒットSE停止
                            audioManager.PlaySE(SE::PL_BounceHit, false, true); // バウンスヒットSE再生
                        }
                    }
                }
                // エネミーBが赤豆腐で追跡中ならエネミーAを吹き飛ばす
                if (enemyB->enemyType_ == EnemyType::RedTofu &&
                    enemyA->enemyType_ == EnemyType::Tofu)
                {
                    if (enemyB->GetStateMachine()->GetStateIndex() == static_cast<int>(RED_TOFU::STATE::Track))
                    {
                        enemyA->SetMoveDirectionX(enemyB->GetMoveDirectionX());
                        enemyA->ApplyDamage(1, 1.0f);

                        // ヒットSEを再生
                        if (enemyA->IsInLengthPlayer(Enemy::playSELengthXLimit_))
                        {
                            AudioManager& audioManager = AudioManager::Instance();
                            audioManager.StopSE(SE::PL_BounceHit);              // バウンスヒットSE停止
                            audioManager.PlaySE(SE::PL_BounceHit, false, true); // バウンスヒットSE再生
                        }
                    }
                }

                // 上下左右のそれぞれ重なっている値を求める
                // 符号がマイナスでもプラスの距離が求められるように絶対値にする
                const float overlapUp     = fabsf(enemyA->aabb_.min.y - enemyB->aabb_.max.y); // 上からめり込んでいる
                const float overlapBottom = fabsf(enemyA->aabb_.max.y - enemyB->aabb_.min.y); // 下からめり込んでいる
                const float overlapRight  = fabsf(enemyA->aabb_.min.x - enemyB->aabb_.max.x); // 右からめり込んでいる
                const float overlapLeft   = fabsf(enemyA->aabb_.max.x - enemyB->aabb_.min.x); // 左からめり込んでいる
                // 一番重なっていない値を求める
                const float overlapY      = (std::min)(overlapUp, overlapBottom);
                const float overlapX      = (std::min)(overlapRight, overlapLeft);
                //const float overlap       = (std::min)(overlapY, overlapX);

                // Y軸に重なっている場合
                if (overlapY <= overlapX)
                {
                    isHitY_A = true;
                    isHitY_B = true;

                    // 上からめり込んでいる場合
                    if (overlapUp <= overlapBottom)
                    {
                        enemyA->SetVelocityY(0.0f); // Y速度をリセット

                        // 重なっている分だけ位置を修正
                        if (overlapUp != 0.0f)
                        {
                            enemyA->GetTransform()->AddPositionY(overlapUp);
                            //enemyA->GetTransform()->AddPositionY( overlapUp * 0.5f);
                            //enemyB->GetTransform()->AddPositionY(-overlapUp * 0.5f);
                        }

                        // 味方の上に乗っかっている
                        enemyA->SetIsOnFriend(true);
                    }
                    // 下からめり込んでいる場合
                    else
                    {
                        enemyB->SetVelocityY(0.0f); // Y速度をリセット

                        // 重なっている分だけ位置を修正
                        if (overlapBottom != 0.0f)
                        {
                            //enemyA->GetTransform()->AddPositionY(-overlapBottom);
                            enemyB->GetTransform()->AddPositionY(overlapBottom);
                            //enemyA->GetTransform()->AddPositionY(-overlapBottom * 0.5f);
                            //enemyB->GetTransform()->AddPositionY( overlapBottom * 0.5f);
                        }

                        // 味方の上に乗っかっている
                        enemyB->SetIsOnFriend(true);
                    }
                }
                // X軸に重なっている場合
                else
                {
                    // 重なりがなければ修正しない
                    if (overlapX == 0.0f) continue;

                    enemyA->SetVelocityX(0.0f); // X速度をリセット
                    enemyB->SetVelocityX(0.0f); // X速度をリセット

                    isHitX_A = true;
                    isHitX_B = true;

                    // 右からめり込んでいる場合
                    if (overlapX == overlapRight)
                    {
                        // 重なっている分だけ位置を修正
                        //enemyA->GetTransform()->AddPositionX(overlapRight + 0.1f);
                        enemyA->GetTransform()->AddPositionX(( overlapRight +  0.05f) * 0.5f); // 無限旋回ループ防止のために少し調整する
                        enemyB->GetTransform()->AddPositionX((-overlapRight + -0.05f) * 0.5f); // 無限旋回ループ防止のために少し調整する
                    }
                    // 左からめり込んでいる場合
                    else if (overlapX == overlapLeft)
                    {
                        // 重なっている分だけ位置を修正
                        //enemyA->GetTransform()->AddPositionX(-overlapLeft + -0.1f);
                        enemyA->GetTransform()->AddPositionX((-overlapLeft + -0.05f) * 0.5f); // 無限旋回ループ防止のために少し調整する
                        enemyB->GetTransform()->AddPositionX(( overlapLeft +  0.05f) * 0.5f); // 無限旋回ループ防止のために少し調整する
                    }

                    // 横から当たっていたら味方に当たった時の処理を行う
                    //enemyA->OnHitFriend();
                    //enemyB->OnHitFriend();
                    if (!enemyA->GetIsHitFriend()) enemyA->OnHitFriend();
                    if (!enemyB->GetIsHitFriend()) enemyB->OnHitFriend();
                    enemyA->SetIsHitFriend(true);
                    enemyB->SetIsHitFriend(true);
                }

                // 押し戻し後のAABBの最小座標と最大座標を更新
                enemyA->UpdateAABB();
                enemyB->UpdateAABB();
            }

            if (!isHitX_B)
            {
                enemyB->SetIsHitFriend(false);
            }                   
            else
            { 
                isHitX_B = false;
            }            
            
            //if (!isHitY_B)
            //{
            //    enemyB->SetIsOnFriend(false);
            //}                   
            //else
            //{ 
            //    isHitY_B = false;
            //}
        }

        if (!isHitX_A)
        {
            enemyA->SetIsHitFriend(false);
        }        
        else
        {
            isHitX_A = false;
        }

        //if (!isHitY_A)
        //{
        //    enemyA->SetIsOnFriend(false);
        //}
        //else
        //{
        //    isHitY_A = false;
        //}
    }
}
