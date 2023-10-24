#include "UserInterface.h"
#include "../Graphics/Graphics.h"
#include "Enemy.h"

#include "../Other/Easing.h"

#include "PlayerManager.h"
#include "WaveManager.h"

#include "EnemyManager.h"

void UserInterface::Initialize()
{
    Graphics& graphics = Graphics::Instance();

    lockOnSprite = std::make_unique<Sprite>(graphics.GetDevice(),
        L"./Resources/Image/UI/LockOn.png");
    lockOnSprite->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(16, 16));
    lockOnSprite->GetSpriteTransform()->SetTexSize(DirectX::XMFLOAT2(256, 256));

    lockOnMaruSprite = std::make_unique<Sprite>(graphics.GetDevice(),
        L"./Resources/Image/UI/LockOnMaru.png");
    lockOnMaruSprite->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(16, 16));

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

    whiteSprite = std::make_unique<Sprite>(graphics.GetDevice(),
        L"./Resources/Image/Game/white.png");
    waveSlideSprite = std::make_unique<Sprite>(graphics.GetDevice(),
        L"./Resources/Image/UI/wave.png");
    numSlideSprite = std::make_unique<Sprite>(graphics.GetDevice(),
        L"./Resources/Image/UI/numbers.png");

    // 初期化
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

    whiteSprite->GetSpriteTransform()->SetPos(DirectX::XMFLOAT2(0, 200));
    whiteSprite->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(1280, 70));
    whiteSprite->GetSpriteTransform()->SetColorA(0.5f);
    waveSlideSprite->GetSpriteTransform()->SetPos(DirectX::XMFLOAT2(490, 220));
    waveSlideSprite->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(102.4f, 51.2f));
    numSlideSprite->GetSpriteTransform()->SetPos(DirectX::XMFLOAT2(610, 200));
    numSlideSprite->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(48, 80)); 
    numSlideSprite->GetSpriteTransform()->SetTexSize(DirectX::XMFLOAT2(60, 100));

    for (int i = 0; i < 4; ++i)
    {
        numPosition[i] = DirectX::XMFLOAT2(600, 200);
    }

    //プログラム全体で一度だけ画像サイズを取得
    static bool once = [&]() {
        hpSpriteSizeX = hpSprite->GetSpriteTransform()->GetSizeX();
        expSpriteSizeX = keikenchiSprite->GetSpriteTransform()->GetSizeX();
        return true;
    }();
}

// 更新
void UserInterface::Update(float elapsedTime)
{
    // ロックオン
    UpdateLockOnSprite(elapsedTime);

    // 流れていくwaveSprite
    UpdateWaveSprite(elapsedTime);

    //ｈｐと経験値のゲージを実際の値と同期
    UpdateHpExpGauge();
}

// 描画
void UserInterface::Render()
{
    backGageSprite[0]->Render();
    backGageSprite[1]->Render();
    hpSprite->Render();
    keikenchiSprite->Render();
    maruSprite->Render();


    RenderLv();
    RenderWave();

    if(isWaveSlideSprite) RenderWaveSlide();
}

void UserInterface::BloomRender()
{
    if (Camera::Instance().activeLockOn && Camera::Instance().GetLockOnTarget())
    {
        lockOnSprite->Render();

        if (isLockOnInitialize) lockOnMaruSprite->Render();
    }
}

void UserInterface::DrawDebug()
{
    if (ImGui::BeginMenu("UserInterface"))
    {
        if (ImGui::Button("waveSlide"))
        {
            isWaveSlideSprite = true;
            slideState = 0;
        }


        whiteSprite->DrawDebug();
        waveSlideSprite->DrawDebug();
        numSlideSprite->DrawDebug();
        

        //numSprite->DrawDebug();
        //lvSprite->DrawDebug();
        //waveSprite->DrawDebug();
        //chonchonSprite->DrawDebug();
        
        //lockOnSprite->DrawDebug();

        hpSprite->DrawDebug();
        keikenchiSprite->DrawDebug();
        maruSprite->DrawDebug();
        //backGageSprite[0]->DrawDebug();
        //backGageSprite[1]->DrawDebug();

        ImGui::EndMenu();
    }

}

