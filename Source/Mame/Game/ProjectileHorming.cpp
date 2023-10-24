#include "ProjectileHorming.h"

#include "../../Taki174/OperatorXMFloat3.h"
#include "../../Taki174/FunctionXMFloat3.h"
#include "../Graphics/Graphics.h"
#include "PlayerManager.h"
#include "EnemyManager.h"

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

    // ImGui名前設定
    SetName("ProjectileHorming : " + std::to_string(nameNum_++));
}


ProjectileHorming::~ProjectileHorming()
{
    --nameNum_;
}


void ProjectileHorming::Initialize()
{
    using DirectX::XMFLOAT3;

    constexpr float scale = 1.0f;
    GetTransform()->SetScale(XMFLOAT3(scale, scale, scale));
}


void ProjectileHorming::Update(const float elapsedTime)
{
    using DirectX::XMFLOAT3;

    EnemyManager& enmManager = EnemyManager::Instance();

    // 寿命処理
    {
        lifeTimer_ = (std::max)(0.0f, lifeTimer_ - elapsedTime);
        if (lifeTimer_ <= 0.0f) Destroy();
    }

    // 移動
    {
        XMFLOAT3 toTargetVecN   = {};       // 目標への単位ベクトル
        float    targetLength   = FLT_MAX;  // 目標との距離

        // 弾に一番近い敵を探す
        const size_t enemyCount = enmManager.GetEnemyCount();
        for (size_t i = 0; i < enemyCount; ++i)
        {
            Enemy* enemy = enmManager.GetEnemy(i);

            const XMFLOAT3& pos    = this->GetPosition();
            const XMFLOAT3& enmPos = enemy->GetPosition();
            const XMFLOAT3  vec    = enmPos - pos;
            float           length = 0.0f;
            const XMFLOAT3  vecN   = ::XMFloat3Normalize(vec, &length);

            if (length < targetLength)
            {
                toTargetVecN = vecN;
                targetLength = length;
            }
        }

        // 目標へのベクトルがあれば目標へ向かい、
        // ベクトルがなければLaunch()のときに入力された方向に飛んでいく
        const float vecLengthSq = ::XMFloat3LengthSq(toTargetVecN);
        const XMFLOAT3 force = {
            (vecLengthSq != 0.0f)
            ? toTargetVecN * speed_
            : direction_   * speed_
        };
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


// 発射
void ProjectileHorming::Launch(
    const DirectX::XMFLOAT3& position,
    const DirectX::XMFLOAT3& direction)
{
    Initialize();
    GetTransform()->SetPosition(position);
    direction_ = direction; // 敵が見つからなかったときに使われる
}
