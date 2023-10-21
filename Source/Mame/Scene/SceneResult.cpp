#include "SceneResult.h"

#include "SceneManager.h"
#include "SceneTitle.h"

#include "../Graphics/Graphics.h"

#include "../Other/Easing.h"
#include "../Other/misc.h"

#include "../Game/EnemyManager.h"

#include "../Game//PlayerManager.h"
#include "../Game/WaveManager.h"

// リソース生成
void SceneResult::CreateResource()
{
    Graphics& graphics = Graphics::Instance();

    backSprite = std::make_unique<Sprite>(graphics.GetDevice(),
        L"./Resources/Image/Result/renga.png");
    
    emmaSprite = std::make_unique<Sprite>(graphics.GetDevice(),
        L"./Resources/Image/Title/loading.png");


    lifeTimeSprite = std::make_unique<Sprite>(graphics.GetDevice(),
        L"./Resources/Image/UI/lifetime.png");
    waveSprite = std::make_unique<Sprite>(graphics.GetDevice(),
        L"./Resources/Image/UI/Wave.png");
    lvSprite = std::make_unique<Sprite>(graphics.GetDevice(),
        L"./Resources/Image/UI/Lv.png");

    xSprite = std::make_unique<Sprite>(graphics.GetDevice(),
        L"./Resources/Image/UI/x.png");

    numSprite = std::make_unique<Sprite>(graphics.GetDevice(),
        L"./Resources/Image/UI/numbers.png");

    chonchonSprite = std::make_unique<Sprite>(graphics.GetDevice(),
        L"./Resources/Image/UI/chonchon.png");

    enemyGolem = std::make_unique<EnemyGolemResult>();
    player = std::make_unique<PlayerResult>();


    CreatePsFromCso(graphics.GetDevice(), "./Resources/Shader/FinalPassTexturePS.cso", finalPassPS.GetAddressOf());
    
    HRESULT hr{ S_OK };    

    D3D11_BUFFER_DESC desc{};
    desc.ByteWidth = sizeof(ResultConstants);
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;
    desc.StructureByteStride = 0;
    hr = graphics.GetDevice()->CreateBuffer(&desc, nullptr, resultConstantBuffer.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
}

void SceneResult::Initialize()
{
    backSprite->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(1280, 720));
    backSprite->GetSpriteTransform()->SetTexPos(DirectX::XMFLOAT2(40, 320));
    backSprite->GetSpriteTransform()->SetTexSize(DirectX::XMFLOAT2(470, 340));
    backSprite->GetSpriteTransform()->SetColor(DirectX::XMFLOAT4(0.42f, 0.42f, 0.42f, 1.0f));

    lifeTimeSprite->GetSpriteTransform()->SetPos(DirectX::XMFLOAT2(10, 180));
    lifeTimeSprite->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(256, 64));
    waveSprite->GetSpriteTransform()->SetPos(DirectX::XMFLOAT2(70, 300));
    waveSprite->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(128, 64));
    lvSprite->GetSpriteTransform()->SetPos(DirectX::XMFLOAT2(65, 420));
    lvSprite->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(128, 64));

    enemyGolem->Initialize();
    player->Initialize();

    enemyResult[0].position = {  1.5f, -1.5f, 0.0f };
    enemyResult[1].position = { -0.6f, -1.5f, 0.0f };
    enemyResult[2].position = { -2.7f, -1.5f, 0.0f };
    enemyResult[0].scale = { 0.4f, 0.4f, 0.4f };
    enemyResult[1].scale = { 0.6f, 0.6f, 0.6f };
    enemyResult[2].scale = { 0.4f, 0.4f, 0.4f };
    enemyResult[0].color = { 0.11f, 0.56f, 1.00f, 1.0f };
    enemyResult[1].color = { 0.0f, 0.0f, 1.0f, 1.0f };
    enemyResult[2].color = { 0.00f, 0.80f, 0.81f, 1.0f };

    // 変数初期化
    for (int i = 0; i < iconMax; ++i)
    {
        iconStruct[i].easingTimer = 0.0f;
        iconStruct[i].scale = 0.0f;
        iconStruct[i].isDisplay = false;
    }

