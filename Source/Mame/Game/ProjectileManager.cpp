#include "ProjectileManager.h"

#include "../../Taki174/Common.h"
#include "../Resource/sprite.h"

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
    for (Projectile*& projectile : projectiles_)
    {
        projectile->Initialize();
    }
}

// 終了化
void ProjectileManager::Finalize()
{
    for (Projectile*& projectile : projectiles_)
    {
        projectile->Finalize();
    }
}

// Updateの前に呼ばれる
void ProjectileManager::Begin()
{
    for (Projectile*& projectile : projectiles_)
    {
        projectile->Begin();
    }
}

// 更新処理
void ProjectileManager::Update(const float elapsedTime)
{
    // 更新処理
    for (Projectile*& projectile : projectiles_)
    {
        projectile->Update(elapsedTime);
    }

    // 破棄処理
    {
        for (Projectile* projectile : removes_)
        {
            // vectorから要素を削除する場合はイテレーターで削除
            auto it = std::find(projectiles_.begin(), projectiles_.end(), projectile);

            // std::vectorで管理されている要素を削除するにはerase()関数を使用する
            // (破棄リストのポインタからイテレーターを検索し、erase関数に渡す)
            if (it != projectiles_.end())
            {
                projectiles_.erase(it);
            }

            // アイテムの破棄
            SafeDeletePtr(projectile);
        }
        // 破棄リストをクリア
        removes_.clear();
    }

}

// Updateの後に呼ばれる
void ProjectileManager::End()
{
    for (Projectile*& projectile : projectiles_)
    {
        projectile->End();
    }
}

// 描画処理
void ProjectileManager::Render(const float scale, ID3D11PixelShader* psShader)
{
    for (Projectile*& projectile : projectiles_)
    {
        // カメラ外なら描画しない
        bool isInCamera = false;
        Sprite::ConvertToScreenPos(projectile->GetTransform()->GetPosition(), &isInCamera);
        if (false == isInCamera) { continue; }

        projectile->Render(scale, psShader);
    }
}

// ImGui用
void ProjectileManager::DrawDebug()
{
    for (Projectile*& projectile : projectiles_)
    {
        projectile->DrawDebug();
    }
}

// 登録
void ProjectileManager::Register(Projectile* projectile)
{
    projectiles_.emplace_back(projectile);
}

// 削除
void ProjectileManager::Remove(Projectile* projectile)
{
    // 破棄リストに追加
    removes_.insert(projectile);
}

// 全削除
void ProjectileManager::Clear()
{
    for (Projectile*& projectile : projectiles_)
    {
        SafeDeletePtr(projectile);
    }
    projectiles_.clear();
    projectiles_.shrink_to_fit();  // vectorの余分なメモリを開放する関数(C++11)
}
