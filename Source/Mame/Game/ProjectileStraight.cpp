#include "ProjectileStraight.h"

#include "../../Taki174/OperatorXMFloat3.h"
#include "../Graphics/Graphics.h"

int ProjectileStraight::nameNum_ = 0;

ProjectileStraight::ProjectileStraight(
    ProjectileManager* manager, const bool isPoison
)
    : Projectile(manager, isPoison)
{
    Graphics& graphics = Graphics::Instance();

    model_ = std::make_unique<Model>(
        graphics.GetDevice(),
        "./Resources/Model/Projectile/sqhere.fbx"
    );

    // ImGui名前設定
    SetName("ProjectileStraite : " + std::to_string(nameNum_++));
}


ProjectileStraight::~ProjectileStraight()
{
    --nameNum_;
}


void ProjectileStraight::Initialize()
{
    using DirectX::XMFLOAT3;

    GetTransform()->SetScaleFactor(10.0f);
}


void ProjectileStraight::Update(const float elapsedTime)
{
    using DirectX::XMFLOAT3;

    // 寿命処理
    {
        lifeTimer_ = (std::max)(0.0f, lifeTimer_ - elapsedTime);
        if (lifeTimer_ <= 0.0f) Destroy();
    }

    // 移動
    {
        const XMFLOAT3 force = direction_ * speed_;
        GetTransform()->AddPosition(force * elapsedTime);
    }

}


void ProjectileStraight::Render(
    const float scale, ID3D11PixelShader* psShader)
{
    Projectile::Render(scale, psShader);
}


void ProjectileStraight::DrawDebug()
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
void ProjectileStraight::Launch(
    const DirectX::XMFLOAT3& direction,
    const DirectX::XMFLOAT3& position)
{
    Initialize();

    direction_ = direction;
    GetTransform()->SetPosition(position);
}
