#include "SceneResult.h"

#include "SceneManager.h"
#include "SceneTitle.h"
#include "SceneLoading.h"

#include "../Graphics/Graphics.h"

#include "../Other/Easing.h"
#include "../Other/misc.h"

#include "../Game/EnemyManager.h"

#include "../Game//PlayerManager.h"
#include "../Game/WaveManager.h"

#include "../framework.h"

#include "../Resource/AudioManager.h"

// リソース生成
void SceneResult::CreateResource()
{
    Graphics& graphics = Graphics::Instance();

    backSprite = std::make_unique<Sprite>(graphics.GetDevice(),
        L"./Resources/Image/Result/renga.png");
    
    emmaSprite = std::make_unique<Sprite>(graphics.GetDevice(),
        L"./Resources/Image/Title/loading.png");

    resultSprite = std::make_unique<Sprite>(graphics.GetDevice(),
        L"./Resources/Image/UI/Result.png");

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

    // 定数バッファー
    {
        D3D11_BUFFER_DESC bufferDesc{};
        bufferDesc.ByteWidth = sizeof(Shader::SceneConstants);
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bufferDesc.CPUAccessFlags = 0;
        bufferDesc.MiscFlags = 0;
        bufferDesc.StructureByteStride = 0;
        // SceneConstants
        hr = graphics.GetDevice()->CreateBuffer(&bufferDesc, nullptr,
            ConstantBuffer.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    }
}

void SceneResult::Initialize()
{
    Camera::Instance().ResultInitialize();

    backSprite->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(1280, 720));
    backSprite->GetSpriteTransform()->SetTexPos(DirectX::XMFLOAT2(40, 320));
    backSprite->GetSpriteTransform()->SetTexSize(DirectX::XMFLOAT2(470, 340));
    //backSprite->GetSpriteTransform()->SetColor(DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f));
    backSprite->GetSpriteTransform()->SetColor(DirectX::XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f));
    //backSprite->GetSpriteTransform()->SetColor(DirectX::XMFLOAT4(0.42f, 0.42f, 0.42f, 1.0f));

    emmaSprite->GetSpriteTransform()->SetColor(DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

    lifeTimeSprite->GetSpriteTransform()->SetPos(DirectX::XMFLOAT2(10, 180));
    lifeTimeSprite->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(256, 64));
    waveSprite->GetSpriteTransform()->SetPos(DirectX::XMFLOAT2(70, 300));
    waveSprite->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(128, 64));
    lvSprite->GetSpriteTransform()->SetPos(DirectX::XMFLOAT2(65, 420));
    lvSprite->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(128, 64));

    enemyGolem->Initialize();
    player->Initialize();

    golemResult.color = { 0.7f, 0.7f, 0.7f , 0.0f };
    enemyResult[0].position = {  1.5f, -1.5f, 0.0f };
    enemyResult[1].position = { -0.6f, -1.5f, 0.0f };
    enemyResult[2].position = { -2.7f, -1.5f, 0.0f };
    enemyResult[0].scale = { 0.4f, 0.4f, 0.4f };
    enemyResult[1].scale = { 0.6f, 0.6f, 0.6f };
    enemyResult[2].scale = { 0.4f, 0.4f, 0.4f };
    enemyResult[0].color = { 0.11f, 0.56f, 1.00f, 0.0f };
    enemyResult[1].color = { 0.0f, 0.0f, 1.0f, 0.0f };
    enemyResult[2].color = { 0.00f, 0.80f, 0.81f, 0.0f };

    // 変数初期化
    for (int i = 0; i < iconMax; ++i)
    {
        iconStruct[i].easingTimer = 0.0f;
        iconStruct[i].scale = 0.0f;
        iconStruct[i].isDisplay = false;
    }


    isIconUpdateEnd = false;
    isLifeTimer = false;
    isWave = false;
    isLv = false;
    isIconUpdateEnd = false;
    isIconNum = false;
    isEnemy = false;
    

    resultState = static_cast<UINT>(STATE::Initialize);

    // リザルトBGM再生
    AudioManager::Instance().PlayBGM(BGM::Result);
}

void SceneResult::Finalize()
{
    //生存時間をリセット
    PlayerManager::Instance().SetLifeTime(0.0f);

    //プレイヤーのレベルをリセット
    PlayerManager::Instance().SetLevel(0);

    // キルした敵の数をリセット
    EnemyManager::Instance().ResetKillNum();

    // オーディオを止める
    AudioManager::Instance().StopAllAudio();
}

void SceneResult::Begin()
{
}

