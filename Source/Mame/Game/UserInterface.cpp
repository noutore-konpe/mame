#include "UserInterface.h"
#include "../Graphics/Graphics.h"
#include "Enemy.h"

void UserInterface::Initialize()
{
    lockOnSprite = std::make_unique<Sprite>(Graphics::Instance().GetDevice(),
        L"./Resources/Image/UI/LockOn1.png");
    lockOnSprite->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(16, 16));

    hpSprite = std::make_unique<Sprite>(Graphics::Instance().GetDevice(),
        L"./Resources/Image/UI/hp.png");
}

void UserInterface::Update(float elapsedTime)
{
    if (Camera::Instance().activeLockOn && Camera::Instance().GetLockOnTarget())
    {
        DirectX::XMFLOAT3 pos = Camera::Instance().GetLockOnTarget()->GetTransform()->GetPosition();
        pos.y += Camera::Instance().GetLockOnTarget()->GetHeight();
        lockOnSprite->GetSpriteTransform()->SetPos(Sprite::ConvertToScreenPos(pos));
        auto sPos = lockOnSprite->GetSpriteTransform()->GetPos();
        sPos.x -= lockOnSprite->GetSpriteTransform()->GetSize().x / 2;
        sPos.y -= lockOnSprite->GetSpriteTransform()->GetSize().y / 2;
        lockOnSprite->GetSpriteTransform()->SetPos(sPos);
        lockOnSprite->Update(elapsedTime);
    }
}

void UserInterface::Render()
{
    hpSprite->Render();
}

void UserInterface::BloomRender()
{
    if (Camera::Instance().activeLockOn && Camera::Instance().GetLockOnTarget())
    {
        lockOnSprite->Render();
    }

}

void UserInterface::DrawDebug()
{
    if (ImGui::BeginMenu("UserInterface"))
    {
        lockOnSprite->DrawDebug();

        hpSprite->DrawDebug();

        ImGui::EndMenu();
    }

}
