#include "UserInterface.h"
#include "../Graphics/Graphics.h"
#include "Enemy.h"

void UserInterface::Initialize()
{
    lockOnSprite = std::make_unique<Sprite>(Graphics::Instance().GetDevice(),
        L"./Resources/Image/UI/LockOn.png");
    lockOnSprite->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(32, 32));
}

void UserInterface::Update(float elapsedTime)
{
    if (Camera::Instance().activeLockOn)
    {
        DirectX::XMFLOAT3 pos = Camera::Instance().GetLockOnTarget()->GetTransform()->GetPosition();
        pos.y += 1;
        lockOnSprite->GetSpriteTransform()->SetPos(Sprite::ConvertToScreenPos(pos));
        auto sPos = lockOnSprite->GetSpriteTransform()->GetPos();
        sPos.x -= 16;
        sPos.y -= 16;
        lockOnSprite->GetSpriteTransform()->SetPos(sPos);
        lockOnSprite->Update(elapsedTime);
    }
}

void UserInterface::Render()
{

}

void UserInterface::BloomRender()
{
    lockOnSprite->Render();
}

void UserInterface::DrawDebug()
{
    if (ImGui::BeginMenu("UserInterface"))
    {
        lockOnSprite->DrawDebug();


        ImGui::EndMenu();
    }

}
