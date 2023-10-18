#include "UserInterface.h"
#include "../Graphics/Graphics.h"
#include "Enemy.h"

void UserInterface::Initialize()
{
    Graphics& graphics = Graphics::Instance();

    lockOnSprite = std::make_unique<Sprite>(graphics.GetDevice(),
        L"./Resources/Image/UI/LockOn.png");
    lockOnSprite->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(16, 16));
    lockOnSprite->GetSpriteTransform()->SetTexSize(DirectX::XMFLOAT2(256, 256));

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

    lvSprite = std::make_unique<Sprite>(graphics.GetDevice(),
        L"./Resources/Image/UI/Lv.png");
    waveSprite = std::make_unique<Sprite>(graphics.GetDevice(),
        L"./Resources/Image/UI/Wave.png");
    numSprite = std::make_unique<Sprite>(graphics.GetDevice(),
        L"./Resources/Image/UI/numbers.png");
    chonchonSprite = std::make_unique<Sprite>(graphics.GetDevice(),
        L"./Resources/Image/UI/chonchon.png");

    // ‰Šú‰»
    hpSprite->GetSpriteTransform()->SetPos(DirectX::XMFLOAT2(25.0f, 0.0f));
    hpSprite->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(380.0f, 50.0f));
    keikenchiSprite->GetSpriteTransform()->SetPos(DirectX::XMFLOAT2(25.0f, 0.0f));
    keikenchiSprite->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(380.0f, 50.0f));
    for (int i = 0; i < 2; ++i)
    {
        backGageSprite[i]->GetSpriteTransform()->SetPos(DirectX::XMFLOAT2(25.0f, 0.0f));
        backGageSprite[i]->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(380.0f, 50.0f));
        backGageSprite[i]->GetSpriteTransform()->SetColor(DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 0.6f));
    }
    maruSprite->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(405.0f, 50.0f));

    lvSprite->GetSpriteTransform()->SetPos(DirectX::XMFLOAT2(5, 45));
    lvSprite->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(64, 30));
    waveSprite->GetSpriteTransform()->SetPos(DirectX::XMFLOAT2(5, 680));
    waveSprite->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(64, 32));
    numSprite->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(24, 40));
    numSprite->GetSpriteTransform()->SetTexSize(DirectX::XMFLOAT2(60, 100));
    chonchonSprite->GetSpriteTransform()->SetPos(DirectX::XMFLOAT2(55.0f, 55.0f));
    chonchonSprite->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(10.0f, 13.0f));
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
        lockOnSprite->PlayAnimation(elapsedTime, 7.0f, 4, false);
    }
}

void UserInterface::Render()
{
    backGageSprite[0]->Render();
    backGageSprite[1]->Render();
    hpSprite->Render();
    keikenchiSprite->Render();
    maruSprite->Render();

    RenderLv();
    RenderWave();
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
        ImGui::SliderInt("Lv", &lv, 0, 1000);
        ImGui::SliderInt("Wave", &wave, 0, 1000);

        numSprite->DrawDebug();
        //lvSprite->DrawDebug();
        //waveSprite->DrawDebug();
        chonchonSprite->DrawDebug();
        
        //lockOnSprite->DrawDebug();

        hpSprite->DrawDebug();
        keikenchiSprite->DrawDebug();
        maruSprite->DrawDebug();
        //backGageSprite[0]->DrawDebug();
        //backGageSprite[1]->DrawDebug();

        ImGui::EndMenu();
    }

}

