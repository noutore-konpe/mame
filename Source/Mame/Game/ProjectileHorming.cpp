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

    GetTransform()->SetScaleFactor(10.0f);
    //constexpr float scale = 1.0f;
    //GetTransform()->SetScale(XMFLOAT3(scale, scale, scale));
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
        float    targetLength   = FLT_MAX;  // 目標との距離

        // 弾に一番近い敵を探す
        const size_t enemyCount = enmManager.GetEnemyCount();
        for (size_t i = 0; i < enemyCount; ++i)
        {
            Enemy* enemy = enmManager.GetEnemy(i);

            // 死亡していたらcontinue;
            if (true == enemy->GetIsDead()) { continue; }

            const XMFLOAT3& pos       = this->GetPosition();
                  XMFLOAT3  enmPos    = enemy->GetPosition();
                            enmPos.y += enemy->GetHeight() * 0.5f; // 高さ調整

            // 敵がひるみ中ならアニメーションで少し後ろにのけぞって
            // 位置ずれが生じているので位置を少し後ろに修正する
            if (true == enemy->GetIsFlinch())
            {
                const float    modifyBack  = 0.1f;
                const XMFLOAT3 enmBackVecN = (-enemy->GetTransform()->CalcForward());
                enmPos = enmPos + enmBackVecN * modifyBack;
            }
            const XMFLOAT3  vec       = enmPos - pos;
                  float     length    = 0.0f;
            const XMFLOAT3  vecN      = ::XMFloat3Normalize(vec, &length);

            if (length < targetLength)
            {
                direction_   = vecN; // 方向を保存
                targetLength = length;
            }
        }

        // 目標に向かう（目標が見つからなければ前回保存した方向に向かう）
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


// 発射
void ProjectileHorming::Launch(
    const DirectX::XMFLOAT3& position,
    const DirectX::XMFLOAT3& direction)
{
    Initialize();
    GetTransform()->SetPosition(position);
    direction_ = direction; // 敵が見つからなかったときに使われる
}
