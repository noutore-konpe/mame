#include "Projectile.h"
#include "ProjectileManager.h"

Projectile::Projectile(
    ProjectileManager* manager,
    const bool isPoison)
    : manager_(manager)
    , isPoison_(isPoison)
{
    manager->Register(this);
}


void Projectile::Initialize()
{
}


void Projectile::Finalize()
{
}


void Projectile::Begin()
{
}


void Projectile::Update(const float elapsedTime)
{
}


void Projectile::End()
{
}


void Projectile::Render(const float scale, ID3D11PixelShader* psShader)
{
    model_->Render(scale, psShader);
}


void Projectile::DrawDebug()
{
#ifdef USE_IMGUI

    GetTransform()->DrawDebug();

#endif // USE_IMGUI
}


void Projectile::Destroy()
{
    manager_->Remove(this);
}