// ロックオン
void UserInterface::UpdateLockOnSprite(const float& elapsedTime)
{
    if (EnemyManager::Instance().GetEnemyCount() <= 0)return;

    // ロックオン中央画像
    if (Camera::Instance().activeLockOn && Camera::Instance().GetLockOnTarget())
    {
        DirectX::XMFLOAT3 pos = Camera::Instance().GetLockOnTarget()->GetTransform()->GetPosition();
        pos.y += Camera::Instance().GetLockOnTarget()->GetLockOnHeight();
        lockOnSprite->GetSpriteTransform()->SetPos(Sprite::ConvertToScreenPos(pos));
        auto sPos = lockOnSprite->GetSpriteTransform()->GetPos();
        sPos.x -= lockOnSprite->GetSpriteTransform()->GetSizeX() / 2;
        sPos.y -= lockOnSprite->GetSpriteTransform()->GetSizeY() / 2;
        lockOnSprite->GetSpriteTransform()->SetPos(sPos);
        lockOnSprite->Update(elapsedTime);
        lockOnSprite->PlayAnimation(elapsedTime, 7.0f, 4, false);
    }

    // ロックオンの最初に出る丸い枠
    if (isLockOnInitialize && Camera::Instance().activeLockOn && Camera::Instance().GetLockOnTarget())
    {
        float maxTime = 0.3f;
        float minSize = 16.0f;
        float maxSize = 64.0f;
        if (lockOnTimer <= maxTime)
        {
            float size = Easing::InSine(lockOnTimer, maxTime, minSize, maxSize);

            lockOnMaruSprite->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(size, size));

            lockOnTimer += elapsedTime;
        }
        else
        {
            lockOnMaruSprite->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(minSize, minSize));
            lockOnTimer = 0.0f;
            isLockOnInitialize = false;
        }

        DirectX::XMFLOAT3 pos = Camera::Instance().GetLockOnTarget()->GetTransform()->GetPosition();
        pos.y += Camera::Instance().GetLockOnTarget()->GetLockOnHeight();
        lockOnMaruSprite->GetSpriteTransform()->SetPos(Sprite::ConvertToScreenPos(pos));
        DirectX::XMFLOAT2 sPos = lockOnMaruSprite->GetSpriteTransform()->GetPos();
        sPos.x -= lockOnMaruSprite->GetSpriteTransform()->GetSizeX() / 2;
        sPos.y -= lockOnMaruSprite->GetSpriteTransform()->GetSizeY() / 2;
        lockOnMaruSprite->GetSpriteTransform()->SetPos(sPos);
    }
    else
    {
        lockOnTimer = 0.0f;
    }
}

