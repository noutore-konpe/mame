#include "StoneBall.h"

#include "../Graphics/Graphics.h"

// コンストラクタ
StoneBall::StoneBall()
{
    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(),
        "./Resources/Model/Character/Enemy/rock.fbx");
}

StoneBall::~StoneBall()
{
}

void StoneBall::Initialize()
{
    GetTransform()->SetScale(DirectX::XMFLOAT3(3.0f, 3.0f, 3.0f));
}

void StoneBall::Finalize()
{
}

void StoneBall::Update(const float& elapsedTime)
{
}

void StoneBall::Render(const float& scale, ID3D11PixelShader* psShader)
{
    Stone::Render(scale, psShader);
}

void StoneBall::DrawDebug()
{
    ImGui::Begin("StoneBall");

    model->DrawDebug();

    ImGui::End();
}
