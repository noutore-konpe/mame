#include "SceneGame.h"

#include "../Other/misc.h"
#include "../Resource/texture.h"

#include "../Graphics/Graphics.h"
#include "../Input/Input.h"

#include "SceneManager.h"
#include "SceneLoading.h"
#include "SceneTitle.h"

#include "../Graphics/EffectManager.h"
#include "../Game/PlayerManager.h"
#include "../Game/ItemManager.h"
#include "../Game/Book.h"
#include "../Game/MagicCircle.h"
#include "../Game/ProjectileManager.h"

#ifdef _DEBUG
bool SceneGame::isDebugRender = false;
#endif // _DEBUG

// リソース生成
void SceneGame::CreateResource()
{
    Graphics& graphics = Graphics::Instance();



    // stage
    {
        stageBase = std::make_unique<Stage>();
        stageWall = std::make_unique<Stage>("./Resources/Model/Stage/stageWall.fbx");
    }

    // enemy
    {
        enemyAura = std::make_unique<EnemyAura>();
        enemyGolem = std::make_unique<EnemyGolem>();
    }

    // player
    {
        PlayerManager::Instance().GetPlayer() = std::make_unique<Player>();
    }

    // item
    {
        ItemManager::Instance().Register(new Book());
        //ItemManager::Instance().Register(new MagicCircle());
    }

    // 魔法陣
    {
        for (int i = 0; i < 10; ++i)
        {
            magicCircleSummon[i] = std::make_unique<MagicCircleSummon>();
        }
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

    // skyBox
    {
        skyBoxSprite = std::make_unique<Sprite>(graphics.GetDevice(),
            L"./Resources/Image/SkyBox/skybox3.jpg");
        skyBox = std::make_unique<SkyBox>(graphics.GetDevice(), skyBoxSprite);
    }

    // particle
    {
        particles = std::make_unique<decltype(particles)::element_type>(graphics.GetDevice(), 1000);
    }

    framebuffers[0] = std::make_unique<FrameBuffer>(graphics.GetDevice(), 1280, 720);
    bitBlockTransfer = std::make_unique<FullscreenQuad>(graphics.GetDevice());

    // baseColor : finalPass 
    CreatePsFromCso(graphics.GetDevice(), "./Resources/Shader/finalPassPs.cso", colorPS.GetAddressOf());

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
}

// 初期化
void SceneGame::Initialize()
{
    // カメラ
    Camera::Instance().Initialize();

    // enemy
    enemyAura->Initialize();
    enemyGolem->Initialize();

    // player
    PlayerManager::Instance().Initialize();

    // item
    ItemManager::Instance().Initialize();

    // stage
    stageBase->Initialize();
    stageWall->Initialize();

    // 魔法陣
    for (int i = 0; i < 10; ++i)
    {
        magicCircleSummon[i]->Initialize();
    }
}

// 終了化
void SceneGame::Finalize()
{
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

    // enemy
    {
        enemyAura->Update(elapsedTime);
        enemyGolem->Update(elapsedTime);
    }

    // player
    PlayerManager::Instance().Update(elapsedTime);

    // item
    ItemManager::Instance().Update(elapsedTime);

    // 魔法陣
    for (int i = 0; i < 10; ++i)
    {
        magicCircleSummon[i]->Update(elapsedTime);
    }

    // effect
    EffectManager::Instance().Update(elapsedTime);

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

    Shader::ShadowConstants shadowConstants{};

    float playerScaleFactor = 0.01f;
    float enemyScaleFactor = 0.001f;

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
            graphics.GetDeviceContext()->UpdateSubresource(shadowConstantBuffer.Get(), 0, 0, &shadowConstants, 0, 0);
            graphics.GetDeviceContext()->VSSetConstantBuffers(1, 1, shadowConstantBuffer.GetAddressOf());

            shadow.shadowMap->Clear(graphics.GetDeviceContext(), 1.0f);
            shadow.shadowMap->Activate(graphics.GetDeviceContext());

            // SHADOW : 影つけたいモデルはここにRenderする
            {
                PlayerManager::Instance().Render(playerScaleFactor);

                enemyAura->Render(enemyScaleFactor);
                enemyGolem->Render(0.01f);
            }

            shadow.shadowMap->Deactivete(graphics.GetDeviceContext());
        }
    }


    // EMISSIVE
    graphics.GetDeviceContext()->PSSetShaderResources(16, 1, emissiveTexture.GetAddressOf());

    framebuffers[0]->Clear(graphics.GetDeviceContext());
    framebuffers[0]->Activate(graphics.GetDeviceContext());

    // skybox
    skyBox->Render(graphics.GetDeviceContext(),
        Camera::Instance().GetViewMatrix(), Camera::Instance().GetProjectionMatrix());

    // SHADOW : bind shadow map at slot 8
    graphics.GetDeviceContext()->PSSetShaderResources(8, 1, shadow.shadowMap->shaderResourceView.GetAddressOf());

    // ※ skyBox の描画の後に書くこと
    {
        // カメラ関係
        RenderContext rc;
        rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };

        shader->Begin(graphics.GetDeviceContext(), rc, shadowConstants);
    }

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

        // enemy
        {
            enemyAura->Render(enemyScaleFactor);
            enemyGolem->Render(0.01f);
            //shader->SetBlendState(static_cast<UINT>(Shader::BLEND_STATE::ALPHA));
            //enemySlime[0]->Render(enemyScaleFactor, sagePS.Get());
            //enemySlime[1]->Render(enemyScaleFactor, emissiveTextureUVScrollPS.Get());
        }

        // item
        {
            ItemManager::Instance().Render(0.01f);
        }

        // 魔法陣
        if (isSeveralNum)
        {
            for (int i = 0; i < 10; ++i)
            {
                magicCircleSummon[i]->Render();
            }
        }
        else
        {
            magicCircleSummon[0]->Render();
        }
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
        bitBlockTransfer->Blit(graphics.GetDeviceContext(), shaderResourceView, 0, _countof(shaderResourceView), colorPS.Get());
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
    };
    bitBlockTransfer->Blit(graphics.GetDeviceContext(), shaderResourceViews, 0, _countof(shaderResourceViews), bloomPS.Get());
}

