#include "LoadingPlayer.h"

#include "../Graphics/Graphics.h"

// コンストラクタ
LoadingPlayer::LoadingPlayer()
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

void LoadingPlayer::Initialize()
{
    float scale = 0.8f;

    GetTransform()->SetPosition(DirectX::XMFLOAT3(3.5f, -2.1f, 0.0f));
    GetTransform()->SetScale(DirectX::XMFLOAT3(scale, scale, scale));
    GetTransform()->SetRotationY(DirectX::XMConvertToRadians(-90));

    swordModel->transform.SetScaleFactor(scale);
    swordModel->PlayAnimation(1, true);
    PlayAnimation(1, true);
}

void LoadingPlayer::Finalize()
{
}

void LoadingPlayer::Update(const float elapsedTime)
{
    swordModel->transform.SetPosition(GetTransform()->GetPosition());
    swordModel->transform.SetRotation(GetTransform()->GetRotation());
    swordModel->UpdateAnimation(elapsedTime);

    UpdateAnimation(elapsedTime);
}

void LoadingPlayer::Render(const float scale, ID3D11PixelShader* psShader)
{
    model->Render(scale, loadPS.Get());
    swordModel->Render(scale, loadPS.Get());
}

void LoadingPlayer::DrawDebug()
{
    model->DrawDebug();
    //swordModel->DrawDebug();
}
