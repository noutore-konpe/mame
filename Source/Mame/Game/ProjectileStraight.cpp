#include "ProjectileStraight.h"

#include "../../Taki174/OperatorXMFloat3.h"
#include "../Graphics/Graphics.h"

int ProjectileStraight::nameNum_ = 0;

// コンストラクタ
ProjectileStraight::ProjectileStraight(ProjectileManager* manager)
    : Projectile(manager)
{
    Graphics& graphics = Graphics::Instance();

    model_ = std::make_unique<Model>(graphics.GetDevice(), "./Resources/Model/Projectile/sqhere.fbx");

    // ImGui名前設定
    SetName("ProjectileStraite_" + std::to_string(nameNum_++));
}


ProjectileStraight::~ProjectileStraight()
{
    --nameNum_;
}


// 初期化
void ProjectileStraight::Initialize()
{
    // とりあえずLaunch関数で呼び出している

    using DirectX::XMFLOAT3;

    //lifeTimer_ = 10.0f;
    constexpr float scale = 100.0f;
    GetTransform()->SetScale(XMFLOAT3(scale, scale, scale));
}

// 更新処理
void ProjectileStraight::Update(const float& elapsedTime)
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
void ProjectileStraight::Render(const float& scale, ID3D11PixelShader* psShader)
{
    Projectile::Render(scale, psShader);
}

// ImGui用
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