#ifdef _DEBUG
    PlayerManager::Instance().Initialize();
#endif

    isIconUpdateEnd = false;
    isLifeTimer = false;
    isWave = false;
    isLv = false;
    

    resultState = static_cast<UINT>(STATE::Initialize);
}

void SceneResult::Finalize()
{
}

void SceneResult::Begin()
{
}

void SceneResult::Update(const float& elapsedTime)
{
    enemyGolem->Update(elapsedTime);
    player->Update(elapsedTime);

    switch (resultState)
    {
    case static_cast<UINT>(STATE::Initialize):
        isLifeTimer = true;
        resultState = static_cast<UINT>(STATE::LifeTime);
        break;
    case static_cast<UINT>(STATE::LifeTime):
        UpdateLifeTimeNum(elapsedTime);
        if (!isLifeTimer)
        {
            isWave = true;
            resultState = static_cast<UINT>(STATE::Wave);
        }
        break;
    case static_cast<UINT>(STATE::Wave):
        UpdateWave(elapsedTime);
        if (!isWave)
        {
            isLv = true;
            resultState = static_cast<UINT>(STATE::Lv);
        }
        break;
    case static_cast<UINT>(STATE::Lv):
        UpdateLv(elapsedTime);
        if (!isLv)
        {
            resultState = static_cast<UINT>(STATE::Icon);
        }
        break;
    case static_cast<UINT>(STATE::Icon):
        UpdateIcon(elapsedTime);
        if (isIconUpdateEnd)
        {
            resultState = static_cast<UINT>(STATE::Enemy);
        }
        break;
    case static_cast<UINT>(STATE::Enemy):
        break;
    case static_cast<UINT>(STATE::EnemyKill):
        break;
    }

    // 敵のキル数とx
    UpdateEnemyKillNumAndx(elapsedTime);    
}

void SceneResult::End()
{
}

void SceneResult::Render(const float& elapsedTime)
{
    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();

    Mame::Scene::BaseScene::RenderInitialize();

    // スプライト描画
    shader->SetBlendState(static_cast<UINT>(Shader::BLEND_STATE::ALPHA));
    shader->SetDepthStencileState(static_cast<UINT>(Shader::DEPTH_STATE::ZT_ON_ZW_ON));
    backSprite->Render(); // 背景
    //emmaSprite->Render();

    
    
    lvSprite->Render();

    numSprite->Render();

    RenderLifeTime(); // lifetime
    RenderWave();
    RenderLv();

    RenderSkill();      // スキルアイコン
    RenderSkillX();     // スキルx
    RenderSkillNum();   // スキルの獲得数
    
    RenderEnemyKillX();   // キル数のx
    RenderEnemyKillNum(); // キル数の数字


    // モデル描画
    RenderEnemyModel();   // 敵のモデル
}

void SceneResult::DrawDebug()
{
    if (ImGui::Button("icon"))
    {
        for (int i = 0; i < iconMax; ++i)
        {
            iconStruct[i].easingTimer = 0.0f;
            iconStruct[i].scale = 0.0f;
            iconStruct[i].isDisplay = false;
        }
        iconStruct[0].isDisplay = true;
    }

    chonchonSprite->DrawDebug();

    xSprite->DrawDebug();
    numSprite->DrawDebug();

    emmaSprite->DrawDebug();
    lifeTimeSprite->DrawDebug();
    waveSprite->DrawDebug();
    lvSprite->DrawDebug();
    

    ImGui::DragFloat("killXAddPosX", &KillX.addPosX);
    ImGui::DragFloat("killNumAddPosX", &killNum.addPosX);
    if (ImGui::Button("slide"))
    {
        isSlide = true;
        KillX.alpha = 0.0f;
        killNum.alpha = 0.0f;
    }

    if (ImGui::Button("isLifeTimer"))
    {
        isLifeTimer = true;
        lifeTimer.alpha = 0.0f;
        lifeTimerNum.alpha = 0.0f;
    }

    backSprite->DrawDebug();

    ImGui::ColorEdit4("golemColor", &resultConstants.color.x);

    ImGui::Begin("golem");
    enemyGolem->DrawDebug();
    ImGui::End();

    ImGui::Begin("player");
    player->DrawDebug();
    ImGui::End();

    if (ImGui::BeginMenu("zako"))
    {
        ImGui::Begin("enemy0");
        ImGui::DragFloat3("position", &enemyResult[0].position.x);
        ImGui::DragFloat3("scale", &enemyResult[0].scale.x);
        ImGui::ColorEdit4("color", &enemyResult[0].color.x);
        ImGui::End();

        ImGui::Begin("enemy1");
        ImGui::DragFloat3("position", &enemyResult[1].position.x);
        ImGui::DragFloat3("scale", &enemyResult[1].scale.x);
        ImGui::ColorEdit4("color", &enemyResult[1].color.x);
        ImGui::End();

        ImGui::Begin("enemy2");
        ImGui::DragFloat3("position", &enemyResult[2].position.x);
        ImGui::DragFloat3("scale", &enemyResult[2].scale.x);
        ImGui::ColorEdit4("color", &enemyResult[2].color.x);
        ImGui::End();

        ImGui::EndMenu();
    }
}