void SceneResult::Update(const float& elapsedTime)
{
#ifdef _DEBUG
    if (GetAsyncKeyState('P') & 0x01)
    {
        Mame::Scene::SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
        return;
    }
#endif

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
            iconStruct[0].isDisplay = true;
            resultState = static_cast<UINT>(STATE::Icon);
        }
        break;
    case static_cast<UINT>(STATE::Icon):
        UpdateIcon(elapsedTime);
        if (isIconUpdateEnd)
        {
            isIconNum = true;
            resultState = static_cast<UINT>(STATE::IconNum);
        }
        break;
    case static_cast<UINT>(STATE::IconNum):
        UpdateIconNum(elapsedTime);
        if (!isIconNum)
        {
            isEnemy = true;
            resultState = static_cast<UINT>(STATE::Enemy);
        }
        break;
    case static_cast<UINT>(STATE::Enemy):
        UpdateEnemy(elapsedTime);
        if (!isEnemy)
        {
            isSlide = true;
            resultState = static_cast<UINT>(STATE::EnemyKill);
        }
        break;
    case static_cast<UINT>(STATE::EnemyKill):
        UpdateEnemyKillNumAndx(elapsedTime);    
        break;
    }
}

void SceneResult::End()
{
}

void SceneResult::Render(const float& elapsedTime)
{
    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();

    Mame::Scene::BaseScene::RenderInitialize();

    Camera& camera = Camera::Instance();
    Camera::Instance().TitleSetPerspectiveFov(graphics.GetDeviceContext());

    Shader::SceneConstants sceneConstants{};
    DirectX::XMStoreFloat4x4(&sceneConstants.viewProjection, camera.GetViewMatrix() * camera.GetProjectionMatrix());
    sceneConstants.lightDirection = shader->GetViewPosition();
    sceneConstants.cameraPosition = shader->GetViewCamera();

    DirectX::XMStoreFloat4x4(&sceneConstants.inverseProjection, DirectX::XMMatrixInverse(NULL, camera.GetProjectionMatrix()));
    DirectX::XMStoreFloat4x4(&sceneConstants.inverseViewProjection, DirectX::XMMatrixInverse(NULL, camera.GetViewMatrix() * camera.GetProjectionMatrix()));
    sceneConstants.time = framework::tictoc.time_stamp();

    graphics.GetDeviceContext()->UpdateSubresource(ConstantBuffer.Get(), 0, 0, &sceneConstants, 0, 0);
    graphics.GetDeviceContext()->VSSetConstantBuffers(1, 1, ConstantBuffer.GetAddressOf());
    graphics.GetDeviceContext()->PSSetConstantBuffers(1, 1, ConstantBuffer.GetAddressOf());

    // スプライト描画
    shader->SetBlendState(static_cast<UINT>(Shader::BLEND_STATE::ALPHA));
    shader->SetDepthStencileState(static_cast<UINT>(Shader::DEPTH_STATE::ZT_ON_ZW_ON));
    backSprite->Render(); // 背景
    emmaSprite->Render();

    resultSprite->Render();
    
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
    Camera::Instance().DrawDebug();

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

    resultSprite->DrawDebug();

    emmaSprite->DrawDebug();
    
    backSprite->DrawDebug();
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
    if (isLv)
    {
        float maxTime = 1.0f;
        float subMaxTime = 0.7f;
        if (lv.easingTimer <= maxTime)
        {
            lv.alpha = Easing::InSine(lv.easingTimer, maxTime, 1.0f, 0.0f);
            lv.addPosX = Easing::InSine(lv.easingTimer, maxTime, 0.0f, -80.0f);

            lv.easingTimer += elapsedTime;
        }
        else
        {
            lv.alpha = 1.0f;
            lv.addPosX = 0.0f;
        }

        if (lv.easingTimer >= maxTime / 2)
        {
            if (lvNum.easingTimer <= subMaxTime)
            {
                lvNum.alpha = Easing::InQuint(lvNum.easingTimer, subMaxTime, 1.0f, 0.0f);
                lvNum.addPosX = Easing::InSine(lvNum.easingTimer, subMaxTime, 0.0f, -80.0f);


                lvNum.easingTimer += elapsedTime;
            }
            else
            {
                lvNum.alpha = 1.0f;
                lvNum.addPosX = 0.0f;
                lvNum.easingTimer = 0.0f;

                lv.easingTimer = 0.0f;
                isLv = false;
            }
        }
    }
}

