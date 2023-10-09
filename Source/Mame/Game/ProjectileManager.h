#pragma once

#include <vector>
#include <set>

#include "Projectile.h"

class ProjectileManager
{
public:
    ProjectileManager();
    ~ProjectileManager();

    void Initialize();                                          // 初期化
    void Finalize();                                            // 終了化
    void Begin();                                               // 毎フレーム一番最初に呼ばれる
    void Update(const float elapsedTime);                      // 更新処理
    void End();                                                 // 毎フレーム一番最後に呼ばれる
    void Render(const float scale, ID3D11PixelShader* psShader = nullptr);  // 描画処理
    void DrawDebug();                                           // ImGui用

public:
    void Register(Projectile* projectile);  // 登録
    void Remove(Projectile* projectile);    // 削除
    void Clear();                           // 全削除

public: // 取得・設定　関連
    const size_t GetProjectileCount() const { return projectiles_.size(); } // 弾丸数取得
    Projectile* GetProjectile(const size_t index) { return projectiles_.at(index); } // 弾丸取得

private:
    std::vector<Projectile*>  projectiles_ = {};
    std::set<Projectile*>     removes_     = {};
};

