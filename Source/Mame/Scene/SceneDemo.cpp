#include "SceneDemo.h"

#include "../Graphics/Graphics.h"
#include "../Resource/texture.h"
#include "../Input/Input.h"

#include "../Graphics/EffectManager.h"

#define GLTF_MODEL 1
#define MODEL 1
#define SPRITE 1

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
            "./Resources/Model/sanaModel/mameoall.fbx");
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
        //effect[0] = std::make_unique<Effect>("./Resources/Effect/ring.efk");
        effect[0] = std::make_unique<Effect>("./Resources/Effect/sample/lightningv002.efk");
        //effect[0] = std::make_unique<Effect>("./Resources/Effect/ring2.efk");
        //effect[0] = std::make_unique<Effect>("./Resources/Effect/old/ring.efk");
    }

}

// 初期化
void SceneDemo::Initialize()
{
    Graphics& graphics = Graphics::Instance();

    // カメラ
    Camera::Instance().Initialize();
}

void SceneDemo::Finalize()
{
}

void SceneDemo::Begin()
{
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
#endif // _DEBUG

    // エフェクト更新処理
    EffectManager::Instance().Update(elapsedTime);
}

void SceneDemo::End()
{
}

// 描画
void SceneDemo::Render(const float& elapsedTime)
{
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

        RenderContext rc;
        rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };

        Shader* shader = graphics.GetShader();
        shader->Begin(graphics.GetDeviceContext(), rc, 0);
    }

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
        model->Render(0.01f);
#endif // MODEL
    }

    // Sprite
    {
#if SPRITE
        sprite->Render();
#endif // SPRITE
    }

    // 3Dエフェクト描画
    {
        Camera& camera = Camera::Instance();
        DirectX::XMFLOAT4X4 view, projection;
        DirectX::XMStoreFloat4x4(&view, camera.GetViewMatrix());
        DirectX::XMStoreFloat4x4(&projection, camera.GetProjectionMatrix());

        EffectManager::Instance().Render(view, projection);
    }
}

// Imgui用
void SceneDemo::DrawDebug()
{
#ifdef USE_IMGUI
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

    Camera::Instance().DrawDebug();

#endif // USE_IMUGI
}
