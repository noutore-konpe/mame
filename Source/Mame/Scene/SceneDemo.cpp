#include "SceneDemo.h"

#include "../Other/misc.h"

#include "../Graphics/Graphics.h"
#include "../Resource/texture.h"
#include "../Input/Input.h"

#include "../Graphics/EffectManager.h"

#include "../Game/Collision.h"

#include "../Game/PlayerManager.h"
#include "../Game/ItemManager.h"
#include "../Game/Book.h"
#include "../Game/ProjectileManager.h"

bool SceneDemo::isDebugRender = true;

// リソース生成
void SceneDemo::CreateResource()
{
    Graphics& graphics = Graphics::Instance();

    // GltfModel
    {
#if GLTF_MODEL
        gltfModels[0] = std::make_unique<GltfModel>(graphics.GetDevice(),
            //"./External/glTF-Sample-Models-master/2.0/ToyCar/glTF/ToyCar.gltf");
            //"./External/glTF-Sample-Models-master/2.0/ABeautifulGame/glTF/ABeautifulGame.gltf");
            //"./External/glTF-Sample-Models-master/2.0/BrainStem/glTF/BrainStem.gltf");
            //"./External/glTF-Sample-Models-master/2.0/CesiumMan/glTF/CesiumMan.gltf");
            "./External/glTF-Sample-Models-master/2.0/DamagedHelmet/glTF/DamagedHelmet.gltf");
            //"./External/glTF-Sample-Models-master/2.0/2CylinderEngine/glTF/2CylinderEngine.gltf");
            //"./Resources/tree.gltf");
            //"./Resources/Model/misuzuModel/ie.gltf");

        D3D11_TEXTURE2D_DESC texture2dDesc;
        load_texture_from_file(graphics.GetDevice(), L"./Resources/Image/environments/sunset_jhbcentral_4k/sunset_jhbcentral_4k.dds",
            shaderResourceViews[0].GetAddressOf(), &texture2dDesc);
        load_texture_from_file(graphics.GetDevice(), L"./Resources/Image/environments/sunset_jhbcentral_4k/diffuse_iem.dds",
            shaderResourceViews[1].GetAddressOf(), &texture2dDesc);
        load_texture_from_file(graphics.GetDevice(), L"./Resources/Image/environments/sunset_jhbcentral_4k/specular_pmrem.dds",
            shaderResourceViews[2].GetAddressOf(), &texture2dDesc);
        load_texture_from_file(graphics.GetDevice(), L"./Resources/Image/environments/lut_ggx.dds",
            shaderResourceViews[3].GetAddressOf(), &texture2dDesc);

        //gltfModels[0]->GetTransform()->SetScale(DirectX::XMFLOAT3(10.0f, 10.0f, 10.0f));
#endif // GLTF_MODEL
    }

    // Model
    {
#if MODEL

        model = std::make_unique<Model>(graphics.GetDevice(),
            //"./Resources/tree.fbx");
            //"./Resources/Model/sanaModel/1.fbx");
            //"./Resources/Model/sanaModel/mameoall.fbx");
            //"./Resources/Model/Collision/sphere.fbx");
            "./Resources/Model/testModel/latha.fbx");
#endif // MODEL
    }

    // Sprite
    {
#if SPRITE
        sprite = std::make_unique<Sprite>(graphics.GetDevice(),
            //L"./Resources/Image/sanaImage/hipDrop.png",
            L"./Resources/Image/sanaImage/load.png",
            "./Resources/Shader/sprite_dissolve_ps.cso");
        //sprite->Initialize(DirectX::XMFLOAT2(0, 0), DirectX::XMFLOAT2(1280, 720), DirectX::XMFLOAT2(1280, 720));
        //sprite->Initialize(DirectX::XMFLOAT2(0, 0), DirectX::XMFLOAT2(1280, 720), DirectX::XMFLOAT2(1452, 771));
        //sprite->Initialize(DirectX::XMFLOAT2(0, 0), DirectX::XMFLOAT2(900, 367), DirectX::XMFLOAT2(900, 367));
#endif // SPRITE
    }

    // Effect
    {
        //effect[0] = std::make_unique<Effect>("./Resources/Effect/ringz.efk");
        effect[0] = std::make_unique<Effect>("./Resources/Effect/sample/lightningv002.efk");
        //effect[0] = std::make_unique<Effect>("./Resources/Effect/ring2.efk");
        //effect[0] = std::make_unique<Effect>("./Resources/Effect/old/ring.efk");
    }

    // slime
    {
        enemySlime[0] = std::make_unique<EnemySlime>();
        enemySlime[1] = std::make_unique<EnemySlime>();
    }

    // player
    {
        PlayerManager::Instance().GetPlayer() = std::make_unique<Player>();
    }

    // item
    {
        ItemManager::Instance().Register(new Book());
    }

    // stage
    {
        stage = std::make_unique<Stage>();
    }

    // skybox
    {
#if SKYBOX
        skyBoxSprite = std::make_shared<Sprite>(graphics.GetDevice(),
            //L"./Resources/Image/SkyBox/skybox0.jpg");
            //L"./Resources/Image/SkyBox/skybox1.png");
            //L"./Resources/Image/SkyBox/skybox2.jpg");
            L"./Resources/Image/SkyBox/skybox3.jpg");
        skyBox = std::make_unique<SkyBox>(graphics.GetDevice(), skyBoxSprite);
#endif // SKYBOX
    }


    framebuffers[0] = std::make_unique<FrameBuffer>(graphics.GetDevice(), 1280, 720);
    bitBlockTransfer = std::make_unique<FullscreenQuad>(graphics.GetDevice());

    // baseColor
    CreatePsFromCso(graphics.GetDevice(), "./Resources/Shader/finalPassPs.cso", baseColorPS.GetAddressOf());

    // BLOOM
#if BLOOM
    bloomer = std::make_unique<Bloom>(graphics.GetDevice(), 1280 / 1, 720 / 1);
    CreatePsFromCso(graphics.GetDevice(), "./Resources/Shader/finalPassPs.cso", bloomPS.ReleaseAndGetAddressOf());
#endif // BLOOM

#if FOG
    framebuffers[1] = std::make_unique<FrameBuffer>(graphics.GetDevice(), 1280 / 1, 720 / 1, false);
    CreatePsFromCso(graphics.GetDevice(), "./Resources/Shader/FogPS.cso", fogPS.GetAddressOf());
#endif

    // SHADOW
    shadow.shadowMap = std::make_unique<ShadowMap>(graphics.GetDevice(),
        shadow.shadowMapWidth, shadow.shadowMapHeight);
    {
        HRESULT hr{ S_OK };
        D3D11_BUFFER_DESC desc{};
        desc.ByteWidth = sizeof(Shader::SceneConstants);
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;
        hr = graphics.GetDevice()->CreateBuffer(&desc, nullptr, SceneConstantBuffer.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    }

#if PARTICLE
    particles = std::make_unique<decltype(particles)::element_type>(graphics.GetDevice(), 1000);
    //particles->Initialize(graphics.GetDeviceContext(), 0);
#endif// PARTICLE
}

// 初期化
void SceneDemo::Initialize()
{
    Graphics& graphics = Graphics::Instance();

    // enemy
    {
        enemySlime[0]->Initialize();
        enemySlime[1]->Initialize();
    }

    // カメラ
    Camera::Instance().Initialize();

    // player
    PlayerManager::Instance().Initialize();

    // item
    ItemManager::Instance().Initialize();

    // stage
    stage->Initialize();
}

void SceneDemo::Finalize()
{
    // item
    ItemManager::Instance().Finalize();

    // player
    PlayerManager::Instance().Finalize();
}

void SceneDemo::Begin()
{
    // item
    ItemManager::Instance().Begin();
}

// 更新
void SceneDemo::Update(const float& elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    //effect[0]->Play({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f });
    
#if PARTICLE
    if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        particles->Initialize(Graphics::Instance().GetDeviceContext(), 0);
    }

    if (integrateParticles)
    {
        particles->Integrate(Graphics::Instance().GetDeviceContext(), elapsedTime);
    }
#endif// PARTICLE

    // GltfModel
    {
#if GLTF_MODEL
        gltfModels[0]->Update(elapsedTime);
#endif // GLTF_MODEL
    }

    // Model
    {
#if MODEL
#endif // MODEL
    }

    // Sprite
    {
#if SPRITE
        sprite->Update(elapsedTime);
        sprite->PlayAnimation(elapsedTime, 30, 11, true);
#endif // SPRITE
    }

#ifdef _DEBUG
    // Debug用カメラ
    if (gamePad.GetButtonDown() & GamePad::BTN_X)isDebugCamera = isDebugCamera ? false : true;
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
    else
#endif // _DEBUG
    {
        Camera::Instance().Update();
    }

    // slime
    {        
        enemySlime[0]->Update(elapsedTime);
        enemySlime[1]->Update(elapsedTime);
        
        //DirectX::XMFLOAT3 enemySlime0offset = enemySlime[0]->GetDebugSqhereOffset();
        //DirectX::XMFLOAT3 enemySlime1offset = enemySlime[1]->GetDebugSqhereOffset();
        //DirectX::XMFLOAT3 enemySlime0position = enemySlime[0]->GetTransform()->GetPosition();
        //DirectX::XMFLOAT3 enemySlime1position = enemySlime[1]->GetTransform()->GetPosition();
        //enemySlime0position = { enemySlime0position.x + enemySlime0offset.x, enemySlime0position.y + enemySlime0offset.y, enemySlime0position.z + enemySlime0offset.z };
        //enemySlime1position = { enemySlime1position.x + enemySlime1offset.x, enemySlime1position.y + enemySlime1offset.y, enemySlime1position.z + enemySlime1offset.z };

        DirectX::XMFLOAT3 outPosition;
        if (Collision::IntersectSphereVsSphere(
            enemySlime[0]->GetTransform()->GetPosition(),
            //enemySlime0position,
            enemySlime[0]->GetRange(),
            enemySlime[1]->GetTransform()->GetPosition(),
            //enemySlime1position,
            enemySlime[1]->GetRange(),
            outPosition
        ))
        {
            enemySlime[1]->GetTransform()->SetPosition(outPosition);
        }
    }

    // player
    PlayerManager::Instance().Update(elapsedTime);
    
    // item
    ItemManager::Instance().Update(elapsedTime);

    // エフェクト更新処理
    EffectManager::Instance().Update(elapsedTime);
}

