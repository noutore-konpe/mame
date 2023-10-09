#include "Projectile.h"
#include "ProjectileManager.h"


// コンストラクタ
Projectile::Projectile(ProjectileManager* manager)
    : manager_(manager)
{
    manager->Register(this);
}

// 初期化
void Projectile::Initialize()
{
}

// 終了化
void Projectile::Finalize()
{
}

// Updateの前に呼ばれる
void Projectile::Begin()
{
}

// 更新処理
void Projectile::Update(const float& elapsedTime)
{
}

// Updateの後に呼ばれる
void Projectile::End()
{
}

// 描画処理
void Projectile::Render(const float& scale, ID3D11PixelShader* psShader)
{
    model_->Render(scale, psShader);
}

// ImGui用
void Projectile::DrawDebug()
{
#ifdef USE_IMGUI

    GetTransform()->DrawDebug();

#endif // USE_IMGUI
}

// 破棄
void Projectile::Destroy()
{
    manager_->Remove(this);
}