void UserInterface::UpdateWaveSprite(const float& elapsedTime)
{
    if (isWaveSlideSprite)
    {
        switch (slideState)
        {
        case static_cast<UINT>(STATE::Idle):
            // 待機
            whiteSprite->GetSpriteTransform()->SetColorA(0.0f);
            waveSlideSprite->GetSpriteTransform()->SetPos(DirectX::XMFLOAT2(1280, 220));
            for (int i = 0; i < 4; ++i)
            {
                numPosition[i] = DirectX::XMFLOAT2(1280, 200);
            }
            easingTimer = 0.0f;
            slideState = static_cast<UINT>(STATE::CreateWhite);
            break;
        case static_cast<UINT>(STATE::CreateWhite):
        {
            float maxTime = 0.2f;
            if (easingTimer <= maxTime)
            {
                float alpha = Easing::InSine(easingTimer, maxTime, 0.5f, 0.0f);

                whiteSprite->GetSpriteTransform()->SetColorA(alpha);

                easingTimer += elapsedTime;
            }
            else
            {
                whiteSprite->GetSpriteTransform()->SetColorA(0.5f);
                easingTimer = 0.0f;
                slideState = static_cast<UINT>(STATE::FadeIn);
            }
        }
        break;
        case static_cast<UINT>(STATE::FadeIn):
            // フェードイン
        {
            float maxTime = 0.8f;
            float maxWave = 490.0f;
            float maxNum0 = 610.0f;
            float maxNum1 = 660.0f;
            float maxNum2 = 710.0f;
            float maxNum3 = 760.0f;
#if 0
            float minWave = 1280.0f;
            float minNum0 = 1400.0f;
            float minNum1 = 1450.0f;
            float minNum2 = 1500.0f;
            float minNum3 = 1550.0f;
#else
            float minWave = 1280.0f;
            float minNum0 = 1280.0f;
            float minNum1 = 1280.0f;
            float minNum2 = 1280.0f;
            float minNum3 = 1280.0f;
#endif

            if (easingTimer <= maxTime)
            {
                float wavePosX = Easing::InSine(easingTimer, maxTime, maxWave, minWave);
                float num0PosX = Easing::InSine(easingTimer, maxTime, maxNum0, minNum0);
                float num1PosX = Easing::InSine(easingTimer, maxTime, maxNum1, minNum1);
                float num2PosX = Easing::InSine(easingTimer, maxTime, maxNum2, minNum2);
                float num3PosX = Easing::InSine(easingTimer, maxTime, maxNum3, minNum3);

                waveSlideSprite->GetSpriteTransform()->SetPosX(wavePosX);

                numPosition[0].x = num0PosX;
                numPosition[1].x = num1PosX;
                numPosition[2].x = num2PosX;
                numPosition[3].x = num3PosX;

                easingTimer += elapsedTime;
            }
            else
            {
                waveSlideSprite->GetSpriteTransform()->SetPosX(maxWave);
                numPosition[0].x = maxNum0;
                numPosition[1].x = maxNum1;
                numPosition[2].x = maxNum2;
                numPosition[3].x = maxNum3;
                easingTimer = 0.0f;
                slideState = static_cast<UINT>(STATE::Display);
            }
        }
        break;
        case static_cast<UINT>(STATE::Display):
            // 表示
        {
            float maxTime = 1.0f;
            if (easingTimer <= maxTime)
            {
                easingTimer += elapsedTime;
            }
            else
            {
                easingTimer = 0.0f;
                slideState = static_cast<UINT>(STATE::FadeOut);
            }
        }
        break;
        case static_cast<UINT>(STATE::FadeOut):
            // フェードアウト
        {
            float maxTime = 0.8f;
            float minWave = 490.0f;
            float minNum0 = 610.0f;
            float minNum1 = 660.0f;
            float minNum2 = 710.0f;
            float minNum3 = 760.0f;

#if 0
            float maxWave = -310.0f;
            float maxNum0 = -190.0f;
            float maxNum1 = -140.0f;
            float maxNum2 = -90.0f;
            float maxNum3 = -40.0f;
#else
            float maxWave = -100.0f;
            float maxNum0 = -40.0f;
            float maxNum1 = -40.0f;
            float maxNum2 = -40.0f;
            float maxNum3 = -40.0f;
#endif

            if (easingTimer <= maxTime)
            {
                float wavePosX = Easing::InSine(easingTimer, maxTime, maxWave, minWave);
                float num0PosX = Easing::InSine(easingTimer, maxTime, maxNum0, minNum0);
                float num1PosX = Easing::InSine(easingTimer, maxTime, maxNum1, minNum1);
                float num2PosX = Easing::InSine(easingTimer, maxTime, maxNum2, minNum2);
                float num3PosX = Easing::InSine(easingTimer, maxTime, maxNum3, minNum3);

                waveSlideSprite->GetSpriteTransform()->SetPosX(wavePosX);

                numPosition[0].x = num0PosX;
                numPosition[1].x = num1PosX;
                numPosition[2].x = num2PosX;
                numPosition[3].x = num3PosX;

                easingTimer += elapsedTime;
            }
            else
            {
                waveSlideSprite->GetSpriteTransform()->SetPosX(maxWave);
                numPosition[0].x = maxNum0;
                numPosition[1].x = maxNum1;
                numPosition[2].x = maxNum2;
                numPosition[3].x = maxNum3;
                easingTimer = 0.0f;
                slideState = static_cast<UINT>(STATE::DeleteWhite);
            }
        }
        break;
        case static_cast<UINT>(STATE::DeleteWhite):
            // 白背景透明化
        {
            float maxTime = 0.2f;
            if (easingTimer <= maxTime)
            {
                float alpha = Easing::InSine(easingTimer, maxTime, 0.0f, 0.5f);

                whiteSprite->GetSpriteTransform()->SetColorA(alpha);

                easingTimer += elapsedTime;
            }
            else
            {
                whiteSprite->GetSpriteTransform()->SetColorA(0.0f);
                easingTimer = 0.0f;
                slideState = static_cast<UINT>(STATE::Idle);
                isWaveSlideSprite = false;
            }
        }
        break;
        }
    }
}

