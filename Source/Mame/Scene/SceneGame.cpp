#include "SceneGame.h"

#include "../../Taki174/Common.h"

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

#include "../framework.h"

#include "SceneManager.h"
#include "SceneLoading.h"
#include "SceneTitle.h"

#ifdef _DEBUG
bool SceneGame::isDebugRender = false;
#endif // _DEBUG

bool SceneGame::isDrawCollision_ = false;

// リソース生成
void SceneGame::CreateResource()
{
    using DirectX::XMFLOAT3;

    Graphics& graphics = Graphics::Instance();

    // stage
    {
        stageBase = std::make_unique<Stage>();
        stageWall = std::make_unique<Stage>
            ("./Resources/Model/Stage/stageWall.fbx",
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
        EnemyGolem* enemyGolem = new EnemyGolem;
        EnemyManager::Instance().Register(enemyGolem);

#if 0
        // max 6~7
        EnemyManager& enemyManager = EnemyManager::Instance();
        // EnemyAI_1
        for (int i = 0; i < 2; ++i)
        {
            EnemyAI_1* enemyAI_1 = new EnemyAI_1;

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

    // ps Shader
    {
        CreatePsFromCso(graphics.GetDevice(), "./Resources/Shader/SagePS.cso", sagePS.GetAddressOf());

        // EMISSIVE
        D3D11_TEXTURE2D_DESC texture2dDesc;
        load_texture_from_file(graphics.GetDevice(),
            L"./Resources/Image/Mask/noise3.png",
            emissiveTexture.GetAddressOf(), &texture2dDesc);

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
    // カメラ
    Camera::Instance().Initialize();

    // player
    PlayerManager::Instance().Initialize();

    // item
    ItemManager::Instance().Initialize();

    // enemy
    EnemyManager& enemyManager = EnemyManager::Instance();
    enemyManager.Initialize();
    //enemyAura->Initialize();

    // stage
    stageBase->Initialize();
    stageWall->Initialize();


    // Exp
    ExperiencePointManager& expManager = ExperiencePointManager::Instance();
    expManager.Initialize();
<<<<<<< HEAD
=======

    //今だけロックオン処理いれとく
    Camera::Instance().SetLockOnTargetPos(enemyGolem->GetTransform());

    isDrawCollision_    = false;

>>>>>>> mame/Taki
}

// 終了化
void SceneGame::Finalize()
{
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

    if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        particles->Initialize(Graphics::Instance().GetDeviceContext(), 0);
    }

    if (integrateParticles)
    {
        particles->Integrate(Graphics::Instance().GetDeviceContext(), elapsedTime);
    }

    

    //if (gamePad.GetButtonDown() & GamePad::BTN_B)
    //    Mame::Scene::SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));

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

        Camera::Instance().UpdateDebug(elapsedTime, moveVectorFloat2);

        SetCursorPos(posX, posY);
    }
#endif // _DEBUG

    //カード演出中はほかの処理を更新しない
    auto* player = PlayerManager::Instance().GetPlayer().get();
    if (player->isSelectingSkill)
    {
        player->SelectSkillUpdate(elapsedTime);

        return;
    }

    {
        Camera::Instance().Update(elapsedTime);
    }

    // player
    PlayerManager::Instance().Update(elapsedTime);

    // item
    ItemManager::Instance().Update(elapsedTime);

    // enemy
    {
        EnemyManager& enemyManager = EnemyManager::Instance();
        enemyManager.Update(elapsedTime);
        //enemyAura->Update(elapsedTime);
    }


    // Exp
    ExperiencePointManager& expManager = ExperiencePointManager::Instance();
    expManager.Update(elapsedTime);

    // effect
    EffectManager::Instance().Update(elapsedTime);


    //カード演出中だけUpdate前にreturn呼んでるから注意！！
}

// Updateの後に呼び出される
void SceneGame::End()
{
}

// 描画処理
void SceneGame::Render(const float& elapsedTime)
{
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


    // EMISSIVE
    graphics.GetDeviceContext()->PSSetShaderResources(16, 1, emissiveTexture.GetAddressOf());

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
    shader->GSSetConstantBuffer();
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
            if (enemy->GetType() == static_cast<UINT>(Enemy::TYPE::Golem))
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
        PlayerManager::Instance().GetPlayer()->SkillImagesRender();


    }
}

// debug用
void SceneGame::DrawDebug()
{
#ifdef USE_IMGUI
    using DirectX::XMFLOAT3;

    Graphics::Instance().GetShader()->DrawDebug();

    PlayerManager& plManager = PlayerManager::Instance();
    EnemyManager& enemyManager = EnemyManager::Instance();
    ExperiencePointManager& expManager = ExperiencePointManager::Instance();


    if (ImGui::Begin("sceneGame"))
    {

        // デバッグプリミティブ描画
        if (ImGui::Button("drawDebug"))
        {
            isDebugRender = isDebugRender ? false : true;
        }

        // 当たり判定描画
        if (ImGui::Button("Draw Collision"))
        {
            isDrawCollision_ = (isDrawCollision_) ? false : true;
        }

        // 本生成
        if (ImGui::Button("createBook"))
        {
            ItemManager::Instance().Register(new Book());
        }

       

        // 経験値生成
        if (ImGui::Button("Create Exp"))
        {
            constexpr DirectX::XMFLOAT3 position = { 0,0,0 };
            const int count = ::RandInt(5, 20);
            ExperiencePointManager::Instance().CreateExp(position, count);
        }

        ImGui::Separator();

        plManager.DrawDebug();

        ItemManager::Instance().DrawDebug();

        particles->DrawDebug();

<<<<<<< HEAD
        EnemyManager& enemyManager = EnemyManager::Instance();
=======
        enemyGolem->DrawDebug();

>>>>>>> mame/Taki
        enemyManager.DrawDebug();

        // Exp
        expManager.DrawDebug();

        // カメラ
        Camera::Instance().DrawDebug();

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