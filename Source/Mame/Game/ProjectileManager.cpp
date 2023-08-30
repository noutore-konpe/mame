#include "ProjectileManager.h"

// コンストラクタ
ProjectileManager::ProjectileManager()
{
}

// デストラクタ
ProjectileManager::~ProjectileManager()
{
    Clear();
}

// 初期化
void ProjectileManager::Initialize()
{
    for (Projectile*& projectile : projectiles)
    {
        projectile->Initialize();
    }
}

// 終了化
void ProjectileManager::Finalize()
{
    for (Projectile*& projectile : projectiles)
    {
        projectile->Finalize();
    }
}

// Updateの前に呼ばれる
void ProjectileManager::Begin()
{
    for (Projectile*& projectile : projectiles)
    {
        projectile->Begin();
    }
}

// 更新処理
void ProjectileManager::Update(const float& elapsedTime)
{
    // 更新処理
    for (Projectile*& projectile : projectiles)
    {
        projectile->Update(elapsedTime);
    }

    // 破棄処理
    {
        for (Projectile* projectile : removes)
        {
            // vectorから要素を削除する場合はイテレーターで削除
            std::vector<Projectile*>::iterator it = std::find(projectiles.begin(), projectiles.end(), projectile);

            // std::vectorで管理されている要素を削除するにはerase()関数を使用する
            // (破棄リストのポインタからイテレーターを検索し、erase関数に渡す)
            if (it != projectiles.end())
            {
                projectiles.erase(it);
            }

            // アイテムの破棄
            delete projectile;
        }
        // 破棄リストをクリア
        removes.clear();
    }
}

// Updateの後に呼ばれる
void ProjectileManager::End()
{
    for (Projectile*& projectile : projectiles)
    {
        projectile->End();
    }
}

// 描画処理
void ProjectileManager::Render(const float& elapsedTime, const float& scale)
{
    for (Projectile*& projectile : projectiles)
    {
        projectile->Render(elapsedTime, scale);
    }
}

// ImGui用
void ProjectileManager::DrawDebug()
{
    for (Projectile*& projectile : projectiles)
    {
        projectile->DrawDebug();
    }
}

// 登録
void ProjectileManager::Register(Projectile* projectile)
{
    projectiles.emplace_back(projectile);
}

// 削除
void ProjectileManager::Remove(Projectile* projectile)
{
    // 破棄リストに追加
    removes.insert(projectile);
}

// 全削除
void ProjectileManager::Clear()
{
    for (Projectile*& projectile : projectiles)
    {
        delete projectile;
    }
    projectiles.clear();
    projectiles.shrink_to_fit();  // vectorの余分なメモリを開放する関数(C++11)
}
