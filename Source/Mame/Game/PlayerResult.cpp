#include "PlayerResult.h"

#include "../Graphics/Graphics.h"

// �R���X�g���N�^
PlayerResult::PlayerResult()
{
    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(),
        "./Resources/Model/Character/Player/P_Motion.fbx");

    swordModel = std::make_unique<Model>(graphics.GetDevice(),
        "./Resources/Model/Character/Sword_Motion.fbx");

    CreatePsFromCso(graphics.GetDevice(),
        "./Resources/Shader/ResultPS.cso",
        loadPS.GetAddressOf());
}

// ������
void PlayerResult::Initialize()
{
    float scale = 0.6f;

    GetTransform()->SetPosition(DirectX::XMFLOAT3(-3.5f, -1.1f, 0.0f));
    GetTransform()->SetScale(DirectX::XMFLOAT3(scale, scale, scale));
    GetTransform()->SetRotationY(DirectX::XMConvertToRadians(90));

    swordModel->transform.SetScaleFactor(1.0f);
    swordModel->PlayAnimation(0, true);
    PlayAnimation(0, true);
}

// �X�V
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

// �`��
void PlayerResult::Render(const float scale, ID3D11PixelShader* psShader, bool isSowrd)
{
    model->Render(scale, loadPS.Get());
    if(isSowrd) swordModel->Render(scale, loadPS.Get());
}

void PlayerResult::DrawDebug()
{
    model->DrawDebug();
}
