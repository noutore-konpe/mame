#include "ProjectileStraite.h"

#include "../../Taki174/OperatorXMFloat3.h"
#include "../Graphics/Graphics.h"

int ProjectileStraite::nameNum_ = 0;

// コンストラクタ
ProjectileStraite::ProjectileStraite(ProjectileManager* manager)
    : Projectile(manager)
{
    Graphics& graphics = Graphics::Instance();

    model_ = std::make_unique<Model>(graphics.GetDevice(), "./Resources/Model/Projectile/sqhere.fbx");

    // ImGui名前設定
    SetName("ProjectileStraite_" + std::to_string(nameNum_++));
}


ProjectileStraite::~ProjectileStraite()
{
    --nameNum_;
}


// 初期化
void ProjectileStraite::Initialize()
{
    lifeTimer_ = 10.0f;
}

// 更新処理
void ProjectileStraite::Update(const float& elapsedTime)
{
    // 寿命処理
    {
        lifeTimer_ = (std::max)(0.0f, lifeTimer_ - elapsedTime);
        if (lifeTimer_ <= 0.0f) Destroy();
    }

    // 移動
    {
        const float speed = speed_ * elapsedTime;
        GetTransform()->AddPosition(direction_ * speed);
    }

}

// 描画処理
void ProjectileStraite::Render(const float& scale, ID3D11PixelShader* psShader)
{
    Projectile::Render(scale, psShader);
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
void ProjectileStraite::Launch(
    const DirectX::XMFLOAT3& direction,
    const DirectX::XMFLOAT3& position)
{
    direction_ = direction;
    GetTransform()->SetPosition(position);
}