void SceneDemo::End()
{
    // item
    ItemManager::Instance().End();
}

// 描画
void SceneDemo::Render(const float& elapsedTime)
{
    // scaleFactor
    //float enemyScaleFactor = 0.01f;
    //float enemyScaleFactor = 0.001f;
    float enemyScaleFactor = 0.1f;
    float playerScaleFactor = 0.01f;

    Graphics& graphics = Graphics::Instance();

    Shader::ShadowConstants shadowConstants{};

    // 描画の初期設定
    {
        // 描画の初期設定※必ず呼ぶこと！！！
        Mame::Scene::BaseScene::RenderInitialize();

        // SHADOW
        shadowConstants.lightDirection = graphics.GetShader()->view.position;
        shadowConstants.cameraPosition = graphics.GetShader()->view.camera;

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

            DirectX::XMStoreFloat4x4(&shadowConstants.viewProjection, V * P);
            shadowConstants.lightViewProjection = shadowConstants.viewProjection;
            graphics.GetDeviceContext()->UpdateSubresource(SceneConstantBuffer.Get(), 0, 0, &shadowConstants, 0, 0);
            graphics.GetDeviceContext()->VSSetConstantBuffers(1, 1, SceneConstantBuffer.GetAddressOf());

            shadow.shadowMap->Clear(graphics.GetDeviceContext(), 1.0f);
            shadow.shadowMap->Activate(graphics.GetDeviceContext());

            // SHADOW : 影つけたいモデルはここにRenderする
            {
                PlayerManager::Instance().Render(elapsedTime, playerScaleFactor);

                enemySlime[0]->Render(elapsedTime, enemyScaleFactor);
                enemySlime[1]->Render(elapsedTime, enemyScaleFactor);
            }

            shadow.shadowMap->Deactivete(graphics.GetDeviceContext());
        }
    }


    framebuffers[0]->Clear(graphics.GetDeviceContext());
    framebuffers[0]->Activate(graphics.GetDeviceContext());