void SceneResult::UpdateLifeTimeNum(const float& elapsedTime)
{
    if (isLifeTimer)
    {
        float maxTime = 1.0f;
        float subMaxTime = 0.7f;
        if (lifeTimer.easingTimer <= maxTime)
        {
            lifeTimer.alpha = Easing::InSine(lifeTimer.easingTimer, maxTime, 1.0f, 0.0f);
            lifeTimer.addPosX = Easing::InSine(lifeTimer.easingTimer, maxTime, 0.0f, -80.0f);

            lifeTimer.easingTimer += elapsedTime;
        }
        else
        {
            lifeTimer.alpha = 1.0f;
            lifeTimer.addPosX = 0.0f;
        }

        if (lifeTimer.easingTimer >= maxTime / 2)
        {
            if (lifeTimerNum.easingTimer <= subMaxTime)
            {
                lifeTimerNum.alpha = Easing::InQuint(lifeTimerNum.easingTimer, subMaxTime, 1.0f, 0.0f);
                lifeTimerNum.addPosX = Easing::InSine(lifeTimerNum.easingTimer, subMaxTime, 0.0f, -80.0f);


                lifeTimerNum.easingTimer += elapsedTime;
            }
            else
            {
                lifeTimerNum.alpha = 1.0f;
                lifeTimerNum.addPosX = 0.0f;
                lifeTimerNum.easingTimer = 0.0f;

                lifeTimer.easingTimer = 0.0f;
                isLifeTimer = false;
            }
        }
    }
}

void SceneResult::UpdateWave(const float& elapsedTime)
{
    if (isWave)
    {
        float maxTime = 1.0f;
        float subMaxTime = 0.7f;
        if (wave.easingTimer <= maxTime)
        {
            wave.alpha = Easing::InSine(wave.easingTimer, maxTime, 1.0f, 0.0f);
            wave.addPosX = Easing::InSine(wave.easingTimer, maxTime, 0.0f, -80.0f);

            wave.easingTimer += elapsedTime;
        }
        else
        {
            wave.alpha = 1.0f;
            wave.addPosX = 0.0f;
        }

        if (wave.easingTimer >= maxTime / 2)
        {
            if (waveNum.easingTimer <= subMaxTime)
            {
                waveNum.alpha = Easing::InQuint(waveNum.easingTimer, subMaxTime, 1.0f, 0.0f);
                waveNum.addPosX = Easing::InSine(waveNum.easingTimer, subMaxTime, 0.0f, -80.0f);


                waveNum.easingTimer += elapsedTime;
            }
            else
            {
                waveNum.alpha = 1.0f;
                waveNum.addPosX = 0.0f;
                waveNum.easingTimer = 0.0f;

                wave.easingTimer = 0.0f;
                isWave = false;
            }
        }
    }
}

void SceneResult::UpdateLv(const float& elapsedTime)
{
}

