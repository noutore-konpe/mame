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

    SetStates();

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

#if BLOOM
    framebuffers[0] = std::make_unique<FrameBuffer>(graphics.GetDevice(), 1280, 720);
    bit_block_transfer = std::make_unique<FullscreenQuad>(graphics.GetDevice());
#endif // BLOOM

    // BLOOM
#if BLOOM
    bloomer = std::make_unique<Bloom>(graphics.GetDevice(), 1280, 720);
    CreatePsFromCso(graphics.GetDevice(), "./Resources/Shader/finalPassPs.cso", pixelShader.ReleaseAndGetAddressOf());
#endif // BLOOM

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

    if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        //for (int i = 0; i < 5; ++i)
        {
            effect[0]->Play({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f });
        }
        //effect[0]->Play({ 0.0f, 0.0f, 0.0f }, { 10.0f,10.0f,10.0f });
    }

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
    float enemyScaleFactor = 0.001f;
    float playerScaleFactor = 0.01f;

    Graphics& graphics = Graphics::Instance();

    // 描画の初期設定
    {
        ID3D11DeviceContext* deviceContext = graphics.GetDeviceContext();
        ID3D11RenderTargetView* renderTargetView = graphics.GetRenderTargetView();
        ID3D11DepthStencilView* depthStencilView = graphics.GetDepthStencilView();

        FLOAT color[]{ 0.2f, 0.2f, 0.2f, 1.0f };
        deviceContext->ClearRenderTargetView(renderTargetView, color);
        deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
        deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

#if SKYBOX
        skyBox->Render(graphics.GetDeviceContext(),
            Camera::Instance().GetViewMatrix(), Camera::Instance().GetProjectionMatrix());
#endif // SKYBOX

        // SHADOW
        Shader::SceneConstants sceneConstant{};
        sceneConstant.lightDirection = graphics.GetShader()->view.position;
        sceneConstant.cameraPosition = graphics.GetShader()->view.camera;

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

            DirectX::XMStoreFloat4x4(&sceneConstant.viewProjection, V * P);
            sceneConstant.lightViewProjection = sceneConstant.viewProjection;
            deviceContext->UpdateSubresource(SceneConstantBuffer.Get(), 0, 0, &sceneConstant, 0, 0);
            deviceContext->VSSetConstantBuffers(1, 1, SceneConstantBuffer.GetAddressOf());

            shadow.shadowMap->Clear(deviceContext, 1.0f);
            shadow.shadowMap->Activate(deviceContext);

            // SHADOW : 影つけたいモデルはここにRenderする
            {
                PlayerManager::Instance().Render(elapsedTime, playerScaleFactor);

                enemySlime[0]->Render(elapsedTime, enemyScaleFactor);
                enemySlime[1]->Render(elapsedTime, enemyScaleFactor);
            }

            shadow.shadowMap->Deactivete(deviceContext);
        }

        // カメラ関係
        RenderContext rc;
        rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };

        Shader* shader = graphics.GetShader();
        shader->Begin(graphics.GetDeviceContext(), rc, sceneConstant);

        // SHADOW : bind shadow map at slot 8
        deviceContext->PSSetShaderResources(8, 1, shadow.shadowMap->shaderResourceView.GetAddressOf());
    }

#if BLOOM
    framebuffers[0]->Clear(graphics.GetDeviceContext());
    framebuffers[0]->Activate(graphics.GetDeviceContext());
#endif // BLOOM


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

    // 3Dエフェクト描画
    {
        Camera& camera = Camera::Instance();
        DirectX::XMFLOAT4X4 view, projection;
        DirectX::XMStoreFloat4x4(&view, camera.GetViewMatrix());
        DirectX::XMStoreFloat4x4(&projection, camera.GetProjectionMatrix());

        EffectManager::Instance().Render(view, projection);
    }

#if BLOOM
    framebuffers[0]->Deactivate(graphics.GetDeviceContext());
#endif // BLOOM

    // BLOOM
