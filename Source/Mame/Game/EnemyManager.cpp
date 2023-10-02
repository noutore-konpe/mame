#include "EnemyManager.h"
#include "../../Taki174/Common.h"

void EnemyManager::Initialize()
{
    for (BaseEnemyAI*& enemy : enemies_)
    {
        enemy->Initialize();
    }
}

void EnemyManager::Finalize()
{
    Clear();
}

// 更新処理
void EnemyManager::Update(const float elapsedTime)
{
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
    //CollisionEnemyVsEnemy();
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
    for (BaseEnemyAI*& enemy : enemies_)
    {
        enemy->DrawDebug();
    }
}
