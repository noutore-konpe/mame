#include "ProjectileHorming.h"

#include "../../Taki174/OperatorXMFloat3.h"
#include "../../Taki174/FunctionXMFloat3.h"
#include "../Graphics/Graphics.h"
#include "PlayerManager.h"
#include "EnemyManager.h"
#include "BaseEnemyAI.h"

int ProjectileHorming::nameNum_ = 0;

ProjectileHorming::ProjectileHorming(
    ProjectileManager* manager, Character* owner, const bool isPoison)
    : Projectile(manager,owner, isPoison)
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

    GetTransform()->SetScaleFactor(10.0f);
    //constexpr float scale = 1.0f;
    //GetTransform()->SetScale(XMFLOAT3(scale, scale, scale));
}


void ProjectileHorming::Update(const float elapsedTime)
{
    using DirectX::XMFLOAT3;

    EnemyManager& enmManager = EnemyManager::Instance();

    // éıñΩèàóù
    {
        lifeTimer_ = (std::max)(0.0f, lifeTimer_ - elapsedTime);
        if (lifeTimer_ <= 0.0f) Destroy();
    }

    // à⁄ìÆ
    {
        float    targetLength   = FLT_MAX;  // ñ⁄ïWÇ∆ÇÃãóó£

        // íeÇ…àÍî‘ãﬂÇ¢ìGÇíTÇ∑
        const size_t enemyCount = enmManager.GetEnemyCount();
        for (size_t i = 0; i < enemyCount; ++i)
        {
            Enemy* enemy = enmManager.GetEnemy(i);

            // éÄñSÇµÇƒÇ¢ÇΩÇÁcontinue;
            if (true == enemy->GetIsDead()) { continue; }

            const XMFLOAT3& pos       = this->GetPosition();
            const XMFLOAT3& enmPos    = enemy->GetHitColliderAt(static_cast<int>(BaseEnemyAI::HitColName::HIP)).position;
            const XMFLOAT3  vec       = enmPos - pos;
                  float     length    = 0.0f;
            const XMFLOAT3  vecN      = ::XMFloat3Normalize(vec, &length);

            if (length < targetLength)
            {
                direction_   = vecN; // ï˚å¸Çï€ë∂
                targetLength = length;
            }
        }

        // ñ⁄ïWÇ…å¸Ç©Ç§Åiñ⁄ïWÇ™å©Ç¬Ç©ÇÁÇ»ÇØÇÍÇŒëOâÒï€ë∂ÇµÇΩï˚å¸Ç…å¸Ç©Ç§Åj
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
void ProjectileHorming::Launch(
    const DirectX::XMFLOAT3& position,
    const DirectX::XMFLOAT3& direction)
{
    Initialize();
    GetTransform()->SetPosition(position);
    direction_ = direction; // ìGÇ™å©Ç¬Ç©ÇÁÇ»Ç©Ç¡ÇΩÇ∆Ç´Ç…égÇÌÇÍÇÈ
}
