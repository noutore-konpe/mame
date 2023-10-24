#include "StoneBall.h"

#include "../Graphics/Graphics.h"
#include "../Resource/texture.h"

#include "PlayerManager.h"
#include "Collision.h"

// コンストラクタ
StoneBall::StoneBall()
{
    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(),
        "./Resources/Model/Character/Enemy/rock.fbx");

    D3D11_TEXTURE2D_DESC texture2dDesc;
    load_texture_from_file(graphics.GetDevice(),
        L"./Resources/Image/Mask/noise3.png",
        stoneBallTexture.GetAddressOf(),
        &texture2dDesc);

    CreatePsFromCso(graphics.GetDevice(),
        "./Resources/Shader/EmissiveTextureUVScrollPS.cso",
        stoneBallPS.GetAddressOf());
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
    for (auto& hitCollider : PlayerManager::Instance().GetPlayer()->GetHitCollider())
    {
        if (Collision::IntersectSphereVsSphere(
            GetTransform()->GetPosition(),
            radius,
            hitCollider.position,
            hitCollider.radius
        ))
        {
            //PlayerManager::Instance().GetPlayer()->ApplyDamage(damage);
        }
    }
}

void StoneBall::Render(const float& scale, ID3D11PixelShader* psShader)
{
    UpdateConstants();

    Stone::Render(scale, stoneBallPS.Get());
}

void StoneBall::DrawDebug()
{
    ImGui::Begin("StoneBall");

    model->DrawDebug();

    DebugRenderer* debug = Graphics::Instance().GetDebugRenderer();
    debug->DrawSphere(GetTransform()->GetPosition(), radius, { 1.0f,0.0f,0.0f,1.0f });

    ImGui::End();
}

void StoneBall::UpdateConstants()
{
    Graphics& graphics = Graphics::Instance();

    graphics.GetDeviceContext()->PSSetShaderResources(16, 1, stoneBallTexture.GetAddressOf());

    // emissiveの強さ
    SetEmissiveIntensity(1.5f);

    // emissiveTexture ScrollDirection
    SetEmissiveScrollDirection(DirectX::XMFLOAT2(0.25f, 0.5f));

    // color
    SetEmissiveColor(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
}
