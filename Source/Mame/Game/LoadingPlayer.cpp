#include "LoadingPlayer.h"

#include "../Graphics/Graphics.h"

// コンストラクタ
LoadingPlayer::LoadingPlayer()
{
    Graphics& graphics = Graphics::Instance();

    model=std::make_unique<Model>(graphics.GetDevice(),
        "./Resources/Model/Character/Player/P_Motion.fbx");

    CreatePsFromCso(graphics.GetDevice(),
        "./Resources/Shader/LoadingPS.cso",
        loadPS.GetAddressOf());
}

void LoadingPlayer::Initialize()
{
    float scale = 0.6f;

    GetTransform()->SetPosition(DirectX::XMFLOAT3(-3.5f, -1.1f, 0.0f));
    GetTransform()->SetScale(DirectX::XMFLOAT3(scale, scale, scale));
    GetTransform()->SetRotationY(DirectX::XMConvertToRadians(90));

    PlayAnimation(1, true);
}

void LoadingPlayer::Finalize()
{
}

void LoadingPlayer::Update(const float elapsedTime)
{
    UpdateAnimation(elapsedTime);
}

void LoadingPlayer::Render(const float scale, ID3D11PixelShader* psShader)
{
    model->Render(scale, loadPS.Get());
}

void LoadingPlayer::DrawDebug()
{
    model->DrawDebug();
}
