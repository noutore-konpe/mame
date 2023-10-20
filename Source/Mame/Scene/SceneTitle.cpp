#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneLoading.h"
#include "SceneResult.h"

#include "../Mame.h"

#include "../Graphics/Graphics.h"
#include "../Input/Input.h"

#include "../Resource/AudioManager.h"

#include "../Other/misc.h"
#include "../Resource/texture.h"

#include "../framework.h"

// リソース生成
void SceneTitle::CreateResource()
{
    Graphics& graphics = Graphics::Instance();

    // stage
    {
        stageBase = std::make_unique<Stage>();
        stageWall = std::make_unique<Stage>
            ("./Resources/Model/Stage/stageWall.fbx",
                "./Resources/Shader/StageWallPS.cso");
    }

    // sprite
    {
        backSprite = std::make_unique<Sprite>(graphics.GetDevice(),
            L"./Resources/Image/Title/title.png");
        emmaSprite = std::make_unique<Sprite>(graphics.GetDevice(),
            L"./Resources/Image/Title/Emma.png");
        pressSprite = std::make_unique<Sprite>(graphics.GetDevice(),
            L"./Resources/Image/Title/PressAnyButton.png");
        fadeSprite = std::make_unique<Sprite>(graphics.GetDevice(),
            L"./Resources/Image/Title/fade.png");
    }

    {
        framebuffers[0] = std::make_unique<FrameBuffer>(graphics.GetDevice(), 1280, 720, false);
        bitBlockTransfer = std::make_unique<FullscreenQuad>(graphics.GetDevice());
        CreatePsFromCso(graphics.GetDevice(), "./Resources/Shader/finalPassPs.cso", finalPassPS.GetAddressOf());

        bloomer = std::make_unique<Bloom>(graphics.GetDevice(), 1280 / 1, 720 / 1);
        CreatePsFromCso(graphics.GetDevice(), "./Resources/Shader/finalPassPs.cso", bloomPS.GetAddressOf());

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
void SceneTitle::Initialize()
{
    // カメラ
    Camera::Instance().TitleInitialize();

    pressSprite->GetSpriteTransform()->SetPos(DirectX::XMFLOAT2(370, 520));
    fadeSprite->GetSpriteTransform()->SetColorA(0.0f);

    //AudioManager::Instance().PlayBGM(BGM::Title, true);
    AudioManager::Instance().PlayBGM(BGM::Title1, true);
    //AudioManager::Instance().PlayBGM(BGM::Title2, true);

    // 変数初期化
    pressTimer = 0.0f;
    isAlphaDown = false;
    fadeTimer = 0.0f;
    isFade = false;
}

// 終了化
void SceneTitle::Finalize()
{
    AudioManager::Instance().StopAllBGM();
}

// 毎フレーム一番最初に呼ばれる
void SceneTitle::Begin()
{
}

// 更新処理
void SceneTitle::Update(const float& elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    Camera::Instance().TitleUpdate(elapsedTime);

    if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        isFade = true;
    }

    // scene切り替え
    if (IsChangeScene(elapsedTime))
    {
        Camera::Instance().TitleInitialize();
        Mame::Scene::SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
        //Mame::Scene::SceneManager::Instance().ChangeScene(new SceneLoading(new SceneResult));
        return;
    }

    // ボタンの点滅処理
    PressAnyButton(elapsedTime);
}

// 毎フレーム一番最後に呼ばれる
void SceneTitle::End()
{
}

// 描画処理
void SceneTitle::Render(const float& elapsedTime)
{
    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();

    Shader::SceneConstants sceneConstants{};

    // 描画の初期設定
    {
        // 描画の初期設定※必ず呼ぶこと！！！
        Mame::Scene::BaseScene::RenderInitialize();

        // Shadow : make shadow map
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

            }

            shadow.shadowMap->Deactivete(graphics.GetDeviceContext());
        }
    }

    Camera& camera = Camera::Instance();

    shader->SetDepthStencileState(static_cast<UINT>(Shader::DEPTH_STATE::ZT_ON_ZW_ON));
    shader->SetBlendState(static_cast<UINT>(Shader::BLEND_STATE::NONE));
    shader->SetRasterizerState(static_cast<UINT>(Shader::RASTER_STATE::SOLID));

    camera.TitleSetPerspectiveFov(graphics.GetDeviceContext());
    DirectX::XMStoreFloat4x4(&sceneConstants.viewProjection, camera.GetViewMatrix() * camera.GetProjectionMatrix());
    sceneConstants.lightDirection = shader->GetViewPosition();
    sceneConstants.cameraPosition = shader->GetViewCamera();

    DirectX::XMStoreFloat4x4(&sceneConstants.inverseProjection, DirectX::XMMatrixInverse(NULL, camera.GetProjectionMatrix()));
    DirectX::XMStoreFloat4x4(&sceneConstants.inverseViewProjection, DirectX::XMMatrixInverse(NULL, camera.GetViewMatrix() * camera.GetProjectionMatrix()));
    sceneConstants.time = framework::tictoc.time_stamp();

    graphics.GetDeviceContext()->UpdateSubresource(ConstantBuffer.Get(), 0, 0, &sceneConstants, 0, 0);
    graphics.GetDeviceContext()->VSSetConstantBuffers(1, 1, ConstantBuffer.GetAddressOf());
    graphics.GetDeviceContext()->PSSetConstantBuffers(1, 1, ConstantBuffer.GetAddressOf());

    // SHADOW : bind shadow map at slot 8
    graphics.GetDeviceContext()->PSSetShaderResources(8, 1, shadow.shadowMap->shaderResourceView.GetAddressOf());

    {
        // カメラ関係
        RenderContext rc;
        rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };

        shader->Begin(graphics.GetDeviceContext(), rc);
    }

    {
        framebuffers[0]->Clear(graphics.GetDeviceContext());
        framebuffers[0]->Activate(graphics.GetDeviceContext());

        // モデル描画
        {
            stageBase->Render(0.01f);
            stageWall->Render(0.01f);
        }
        // スプライト描画
        {
            shader->SetBlendState(static_cast<UINT>(Shader::BLEND_STATE::ALPHA));
            emmaSprite->Render();
        }

        framebuffers[0]->Deactivate(graphics.GetDeviceContext());

        framebuffers[1]->Clear(graphics.GetDeviceContext());
        framebuffers[1]->Activate(graphics.GetDeviceContext());
        shader->SetDepthStencileState(static_cast<UINT>(Shader::DEPTH_STATE::ZT_OFF_ZW_OFF));
        shader->SetRasterizerState(static_cast<UINT>(Shader::RASTER_STATE::CULL_NONE));
        shader->SetBlendState(static_cast<UINT>(Shader::BLEND_STATE::ALPHA));
        bitBlockTransfer->Blit(graphics.GetDeviceContext(), framebuffers[0]->shaderResourceViews[1].GetAddressOf(), 0, 1, fogPS.Get());

        framebuffers[1]->Deactivate(graphics.GetDeviceContext());

        shader->SetDepthStencileState(static_cast<UINT>(Shader::DEPTH_STATE::ZT_OFF_ZW_OFF));
        shader->SetRasterizerState(static_cast<UINT>(Shader::RASTER_STATE::CULL_NONE));
        ID3D11ShaderResourceView* shaderResourceView[]
        {
            framebuffers[0]->shaderResourceViews[0].Get(),
            framebuffers[0]->shaderResourceViews[0].Get(),/*dummy*/
            framebuffers[1]->shaderResourceViews[0].Get(),
        };

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

        bitBlockTransfer->Blit(graphics.GetDeviceContext(), shaderResourceViews, 0, _countof(shaderResourceViews), bloomPS.Get());
    }

    // spirte
    {
        shader->SetBlendState(static_cast<UINT>(Shader::BLEND_STATE::ALPHA));
        backSprite->Render();


        pressSprite->Render();

        // これを最後にRenderする
        fadeSprite->Render();

        // ※これより下に何も書かない
    }

}