#if SKYBOX
    skyBox->Render(graphics.GetDeviceContext(),
        Camera::Instance().GetViewMatrix(), Camera::Instance().GetProjectionMatrix());
#endif // SKYBOX

    // カメラ関係
    RenderContext rc;
    rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };

    Shader* shader = graphics.GetShader();
    shader->Begin(graphics.GetDeviceContext(), rc, shadowConstants);

    // SHADOW : bind shadow map at slot 8
    graphics.GetDeviceContext()->PSSetShaderResources(8, 1, shadow.shadowMap->shaderResourceView.GetAddressOf());

    // GltfModel
    {
#if GLTF_MODEL
        graphics.GetDeviceContext()->PSSetShaderResources(32, 1, shaderResourceViews[0].GetAddressOf());
        graphics.GetDeviceContext()->PSSetShaderResources(33, 1, shaderResourceViews[1].GetAddressOf());
        graphics.GetDeviceContext()->PSSetShaderResources(34, 1, shaderResourceViews[2].GetAddressOf());
        graphics.GetDeviceContext()->PSSetShaderResources(35, 1, shaderResourceViews[3].GetAddressOf());

        gltfModels[0]->Render(1.0f);
#endif // GLTF_MODEL
    }

    // Model
    {
#if MODEL
        model->Render(1.0f);
#endif // MODEL
    }

    // Sprite
    {
#if SPRITE
        sprite->Render();
#endif // SPRITE
    }

    // slime
    {
        enemySlime[0]->Render(elapsedTime, enemyScaleFactor);
        enemySlime[1]->Render(elapsedTime, enemyScaleFactor);
    }

    // player
    {
        PlayerManager::Instance().Render(elapsedTime, playerScaleFactor);
    }

    // item
    {
        ItemManager::Instance().Render(0.01f);
    }

    // stage
    {
        stage->Render(elapsedTime, 1.0f);
    }

