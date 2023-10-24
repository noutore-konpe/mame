#include "StoneBall.h"

#include "../Graphics/Graphics.h"
#include "../Resource/texture.h"

#include "PlayerManager.h"
#include "Collision.h"

#include "Enemy.h"

// コンストラクタ
StoneBall::StoneBall(Enemy* owner) : owner(owner)
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

void StoneBall::Initialize()
{
    GetTransform()->SetScale(DirectX::XMFLOAT3(3.0f, 3.0f, 3.0f));
    isDestroy = false;
}

void StoneBall::Finalize()
{
}

void StoneBall::Update(const float& elapsedTime)
{
    if (isDestroy)return;

    for (auto& hitCollider : PlayerManager::Instance().GetPlayer()->GetHitCollider())
    {
        if (Collision::IntersectSphereVsSphere(
            GetTransform()->GetPosition(),
            radius,
            hitCollider.position,
            hitCollider.radius
        ))
        {
            //TODO:ゴーレム　岩攻撃
            PlayerManager::Instance().GetPlayer()->ApplyDamage(damage,hitCollider.position, Player::HitReaction::HARD,owner);
            
            Input::Instance().GetGamePad().Vibration(0.3f, 0.3f);

            Destroy(); // 仮消去
        }
    }
}

void StoneBall::Render(const float& scale, ID3D11PixelShader* psShader)
{
    UpdateConstants();

    if (isDestroy)return;
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
