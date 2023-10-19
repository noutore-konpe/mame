#include "PlayerResult.h"

#include "../Graphics/Graphics.h"

// コンストラクタ
PlayerResult::PlayerResult()
{
    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(),
        "./Resources/Model/Character/Player/P_Motion.fbx");

    swordModel = std::make_unique<Model>(graphics.GetDevice(),
        "./Resources/Model/Character/Sword_Motion.fbx");

    CreatePsFromCso(graphics.GetDevice(),
        "./Resources/Shader/LoadingPS.cso",
        loadPS.GetAddressOf());
}

// 初期化
void PlayerResult::Initialize()
{
    float scale = 0.6f;

    GetTransform()->SetPosition(DirectX::XMFLOAT3(-3.5f, -1.1f, 0.0f));
    GetTransform()->SetScale(DirectX::XMFLOAT3(scale, scale, scale));
    GetTransform()->SetRotationY(DirectX::XMConvertToRadians(90));

    swordModel->transform.SetScaleFactor(0.7f);
    swordModel->PlayAnimation(0, true);
    PlayAnimation(0, true);
}

// 更新
void PlayerResult::Update(const float elapsedTime)
{
    swordModel->transform.SetPosition(GetTransform()->GetPosition());
    swordModel->transform.SetRotation(GetTransform()->GetRotation());
    swordModel->UpdateAnimation(elapsedTime);

    UpdateAnimation(elapsedTime);

    GetTransform()->AddRotationY(elapsedTime);
    float rotY = GetTransform()->GetRotation().y;
    if (rotY > DirectX::XMConvertToRadians(360))
        rotY -= DirectX::XMConvertToRadians(360);
    GetTransform()->SetRotationY(rotY);
}

// 描画
void PlayerResult::Render(const float scale, ID3D11PixelShader* psShader)
{
    model->Render(scale, loadPS.Get());
    swordModel->Render(scale, loadPS.Get());
}

void PlayerResult::DrawDebug()
{
    model->DrawDebug();
}
