#include "Projectile.h"
#include "ProjectileManager.h"


// �R���X�g���N�^
Projectile::Projectile(ProjectileManager* manager)
    : manager_(manager)
{
    manager->Register(this);
}

// ������
void Projectile::Initialize()
{
}

// �I����
void Projectile::Finalize()
{
}

// Update�̑O�ɌĂ΂��
void Projectile::Begin()
{
}

// �X�V����
void Projectile::Update(const float& elapsedTime)
{
}

// Update�̌�ɌĂ΂��
void Projectile::End()
{
}

// �`�揈��
void Projectile::Render(const float& scale, ID3D11PixelShader* psShader)
{
    model_->Render(scale, psShader);
}

// ImGui�p
void Projectile::DrawDebug()
{
#ifdef USE_IMGUI

    GetTransform()->DrawDebug();

#endif // USE_IMGUI
}

// �j��
void Projectile::Destroy()
{
    manager_->Remove(this);
}
