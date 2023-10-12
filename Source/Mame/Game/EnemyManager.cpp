#include "EnemyManager.h"

#include "../../Taki174/Common.h"
#include "../../Taki174/FunctionXMFloat3.h"
#include "../Game/Collision.h"

void EnemyManager::Initialize()
{
    // CRA : 0.Initialize : 初期化
    craActionCoolTimer_ = 0.0f;
    isRunningCRAAction_ = false;

    for (Enemy*& enemy : enemies_)
    {
        enemy->Initialize();
    }

    projectileManager_.Initialize();
}

void EnemyManager::Finalize()
{
    projectileManager_.Finalize();
    projectileManager_.Clear();

    for (Enemy*& enemy : enemies_)
    {
        enemy->Finalize();
    }
    Clear();
}

void EnemyManager::Begin()
{
    for (Enemy*& enemy : enemies_)
    {
        enemy->Begin();
    }

    projectileManager_.Begin();
}

// 更新処理
void EnemyManager::Update(const float elapsedTime)
{
    // CRA : 1.Update : 近接攻撃行動クールタイマー減少
    craActionCoolTimer_ = (std::max)(0.0f, craActionCoolTimer_ - elapsedTime);

    //生成
    for (Enemy* enemy : generates_)
    {
        enemies_.emplace_back(enemy);
    }
    generates_.clear();

    // 更新
    for (Enemy*& enemy : enemies_)
    {
        enemy->Update(elapsedTime);
    }

    // 破棄処理
    {
        for (Enemy* enemy : removes_)
        {
            // vectorから要素を削除する場合はイテレーターで削除
            auto it = std::find(enemies_.begin(), enemies_.end(), enemy);

            // std::vectorで管理されている要素を削除するにはerase()関数を使用する
            // (破棄リストのポインタからイテレーターを検索し、erase関数に渡す)
            if (it != enemies_.end())
            {
                enemies_.erase(it);
            }

            // 消去される敵が弾丸の親の場合、弾丸の親情報を消去する
            const size_t projectileCount = projectileManager_.GetProjectileCount();
            for (size_t i = 0; i < projectileCount; ++i)
            {
                Projectile* projectile = projectileManager_.GetProjectile(i);

                if (nullptr == projectile->GetParent()) continue;
                if (enemy != projectile->GetParent()) continue;

                projectile->SetParent(nullptr);
            }

            // 敵の破棄処理
            SafeDeletePtr(enemy);
        }
        // 破棄リストをクリア
        removes_.clear();
    }

    projectileManager_.Update(elapsedTime);

    // エネミー同士の衝突判定処理
    CollisionEnemyVsEnemy(elapsedTime);

}

void EnemyManager::End()
{
    for (Enemy*& enemy : enemies_)
    {
        enemy->End();
    }

    projectileManager_.End();
}


// 描画処理
void EnemyManager::Render(const float scale, ID3D11PixelShader* psShader)
{
    for (Enemy*& enemy : enemies_)
    {
        enemy->Render(scale, psShader);
    }

    projectileManager_.Render(scale, psShader);

}


// エネミー登録
void EnemyManager::Register(Enemy* enemy)
{
    //enemies_.emplace_back(enemy);
    generates_.insert(enemy);
}


// エネミー削除
void EnemyManager::Remove(Enemy* enemy)
{
    // 破棄リストに追加
    removes_.insert(enemy);
}


// エネミー全削除
void EnemyManager::Clear()
{
    for (Enemy*& enemy : enemies_)
    {
        SafeDeletePtr(enemy);
    }
    enemies_.clear();
    enemies_.shrink_to_fit();    // vectorの余分なメモリを解放
}


// デバッグ
void EnemyManager::DrawDebug()
{
#ifdef USE_IMGUI
    if (ImGui::BeginMenu("Enemies"))
    {
        for (Enemy*& enemy : enemies_)
        {
            enemy->DrawDebug();
        }
        ImGui::Separator();

        // Behaviorを簡易に見る用
        //for (Enemy*& enemy : enemies_)
        //{
        //    std::string str = (enemy->GetActiveNode() != nullptr)
        //        ? enemy->GetActiveNode()->GetName()
        //        : u8"なし";
        //    ImGui::Text(u8"Behavior：%s", str.c_str());
        //}

        ImGui::EndMenu();
    }
    ImGui::Separator();


    if (ImGui::BeginMenu("Enemy_ProjectileManager"))
    {
        projectileManager_.DrawDebug();
        ImGui::EndMenu();
    }
    ImGui::Separator();


#endif
}

void EnemyManager::RenderShadow(const float scale, ID3D11PixelShader* psShader)
{
    for (Enemy*& enemy : enemies_)
    {
        // ゴーレムじゃなかったら描画
        if (enemy->GetType() != static_cast<UINT>(Enemy::TYPE::Golem))
        {
            enemy->Render(scale, psShader);
        }
        else
        {   // ゴーレムの影
            enemy->Render(scale, false);
        }
    }


    projectileManager_.Render(scale, psShader);
}


void EnemyManager::CollisionEnemyVsEnemy(const float /*elapsedTime*/)
{
    using DirectX::XMFLOAT3;

    EnemyManager& enemyManager = EnemyManager::Instance();

    const size_t enemyCount = enemyManager.GetEnemyCount();
    for (size_t a = 0; a < enemyCount; ++a)
    {
        Enemy* enemyA = enemyManager.GetEnemy(a);

        // a以降の敵と判定を行う（a以前はすでに判定済みのため）
        for (size_t b = a + 1; b < enemyCount; ++b)
        {
            Enemy* enemyB = enemyManager.GetEnemy(b);

            const XMFLOAT3  positionA   = enemyA->GetPosition();
            const XMFLOAT3  positionB   = enemyB->GetPosition();
            const float     radiusA     = enemyA->GetRadius();
            const float     radiusB     = enemyB->GetRadius();

            const XMFLOAT3  vecAtoB     = positionB - positionA;
            const float     lengthSq    = ::XMFloat3LengthSq(vecAtoB);
            const float     range       = radiusA + radiusB;


            if (lengthSq > (range * range)) continue;


            const XMFLOAT3 vecAtoB_N = ::XMFloat3Normalize(vecAtoB);

            // CRA : 7.Collisition : 近接攻撃中じゃない方の敵を押し出す
#if 0
            if (enemyB->GetActiveNode() != nullptr &&
                "CloseRangeAttack" == enemyB->GetActiveNode()->GetName())
            {
                const XMFLOAT3 vecBtoA_N = vecAtoB_N * (-1.0f);
                enemyA->SetPosition(positionB + vecBtoA_N * range);
            }
            else
            {
                enemyB->SetPosition(positionA + vecAtoB_N * range);
            }
#else
            enemyB->SetPosition(positionA + vecAtoB_N * range);
#endif

        }

    }

}
