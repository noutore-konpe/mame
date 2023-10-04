#include "EnemyManager.h"

#include "../../Taki174/Common.h"
#include "../../Taki174/FunctionXMFloat3.h"
#include "../Game/Collision.h"

void EnemyManager::Initialize()
{
    // CRA : 0.Initialize : 初期化
    isRunningCRAAction_ = false;
    craActionCoolTimer_ = 0.0f;

    for (BaseEnemyAI*& enemy : enemies_)
    {
        enemy->Initialize();
    }
}

void EnemyManager::Finalize()
{
    for (BaseEnemyAI*& enemy : enemies_)
    {
        enemy->Finalize();
    }
    Clear();
}

void EnemyManager::Begin()
{
    for (BaseEnemyAI*& enemy : enemies_)
    {
        enemy->Begin();
    }
}

// 更新処理
void EnemyManager::Update(const float elapsedTime)
{
    // CRA : 1.Update : 近接攻撃行動クールタイマー減少
    craActionCoolTimer_ = (std::max)(0.0f, craActionCoolTimer_ - elapsedTime);

    // 更新
    for (BaseEnemyAI*& enemy : enemies_)
    {
        enemy->Update(elapsedTime);
    }

    // 破棄処理
    {
        for (BaseEnemyAI* enemy : removes_)
        {
            // vectorから要素を削除する場合はイテレーターで削除
            std::vector<BaseEnemyAI*>::iterator it = std::find(enemies_.begin(), enemies_.end(), enemy);

            // std::vectorで管理されている要素を削除するにはerase()関数を使用する
            // (破棄リストのポインタからイテレーターを検索し、erase関数に渡す)
            if (it != enemies_.end())
            {
                enemies_.erase(it);
            }

            // 敵の破棄処理
            SafeDeletePtr(enemy);
        }
        // 破棄リストをクリア
        removes_.clear();
    }

    // エネミー同士の衝突判定処理
    CollisionEnemyVsEnemy(elapsedTime);

}

void EnemyManager::End()
{
    for (BaseEnemyAI*& enemy : enemies_)
    {
        enemy->End();
    }
}


// 描画処理
void EnemyManager::Render(const float scale, ID3D11PixelShader* psShader)
{
    for (BaseEnemyAI*& enemy : enemies_)
    {
        enemy->Render(scale, psShader);
    }
}


// エネミー登録
void EnemyManager::Register(BaseEnemyAI* enemy)
{
    enemies_.emplace_back(enemy);
}


// エネミー削除
void EnemyManager::Remove(BaseEnemyAI* enemy)
{
    // 破棄リストに追加
    removes_.insert(enemy);
}


// エネミー全削除
void EnemyManager::Clear()
{
    for (BaseEnemyAI*& enemy : enemies_)
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
        for (BaseEnemyAI*& enemy : enemies_)
        {
            enemy->DrawDebug();
        }

        ImGui::EndMenu();

        for (BaseEnemyAI*& enemy : enemies_)
        {
            std::string str = (enemy->GetActiveNode() != nullptr)
                ? enemy->GetActiveNode()->GetName()
                : u8"なし";
            ImGui::Text(u8"Behavior：%s", str.c_str());
        }
    }
    ImGui::Separator();

#endif
}


void EnemyManager::CollisionEnemyVsEnemy(const float elapsedTime)
{
    using DirectX::XMFLOAT3;

    EnemyManager& enemyManager = EnemyManager::Instance();

    const size_t enemyCount = enemyManager.GetEnemyCount();
    for (size_t a = 0; a < enemyCount; ++a)
    {
        BaseEnemyAI* enemyA = enemyManager.GetEnemy(a);

        // a以降の敵と判定を行う（a以前はすでに判定済みのため）
        for (size_t b = a + 1; b < enemyCount; ++b)
        {
            BaseEnemyAI* enemyB = enemyManager.GetEnemy(b);

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

        }

    }

}