// debug用
void SceneGame::DrawDebug()
{
#ifdef USE_IMGUI
    ImGui::Begin("sceneGame");

    // デバッグプリミティブ描画
    if (ImGui::Button("drawDebug"))
    {
        isDebugRender = isDebugRender ? false : true;
    }

    // 本生成
    if (ImGui::Button("createBook"))
    {
        ItemManager::Instance().Register(new Book());
    }

    if (ImGui::Button("isSeveral"))
    {
        isSeveralNum = isSeveralNum ? false : true;
    }

    // 魔法陣再生
    if (ImGui::Button("magicCircle"))
    {
        if (isSeveralNum)
        {
            for (int i = 0; i < 10; ++i)
            {
                for (int j = 0; j < 3; ++j)
                {
                    magicCircleSummon[i]->magicCircle[j]->GetTransform()->
                        SetPosition(DirectX::XMFLOAT3(cosf(i) * 7.0f, 0.0f, sinf(i) * 7.0f));
                }

                magicCircleSummon[i]->GetStateMachine()->ChangeState(static_cast<UINT>(MagicCircleSummon::StateMachineState::AppearState));
            }
        }
        else
        {
            for (int i = 0; i < 3; ++i)
            {
                magicCircleSummon[0]->magicCircle[i]->GetTransform()->
                    SetPosition(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
            }

            magicCircleSummon[0]->GetStateMachine()->ChangeState(static_cast<UINT>(MagicCircleSummon::StateMachineState::AppearState));
        }
    }

    PlayerManager::Instance().DrawDebug();

    ItemManager::Instance().DrawDebug();
    
    particles->DrawDebug();

    enemyAura->DrawDebug();
    enemyGolem->DrawDebug();

    // カメラ
    Camera::Instance().DrawDebug();

    ImGui::End();
#endif
}