#if BLOOM
    bloomer->Make(graphics.GetDeviceContext(), framebuffers[0]->shaderResourceViews[0].Get());

    graphics.GetDeviceContext()->OMSetDepthStencilState(depthStencilStates[static_cast<size_t>(DEPTH_STATE::ZT_OFF_ZW_OFF)].Get(), 0);
    graphics.GetDeviceContext()->RSSetState(rasterizerStates[static_cast<size_t>(RASTER_STATE::CULL_NONE)].Get());
    graphics.GetDeviceContext()->OMSetBlendState(blendStates[static_cast<size_t>(BLEND_STATE::ALPHA)].Get(), nullptr, 0xFFFFFFFF);
    ID3D11ShaderResourceView* shader_resource_views[] =
    {
        framebuffers[0]->shaderResourceViews[0].Get(),
        bloomer->ShaderResourceView(),
    };
    bit_block_transfer->Blit(graphics.GetDeviceContext(), shader_resource_views, 0, 2, pixel_shaders[0].Get());
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
        ImGui::SliderFloat("lightViewDistance", &shadow.lightViewDistance, 1.0f, 100.0f);
        ImGui::SliderFloat("lightViewSize", &shadow.lightViewSize, 1.0f, 100.0f);
        ImGui::SliderFloat("lightViewNearZ", &shadow.lightViewNearZ, 1.0f, shadow.lightViewFarZ - 1.0f);
        ImGui::SliderFloat("lightViewFarZ", &shadow.lightViewFarZ, shadow.lightViewNearZ + 1.0f, 100.0f);
        ImGui::Image(reinterpret_cast<void*>(shadow.shadowMap->shaderResourceView.Get()), ImVec2(shadow.shadowMapWidth / 5.0f, shadow.shadowMapHeight / 5.0f));
        ImGui::End();
    }

    Graphics::Instance().GetShader()->DrawDebug();

    ImGui::End(); // sceneDemo
#endif // USE_IMUGI
}