void UserInterface::UpdateHpExpGauge()
{
    auto* player = PlayerManager::Instance().GetPlayer().get();

    hpSprite->GetSpriteTransform()->SetSizeX(hpSpriteSizeX * (player->GetHealth() / player->GetMaxHealth()));
    keikenchiSprite->GetSpriteTransform()->SetSizeX(expSpriteSizeX * (player->GetCurExp() / player->GetLevelUpExp()));


}

void UserInterface::RenderWaveSlide()
{
    const int wave = WaveManager::Instance().GetCurrentWaveIndex();
    float texSizeX = 60;
    float one = wave % 10 * texSizeX;
    float ten = wave / 10 % 10 * texSizeX;
    float hundred = wave / 100 % 10 * texSizeX;
    float thousand = wave / 1000 % 10 * texSizeX;

    whiteSprite->Render();
    waveSlideSprite->Render();

    numSlideSprite->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(48, 80));
    numSlideSprite->GetSpriteTransform()->SetPosY(200);

    if (wave < 10)
    {
        numSlideSprite->GetSpriteTransform()->SetPos(numPosition[0]);
        numSlideSprite->GetSpriteTransform()->SetTexPosX(one);
        numSlideSprite->Render();
    }
    else if (wave < 100)
    {
        numSlideSprite->GetSpriteTransform()->SetPos(numPosition[0]);
        numSlideSprite->GetSpriteTransform()->SetTexPosX(ten);
        numSlideSprite->Render();
        numSlideSprite->GetSpriteTransform()->SetPos(numPosition[1]);
        numSlideSprite->GetSpriteTransform()->SetTexPosX(one);
        numSlideSprite->Render();
    }
    else if (wave < 1000)
    {
        numSlideSprite->GetSpriteTransform()->SetPos(numPosition[0]);
        numSlideSprite->GetSpriteTransform()->SetTexPosX(hundred);
        numSlideSprite->Render();
        numSlideSprite->GetSpriteTransform()->SetPos(numPosition[1]);
        numSlideSprite->GetSpriteTransform()->SetTexPosX(ten);
        numSlideSprite->Render();
        numSlideSprite->GetSpriteTransform()->SetPos(numPosition[2]);
        numSlideSprite->GetSpriteTransform()->SetTexPosX(one);
        numSlideSprite->Render();
    }
    else if (wave < 10000)
    {
        numSlideSprite->GetSpriteTransform()->SetPos(numPosition[0]);
        numSlideSprite->GetSpriteTransform()->SetTexPosX(thousand);
        numSlideSprite->Render();
        numSlideSprite->GetSpriteTransform()->SetPos(numPosition[1]);
        numSlideSprite->GetSpriteTransform()->SetTexPosX(hundred);
        numSlideSprite->Render();
        numSlideSprite->GetSpriteTransform()->SetPos(numPosition[2]);
        numSlideSprite->GetSpriteTransform()->SetTexPosX(ten);
        numSlideSprite->Render();
        numSlideSprite->GetSpriteTransform()->SetPos(numPosition[3]);
        numSlideSprite->GetSpriteTransform()->SetTexPosX(one);
        numSlideSprite->Render();
    }
}

void UserInterface::RenderLv()
{
    const int lv = PlayerManager::Instance().GetPlayer()->GetLevel();
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
    const int wave = WaveManager::Instance().GetCurrentWaveIndex();
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