void SceneResult::UpdateIcon(const float& elapsedTime)
{
    float maxTime = 0.5f;
    float firstSize = 300.0f;
    float finalSize = 100.0f;
    for (int i = 0; i < iconMax; ++i)
    {
        if (iconStruct[i].isDisplay)
        {
            if (iconStruct[i].easingTimer <= maxTime)
            {
                iconStruct[i].scale = Easing::InSine(iconStruct[i].easingTimer, maxTime, finalSize, firstSize);

                iconStruct[i].easingTimer += elapsedTime;
            }
            else
            {
                int next = i + 1;
                if (next != iconMax)
                {
                    iconStruct[next].isDisplay = true;
                }
                else
                {
                    isIconUpdateEnd = true;
                }
            }
        }
    }
}

void SceneResult::UpdateEnemyKillNumAndx(const float& elapsedTime)
{
    if (isSlide)
    {
        float maxTime = 1.0f;
        float subMaxTime = 0.7f;
        if (KillX.easingTimer <= maxTime)
        {
            KillX.alpha = Easing::InSine(KillX.easingTimer, maxTime, 1.0f, 0.0f);
            KillX.addPosX = Easing::InSine(KillX.easingTimer, maxTime, 0.0f, -80.0f);

            KillX.easingTimer += elapsedTime;
        }
        else
        {
            KillX.alpha = 1.0f;
            KillX.addPosX = 0.0f;
        }

        if (KillX.easingTimer >= maxTime / 2)
        {
            if (killNum.easingTimer <= subMaxTime)
            {
                killNum.alpha = Easing::InQuint(killNum.easingTimer, subMaxTime, 1.0f, 0.0f);
                killNum.addPosX = Easing::InSine(killNum.easingTimer, subMaxTime, 0.0f, -80.0f);


                killNum.easingTimer += elapsedTime;
            }
            else
            {
                killNum.alpha = 1.0f;
                killNum.addPosX = 0.0f;
                killNum.easingTimer = 0.0f;

                KillX.easingTimer = 0.0f;
                isSlide = false;
            }
        }
    }
}

void SceneResult::RenderLifeTime()
{
    lifeTimeSprite->GetSpriteTransform()->SetPosX(10 + lifeTimer.addPosX);
    lifeTimeSprite->GetSpriteTransform()->SetPosY(180);
    lifeTimeSprite->GetSpriteTransform()->SetColorA(lifeTimer.alpha);
    lifeTimeSprite->Render();

    int lifetime = PlayerManager::Instance().GetLifeTime();
    int minutes = lifetime / 60; // 分
    int seconds = lifetime % 60; // 秒

    numSprite->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(60, 100));
    numSprite->GetSpriteTransform()->SetTexSize(DirectX::XMFLOAT2(60, 100));

    numSprite->GetSpriteTransform()->SetColorA(lifeTimerNum.alpha);

    int texSizeX = 60;
    int first  = minutes / 10 % 10 * texSizeX;  // 1桁目
    int second = minutes % 10 * texSizeX;       // 2桁目
    
    numSprite->GetSpriteTransform()->SetPosY(160.0f);

    numSprite->GetSpriteTransform()->SetPosX(lifeTimePos[0] + lifeTimerNum.addPosX);
    numSprite->GetSpriteTransform()->SetTexPosX(first);
    numSprite->Render();
    numSprite->GetSpriteTransform()->SetPosX(lifeTimePos[1] + lifeTimerNum.addPosX);
    numSprite->GetSpriteTransform()->SetTexPosX(second);
    numSprite->Render();

    first  = seconds / 10 % 10 * texSizeX;  // 1桁目
    second = seconds % 10 * texSizeX;       // 2桁目

    numSprite->GetSpriteTransform()->SetPosX(lifeTimePos[2] + lifeTimerNum.addPosX);
    numSprite->GetSpriteTransform()->SetTexPosX(first);
    numSprite->Render();
    numSprite->GetSpriteTransform()->SetPosX(lifeTimePos[3] + lifeTimerNum.addPosX);
    numSprite->GetSpriteTransform()->SetTexPosX(second);
    numSprite->Render();

    chonchonSprite->GetSpriteTransform()->SetPosX(410 + lifeTimerNum.addPosX);
    chonchonSprite->GetSpriteTransform()->SetPosY(190);
    chonchonSprite->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(42, 42));
    chonchonSprite->GetSpriteTransform()->SetColorA(lifeTimerNum.alpha);
    chonchonSprite->Render();
}