void UserInterface::RenderLv()
{
    float texSizeX = 60;
    float one = lv % 10 * texSizeX;
    float ten = lv / 10 % 10 * texSizeX;
    float hundred = lv / 100 % 10 * texSizeX;
    float thousand = lv / 1000 % 10 * texSizeX;
    float firstPosX = 70.0f;
    float secondPosX = 90.0f;
    float thirdPosX = 110.0f;
    float fourthPosX = 130.0f;

    numSprite->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(24, 40));
    numSprite->GetSpriteTransform()->SetPosY(39);

    if (lv < 10)
    {
        numSprite->GetSpriteTransform()->SetPosX(firstPosX);
        numSprite->GetSpriteTransform()->SetTexPosX(one);
        numSprite->Render();
    }
    else if (lv < 100)
    {
        numSprite->GetSpriteTransform()->SetPosX(secondPosX);
        numSprite->GetSpriteTransform()->SetTexPosX(one);
        numSprite->Render();
        numSprite->GetSpriteTransform()->SetPosX(firstPosX);
        numSprite->GetSpriteTransform()->SetTexPosX(ten);
        numSprite->Render();
    }
    else if (lv < 1000)
    {
        numSprite->GetSpriteTransform()->SetPosX(thirdPosX);
        numSprite->GetSpriteTransform()->SetTexPosX(one);
        numSprite->Render();
        numSprite->GetSpriteTransform()->SetPosX(secondPosX);
        numSprite->GetSpriteTransform()->SetTexPosX(ten);
        numSprite->Render();
        numSprite->GetSpriteTransform()->SetPosX(firstPosX);
        numSprite->GetSpriteTransform()->SetTexPosX(hundred);
        numSprite->Render();
    }
    else if (lv < 10000)
    {
        numSprite->GetSpriteTransform()->SetPosX(fourthPosX);
        numSprite->GetSpriteTransform()->SetTexPosX(one);
        numSprite->Render();
        numSprite->GetSpriteTransform()->SetPosX(thirdPosX);
        numSprite->GetSpriteTransform()->SetTexPosX(ten);
        numSprite->Render();
        numSprite->GetSpriteTransform()->SetPosX(secondPosX);
        numSprite->GetSpriteTransform()->SetTexPosX(hundred);
        numSprite->Render();
        numSprite->GetSpriteTransform()->SetPosX(firstPosX);
        numSprite->GetSpriteTransform()->SetTexPosX(thousand);
        numSprite->Render();
    }

    lvSprite->Render();     // Lv
    chonchonSprite->GetSpriteTransform()->SetPos(DirectX::XMFLOAT2(55.0f, 55.0f));
    chonchonSprite->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(10.0f, 13.0f));
    chonchonSprite->Render();
}

void UserInterface::RenderWave()
{
    float texSizeX = 60;
    float one = wave % 10 * texSizeX;
    float ten = wave / 10 % 10 * texSizeX;
    float hundred = wave / 100 % 10 * texSizeX;
    float thousand = wave / 1000 % 10 * texSizeX;
    float firstPosX = 85.0f;
    float secondPosX = 110.0f;
    float thirdPosX = 135.0f;
    float fourthPosX = 160.0f;

    numSprite->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(30, 50));
    numSprite->GetSpriteTransform()->SetPosY(672);

    if (wave < 10)
    {
        numSprite->GetSpriteTransform()->SetPosX(firstPosX);
        numSprite->GetSpriteTransform()->SetTexPosX(one);
        numSprite->Render();
    }
    else if (wave < 100)
    {
        numSprite->GetSpriteTransform()->SetPosX(secondPosX);
        numSprite->GetSpriteTransform()->SetTexPosX(one);
        numSprite->Render();
        numSprite->GetSpriteTransform()->SetPosX(firstPosX);
        numSprite->GetSpriteTransform()->SetTexPosX(ten);
        numSprite->Render();
    }
    else if (wave < 1000)
    {
        numSprite->GetSpriteTransform()->SetPosX(thirdPosX);
        numSprite->GetSpriteTransform()->SetTexPosX(one);
        numSprite->Render();
        numSprite->GetSpriteTransform()->SetPosX(secondPosX);
        numSprite->GetSpriteTransform()->SetTexPosX(ten);
        numSprite->Render();
        numSprite->GetSpriteTransform()->SetPosX(firstPosX);
        numSprite->GetSpriteTransform()->SetTexPosX(hundred);
        numSprite->Render();
    }
    else if (wave < 10000)
    {
        numSprite->GetSpriteTransform()->SetPosX(fourthPosX);
        numSprite->GetSpriteTransform()->SetTexPosX(one);
        numSprite->Render();
        numSprite->GetSpriteTransform()->SetPosX(thirdPosX);
        numSprite->GetSpriteTransform()->SetTexPosX(ten);
        numSprite->Render();
        numSprite->GetSpriteTransform()->SetPosX(secondPosX);
        numSprite->GetSpriteTransform()->SetTexPosX(hundred);
        numSprite->Render();
        numSprite->GetSpriteTransform()->SetPosX(firstPosX);
        numSprite->GetSpriteTransform()->SetTexPosX(thousand);
        numSprite->Render();
    }

    waveSprite->Render();   // Wave
    chonchonSprite->GetSpriteTransform()->SetPos(DirectX::XMFLOAT2(70.0f, 692.0f));
    chonchonSprite->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(10.0f, 15.0f));
    chonchonSprite->Render();
}