#if PARTICLE
    shader->SetDepthStencileState(static_cast<size_t>(Shader::DEPTH_STATE::ZT_ON_ZW_ON));
    shader->SetRasterizerState(static_cast<size_t>(Shader::RASTER_STATE::CULL_NONE));
    shader->SetBlendState(static_cast<size_t>(Shader::BLEND_STATE::ADD));
    shader->GSSetConstantBuffer();
    particles->Render(graphics.GetDeviceContext());
#endif// PARTICLE

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
#if FOG
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
        bitBlockTransfer->Blit(graphics.GetDeviceContext(), shaderResourceView, 0, _countof(shaderResourceView), baseColorPS.Get());
    }
#else
    //ID3D11ShaderResourceView* shaderResourceView[]
    //{
    //    framebuffers[0]->shaderResourceViews[0].Get(),
    //};
    //bitBlockTransfer->Blit(graphics.GetDeviceContext(), shaderResourceView, 0, _countof(shaderResourceView), baseColorPS.Get());
#endif

    // BLOOM
#if BLOOM
    bloomer->Make(graphics.GetDeviceContext(), framebuffers[0]->shaderResourceViews[0].Get());

    shader->SetDepthStencileState(static_cast<UINT>(Shader::DEPTH_STATE::ZT_OFF_ZW_OFF));
    shader->SetRasterizerState(static_cast<UINT>(Shader::RASTER_STATE::CULL_NONE));
    shader->SetBlendState(static_cast<UINT>(Shader::BLEND_STATE::ALPHA));

    ID3D11ShaderResourceView* shaderResourceViews[] =
    {
        framebuffers[0]->shaderResourceViews[0].Get(),
        bloomer->ShaderResourceView(),
        framebuffers[1]->shaderResourceViews[0].Get(),
    };
    bitBlockTransfer->Blit(graphics.GetDeviceContext(), shaderResourceViews, 0, _countof(shaderResourceViews), bloomPS.Get());