// デバッグ用
void SceneTitle::DrawDebug()
{
    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();
    
    // ライトとか
    shader->DrawDebug();

    backSprite->DrawDebug();
    emmaSprite->DrawDebug();
    pressSprite->DrawDebug();

}

void SceneTitle::PressAnyButton(const float& elapsedTime)
{
    float maxTime = 1.0f;
    float maxAlpha = 0.6f;
    float minAlpha = 0.1f;

    if (pressTimer <= maxTime)
    {
        DirectX::XMFLOAT4 color = pressSprite->GetSpriteTransform()->GetColor();
        if (isAlphaDown)
            color.w = Easing::InSine(pressTimer, maxTime, minAlpha, maxAlpha);
        else
            color.w = Easing::InSine(pressTimer, maxTime, maxAlpha, minAlpha);

        pressSprite->GetSpriteTransform()->SetColor(color);

        pressTimer += elapsedTime;
    }
    else
    {
        isAlphaDown = isAlphaDown ? false : true;
        pressTimer = 0.0f;
    }
}

bool SceneTitle::IsChangeScene(const float& elapsedTime)
{
    if (isFade)
    {
        float maxTime = 1.0f;
        if (fadeTimer <= maxTime)
        {
            float alpha = Easing::InSine(fadeTimer, maxTime, 1.0f, 0.0f);
            fadeSprite->GetSpriteTransform()->SetColorA(alpha);
            fadeTimer += elapsedTime;
        }
        else
        {
            fadeSprite->GetSpriteTransform()->SetColorA(1.0f);
            return true;
        }
    }

    return false;
}
