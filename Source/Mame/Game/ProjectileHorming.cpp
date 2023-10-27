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

    // ImGui���O�ݒ�
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

    // ��������
    {
        lifeTimer_ = (std::max)(0.0f, lifeTimer_ - elapsedTime);
        if (lifeTimer_ <= 0.0f) Destroy();
    }

    // �ړ�
    {
        float    targetLength   = FLT_MAX;  // �ڕW�Ƃ̋���

        // �e�Ɉ�ԋ߂��G��T��
        const size_t enemyCount = enmManager.GetEnemyCount();
        for (size_t i = 0; i < enemyCount; ++i)
        {
            Enemy* enemy = enmManager.GetEnemy(i);

            // ���S���Ă�����continue;
            if (true == enemy->GetIsDead()) { continue; }

            const XMFLOAT3& pos       = this->GetPosition();
            const XMFLOAT3& enmPos    = enemy->GetHitColliderAt(static_cast<int>(BaseEnemyAI::HitColName::HIP)).position;
            const XMFLOAT3  vec       = enmPos - pos;
                  float     length    = 0.0f;
            const XMFLOAT3  vecN      = ::XMFloat3Normalize(vec, &length);

            if (length < targetLength)
            {
                direction_   = vecN; // ������ۑ�
                targetLength = length;
            }
        }

        // �ڕW�Ɍ������i�ڕW��������Ȃ���ΑO��ۑ����������Ɍ������j
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


// ����
void ProjectileHorming::Launch(
    const DirectX::XMFLOAT3& position,
    const DirectX::XMFLOAT3& direction)
{
    Initialize();
    GetTransform()->SetPosition(position);
    direction_ = direction; // �G��������Ȃ������Ƃ��Ɏg����
}
