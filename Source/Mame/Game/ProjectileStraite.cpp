#include "ProjectileStraite.h"

#include "../Graphics/Graphics.h"

#include "ItemManager.h"

// コンストラクタ
ProjectileStraite::ProjectileStraite(ProjectileManager* manager)
    :Projectile(manager)
{
    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(), "./Resources/Model/Projectile/sqhere.fbx");

    lifeTimer = 10.0f;

    // ImGui名前設定
    SetName("ProjectileStraite" + std::to_string(nameNum++));
}

// 初期化
void ProjectileStraite::Initialize()
{
}

// 更新処理
void ProjectileStraite::Update(const float& elapsedTime)
{
    // 寿命処理
    lifeTimer -= elapsedTime;
    if (lifeTimer < 0)
    {
        // 自分を削除
        Destroy();
    }

    DirectX::XMFLOAT3 position = GetTransform()->GetPosition();

    // 移動
    {
        float speed = this->speed * elapsedTime;
        position.x += direction.x * speed;
        position.y += direction.y * speed;
        position.z += direction.z * speed;
    }

    GetTransform()->SetPosition(position);
}

// 描画処理
void ProjectileStraite::Render(const float& elapsedTime, const float& scale)
{
    Projectile::Render(elapsedTime, scale);
}

// ImGui用
void ProjectileStraite::DrawDebug()
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
void ProjectileStraite::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position)
{
    this->direction = direction;
    GetTransform()->SetPosition(position);
}