void SceneResult::RenderWave()
{
    waveSprite->GetSpriteTransform()->SetPosX(70 + wave.addPosX);
    waveSprite->GetSpriteTransform()->SetPosY(300);
    waveSprite->GetSpriteTransform()->SetColorA(wave.alpha);
    waveSprite->Render();
    
    int waveIndex = WaveManager::Instance().GetCurrentWaveIndex();
    
    numSprite->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(60, 100));
    numSprite->GetSpriteTransform()->SetTexSize(DirectX::XMFLOAT2(60, 100));
    numSprite->GetSpriteTransform()->SetColorA(waveNum.alpha);

    numSprite->GetSpriteTransform()->SetPosY(280);
    RenderNum(waveIndex, 300 + waveNum.addPosX, 350 + waveNum.addPosX, 400 + waveNum.addPosX, 450 + waveNum.addPosX);
}

void SceneResult::RenderLv()
{
    //int lvIndex = PlayerManager::Instance().GetPlayer()->GetLevel();

    numSprite->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(60, 100));
    numSprite->GetSpriteTransform()->SetTexSize(DirectX::XMFLOAT2(60, 100));

    numSprite->GetSpriteTransform()->SetPosY(400);
    numSprite->GetSpriteTransform()->SetPosX(300);
}

void SceneResult::RenderSkill()
{
    int iconNum = 0;
    std::vector<BaseSkill*> skillArray = PlayerManager::Instance().GetSkillArray();
    for (auto& skill : skillArray)
    {
        // スキルを取ってなければ戻る
#ifdef _DEBUG
#else
        if (skill->GetOverlapNum() <= 0) continue;
#endif

        skill->icon->GetSpriteTransform()->SetSpriteCenterPos(iconPos[iconNum]);
        skill->icon->GetSpriteTransform()->SetSizeXY(iconStruct[iconNum].scale);
        if (iconStruct[iconNum].isDisplay)
        {
            skill->icon->Render();
        }
        ++iconNum;
    }
}

void SceneResult::RenderSkillX()
{
    xSprite->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(32, 32));
    xSprite->GetSpriteTransform()->SetColorA(skillX.alpha);

    int iconNum = 0;
    std::vector<BaseSkill*> skillArray = PlayerManager::Instance().GetSkillArray();
    for (auto& skill : skillArray)
    {
        // スキルを取ってなければ戻る
#ifdef _DEBUG
#else
        if (skill->GetOverlapNum() <= 0) continue;
#endif

        xSprite->GetSpriteTransform()->SetPosX(skillXPos[iconNum].x + skillX.addPosX);
        xSprite->GetSpriteTransform()->SetPosY(skillXPos[iconNum].y);
        xSprite->Render();

        ++iconNum;
    }
}

void SceneResult::RenderSkillNum()
{
    numSprite->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(30, 50));
    numSprite->GetSpriteTransform()->SetTexSize(DirectX::XMFLOAT2(60, 100));
    numSprite->GetSpriteTransform()->SetColorA(skillNum.alpha);

    int iconNum = 0;
    std::vector<BaseSkill*> skillArray = PlayerManager::Instance().GetSkillArray();
    for (auto& skill : skillArray)
    {
        // スキルを取ってなければ戻る
#ifdef _DEBUG
#else
        if (skill->GetOverlapNum() <= 0) continue;
#endif
        numSprite->GetSpriteTransform()->SetPosY(skillNumPos[iconNum].y);
        float posX = skillNumPos[iconNum].x;
        RenderNum(skill->GetOverlapNum(), posX, posX + 20.0f, posX + 40.0f, posX + 60.0f);
        
        ++iconNum;
    }
}

