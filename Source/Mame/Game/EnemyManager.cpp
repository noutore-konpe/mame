#include "EnemyManager.h"

#include "../../Taki174/Common.h"
#include "../../Taki174/FunctionXMFloat3.h"
#include "../../Taki174/NumeralManager.h"
#include "../Scene/SceneGame.h"
#include "Collision.h"
#include "PlayerManager.h"

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

            // 消去される敵が数字表示の親の場合、数字表示の親情報を消去する
            NumeralManager& numeralManager = NumeralManager::Instance();
            const size_t numeralCount = numeralManager.GetNumeralCount();
            for (size_t i = 0; i < numeralCount; ++i)
            {
                Numeral* numeral = numeralManager.GetNumeral(i);

                if (nullptr == numeral->GetParent()) continue;
                if (enemy != numeral->GetParent()) continue;

                numeral->SetParent(nullptr);
            }



            // 敵の破棄処理
            SafeDeletePtr(enemy);
        }
        // 破棄リストをクリア
        removes_.clear();
    }

    projectileManager_.Update(elapsedTime);

    // 弾丸とプレイヤーの衝突処理
    CollisionProjectileVsPlayer();

    // エネミー同士の衝突処理
    CollisionEnemyVsEnemy(elapsedTime);

    // 敵とステージとの衝突処理
    CollisionEnemyVsStage(elapsedTime);

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
        // カメラ外なら描画しない
        bool isInCamera = false;
        Sprite::ConvertToScreenPos(enemy->GetTransform()->GetPosition(), &isInCamera);
        if (false == isInCamera) { continue; }

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

        // 敵ひるませるボタン
        if (ImGui::Button("FlinchEnemies"))
        {
            for (Enemy*& enemy : enemies_)
            {
                enemy->Flinch();
            }
        }

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
        if (enemy->GetType() != Enemy::TYPE::Golem)
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

void EnemyManager::CollisionProjectileVsPlayer()
{
    using DirectX::XMFLOAT3;

    PlayerManager& plManager = PlayerManager::Instance();

    const size_t projCount = projectileManager_.GetProjectileCount();
    for (size_t projIndex = 0; projIndex < projCount; ++projIndex)
    {
        // 弾丸取得
        Projectile* proj = projectileManager_.GetProjectile(projIndex);

        // プレイヤー取得
        const std::unique_ptr<Player>& player = plManager.GetPlayer();

        // プレイヤーが死んでいたらbreak
        if (true == player->GetIsDead()) break;

        const size_t plHitColliderCount = player->GetHitColliderCount();
        for (size_t plHitColliderIndex = 0; plHitColliderIndex < plHitColliderCount; ++plHitColliderIndex)
        {
            // プレイヤーの食らい判定取得
            const Character::SphereCollider plHitCollder = player->GetHitColliderAt(plHitColliderIndex);

            const XMFLOAT3& projPos             = proj->GetPosition();
            const XMFLOAT3& plHitColliderPos    = plHitCollder.position;
            const float     projRadius          = proj->GetRadius();
            const float     plHitColliderRadius = plHitCollder.radius;

            // 当たってなければcontinue
            if (false == Collision::IntersectSphereVsSphere(
                projPos, projRadius, plHitColliderPos, plHitColliderRadius))
            {
                continue;
            }

            // プレイヤーにダメージを与える
            player->ApplyDamage(proj->GetAttack(),plHitColliderPos,proj->GetOwner());

            // 弾を消去する
            proj->Destroy();

            // 当たったのでbreak
            break;
        }

    }

}

void EnemyManager::CollisionEnemyVsEnemy(const float /*elapsedTime*/)
{
    using DirectX::XMFLOAT3;

    const size_t enemyCount = GetEnemyCount();
    for (size_t a = 0; a < enemyCount; ++a)
    {
        Enemy* enemyA = GetEnemy(a);

        // a以降の敵と判定を行う（a以前はすでに判定済みのため）
        for (size_t b = a + 1; b < enemyCount; ++b)
        {
            Enemy* enemyB = GetEnemy(b);

            const XMFLOAT3& positionA = enemyA->GetPosition();
            const XMFLOAT3& positionB = enemyB->GetPosition();
            const float     radiusA   = enemyA->GetRadius();
            const float     radiusB   = enemyB->GetRadius();

            const XMFLOAT3  vecAtoB   = positionB - positionA;
            const float     lengthSq  = ::XMFloat3LengthSq(vecAtoB);
            const float     range     = radiusA + radiusB;

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


void EnemyManager::CollisionEnemyVsStage(const float /*elapsedTime*/)
{
    using DirectX::XMFLOAT3;
    EnemyManager& enemyManager = EnemyManager::Instance();

    // 敵のステージ内外判定処理を行う
    const size_t enemyCount = enemyManager.GetEnemyCount();
    for (size_t i = 0; i < enemyCount; ++i)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);

        // 敵がまだステージに入場していなければ判定を行わない
        if (false == enemy->GetEntryStageFlag()) continue;

        // 距離の二乗を取得
        Transform*     enemyT   = enemy->GetTransform();
        const XMFLOAT3 vec      = enemyT->GetPosition() - SceneGame::stageCenter;
        const float    lengthSq = ::XMFloat3LengthSq(vec);

        // ステージの半径の二乗を取得
        const float stageRadiusSq = SceneGame::stageRadius * SceneGame::stageRadius;

        // ステージ内にいるならcontinue
        if (lengthSq <= stageRadiusSq) continue;

        // ベクトルを正規化
        const XMFLOAT3 vecN = vec / ::sqrtf(lengthSq);

        // 修正した位置を代入
        enemyT->SetPosition(SceneGame::stageCenter + vecN * SceneGame::stageRadius);
    }

}

bool EnemyManager::AttackCollisionPlayerToEnemy(Enemy* my,DirectX::XMFLOAT3& hitPos)
{
    for (auto& atkCollider : my->GetAttackCollider())
    {
        for (auto& hitCollider : PlayerManager::Instance().GetPlayer()->GetHitCollider())
        {
            if (Collision::IntersectSphereVsSphere(
                atkCollider.position, atkCollider.radius,
                hitCollider.position, hitCollider.radius))
            {
                hitPos = hitCollider.position;
                return true;
            }
        }
    }
    return false;
}

bool EnemyManager::ACAttackCollisionPlayerToEnemy(Enemy* my)
{
    for (auto& atkCollider : my->GetAttackCollider())
    {
        if (atkCollider.activeAttack)continue;

        for (auto& hitCollider : PlayerManager::Instance().GetPlayer()->GetHitCollider())
        {
            if (Collision::IntersectSphereVsSphere(
                atkCollider.position, atkCollider.radius,
                hitCollider.position, hitCollider.radius))
            {
                return true;
            }
        }
    }
    return false;
}