void SceneResult::UpdateIcon(const float& elapsedTime)
{
    int iconNum = 0;
    std::vector<BaseSkill*> skillArray = PlayerManager::Instance().GetSkillArray();
    for (auto& skill : skillArray)
    {
        if(skill->GetOverlapNum()>0)
            ++iconNum;
    }

    if (iconNum == 0)
    {
        isIconUpdateEnd = true;
        return;
    }

    if (iconStruct[iconNum].isDisplay)
    {
        isIconUpdateEnd = true;
        return;
    }

    float maxTime = 0.5f;
    float firstSize = 300.0f;
    float finalSize = 100.0f;
    for (int i = 0; i < iconNum; ++i)
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
            }
        }
    }
}

void SceneResult::UpdateIconNum(const float& elapsedTime)
{
    if (isIconNum)
    {
        float maxTime = 1.0f;
        float subMaxTime = 0.7f;
        if (skillX.easingTimer <= maxTime)
        {
            skillX.alpha = Easing::InSine(skillX.easingTimer, maxTime, 1.0f, 0.0f);
            skillX.addPosX = Easing::InSine(skillX.easingTimer, maxTime, 0.0f, -60.0f);

            skillX.easingTimer += elapsedTime;
        }
        else
        {
            skillX.alpha = 1.0f;
            skillX.addPosX = 0.0f;
        }

        if (skillX.easingTimer >= maxTime / 2)
        {
            if (skillNum.easingTimer <= subMaxTime)
            {
                skillNum.alpha = Easing::InQuint(skillNum.easingTimer, subMaxTime, 1.0f, 0.0f);
                skillNum.addPosX = Easing::InSine(skillNum.easingTimer, subMaxTime, 0.0f, -60.0f);


                skillNum.easingTimer += elapsedTime;
            }
            else
            {
                skillNum.alpha = 1.0f;
                skillNum.addPosX = 0.0f;
                skillNum.easingTimer = 0.0f;

                skillX.easingTimer = 0.0f;
                isIconNum = false;
            }
        }
    }
}

void SceneResult::UpdateEnemy(const float& elapsedTime)
{
    if (isEnemy)
    {
        float maxTime = 1.0f;
        if (enemy.easingTimer <= maxTime)
        {
            float alpha = Easing::InSine(enemy.easingTimer, maxTime, 1.0f, 0.0f);

            golemResult.color.w = alpha;
            for (int i = 0; i < 3; ++i)
            {
                enemyResult[i].color.w = alpha;
            }

            enemy.easingTimer += elapsedTime;
        }
        else
        {
            golemResult.color.w = 1.0f;
            for (int i = 0; i < 3; ++i)
            {
                enemyResult[i].color.w = 1.0f;
            }

            isEnemy = false;
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
    lvSprite->GetSpriteTransform()->SetPosX(65 + lv.addPosX);
    lvSprite->GetSpriteTransform()->SetPosY(420);
    lvSprite->GetSpriteTransform()->SetColorA(lv.alpha);
    lvSprite->Render();

    int lvIndex = PlayerManager::Instance().GetLevel();

    numSprite->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(60, 100));
    numSprite->GetSpriteTransform()->SetTexSize(DirectX::XMFLOAT2(60, 100));
    numSprite->GetSpriteTransform()->SetColorA(lvNum.alpha);

    numSprite->GetSpriteTransform()->SetPosY(400);
    RenderNum(lvIndex, 300 + lvNum.addPosX, 350 + lvNum.addPosX, 400 + lvNum.addPosX, 450 + lvNum.addPosX);
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
    int ene1 = EnemyManager::Instance().GetEnemy1KillNum();
    int ene2 = EnemyManager::Instance().GetEnemy2KillNum();
    int ene3 = EnemyManager::Instance().GetEnemy3KillNum();
    int eneG = EnemyManager::Instance().GetEnemyGolemKillNum();

    numSprite->GetSpriteTransform()->SetPosY(615);
    numSprite->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(60, 100));
    numSprite->GetSpriteTransform()->SetTexSize(DirectX::XMFLOAT2(60, 100));
    numSprite->GetSpriteTransform()->SetColorA(killNum.alpha);

    RenderNum(eneG, 210 + killNum.addPosX, 260 + killNum.addPosX, 310 + killNum.addPosX, 360 + killNum.addPosX);
    RenderNum(ene1, 490 + killNum.addPosX, 540 + killNum.addPosX, 590 + killNum.addPosX, 640 + killNum.addPosX);
    RenderNum(ene2, 780 + killNum.addPosX, 830 + killNum.addPosX, 880 + killNum.addPosX, 930 + killNum.addPosX);
    RenderNum(ene3, 1055 + killNum.addPosX, 1105 + killNum.addPosX, 1155 + killNum.addPosX, 1205 + killNum.addPosX);
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

    shader->SetBlendState(static_cast<UINT>(Shader::BLEND_STATE::ALPHA));
    resultConstants.color = golemResult.color;
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
