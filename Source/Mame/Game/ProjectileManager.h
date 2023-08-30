#pragma once

#include "Projectile.h"
#include <vector>
#include <set>

class ProjectileManager
{
public:
    ProjectileManager();
    ~ProjectileManager();

    void Initialize();                                          // 初期化
    void Finalize();                                            // 終了化
    void Begin();                                               // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime);                      // 更新処理
    void End();                                                 // 毎フレーム一番最後に呼ばれる
    void Render(const float& elapsedTime, const float& scale);  // 描画処理
    void DrawDebug();                                           // ImGui用

public:
    void Register(Projectile* projectile);  // 登録
    void Remove(Projectile* projectile);    // 削除
    void Clear();                           // 全削除

public: // 取得・設定　関連
    const int GetProjectileCount() const { return static_cast<int>(projectiles.size()); } // 弾丸数取得

    Projectile* GetProjectile(int index) { return projectiles.at(index); } // 弾丸取得

private:
    std::vector<Projectile*>  projectiles = {};
    std::set<Projectile*>     removes = {};
};