#endif // BLOOM

}

// Imgui用
void SceneDemo::DrawDebug()
{
#ifdef USE_IMGUI
    ImGui::Begin("sceneDemo");

    if (ImGui::Button("drawdebug"))
    {
        isDebugRender = (isDebugRender) ? false : true;
    }

    if (ImGui::Button("create book"))
    {
        ItemManager::Instance().Register(new Book());
    }

    // GltfModel
    {
#if GLTF_MODEL
        gltfModels[0]->DrawDebug();
#endif // GLTF_MODEL
    }

    // Model
    {
#if MODEL
        ImGui::Begin("Model");
        model->DrawDebug();
        ImGui::End();
#endif // MODEL
    }

    // Sprite
    {
#if SPRITE
        sprite->DrawDebug();
#endif // SPRITE
    }

    // slime
    {
        enemySlime[0]->DrawDebug();
        enemySlime[1]->DrawDebug();
    }

    // player
    {
        PlayerManager::Instance().DrawDebug();
    }

    // item
    {
        ItemManager::Instance().DrawDebug();
    }

    // stage
    {
        stage->DrawDebug();
    }

    Camera::Instance().DrawDebug();

    // BLOOM
#if BLOOM
    if (ImGui::BeginMenu("bloom"))
    {
        ImGui::SliderFloat("bloom_extraction_threshold", &bloomer->bloomExtractionThreshold, +0.0f, +5.0f);
        ImGui::SliderFloat("bloom_intensity", &bloomer->bloomIntensity, +0.0f, +5.0f);
        ImGui::EndMenu();
    }
#endif // BLOOM

    // SHADOW
    {
        ImGui::Begin("shadow");
        ImGui::DragFloat4("lightViewFocus", &shadow.lightViewFocus.x);
        ImGui::SliderFloat("lightViewDistance", &shadow.lightViewDistance, 1.0f, 100.0f);
        ImGui::SliderFloat("lightViewSize", &shadow.lightViewSize, 1.0f, 100.0f);
        ImGui::SliderFloat("lightViewNearZ", &shadow.lightViewNearZ, 1.0f, shadow.lightViewFarZ - 1.0f);
        ImGui::SliderFloat("lightViewFarZ", &shadow.lightViewFarZ, shadow.lightViewNearZ + 1.0f, 100.0f);
        ImGui::Image(reinterpret_cast<void*>(shadow.shadowMap->shaderResourceView.Get()), ImVec2(shadow.shadowMapWidth / 5.0f, shadow.shadowMapHeight / 5.0f));
        ImGui::End();
    }

#if PARTICLE
    if (ImGui::BeginMenu("particle"))
    {
        particles->DrawDebug();
        ImGui::ColorEdit4("particleDataColor", &particles->particleData.color.x);
        ImGui::SliderFloat("particleDataSize", &particles->particleData.particleSize, 0.0f, 1.0f, "%.4f");

        ImGui::EndMenu();
    }
#endif// PARTICLE

    Graphics::Instance().GetShader()->DrawDebug();

    ImGui::End(); // sceneDemo
#endif // USE_IMUGI
}

