#include "SceneGame.h"

#include "../../Taki174/Common.h"
#include "../../Taki174/FunctionXMFloat3.h"
#include "../../Taki174/NumeralManager.h"

#include "../Graphics/Graphics.h"
#include "../Graphics/EffectManager.h"

#include "../Input/Input.h"
#include "../Other/misc.h"
#include "../Resource/texture.h"

#include "../Game/PlayerManager.h"
#include "../Game/ItemManager.h"
#include "../Game/Book.h"
#include "../Game/AbilityManager.h"
#include "../Game/BlackHole.h"

#include "../Game/MagicCircle.h"

#include "../Game/EnemyManager.h"
#include "../Game/EnemyTestAI.h"
#include "../Game/EnemyAI_1.h"
#include "../Game/EnemyAI_2.h"
#include "../Game/EnemyAI_3.h"

#include "../Game/ProjectileManager.h"
#include "../Game/ProjectileHorming.h"

#include "../Game/ExperiencePointManager.h"
#include "../Game/UserInterface.h"
#include "../Game/WaveManager.h"
#include "../Game/SlowMotionManager.h"
#include "../Game/LightColorManager.h"

#include "../framework.h"

#include "SceneManager.h"
#include "SceneLoading.h"
#include "SceneTitle.h"
#include "SceneResult.h"


#ifdef _DEBUG
bool SceneGame::isDebugRender = false;
#endif // _DEBUG

bool SceneGame::isDispCollision_ = false;

DirectX::XMFLOAT3 SceneGame::stageCenter = {};

