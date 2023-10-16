#include "UserInterface.h"
#include "../Graphics/Graphics.h"
#include "Enemy.h"

void UserInterface::Initialize()
{
    Graphics& graphics = Graphics::Instance();

    lockOnSprite = std::make_unique<Sprite>(graphics.GetDevice(),
        L"./Resources/Image/UI/LockOn.png");
    //L"./Resources/Image/UI/LockOn1.png");
    lockOnSprite->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(16, 16));

    hpSprite = std::make_unique<Sprite>(graphics.GetDevice(),
        L"./Resources/Image/UI/hp.png");
    keikenchiSprite = std::make_unique<Sprite>(graphics.GetDevice(),
        L"./Resources/Image/UI/keikenchi.png");
    maruSprite = std::make_unique<Sprite>(graphics.GetDevice(),
        L"./Resources/Image/UI/maru.png");
    backGageSprite[0] = std::make_unique<Sprite>(graphics.GetDevice(),
        L"./Resources/Image/UI/hp.png");
    backGageSprite[1] = std::make_unique<Sprite>(graphics.GetDevice(),
        L"./Resources/Image/UI/keikenchi.png");

    // ‰Šú‰»
    hpSprite->GetSpriteTransform()->SetPos(DirectX::XMFLOAT2(20.0f, 0.0f));
    hpSprite->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(405.0f, 50));
    keikenchiSprite->GetSpriteTransform()->SetPos(DirectX::XMFLOAT2(20.0f, 0.0f));
    keikenchiSprite->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(405.0f, 50));
    for (int i = 0; i < 2; ++i)
    {
        backGageSprite[i]->GetSpriteTransform()->SetPos(DirectX::XMFLOAT2(20.0f, 0.0f));
        backGageSprite[i]->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(405.0f, 50));
        backGageSprite[i]->GetSpriteTransform()->SetColor(DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 0.6f));
    }
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
    backGageSprite[0]->Render();
    backGageSprite[1]->Render();
    hpSprite->Render();
    keikenchiSprite->Render();
    maruSprite->Render();
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
        keikenchiSprite->DrawDebug();
        maruSprite->DrawDebug();
        backGageSprite[0]->DrawDebug();
        backGageSprite[1]->DrawDebug();

        ImGui::EndMenu();
    }

}
