#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneLoading.h"
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
    Camera::Instance().Initialize();
}

// 終了化
void SceneTitle::Finalize()
{
}

// 毎フレーム一番最初に呼ばれる
void SceneTitle::Begin()
{
}

// 更新処理
void SceneTitle::Update(const float& elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    if (gamePad.GetButtonDown() & GamePad::BTN_A)
        Mame::Scene::SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
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

    // SHADOW : bind shadow map at slot 8
    graphics.GetDeviceContext()->PSSetShaderResources(8, 1, shadow.shadowMap->shaderResourceView.GetAddressOf());

    {
        // カメラ関係
        RenderContext rc;
        rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };

        shader->Begin(graphics.GetDeviceContext(), rc);
    }

    // モデル描画
    {
        stageBase->Render(0.01f);
        stageWall->Render(0.01f);
    }

}


// デバッグ用
void SceneTitle::DrawDebug()
{
    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();
    
    // ライトとか
    shader->DrawDebug();
}
