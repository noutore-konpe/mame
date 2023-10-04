#include "EnemyManager.h"
#include "../../Taki174/Common.h"

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
    //CollisionEnemyVsEnemy();
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