void SceneResult::RenderEnemyKillX()
{
    xSprite->GetSpriteTransform()->SetColorA(KillX.alpha);
    xSprite->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(32, 32));
    xSprite->GetSpriteTransform()->SetPosY(665);
    xSprite->GetSpriteTransform()->SetPosX(180 + KillX.addPosX);
    xSprite->Render();
    xSprite->GetSpriteTransform()->SetPosX(460 + KillX.addPosX);
    xSprite->Render();
    xSprite->GetSpriteTransform()->SetPosX(750 + KillX.addPosX);
    xSprite->Render();
    xSprite->GetSpriteTransform()->SetPosX(1025 + KillX.addPosX);
    xSprite->Render();
}

void SceneResult::RenderEnemyKillNum()
{
    int ene1 = 5678;//EnemyManager::Instance().GetEnemy1KillNum();
    int ene2 = 1234;//EnemyManager::Instance().GetEnemy2KillNum();
    int ene3 = EnemyManager::Instance().GetEnemy3KillNum();
    int eneG = EnemyManager::Instance().GetEnemyGolemKillNum();

    numSprite->GetSpriteTransform()->SetPosY(615);
    numSprite->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(60, 100));
    numSprite->GetSpriteTransform()->SetTexSize(DirectX::XMFLOAT2(60, 100));
    numSprite->GetSpriteTransform()->SetColorA(killNum.alpha);

    RenderNum(ene1, 210 + killNum.addPosX, 260 + killNum.addPosX, 310 + killNum.addPosX, 360 + killNum.addPosX);
    RenderNum(ene2, 490 + killNum.addPosX, 540 + killNum.addPosX, 590 + killNum.addPosX, 640 + killNum.addPosX);
    RenderNum(ene3, 780 + killNum.addPosX, 830 + killNum.addPosX, 880 + killNum.addPosX, 930 + killNum.addPosX);
    RenderNum(eneG, 1055 + killNum.addPosX, 1105 + killNum.addPosX, 1155 + killNum.addPosX, 1205 + killNum.addPosX);
}

void SceneResult::RenderNum(const int who, const float firstPosX, const float secondPosX, const float thirdPosX, const float fourthPosX)
{
    float texSizeX = 60;
    float one = who % 10 * texSizeX;
    float ten = who / 10 % 10 * texSizeX;
    float hundred = who / 100 % 10 * texSizeX;
    float thousand = who / 1000 % 10 * texSizeX;

    if (who < 10)
    {
        numSprite->GetSpriteTransform()->SetPosX(firstPosX);
        numSprite->GetSpriteTransform()->SetTexPosX(one);
        numSprite->Render();
    }
    else if (who < 100)
    {
        numSprite->GetSpriteTransform()->SetPosX(secondPosX);
        numSprite->GetSpriteTransform()->SetTexPosX(one);
        numSprite->Render();
        numSprite->GetSpriteTransform()->SetPosX(firstPosX);
        numSprite->GetSpriteTransform()->SetTexPosX(ten);
        numSprite->Render();
    }
    else if (who < 1000)
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
    else if (who < 10000)
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
}

void SceneResult::RenderEnemyModel()
{
    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();

    shader->SetBlendState(static_cast<UINT>(Shader::BLEND_STATE::NONE));
    resultConstants.color = { 0.7f, 0.7f, 0.7f ,1.0f };
    graphics.GetDeviceContext()->UpdateSubresource(resultConstantBuffer.Get(), 0, 0, &resultConstants, 0, 0);
    graphics.GetDeviceContext()->PSSetConstantBuffers(5, 1, resultConstantBuffer.GetAddressOf());
    enemyGolem->Render(0.01f);

    for (int i = 0; i < 3; ++i)
    {
        resultConstants.color = enemyResult[i].color;
        graphics.GetDeviceContext()->UpdateSubresource(resultConstantBuffer.Get(), 0, 0, &resultConstants, 0, 0);
        graphics.GetDeviceContext()->PSSetConstantBuffers(5, 1, resultConstantBuffer.GetAddressOf());
        player->GetTransform()->SetPosition(enemyResult[i].position);
        player->GetTransform()->SetScale(enemyResult[i].scale);
        player->swordModel->GetTransform()->SetPosition(enemyResult[i].position);
        player->swordModel->GetTransform()->SetScale(enemyResult[i].scale);

        if (i != 2)
            player->Render(0.01f);
        else
            player->Render(0.01f, nullptr, false);
    }
}
