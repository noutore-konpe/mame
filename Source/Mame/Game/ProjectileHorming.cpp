#include "ProjectileHorming.h"

#include "../../Taki174/OperatorXMFloat3.h"
#include "../Graphics/Graphics.h"
#include "PlayerManager.h"
#include "EnemyManager.h"

int ProjectileHorming::nameNum_ = 0;

ProjectileHorming::ProjectileHorming(ProjectileManager* manager)
    : Projectile(manager)
{
    Graphics& graphics = Graphics::Instance();

    model_ = std::make_unique<Model>(
        graphics.GetDevice(),
        "./Resources/Model/Projectile/sqhere.fbx"
    );

    // ImGuiñºëOê›íË
    SetName("ProjectileHorming : " + std::to_string(nameNum_++));
}


ProjectileHorming::~ProjectileHorming()
{
    --nameNum_;
}


void ProjectileHorming::Initialize()
{
    using DirectX::XMFLOAT3;

    constexpr float scale = 100.0f;
    GetTransform()->SetScale(XMFLOAT3(scale, scale, scale));
}


void ProjectileHorming::Update(const float elapsedTime)
{
    using DirectX::XMFLOAT3;

    PlayerManager& plManager = PlayerManager::Instance();
    EnemyManager& enmManager = EnemyManager::Instance();

    // éıñΩèàóù
    {
        lifeTimer_ = (std::max)(0.0f, lifeTimer_ - elapsedTime);
        if (lifeTimer_ <= 0.0f) Destroy();
    }

    // à⁄ìÆ
    {
        const size_t enemyCount = enmManager.GetEnemyCount();
        for (size_t i = 0; i < enemyCount; ++i)
        {
            Enemy
        }


        const XMFLOAT3 force = direction_ * speed_;
        GetTransform()->AddPosition(force * elapsedTime);
    }

}


void ProjectileHorming::Render(
    const float scale, ID3D11PixelShader* psShader)
{
    Projectile::Render(scale, psShader);
}


void ProjectileHorming::DrawDebug()
{
#ifdef USE_IMGUI

    Projectile::DrawDebug();

    if (ImGui::BeginMenu(GetName()))
    {
        ImGui::EndMenu();
    }

#endif // USE_IMGUI
}


// î≠éÀ
void ProjectileHorming::Launch(const DirectX::XMFLOAT3& position)
{
    Initialize();
    GetTransform()->SetPosition(position);
}