// リソース生成
void SceneGame::CreateResource()
{
    using DirectX::XMFLOAT3;
    using std::make_unique;

    Graphics& graphics = Graphics::Instance();

    // ロード用のダミー
    enemyGolem = std::make_unique<EnemyGolem>();

    // stage
    {
        stageBase = std::make_unique<Stage>();
        stageWall = std::make_unique<Stage>
            //("./Resources/Model/Stage/hashira.fbx",
            ("./Resources/Model/Stage/stageWall.fbx",
                //"./Resources/Shader/CharacterPS.cso");
                "./Resources/Shader/StageWallPS.cso");
    }


    // player
    {
        PlayerManager::Instance().GetPlayer() = std::make_unique<Player>();
    }

    // item
    {
        //ItemManager::Instance().Register(new Book());
        //ItemManager::Instance().Register(new MagicCircle());
    }

    // enemy
    {
#if 0

        EnemyManager& enemyManager = EnemyManager::Instance();
    //EnemyGolem* enemyGolem = new EnemyGolem;
    //enemyGolem->Initialize();
    //enemyGolem->SetHealth(20);
    //enemyManager.Register(enemyGolem);
    //    EnemyManager& enemyManager = EnemyManager::Instance();
    //    EnemyGolem* enemyGolem = new EnemyGolem;
    //    enemyGolem->Initialize();
    //    enemyManager.Register(enemyGolem);

        // EnemyGolem* enemyGolem = new EnemyGolem;
        // enemyManager.Register(enemyGolem);



        // max 6~7
        // EnemyAI_1
        for (int i = 0; i < 2; ++i)
        {
            EnemyAI_1* enemyAI_1 = new EnemyAI_1;
            enemyAI_1->Initialize();
            const XMFLOAT3 setPosition = {
                ::RandFloat(-10.0f, +10.0f),
                0.0f,
                ::RandFloat(-10.0f, +10.0f)
            };
            enemyAI_1->SetPosition(setPosition);

            enemyManager.Register(enemyAI_1);
        }
        // EnemyAI_2
        for (int i = 0; i < 2; ++i)
        {
            EnemyAI_2* enemyAI_2 = new EnemyAI_2;
            enemyAI_2->Initialize();

            const XMFLOAT3 setPosition = {
                ::RandFloat(-10.0f, +10.0f),
                0.0f,
                ::RandFloat(-10.0f, +10.0f)
            };
            enemyAI_2->SetPosition(setPosition);

            enemyManager.Register(enemyAI_2);
        }
        // EnemyAI_3
        for (int i = 0; i < 2; ++i)
        {
            EnemyAI_3* enemyAI_3 = new EnemyAI_3;
            enemyAI_3->Initialize();

            const XMFLOAT3 setPosition = {
                ::RandFloat(-10.0f, +10.0f),
                0.0f,
                ::RandFloat(-10.0f, +10.0f)
            };
            enemyAI_3->SetPosition(setPosition);

            enemyManager.Register(enemyAI_3);
        }
#endif
    }

    // sprite
    {
        whiteSprite = std::make_unique<Sprite>(graphics.GetDevice(),
            L"./Resources/Image/Game/white.png");
    }

    // ps Shader
    {
        CreatePsFromCso(graphics.GetDevice(), "./Resources/Shader/SagePS.cso", sagePS.GetAddressOf());

        // EMISSIVE
        D3D11_TEXTURE2D_DESC texture2dDesc;
        load_texture_from_file(graphics.GetDevice(),
            L"./Resources/Image/Mask/noise3.png",
            emissiveTexture[0].GetAddressOf(), &texture2dDesc);
        load_texture_from_file(graphics.GetDevice(),
            L"./Resources/Image/Mask/noise10.png",
            emissiveTexture[1].GetAddressOf(), &texture2dDesc);

        load_texture_from_file(graphics.GetDevice(),
            L"./Resources/Image/Particle/circle.png",
            particleTexture.GetAddressOf(), &texture2dDesc);

        //CreatePsFromCso(graphics.GetDevice(),"./Resources/Shader/EmissiveTextureUVScrollPS.cso",emissiveTexture)
    }


    // particle
    {
        particles = std::make_unique<decltype(particles)::element_type>(graphics.GetDevice(), 1000);
    }

    framebuffers[0] = std::make_unique<FrameBuffer>(graphics.GetDevice(), 1280, 720);
    bitBlockTransfer = std::make_unique<FullscreenQuad>(graphics.GetDevice());

    // baseColor : finalPass
    //CreatePsFromCso(graphics.GetDevice(), "./Resources/Shader/finalPassPs.cso", colorPS.GetAddressOf());


    // bloom
    {
        bloomer = std::make_unique<Bloom>(graphics.GetDevice(), 1280 / 1, 720 / 1);
        CreatePsFromCso(graphics.GetDevice(), "./Resources/Shader/finalPassPs.cso", bloomPS.GetAddressOf());
    }

    // fog
    {
        framebuffers[1] = std::make_unique<FrameBuffer>(graphics.GetDevice(), 1280 / 1, 720 / 1, false);
        CreatePsFromCso(graphics.GetDevice(), "./Resources/Shader/FogPS.cso", fogPS.GetAddressOf());
    }

    // bokeh
    CreatePsFromCso(graphics.GetDevice(), "./Resources/Shader/FinalPassBokehPS.cso", bokehPS.GetAddressOf());

    // shadow
    {
        shadow.shadowMap = std::make_unique<ShadowMap>(graphics.GetDevice(),
            shadow.shadowMapWidth, shadow.shadowMapHeight);

        HRESULT hr{ S_OK };
        D3D11_BUFFER_DESC desc{};
        desc.ByteWidth = sizeof(Shader::SceneConstants);
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;
        hr = graphics.GetDevice()->CreateBuffer(&desc, nullptr, shadowConstantBuffer.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    }

    // effect
    {
        effect[0] = new Effect("./Resources/Effect/explosion4.efk");
        //effect[0] = std::make_unique<Effect>("./Resources/Effect/explosion4.efk");
    }

    // 定数バッファー
    {
        HRESULT hr{ S_OK };

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

// 初期化
void SceneGame::Initialize()
{
    // player
    PlayerManager::Instance().Initialize();

    // カメラ
    Camera::Instance().Initialize();

    //ライト、ビネット
    LightColorManager::Instance().Initialize();

    // item
    ItemManager::Instance().Initialize();

    // enemy
    EnemyManager& enemyManager = EnemyManager::Instance();
    enemyManager.Initialize();
    //enemyAura->Initialize();

    // stage
    stageBase->Initialize();
    stageCenter = stageBase->GetTransform()->GetPosition();
    stageWall->Initialize();

    // Exp
    ExperiencePointManager& expManager = ExperiencePointManager::Instance();
    expManager.Initialize();



    //今だけロックオン処理いれとく
    //Camera::Instance().SetLockOnTargetPos(enemyGolem->GetTransform());

    isDispCollision_    = false;

    //isDrawCollision_    = false;

    //UI
    {
        // Numeral
        NumeralManager& numeralManager = NumeralManager::Instance();
        numeralManager.Initialize();

        UserInterface::Instance().Initialize();
    }


    isParticleInitialize = false; // particle用
    isWhiteSpriteRender = true;
    whiteSpriteTimer = 0.0f;

    // Wave Initialize
    {
        WaveManager& waveManager = WaveManager::Instance();

        // ウェーブ初期化（ウェーブ番号を指定すると好きなウェーブから開始できる（※デバッグ時のみ有効））
        static constexpr int startWaveIndex = -1;
        waveManager.InitWave(startWaveIndex);
    }

    // ExecuteSlowMotion Initialize
    SlowMotionManager::Instance().Initialize();

}

// 終了化
void SceneGame::Finalize()
{
    // Numeral
    NumeralManager& numeralManager = NumeralManager::Instance();
    numeralManager.Finalize();

    // Exp
    ExperiencePointManager& expManager = ExperiencePointManager::Instance();
    expManager.Finalize();

    EnemyManager& enemyManager = EnemyManager::Instance();
    enemyManager.Finalize();

    ItemManager::Instance().Finalize();

    PlayerManager::Instance().Finalize();

    // effect
    {
        for (int i = 0; i < 4; ++i)
        {
            if (effect[i])
            {
                effect[i]->Stop(effect[i]->handle);
                delete effect[i];
                effect[i] = nullptr;
            }
        }
    }
}

// Updateの前に呼び出される
void SceneGame::Begin()
{
}

// 更新処理
void SceneGame::Update(const float& elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    // 最初の白飛びのスプライト
    UpdateWhiteSprite(elapsedTime);

    //カード演出中はほかの処理を更新しない
    Player* player = PlayerManager::Instance().GetPlayer().get();
    if (player->isSelectingSkill)
    {
        player->SelectSkillUpdate(elapsedTime);
        return;
    }

    // スローモーション更新
    SlowMotionManager& slowMotion = SlowMotionManager::Instance();
    slowMotion.Update(elapsedTime);

    // スローモーションを適用した経過時間
    const float slowMotionElapsedTime = {
        (true == slowMotion.GetSlowMotionFlag())
        ? elapsedTime * slowMotion.GetCurrentPercentage()
        : elapsedTime
    };

    //ライト、ビネット更新
    LightColorManager::Instance().Update(elapsedTime);

    // 最初の白飛びのスプライト
    UpdateWhiteSprite(slowMotionElapsedTime);

    if (!isParticleInitialize)
    {
        isParticleInitialize = false;
        particles->Initialize(Graphics::Instance().GetDeviceContext(), 0);
    }

    if (integrateParticles)
    {
        particles->Integrate(Graphics::Instance().GetDeviceContext(), slowMotionElapsedTime);
    }

    if (GetAsyncKeyState('P') & 0x01)
    {
        //Mame::Scene::SceneManager::Instance().ChangeScene(new SceneResult);
        Mame::Scene::SceneManager::Instance().ChangeScene(new SceneLoading(new SceneResult));
        return;
    }
#ifdef _DEBUG
#endif

#ifdef _DEBUG
    // Debug用カメラ
    if (/*gamePad.GetButtonDown() & GamePad::BTN_X*/GetAsyncKeyState('U') & 0x01)isDebugCamera = isDebugCamera ? false : true;
    if (isDebugCamera)
    {
        int posX = 1980 / 2;
        int posY = 1080 / 2;

        POINT pos;
        GetCursorPos(&pos);

        DirectX::XMFLOAT2 nowPosition{ static_cast<float>(pos.x),static_cast<float>(pos.y) };
        DirectX::XMFLOAT2 oldPosition{ static_cast<float>(posX),static_cast<float>(posY) };
        DirectX::XMVECTOR nowVector = DirectX::XMLoadFloat2(&nowPosition);
        DirectX::XMVECTOR oldVector = DirectX::XMLoadFloat2(&oldPosition);
        DirectX::XMVECTOR moveVector = DirectX::XMVectorSubtract(nowVector, oldVector);
        DirectX::XMFLOAT2 moveVectorFloat2;
        DirectX::XMStoreFloat2(&moveVectorFloat2, moveVector);

        Camera::Instance().UpdateDebug(slowMotionElapsedTime, moveVectorFloat2);

        SetCursorPos(posX, posY);
    }
#endif // _DEBUG

    {
        Camera::Instance().Update(slowMotionElapsedTime);
    }

    // player
    PlayerManager::Instance().Update(slowMotionElapsedTime);

    // item
    ItemManager::Instance().Update(slowMotionElapsedTime);

    // enemy
    {
        EnemyManager& enemyManager = EnemyManager::Instance();
        enemyManager.Update(slowMotionElapsedTime);
        //enemyAura->Update(elapsedTime);
    }

    // Exp
    ExperiencePointManager& expManager = ExperiencePointManager::Instance();
    expManager.Update(slowMotionElapsedTime);

    // effect
    EffectManager::Instance().Update(slowMotionElapsedTime);

    // UI
    {
        // Numeral
        NumeralManager& numeralManager = NumeralManager::Instance();
        numeralManager.Update(slowMotionElapsedTime);

        UserInterface::Instance().Update(slowMotionElapsedTime);
    }

    // Wave Update
    {
        WaveManager& waveManager = WaveManager::Instance();

        // ウェーブ更新
        waveManager.UpdateWave(slowMotionElapsedTime);
    }

    //カード演出中だけUpdate前にreturn呼んでるから注意！！
}

// Updateの後に呼び出される
void SceneGame::End()
{
}

// 描画処理
void SceneGame::Render(const float& /*elapsedTime*/)
{
    using DirectX::XMFLOAT3;
    using DirectX::XMFLOAT4;

    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();

    Shader::SceneConstants sceneConstants{};


    float playerScaleFactor = 0.01f;
    float enemyScaleFactor = 0.01f;

    // 描画の初期設定
    {
        // 描画の初期設定※必ず呼ぶこと！！！
        Mame::Scene::BaseScene::RenderInitialize();


        // SHADOW : make shadow map
        {
            const float aspectRatio = shadow.shadowMap->viewport.Width / shadow.shadowMap->viewport.Height;
            DirectX::XMVECTOR F =
            {
                DirectX::XMLoadFloat4(&shadow.lightViewFocus)
            };
            DirectX::XMVECTOR E =
            {
                DirectX::XMVectorSubtract(F,
                DirectX::XMVectorScale(
                    DirectX::XMVector3Normalize(
                        DirectX::XMLoadFloat4(&graphics.GetShader()->view.position)),shadow.lightViewDistance))
            };
            DirectX::XMVECTOR U =
            {
                DirectX::XMVectorSet(0.0f,1.0f,0.0f,0.0f)
            };
            DirectX::XMMATRIX V =
            {
                DirectX::XMMatrixLookAtLH(E,F,U)
            };
            DirectX::XMMATRIX P =
            {
                DirectX::XMMatrixOrthographicLH(shadow.lightViewSize * aspectRatio,
                shadow.lightViewSize,shadow.lightViewNearZ,shadow.lightViewFarZ)
            };

            DirectX::XMStoreFloat4x4(&sceneConstants.viewProjection, V * P);
            sceneConstants.lightViewProjection = sceneConstants.viewProjection;
            graphics.GetDeviceContext()->UpdateSubresource(shadowConstantBuffer.Get(), 0, 0, &sceneConstants, 0, 0);
            graphics.GetDeviceContext()->VSSetConstantBuffers(1, 1, shadowConstantBuffer.GetAddressOf());

            shadow.shadowMap->Clear(graphics.GetDeviceContext(), 1.0f);
            shadow.shadowMap->Activate(graphics.GetDeviceContext());

            // ステートセット
            shader->SetDepthStencileState(static_cast<UINT>(Shader::DEPTH_STATE::ZT_ON_ZW_ON));
            shader->SetBlendState(static_cast<UINT>(Shader::BLEND_STATE::NONE));
            shader->SetRasterizerState(static_cast<UINT>(Shader::RASTER_STATE::SOLID));

            // SHADOW : 影つけたいモデルはここにRenderする
            {
                PlayerManager::Instance().Render(playerScaleFactor);

                EnemyManager& enemyManager = EnemyManager::Instance();
                enemyManager.RenderShadow(enemyScaleFactor);


            }

            shadow.shadowMap->Deactivete(graphics.GetDeviceContext());
        }
    }

    Camera& camera = Camera::Instance();

    shader->SetDepthStencileState(static_cast<UINT>(Shader::DEPTH_STATE::ZT_ON_ZW_ON));
    shader->SetBlendState(static_cast<UINT>(Shader::BLEND_STATE::NONE));
    shader->SetRasterizerState(static_cast<UINT>(Shader::RASTER_STATE::SOLID));

    camera.SetPerspectiveFov(graphics.GetDeviceContext());
    DirectX::XMStoreFloat4x4(&sceneConstants.viewProjection, camera.GetViewMatrix() * camera.GetProjectionMatrix());
    sceneConstants.lightDirection = shader->GetViewPosition();
    sceneConstants.cameraPosition = shader->GetViewCamera();

    DirectX::XMStoreFloat4x4(&sceneConstants.inverseProjection, DirectX::XMMatrixInverse(NULL, camera.GetProjectionMatrix()));
    DirectX::XMStoreFloat4x4(&sceneConstants.inverseViewProjection, DirectX::XMMatrixInverse(NULL, camera.GetViewMatrix() * camera.GetProjectionMatrix()));
    sceneConstants.time = framework::tictoc.time_stamp();

    graphics.GetDeviceContext()->UpdateSubresource(ConstantBuffer.Get(), 0, 0, &sceneConstants, 0, 0);
    graphics.GetDeviceContext()->VSSetConstantBuffers(1, 1, ConstantBuffer.GetAddressOf());
    graphics.GetDeviceContext()->PSSetConstantBuffers(1, 1, ConstantBuffer.GetAddressOf());

    {
        // カメラ関係
        RenderContext rc = {};
        rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };

        shader->Begin(graphics.GetDeviceContext(), rc);
    }

    {
        // EMISSIVE
        ID3D11ShaderResourceView* shaderResourceViews[] =
        {
            emissiveTexture[0].Get(),
            emissiveTexture[1].Get(),
        };
        graphics.GetDeviceContext()->PSSetShaderResources(16, _countof(shaderResourceViews), shaderResourceViews);
    }

    // SHADOW : bind shadow map at slot 8
    graphics.GetDeviceContext()->PSSetShaderResources(8, 1, shadow.shadowMap->shaderResourceView.GetAddressOf());

    framebuffers[0]->Clear(graphics.GetDeviceContext());
    framebuffers[0]->Activate(graphics.GetDeviceContext());


    // モデル描画
    {
        // stage
        {
            stageBase->Render(0.01f);
            stageWall->Render(0.01f);
        }

        // player
        {
            PlayerManager::Instance().Render(playerScaleFactor);
        }

        // item
        {
            ItemManager::Instance().Render(0.01f);
        }

        // enemy
        {
            EnemyManager& enemyManager = EnemyManager::Instance();
            enemyManager.Render(enemyScaleFactor);
        }

        // Exp
        ExperiencePointManager& expManager = ExperiencePointManager::Instance();
        expManager.Render(1.0f);

    }

    shader->SetDepthStencileState(static_cast<size_t>(Shader::DEPTH_STATE::ZT_ON_ZW_ON));
    shader->SetRasterizerState(static_cast<size_t>(Shader::RASTER_STATE::CULL_NONE));
    shader->SetBlendState(static_cast<size_t>(Shader::BLEND_STATE::ADD));
    graphics.GetDeviceContext()->GSSetConstantBuffers(1, 1, ConstantBuffer.GetAddressOf());
    graphics.GetDeviceContext()->PSSetShaderResources(16, 1, particleTexture.GetAddressOf());
    particles->Render(graphics.GetDeviceContext());

    // シェーダーエフェクト
    {
#if 0
        auraEmitters.clear();
        auraEmitters.push_back({ enemySlime[0]->model->skinned_meshes->DetectJointPosition("PLT:Shoulder_R_BK",&enemySlime[0]->model->keyframe),0 });
        auraEmitters.push_back({ enemySlime[0]->model->skinned_meshes->DetectJointPosition("PLT:Elbow_L_BK",&enemySlime[0]->model->keyframe),1 });
        auraEmitters.push_back({ enemySlime[0]->model->skinned_meshes->DetectJointPosition("PLT:branch4_R_BK",&enemySlime[0]->model->keyframe),2 });
        auraEmitters.push_back({ enemySlime[0]->model->skinned_meshes->DetectJointPosition("PLT:Wrist_R_BK",&enemySlime[0]->model->keyframe),3 });
        auraEmitters.push_back({ enemySlime[0]->model->skinned_meshes->DetectJointPosition("PLT:ThumbFinger2_L_BK",&enemySlime[0]->model->keyframe),4 });
        {
            framebuffers[2]->Clear(graphics.GetDeviceContext(), 0, 0, 0, 0);
            framebuffers[2]->Activate(graphics.GetDeviceContext());
            shader->SetDepthStencileState(static_cast<UINT>(Shader::DEPTH_STATE::ZT_OFF_ZW_OFF));
            shader->SetRasterizerState(static_cast<UINT>(Shader::RASTER_STATE::CULL_NONE));
            bitBlockTransfer->Blit(graphics.GetDeviceContext(), NULL, 0, 0, auraEffectPS.Get());
            framebuffers[2]->Deactivate(graphics.GetDeviceContext());
        }
        shader->SetBlendState(static_cast<UINT>(Shader::BLEND_STATE::ADD));
        shader->SetDepthStencileState(static_cast<UINT>(Shader::DEPTH_STATE::ZT_ON_ZW_OFF));
        shader->SetRasterizerState(static_cast<UINT>(Shader::RASTER_STATE::CULL_NONE));
        DirectX::XMFLOAT4X4 world;
        DirectX::XMStoreFloat4x4(&world, enemySlime[0]->model->GetTransform()->CalcWorldMatrix(enemyScaleFactor));
        auraEffect->Render(graphics.GetDeviceContext(), auraEmitters, world, framebuffers[2]->shaderResourceViews[0].Get());
#endif
    }

    // 3Dエフェクト描画
    {
        Camera& camera = Camera::Instance();
        DirectX::XMFLOAT4X4 view, projection;
        DirectX::XMStoreFloat4x4(&view, camera.GetViewMatrix());
        DirectX::XMStoreFloat4x4(&projection, camera.GetProjectionMatrix());

        EffectManager::Instance().Render(view, projection);
    }

    //ブルームあり２D
    {
        // Numeral
        NumeralManager& numeralManager = NumeralManager::Instance();
        numeralManager.Render();

        // スキル選択中
        auto* player = PlayerManager::Instance().GetPlayer().get();
        if (player->isSelectingSkill)
        {
            PlayerManager::Instance().SkillImagesBloomRender();
        }

        UserInterface::Instance().BloomRender();
    }

    framebuffers[0]->Deactivate(graphics.GetDeviceContext());


    // FOG
    {
        framebuffers[1]->Clear(graphics.GetDeviceContext());
        framebuffers[1]->Activate(graphics.GetDeviceContext());
        shader->SetDepthStencileState(static_cast<UINT>(Shader::DEPTH_STATE::ZT_OFF_ZW_OFF));
        shader->SetRasterizerState(static_cast<UINT>(Shader::RASTER_STATE::CULL_NONE));
        shader->SetBlendState(static_cast<UINT>(Shader::BLEND_STATE::NONE));

        bitBlockTransfer->Blit(graphics.GetDeviceContext(), framebuffers[0]->shaderResourceViews[1].GetAddressOf()/*Depth*/, 0, 1, fogPS.Get());

        framebuffers[1]->Deactivate(graphics.GetDeviceContext());

        shader->SetDepthStencileState(static_cast<UINT>(Shader::DEPTH_STATE::ZT_OFF_ZW_OFF));
        shader->SetRasterizerState(static_cast<UINT>(Shader::RASTER_STATE::CULL_NONE));
        ID3D11ShaderResourceView* shaderResourceView[]
        {
            framebuffers[0]->shaderResourceViews[0].Get(),
            framebuffers[0]->shaderResourceViews[0].Get(),/*dummy*/
            framebuffers[1]->shaderResourceViews[0].Get(),
        };
        //bitBlockTransfer->Blit(graphics.GetDeviceContext(), shaderResourceView, 0, _countof(shaderResourceView), colorPS.Get());
    }

    // BLOOM
    bloomer->Make(graphics.GetDeviceContext(), framebuffers[0]->shaderResourceViews[0].Get());

    shader->SetDepthStencileState(static_cast<UINT>(Shader::DEPTH_STATE::ZT_OFF_ZW_OFF));
    shader->SetRasterizerState(static_cast<UINT>(Shader::RASTER_STATE::CULL_NONE));
    shader->SetBlendState(static_cast<UINT>(Shader::BLEND_STATE::ALPHA));

    ID3D11ShaderResourceView* shaderResourceViews[] =
    {
        framebuffers[0]->shaderResourceViews[0].Get(),
        bloomer->ShaderResourceView(),
        framebuffers[1]->shaderResourceViews[0].Get(),
        framebuffers[0]->shaderResourceViews[1].Get(),
    };

    // Golemによるボケ処理
    {
        Enemy* enemyGolem = nullptr;
        bool isBokeh = false;
        for (Enemy* enemy : EnemyManager::Instance().GetEnemies())
        {
            if (enemy->GetType() == Enemy::TYPE::Golem)
            {
                enemyGolem = enemy;
                if (enemyGolem->GetCurrentState() == static_cast<UINT>(EnemyGolem::StateMachineState::RoarState))
                    isBokeh = true;
            }
        }

        if (isBokeh)
        {
            bitBlockTransfer->Blit(graphics.GetDeviceContext(), shaderResourceViews, 0, _countof(shaderResourceViews), bokehPS.Get());
        }
        else
        {
            bitBlockTransfer->Blit(graphics.GetDeviceContext(), shaderResourceViews, 0, _countof(shaderResourceViews), bloomPS.Get());
        }
    }

    //ブルーム無し
    {
        // Numeral
        NumeralManager& numeralManager = NumeralManager::Instance();
        numeralManager.Render();

        UserInterface::Instance().Render();
        PlayerManager::Instance().SkillImagesRender();

        // ※これより下に何も描画しない
        if(isWhiteSpriteRender) whiteSprite->Render();
        // ※これより下に何も描画しない
    }

#ifdef _DEBUG
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    // ステージの範囲デバッグ描画
    if (true == isDispCollision_)
    {
        static constexpr float    height = 0.5f;
        static constexpr XMFLOAT4 color  = { 1,1,1,1 };
        debugRenderer->DrawCylinder(stageCenter, stageRadius, height, color);
    }

    // デバッグレンダラ描画
    DirectX::XMFLOAT4X4 view, projection;
    DirectX::XMStoreFloat4x4(&view, camera.GetViewMatrix());
    DirectX::XMStoreFloat4x4(&projection, camera.GetProjectionMatrix());

    debugRenderer->Render(graphics.GetDeviceContext(), view, projection);
#endif // _DEBUG

}

// debug用
void SceneGame::DrawDebug()
{
#ifdef USE_IMGUI
    using DirectX::XMFLOAT3;

    Graphics::Instance().GetShader()->DrawDebug();

    PlayerManager&          plManager    = PlayerManager::Instance();
    EnemyManager&           enemyManager = EnemyManager::Instance();
    ExperiencePointManager& expManager   = ExperiencePointManager::Instance();

    if (ImGui::Begin("sceneGame"))
    {
        Graphics::Instance().GetShader()->DrawDebug();

        // デバッグプリミティブ描画
        if (ImGui::Button("drawDebug"))
        {
            isDebugRender = isDebugRender ? false : true;
        }

        // 当たり判定描画
        if (ImGui::Button("Disp Collision"))
        {
            isDispCollision_ = (isDispCollision_) ? false : true;
        }

        // 本生成
        if (ImGui::Button("createBook"))
        {
            Book* book = new Book();
            book->Initialize();
            ItemManager::Instance().Register(book);
        }

        // 経験値生成
        if (ImGui::Button("Create Exp"))
        {
            constexpr DirectX::XMFLOAT3 position = { 0,0,0 };
            const int count = ::RandInt(5, 20);
            ExperiencePointManager::Instance().CreateExp(position, count);
        }

        // スローモーション
        if (ImGui::Button("Slow Motion"))
        {
#if 0
            const float endTime          = 1.0f;
            const float targetPercentage = 0.5f;
            const float easeOutTime      = 0.0f;
            const float easeInTime       = 1.0f;
            SlowMotionManager::Instance().ExecuteSlowMotion(
                endTime, targetPercentage,
                easeOutTime, easeInTime
            );
#else
            SlowMotionManager::Instance().ExecuteSlowMotion();
#endif
        }

        ImGui::Separator();

        // ゲートから敵を生成する
        DebugCreateEnemyFromGateway();

        ImGui::Separator();

        stageBase->DrawDebug();
        stageWall->DrawDebug();

        plManager.DrawDebug();

        ItemManager::Instance().DrawDebug();

        particles->DrawDebug();


        EnemyManager& enemyManager = EnemyManager::Instance();
        enemyManager.DrawDebug();

        // Exp
        expManager.DrawDebug();

        // カメラ
        Camera::Instance().DrawDebug();

        // Numeral
        NumeralManager& numeralManager = NumeralManager::Instance();
        numeralManager.DrawDebug();

        UserInterface::Instance().DrawDebug();

        // Wave DrawDebug
        {
            WaveManager& waveManager = WaveManager::Instance();

            // ウェーブデバッグ描画
            waveManager.DrawDebug();
        }

        // SlowMotion DrawDebug
        SlowMotionManager::Instance().DrawDebug();


        LightColorManager::Instance().DrawDebug();

        ImGui::End();
    }

    // SHADOW
    {
        if (ImGui::Begin("shadow"))
        {
            ImGui::DragFloat4("lightViewFocus", &shadow.lightViewFocus.x);
            ImGui::SliderFloat("lightViewDistance", &shadow.lightViewDistance, 1.0f, 100.0f);
            ImGui::SliderFloat("lightViewSize", &shadow.lightViewSize, 1.0f, 100.0f);
            ImGui::SliderFloat("lightViewNearZ", &shadow.lightViewNearZ, 1.0f, shadow.lightViewFarZ - 1.0f);
            ImGui::SliderFloat("lightViewFarZ", &shadow.lightViewFarZ, shadow.lightViewNearZ + 1.0f, 100.0f);
            ImGui::Image(reinterpret_cast<void*>(shadow.shadowMap->shaderResourceView.Get()), ImVec2(shadow.shadowMapWidth / 5.0f, shadow.shadowMapHeight / 5.0f));
            ImGui::End();
        }
    }

#endif
}


// ゲートから敵を生成する関数(デバッグ用)
void SceneGame::DebugCreateEnemyFromGateway()
{
    using DirectX::XMFLOAT3;

    EnemyManager& enemyManager = EnemyManager::Instance();

    static int gatewayIndex = -1;    // 敵を出現させるゲートの番号(-1でランダム)

    // 敵を出現させるゲートの指定(-1ならランダムで番号を決める）
    ImGui::SliderInt(
        "gateWayIndex(-1 is RandomSpawn)",
        &gatewayIndex, -1, (GATEWAY_COUNT_ - 1)
    );

    // ゲートから敵を生成
    if (ImGui::Button("CreateEnemyFromGateway"))
    {
        // 360度をゲート数分に等分したときの角度
        static constexpr float angle = 360.0f / static_cast<float>(GATEWAY_COUNT_);

        // Y回転値テーブルを作成
        static float rotationY_table[GATEWAY_COUNT_] = {};      // Y回転値テーブル(static)
        static bool  createRotationY_TableFlag       = false;   // Y回転値テーブル作成フラグ(static)
        if (false == createRotationY_TableFlag)
        {
            // Y回転値テーブルを作成
            for (int i = 0; i < SceneGame::GATEWAY_COUNT_; ++i)
            {
                rotationY_table[i] = ::ToRadian(angle * static_cast<float>(i));
            }

            // Y回転値テーブルを作成した(フラグON)
            createRotationY_TableFlag = true;
        }

        // Y回転値を取得
        const int   rotationY_index = (gatewayIndex > -1) ? gatewayIndex: ::RandInt(0, GATEWAY_COUNT_);
        const float rotationY       = rotationY_table[rotationY_index];

        // ステージの中心からゲートの奥ぐらいまでの半径
        static constexpr float radius = 35.0f;

        // 生成位置設定(XZ平面)
        XMFLOAT3 createPos = {};
        createPos.x = stageCenter.x + ::sinf(rotationY) * radius;
        createPos.z = stageCenter.z + ::cosf(rotationY) * radius;

        // 敵生成(EnemyAI_1)
        EnemyAI_1* enemyAI_1 = new EnemyAI_1();
        enemyAI_1->Initialize();
        enemyAI_1->GetTransform()->SetPosition(createPos);
        enemyManager.Register(enemyAI_1);

    }

}

void SceneGame::UpdateWhiteSprite(const float& elapsedTime)
{
    if (isWhiteSpriteRender)
    {
        float maxTime = 1.0f;
        if (whiteSpriteTimer <= maxTime)
        {
            float alpha = Easing::InSine(whiteSpriteTimer, maxTime, 0.0f, 1.0f);
            whiteSprite->GetSpriteTransform()->SetColorA(alpha);
            whiteSpriteTimer += elapsedTime;
        }
        else
        {
            isWhiteSpriteRender = false;
        }
    }
}