void SceneDemo::SetStates()
{
    Graphics& graphics = Graphics::Instance();
    HRESULT hr{ S_OK };

    D3D11_SAMPLER_DESC samplerDesc{};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0;
    samplerDesc.MaxAnisotropy = 16;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[0] = 0;
    samplerDesc.BorderColor[1] = 0;
    samplerDesc.BorderColor[2] = 0;
    samplerDesc.BorderColor[3] = 0;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
    hr = graphics.GetDevice()->CreateSamplerState(&samplerDesc, samplerStates[static_cast<size_t>(SAMPLER_STATE::POINT)].GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    hr = graphics.GetDevice()->CreateSamplerState(&samplerDesc, samplerStates[static_cast<size_t>(SAMPLER_STATE::LINEAR)].GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    hr = graphics.GetDevice()->CreateSamplerState(&samplerDesc, samplerStates[static_cast<size_t>(SAMPLER_STATE::ANISOTROPIC)].GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
    samplerDesc.BorderColor[0] = 0;
    samplerDesc.BorderColor[1] = 0;
    samplerDesc.BorderColor[2] = 0;
    samplerDesc.BorderColor[3] = 0;
    hr = graphics.GetDevice()->CreateSamplerState(&samplerDesc, samplerStates[static_cast<size_t>(SAMPLER_STATE::LINEAR_BORDER_BLACK)].GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
    samplerDesc.BorderColor[0] = 1;
    samplerDesc.BorderColor[1] = 1;
    samplerDesc.BorderColor[2] = 1;
    samplerDesc.BorderColor[3] = 1;
    hr = graphics.GetDevice()->CreateSamplerState(&samplerDesc, samplerStates[static_cast<size_t>(SAMPLER_STATE::LINEAR_BORDER_WHITE)].GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    D3D11_DEPTH_STENCIL_DESC depth_stencil_desc{};
    depth_stencil_desc.DepthEnable = TRUE;
    depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
    hr = graphics.GetDevice()->CreateDepthStencilState(&depth_stencil_desc, depthStencilStates[static_cast<size_t>(DEPTH_STATE::ZT_ON_ZW_ON)].GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    depth_stencil_desc.DepthEnable = TRUE;
    depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
    hr = graphics.GetDevice()->CreateDepthStencilState(&depth_stencil_desc, depthStencilStates[static_cast<size_t>(DEPTH_STATE::ZT_ON_ZW_OFF)].GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    depth_stencil_desc.DepthEnable = FALSE;
    depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
    hr = graphics.GetDevice()->CreateDepthStencilState(&depth_stencil_desc, depthStencilStates[static_cast<size_t>(DEPTH_STATE::ZT_OFF_ZW_ON)].GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    depth_stencil_desc.DepthEnable = FALSE;
    depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
    hr = graphics.GetDevice()->CreateDepthStencilState(&depth_stencil_desc, depthStencilStates[static_cast<size_t>(DEPTH_STATE::ZT_OFF_ZW_OFF)].GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    D3D11_BLEND_DESC blend_desc{};
    blend_desc.AlphaToCoverageEnable = FALSE;
    blend_desc.IndependentBlendEnable = FALSE;
    blend_desc.RenderTarget[0].BlendEnable = FALSE;
    blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
    blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
    blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    hr = graphics.GetDevice()->CreateBlendState(&blend_desc, blendStates[static_cast<size_t>(BLEND_STATE::NONE)].GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    blend_desc.AlphaToCoverageEnable = FALSE;
    blend_desc.IndependentBlendEnable = FALSE;
    blend_desc.RenderTarget[0].BlendEnable = TRUE;
    blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
    blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    hr = graphics.GetDevice()->CreateBlendState(&blend_desc, blendStates[static_cast<size_t>(BLEND_STATE::ALPHA)].GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    blend_desc.AlphaToCoverageEnable = FALSE;
    blend_desc.IndependentBlendEnable = FALSE;
    blend_desc.RenderTarget[0].BlendEnable = TRUE;
    blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; //D3D11_BLEND_ONE D3D11_BLEND_SRC_ALPHA
    blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
    blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
    blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
    blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    hr = graphics.GetDevice()->CreateBlendState(&blend_desc, blendStates[static_cast<size_t>(BLEND_STATE::ADD)].GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    blend_desc.AlphaToCoverageEnable = FALSE;
    blend_desc.IndependentBlendEnable = FALSE;
    blend_desc.RenderTarget[0].BlendEnable = TRUE;
    blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO; //D3D11_BLEND_DEST_COLOR
    blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_SRC_COLOR; //D3D11_BLEND_SRC_COLOR
    blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_DEST_ALPHA;
    blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    hr = graphics.GetDevice()->CreateBlendState(&blend_desc, blendStates[static_cast<size_t>(BLEND_STATE::MULTIPLY)].GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    D3D11_RASTERIZER_DESC rasterizer_desc{};
    rasterizer_desc.FillMode = D3D11_FILL_SOLID;
    rasterizer_desc.CullMode = D3D11_CULL_BACK;
    // UNIT.21
    //rasterizer_desc.FrontCounterClockwise = FALSE;
    rasterizer_desc.FrontCounterClockwise = TRUE;
    rasterizer_desc.DepthBias = 0;
    rasterizer_desc.DepthBiasClamp = 0;
    rasterizer_desc.SlopeScaledDepthBias = 0;
    rasterizer_desc.DepthClipEnable = TRUE;
    rasterizer_desc.ScissorEnable = FALSE;
    rasterizer_desc.MultisampleEnable = FALSE;
    rasterizer_desc.AntialiasedLineEnable = FALSE;
    hr = graphics.GetDevice()->CreateRasterizerState(&rasterizer_desc, rasterizerStates[static_cast<size_t>(RASTER_STATE::SOLID)].GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    rasterizer_desc.FillMode = D3D11_FILL_WIREFRAME;
    rasterizer_desc.CullMode = D3D11_CULL_BACK;
    rasterizer_desc.AntialiasedLineEnable = TRUE;
    hr = graphics.GetDevice()->CreateRasterizerState(&rasterizer_desc, rasterizerStates[static_cast<size_t>(RASTER_STATE::WIREFRAME)].GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    rasterizer_desc.FillMode = D3D11_FILL_SOLID;
    rasterizer_desc.CullMode = D3D11_CULL_NONE;
    rasterizer_desc.AntialiasedLineEnable = TRUE;
    hr = graphics.GetDevice()->CreateRasterizerState(&rasterizer_desc, rasterizerStates[static_cast<size_t>(RASTER_STATE::CULL_NONE)].GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    rasterizer_desc.FillMode = D3D11_FILL_WIREFRAME;
    rasterizer_desc.CullMode = D3D11_CULL_NONE;
    rasterizer_desc.AntialiasedLineEnable = TRUE;
    hr = graphics.GetDevice()->CreateRasterizerState(&rasterizer_desc, rasterizerStates[static_cast<size_t>(RASTER_STATE::WIREFRAME_CULL_NONE)].GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
}
