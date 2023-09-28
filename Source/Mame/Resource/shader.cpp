#include "shader.h"
#include "../Graphics/Camera.h"

#include <fstream>

#include "../Other/misc.h"

#include "skinned_mesh.h"

#include "../Graphics/Graphics.h"

#include "../framework.h"

HRESULT CreateVsFromCso(ID3D11Device* device, const char* cso_name, ID3D11VertexShader** vertex_shader,
    ID3D11InputLayout** input_layout, D3D11_INPUT_ELEMENT_DESC* input_element_desc, UINT num_elements)
{
    FILE* fp{ nullptr };
    fopen_s(&fp, cso_name, "rb");
    _ASSERT_EXPR_A(fp, "CSO File not found");

    fseek(fp, 0, SEEK_END);
    long cso_sz{ ftell(fp) };
    fseek(fp, 0, SEEK_SET);

    std::unique_ptr<unsigned char[]>cso_data{ std::make_unique<unsigned char[]>(cso_sz) };
    fread(cso_data.get(), cso_sz, 1, fp);
    fclose(fp);

    HRESULT hr{ S_OK };
    hr = device->CreateVertexShader(cso_data.get(), cso_sz, nullptr, vertex_shader);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    if (input_layout)
    {
        hr = device->CreateInputLayout(input_element_desc, num_elements,
            cso_data.get(), cso_sz, input_layout);
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    }

    return hr;
}

HRESULT CreatePsFromCso(ID3D11Device* device, const char* cso_name, ID3D11PixelShader** pixel_shader)
{
    FILE* fp{ nullptr };
    fopen_s(&fp, cso_name, "rb");
    _ASSERT_EXPR_A(fp, "CSO File not found");

    fseek(fp, 0, SEEK_END);
    long cso_sz{ ftell(fp) };
    fseek(fp, 0, SEEK_SET);

    std::unique_ptr<unsigned char[]>cso_data{ std::make_unique<unsigned char[]>(cso_sz) };
    fread(cso_data.get(), cso_sz, 1, fp);
    fclose(fp);

    HRESULT hr{ S_OK };
    hr = device->CreatePixelShader(cso_data.get(), cso_sz, nullptr, pixel_shader);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    return hr;
}

HRESULT CreateGsFromCso(ID3D11Device* device, const char* csoName, ID3D11GeometryShader** geometryShader)
{
    FILE* fp = nullptr;
    fopen_s(&fp, csoName, "rb");
    _ASSERT_EXPR_A(fp, "CSO File not found");

    fseek(fp, 0, SEEK_END);
    long csoSz = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    std::unique_ptr<unsigned char[]> csoData = std::make_unique<unsigned char[]>(csoSz);
    fread(csoData.get(), csoSz, 1, fp);
    fclose(fp);

    HRESULT hr = device->CreateGeometryShader(csoData.get(), csoSz, nullptr, geometryShader);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    return hr;
}

HRESULT CreateCsFromCso(ID3D11Device* device, const char* csoName, ID3D11ComputeShader** computeShader)
{
    FILE* fp = nullptr;
    fopen_s(&fp, csoName, "rb");
    _ASSERT_EXPR_A(fp, "CSO File not found");

    fseek(fp, 0, SEEK_END);
    long csoSz = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    std::unique_ptr<unsigned char[]> csoData = std::make_unique<unsigned char[]>(csoSz);
    fread(csoData.get(), csoSz, 1, fp);
    fclose(fp);

    HRESULT hr = device->CreateComputeShader(csoData.get(), csoSz, nullptr, computeShader);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    return hr;
}

Shader::Shader(ID3D11Device* device)
{
    // 定数バッファ
    {
        HRESULT hr{ S_OK };

        D3D11_BUFFER_DESC bufferDesc{};
        bufferDesc.ByteWidth = sizeof(SceneConstants);
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bufferDesc.CPUAccessFlags = 0;
        bufferDesc.MiscFlags = 0;
        bufferDesc.StructureByteStride = 0;
        // SceneConstants
        hr = device->CreateBuffer(&bufferDesc, nullptr,
            sceneConstantBuffer[static_cast<UINT>(CONSTANT_BUFFER::SCENE_CONSTANT)].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


        bufferDesc.ByteWidth = sizeof(CBParametric);
        // CBParametric
        hr = device->CreateBuffer(&bufferDesc, nullptr,
            sceneConstantBuffer[static_cast<UINT>(CONSTANT_BUFFER::CONSTANT_BUFFER_PARAMETRIC)].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


        // POINT_LIGHT
        bufferDesc.ByteWidth = sizeof(LightConstants);
        // LightConstants
        hr = device->CreateBuffer(&bufferDesc, nullptr,
            sceneConstantBuffer[static_cast<UINT>(CONSTANT_BUFFER::LIGHT_CONSTANT)].GetAddressOf());

        // SHADOW
        bufferDesc.ByteWidth = sizeof(ShadowConstants);
        hr = device->CreateBuffer(&bufferDesc, nullptr,
            sceneConstantBuffer[static_cast<UINT>(CONSTANT_BUFFER::SHADOW_CONSTANT)].GetAddressOf());

        // FOG
        bufferDesc.ByteWidth = sizeof(FogConstants);
        hr = device->CreateBuffer(&bufferDesc, nullptr,
            sceneConstantBuffer[static_cast<UINT>(CONSTANT_BUFFER::FOG_CONSTANT)].GetAddressOf());
    }

    // ブレンドステート
    {
        HRESULT hr{ S_OK };

        D3D11_BLEND_DESC desc{};
        desc.AlphaToCoverageEnable = FALSE;
        desc.IndependentBlendEnable = FALSE;
        desc.RenderTarget[0].BlendEnable = FALSE;
        desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
        desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
        desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
        desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        hr = device->CreateBlendState(&desc, blendStates[static_cast<size_t>(BLEND_STATE::NONE)].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        desc.AlphaToCoverageEnable = FALSE;
        desc.IndependentBlendEnable = FALSE;
        desc.RenderTarget[0].BlendEnable = TRUE;
        desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
        desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        hr = device->CreateBlendState(&desc, blendStates[static_cast<size_t>(BLEND_STATE::ALPHA)].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        desc.AlphaToCoverageEnable = FALSE;
        desc.IndependentBlendEnable = FALSE;
        desc.RenderTarget[0].BlendEnable = TRUE;
        desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; //D3D11_BLEND_ONE D3D11_BLEND_SRC_ALPHA
        desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
        desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
        desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
        desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        hr = device->CreateBlendState(&desc, blendStates[static_cast<size_t>(BLEND_STATE::ADD)].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        desc.AlphaToCoverageEnable = FALSE;
        desc.IndependentBlendEnable = FALSE;
        desc.RenderTarget[0].BlendEnable = TRUE;
        desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO; //D3D11_BLEND_DEST_COLOR
        desc.RenderTarget[0].DestBlend = D3D11_BLEND_SRC_COLOR; //D3D11_BLEND_SRC_COLOR
        desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_DEST_ALPHA;
        desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
        desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        hr = device->CreateBlendState(&desc, blendStates[static_cast<size_t>(BLEND_STATE::MULTIPLY)].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    }

    // 深度ステンシルステート
    {
        HRESULT hr{ S_OK };

        D3D11_DEPTH_STENCIL_DESC desc{};
        // 0 深度テスト：オン　深度ライト：オン
        desc.DepthEnable = TRUE;	// 深度テストを有効にします
        desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;	// 深度データで変更できる深度ステンシルバッファーの一部を特定します
        desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL; // 深度データと既存データを比較する関数
        hr = device->CreateDepthStencilState(&desc, depthStencilStates[0].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        // 1 深度テスト：オン　深度ライト：オフ
        desc.DepthEnable = TRUE;
        desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
        hr = device->CreateDepthStencilState(&desc, depthStencilStates[1].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        // 2 深度テスト：オフ　深度ライト：オン
        desc.DepthEnable = FALSE;
        desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        hr = device->CreateDepthStencilState(&desc, depthStencilStates[2].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        // 3 深度テスト：オフ　深度ライト：オフ
        desc.DepthEnable = FALSE;
        desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
        hr = device->CreateDepthStencilState(&desc, depthStencilStates[3].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    }

    // ラスタライザーステート
    {
        HRESULT hr{ S_OK };

        D3D11_RASTERIZER_DESC desc{};
        desc.FillMode = D3D11_FILL_SOLID;
        desc.CullMode = D3D11_CULL_BACK;
        desc.FrontCounterClockwise = TRUE;
        desc.DepthBias = 0;
        desc.DepthBiasClamp = 0;
        desc.SlopeScaledDepthBias = 0;
        desc.DepthClipEnable = TRUE;
        desc.ScissorEnable = FALSE;
        desc.MultisampleEnable = FALSE;
        desc.AntialiasedLineEnable = FALSE;
        hr = device->CreateRasterizerState(&desc, rasterizerStates[static_cast<size_t>(RASTER_STATE::SOLID)].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        desc.FillMode = D3D11_FILL_WIREFRAME;
        desc.CullMode = D3D11_CULL_BACK;
        desc.AntialiasedLineEnable = TRUE;
        hr = device->CreateRasterizerState(&desc, rasterizerStates[static_cast<size_t>(RASTER_STATE::WIREFRAME)].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        desc.FillMode = D3D11_FILL_SOLID;
        desc.CullMode = D3D11_CULL_NONE;
        desc.AntialiasedLineEnable = TRUE;
        hr = device->CreateRasterizerState(&desc, rasterizerStates[static_cast<size_t>(RASTER_STATE::CULL_NONE)].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        desc.FillMode = D3D11_FILL_WIREFRAME;
        desc.CullMode = D3D11_CULL_NONE;
        desc.AntialiasedLineEnable = TRUE;
        hr = device->CreateRasterizerState(&desc, rasterizerStates[static_cast<size_t>(RASTER_STATE::WIREFRAME_CULL_NONE)].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    }

    // サンプラステート
    {
        HRESULT hr{ S_OK };

        D3D11_SAMPLER_DESC desc{};
        desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
        desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.MipLODBias = 0;
        desc.MaxAnisotropy = 16;
        desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
        desc.BorderColor[0] = 0;
        desc.BorderColor[1] = 0;
        desc.BorderColor[2] = 0;
        desc.BorderColor[3] = 0;
        desc.MinLOD = 0;
        desc.MaxLOD = D3D11_FLOAT32_MAX;
        hr = device->CreateSamplerState(&desc, samplerState[static_cast<size_t>(SAMPLER_STATE::POINT)].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        hr = device->CreateSamplerState(&desc, samplerState[static_cast<size_t>(SAMPLER_STATE::LINEAR)].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        desc.Filter = D3D11_FILTER_ANISOTROPIC;
        desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        hr = device->CreateSamplerState(&desc, samplerState[static_cast<size_t>(SAMPLER_STATE::ANISOTROPIC)].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
        desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
        desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
        desc.BorderColor[0] = 0;
        desc.BorderColor[1] = 0;
        desc.BorderColor[2] = 0;
        desc.BorderColor[3] = 0;
        hr = device->CreateSamplerState(&desc, samplerState[static_cast<size_t>(SAMPLER_STATE::LINEAR_BORDER_BLACK)].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
        desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
        desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
        desc.BorderColor[0] = 1;
        desc.BorderColor[1] = 1;
        desc.BorderColor[2] = 1;
        desc.BorderColor[3] = 1;
        hr = device->CreateSamplerState(&desc, samplerState[static_cast<size_t>(SAMPLER_STATE::LINEAR_BORDER_WHITE)].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        // SHADOW
        desc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
        desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
        desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
        desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
        desc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL; // D3D11_COMPARISON_LESS_EQUAL
        desc.BorderColor[0] = 1;
        desc.BorderColor[1] = 1;
        desc.BorderColor[2] = 1;
        desc.BorderColor[3] = 1;
        hr = device->CreateSamplerState(&desc, samplerState[static_cast<size_t>(SAMPLER_STATE::COMPARISON_LINEAR_BORDER_WHITE)].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    }

    // LightConstants
    {
        Graphics& graphics = Graphics::Instance();
#ifdef _DEBUG
        // POINT_LIGHT
        pointLightModel = std::make_unique<Model>(graphics.GetDevice(),
            "./Resources/Model/Collision/sqhere.fbx");
#endif// _DEBUG

        // DIRECTION_LIGHT
        {
            lightConstant.directionLight.direction = DirectX::XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f);
            lightConstant.directionLight.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
        }

        // POINT_LIGHT
        {
#if POINT_LIGHT_ONE
            lightConstant.pointLight.position = DirectX::XMFLOAT4(0.0f, 1.5f, 3.0f, 0.0f);
            lightConstant.pointLight.color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
            lightConstant.pointLight.range = 5.0f;
#else
            for (int i = 0; i < pointLightMax; ++i)
            {
                lightConstant.pointLight[i].position=DirectX::XMFLOAT4(3.0f*i, 1.5f, 3.0f, 0.0f);
                lightConstant.pointLight[i].color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
                lightConstant.pointLight[i].range = 5.0f;
            }
#endif// POINT_LIGHT_ONE
        }

        // SPOT_LIGHT
        {
            lightConstant.spotLight.position = DirectX::XMFLOAT4(0.0f, 1.0f, 15.0f, 0.0f);
            lightConstant.spotLight.color = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
            lightConstant.spotLight.direction = DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f);

            lightConstant.spotLight.range = 10.0f;

            // 正規化
            DirectX::XMVECTOR spotLightDirectionVec = DirectX::XMLoadFloat3(&lightConstant.spotLight.direction);
            spotLightDirectionVec = DirectX::XMVector3Normalize(spotLightDirectionVec);
            DirectX::XMStoreFloat3(&lightConstant.spotLight.direction, spotLightDirectionVec);

            lightConstant.spotLight.angle = DirectX::XMConvertToRadians(-5.0f);
        }

        // HEMISPHERE_LIGHT
        {
            // 地面色
            lightConstant.hemisphereLight.groundColor.x = 0.7f;
            lightConstant.hemisphereLight.groundColor.y = 0.5f;
            lightConstant.hemisphereLight.groundColor.z = 0.3f;

            // 天球色
            lightConstant.hemisphereLight.skyColor.x = 0.15f;
            lightConstant.hemisphereLight.skyColor.y = 0.7f;
            lightConstant.hemisphereLight.skyColor.z = 0.95f;

            // 地面の法線を設定
            lightConstant.hemisphereLight.groundNormal.x = 0.0f;
            lightConstant.hemisphereLight.groundNormal.y = 1.0f;
            lightConstant.hemisphereLight.groundNormal.z = 0.0f;
        }
    }
}

void Shader::Initialize()
{
    Camera& camera = Camera::Instance();
    camera.Initialize();
}

void Shader::Begin(ID3D11DeviceContext* deviceContext, const RenderContext& rc)
{
    Camera& camera = Camera::Instance();

    // サンプラーステート
    deviceContext->PSSetSamplers(0, 1, samplerState[static_cast<size_t>(SAMPLER_STATE::POINT)].GetAddressOf());
    deviceContext->PSSetSamplers(1, 1, samplerState[static_cast<size_t>(SAMPLER_STATE::LINEAR)].GetAddressOf());
    deviceContext->PSSetSamplers(2, 1, samplerState[static_cast<size_t>(SAMPLER_STATE::ANISOTROPIC)].GetAddressOf());
    deviceContext->PSSetSamplers(3, 1, samplerState[static_cast<size_t>(SAMPLER_STATE::LINEAR_BORDER_BLACK)].GetAddressOf());
    deviceContext->PSSetSamplers(4, 1, samplerState[static_cast<size_t>(SAMPLER_STATE::LINEAR_BORDER_WHITE)].GetAddressOf());

    deviceContext->OMSetDepthStencilState(depthStencilStates[0].Get(), 1);

    deviceContext->OMSetBlendState(blendStates[0].Get(), nullptr, 0xFFFFFFFF);

    deviceContext->RSSetState(rasterizerStates[0].Get());

    // ビュープロジェクション変換行列を計算し、それを定数バッファにセットする
    camera.SetPerspectiveFov(deviceContext);


    SceneConstants scene{};
    DirectX::XMStoreFloat4x4(&scene.viewProjection, camera.GetViewMatrix() * camera.GetProjectionMatrix());

    scene.lightDirection = { view.position.x,view.position.y,view.position.z,view.position.w };
    scene.cameraPosition = { view.camera.x,view.camera.y,view.camera.z,view.camera.w };

    // FOG
    {
        DirectX::XMStoreFloat4x4(&scene.inverseProjection, DirectX::XMMatrixInverse(NULL, camera.GetProjectionMatrix()));
        DirectX::XMStoreFloat4x4(&scene.inverseViewProjection, DirectX::XMMatrixInverse(NULL, camera.GetViewMatrix() * camera.GetProjectionMatrix()));
        scene.time = framework::tictoc.time_stamp();

        deviceContext->UpdateSubresource(sceneConstantBuffer[static_cast<UINT>(CONSTANT_BUFFER::FOG_CONSTANT)].Get(), 0, 0, &fogConstants, 0, 0);
        deviceContext->PSSetConstantBuffers(2, 1, sceneConstantBuffer[static_cast<UINT>(CONSTANT_BUFFER::FOG_CONSTANT)].GetAddressOf());
    }


    deviceContext->UpdateSubresource(sceneConstantBuffer[0].Get(), 0, 0, &scene, 0, 0);
    deviceContext->VSSetConstantBuffers(1, 1, sceneConstantBuffer[0].GetAddressOf());
    deviceContext->PSSetConstantBuffers(1, 1, sceneConstantBuffer[0].GetAddressOf());


    // POINT_LIGHT
    {
        deviceContext->UpdateSubresource(sceneConstantBuffer[static_cast<UINT>(CONSTANT_BUFFER::LIGHT_CONSTANT)].Get(), 0, 0, &lightConstant, 0, 0);
        deviceContext->VSSetConstantBuffers(5, 1, sceneConstantBuffer[static_cast<UINT>(CONSTANT_BUFFER::LIGHT_CONSTANT)].GetAddressOf());
        deviceContext->PSSetConstantBuffers(5, 1, sceneConstantBuffer[static_cast<UINT>(CONSTANT_BUFFER::LIGHT_CONSTANT)].GetAddressOf());

#ifdef _DEBUG
#if POINT_LIGHT_ONE
        DirectX::XMFLOAT3 ptPos = { lightConstant.pointLight.position.x, lightConstant.pointLight.position.y, lightConstant.pointLight.position.z };
        pointLightModel->GetTransform()->SetPosition(ptPos);
        pointLightModel->Render(0.1f);
#endif// POINT_LIGHT_ONE
#endif// _DEBUG
    }


#ifdef USE_IMGUI
    //DrawDebug();
#endif
}

void Shader::Begin(ID3D11DeviceContext* deviceContext, const RenderContext& rc, const ShadowConstants& shadowConstant)
{
    Camera& camera = Camera::Instance();

    // サンプラーステート
    {
        deviceContext->PSSetSamplers(0, 1, samplerState[static_cast<size_t>(SAMPLER_STATE::POINT)].GetAddressOf());
        deviceContext->PSSetSamplers(1, 1, samplerState[static_cast<size_t>(SAMPLER_STATE::LINEAR)].GetAddressOf());
        deviceContext->PSSetSamplers(2, 1, samplerState[static_cast<size_t>(SAMPLER_STATE::ANISOTROPIC)].GetAddressOf());
        deviceContext->PSSetSamplers(3, 1, samplerState[static_cast<size_t>(SAMPLER_STATE::LINEAR_BORDER_BLACK)].GetAddressOf());
        deviceContext->PSSetSamplers(4, 1, samplerState[static_cast<size_t>(SAMPLER_STATE::LINEAR_BORDER_WHITE)].GetAddressOf());
        // SHADOW
        deviceContext->PSSetSamplers(5, 1, samplerState[static_cast<size_t>(SAMPLER_STATE::COMPARISON_LINEAR_BORDER_WHITE)].GetAddressOf());
    }

    deviceContext->OMSetDepthStencilState(depthStencilStates[0].Get(), 1);

    deviceContext->OMSetBlendState(blendStates[0].Get(), nullptr, 0xFFFFFFFF);

    deviceContext->RSSetState(rasterizerStates[0].Get());

    // ビュープロジェクション変換行列を計算し、それを定数バッファにセットする
    camera.SetPerspectiveFov(deviceContext);

    // SHADOW
    {
        ShadowConstants shadow{};
        DirectX::XMStoreFloat4x4(&shadow.viewProjection, camera.GetViewMatrix() * camera.GetProjectionMatrix());

        shadow.lightDirection = { view.position.x,view.position.y,view.position.z,view.position.w };
        shadow.cameraPosition = { view.camera.x,view.camera.y,view.camera.z,view.camera.w };
        //shadow.lightDirection = shadowConstant.lightDirection;
        //shadow.cameraPosition = shadowConstant.cameraPosition;
        shadow.lightViewProjection = shadowConstant.lightViewProjection;

        deviceContext->UpdateSubresource(sceneConstantBuffer[static_cast<UINT>(CONSTANT_BUFFER::SHADOW_CONSTANT)].Get(), 0, 0, &shadow, 0, 0);
        deviceContext->VSSetConstantBuffers(10, 1, sceneConstantBuffer[static_cast<UINT>(CONSTANT_BUFFER::SHADOW_CONSTANT)].GetAddressOf());
        deviceContext->PSSetConstantBuffers(10, 1, sceneConstantBuffer[static_cast<UINT>(CONSTANT_BUFFER::SHADOW_CONSTANT)].GetAddressOf());
    }

    SceneConstants scene{};
    DirectX::XMStoreFloat4x4(&scene.viewProjection, camera.GetViewMatrix() * camera.GetProjectionMatrix());

    scene.lightDirection = { view.position.x,view.position.y,view.position.z,view.position.w };
    scene.cameraPosition = { view.camera.x,view.camera.y,view.camera.z,view.camera.w };

    // FOG
    {
        DirectX::XMStoreFloat4x4(&scene.inverseProjection, DirectX::XMMatrixInverse(NULL, camera.GetProjectionMatrix()));
        DirectX::XMStoreFloat4x4(&scene.inverseViewProjection, DirectX::XMMatrixInverse(NULL, camera.GetViewMatrix() * camera.GetProjectionMatrix()));
        scene.time = framework::tictoc.time_stamp();

        deviceContext->UpdateSubresource(sceneConstantBuffer[static_cast<UINT>(CONSTANT_BUFFER::FOG_CONSTANT)].Get(), 0, 0, &fogConstants, 0, 0);
        deviceContext->PSSetConstantBuffers(2, 1, sceneConstantBuffer[static_cast<UINT>(CONSTANT_BUFFER::FOG_CONSTANT)].GetAddressOf());
    }

    deviceContext->UpdateSubresource(sceneConstantBuffer[static_cast<UINT>(CONSTANT_BUFFER::SCENE_CONSTANT)].Get(), 0, 0, &scene, 0, 0);
    deviceContext->VSSetConstantBuffers(1, 1, sceneConstantBuffer[static_cast<UINT>(CONSTANT_BUFFER::SCENE_CONSTANT)].GetAddressOf());
    deviceContext->PSSetConstantBuffers(1, 1, sceneConstantBuffer[static_cast<UINT>(CONSTANT_BUFFER::SCENE_CONSTANT)].GetAddressOf());

    // POINT_LIGHT
    {
        deviceContext->UpdateSubresource(sceneConstantBuffer[static_cast<UINT>(CONSTANT_BUFFER::LIGHT_CONSTANT)].Get(), 0, 0, &lightConstant, 0, 0);
        deviceContext->VSSetConstantBuffers(5, 1, sceneConstantBuffer[static_cast<UINT>(CONSTANT_BUFFER::LIGHT_CONSTANT)].GetAddressOf());
        deviceContext->PSSetConstantBuffers(5, 1, sceneConstantBuffer[static_cast<UINT>(CONSTANT_BUFFER::LIGHT_CONSTANT)].GetAddressOf());

#ifdef _DEBUG
#if POINT_LIGHT_ONE
        DirectX::XMFLOAT3 ptPos = { lightConstant.pointLight.position.x, lightConstant.pointLight.position.y, lightConstant.pointLight.position.z };
        pointLightModel->GetTransform()->SetPosition(ptPos);
        pointLightModel->Render(0.1f);
#endif// POINT_LIGHT_ONE
#endif// _DEBUG
    }

    // ZELDA
    deviceContext->GSSetConstantBuffers(1, 1, sceneConstantBuffer[static_cast<UINT>(CONSTANT_BUFFER::SCENE_CONSTANT)].GetAddressOf());

    EntryLight();
    //EntryLight2();
}

void Shader::SetState(ID3D11DeviceContext* dc, int RasterizerState, int DepthStencilState, int SamplerState)
{
    dc->PSSetSamplers(SamplerState, 1, samplerState[SamplerState].GetAddressOf());
    dc->RSSetState(rasterizerStates[RasterizerState].Get());
    dc->OMSetDepthStencilState(depthStencilStates[DepthStencilState].Get(), 1);
}

void Shader::SetState(ID3D11DeviceContext* deviceContext, int SamplerState, int DepthStencileState, int BlendState, int RasterizerState)
{
    deviceContext->PSSetSamplers(SamplerState, 1, samplerState[SamplerState].GetAddressOf());
    deviceContext->OMSetDepthStencilState(depthStencilStates[DepthStencileState].Get(), 1);
    deviceContext->OMSetBlendState(blendStates[BlendState].Get(), nullptr, 0xFFFFFFFF);
    deviceContext->RSSetState(rasterizerStates[RasterizerState].Get());
}

void Shader::Draw(ID3D11DeviceContext* dc, Model* model)
{

}

void Shader::End(ID3D11DeviceContext* dc)
{
}

void Shader::DrawDebug()
{
    Camera& camera = Camera::Instance();

    ImGui::Begin("view");

    if (ImGui::TreeNode("camera"))
    {
        ImGui::SliderFloat("wight", &view.viewWitdh,1.0f,10000.0f);
        ImGui::SliderFloat("height", &view.viewHeight, 1.0f, 10000.0f);
        ImGui::SliderFloat("nearZ", &view.nearZ, 1.0f, 10000.0f);
        ImGui::SliderFloat("farZ", &view.farZ, 1.0f, 10000.0f);

        ImGui::DragFloat4("positon", &view.position.x);
        ImGui::DragFloat4("camera", &view.camera.x);

        ImGui::TreePop();
    }

    camera.DrawDebug();

    ImGui::End();

    if (ImGui::Begin("Light"))
    {
        // DIRECTION_LIGHT
        if (ImGui::TreeNode("DirectionLight"))
        {
            ImGui::DragFloat4("directiono", &lightConstant.directionLight.direction.x);
            ImGui::ColorEdit4("color", &lightConstant.directionLight.color.x);
            ImGui::TreePop();
        }

        // POINT_LIGHT
#if POINT_LIGHT_ONE
        if (ImGui::TreeNode("PointLight"))
        {
            ImGui::DragFloat4("position", &lightConstant.pointLight.position.x);
            ImGui::ColorEdit4("color", &lightConstant.pointLight.color.x);
            ImGui::DragFloat("range", &lightConstant.pointLight.range);
            ImGui::TreePop();
        }
#else
        for (int i = 0; i < pointLightMax; ++i)
        {
            ImGui::Begin(("pointLight" + std::to_string(i)).c_str());
            ImGui::DragFloat4("position", &lightConstant.pointLight[i].position.x);
            ImGui::ColorEdit4("color", &lightConstant.pointLight[i].color.x);
            ImGui::DragFloat("range", &lightConstant.pointLight[i].range);
            ImGui::End();
        }
        //if (ImGui::TreeNode("PointLight"))
        //{
        //    for (int i = 0; i < pointLightMax; ++i)
        //    {
        //        //ImGui::BeginMenu(("pointLight" + std::to_string(i)).c_str());
        //        ImGui::TreeNode(("pointLight" + std::to_string(i)).c_str());

        //        ImGui::DragFloat4("position", &lightConstant.pointLight[i].position.x);
        //        ImGui::ColorEdit4("color", &lightConstant.pointLight[i].color.x);
        //        ImGui::DragFloat("range", &lightConstant.pointLight[i].range);

        //        ImGui::TreePop();
        //        //ImGui::EndMenu();
        //    }
        //    ImGui::TreePop();
        //}
#endif// POINT_LIGHT_ONE

        // SPOT_LIGHT
        if (ImGui::TreeNode("SpotLight"))
        {
            ImGui::DragFloat4("position", &lightConstant.spotLight.position.x);
            ImGui::ColorEdit4("color", &lightConstant.spotLight.color.x);
            ImGui::DragFloat("range", &lightConstant.spotLight.range);
            ImGui::DragFloat3("direction", &lightConstant.spotLight.direction.x);
            float angle = DirectX::XMConvertToDegrees(lightConstant.spotLight.angle);
            ImGui::DragFloat("angle", &angle);
            lightConstant.spotLight.angle = DirectX::XMConvertToRadians(angle);
            ImGui::TreePop();
        }

        // HEMISPHERE_LIGHT
        if (ImGui::TreeNode("HemisphereLight"))
        {
            ImGui::ColorEdit4("groundColor", &lightConstant.hemisphereLight.groundColor.x);
            ImGui::ColorEdit4("skyColor", &lightConstant.hemisphereLight.skyColor.x);
            ImGui::DragFloat4("groundNormal", &lightConstant.hemisphereLight.groundNormal.x);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Fog"))
        {
            ImGui::ColorEdit4("fogColor", &fogConstants.fogColor.x);
            ImGui::SliderFloat("fogDensity", &fogConstants.fogDensity, 0.0f, 0.005f, "%.4f");
            ImGui::SliderFloat("fogHeightFalloff", &fogConstants.fogHeightFalloff, 0.0001f, 100.0f, "%.4f");
            ImGui::SliderFloat("fogCutoffDistance", &fogConstants.fogCutoffDistance, 0.0f, 1000.0f, "%.4f");
            ImGui::SliderFloat("startDistance", &fogConstants.startDistance, 0.0f, 100.0f, "%.4f");

            ImGui::SliderFloat("timeScale", &fogConstants.timeScale, 0.0f, 10.0f, "%.4f");
            ImGui::SliderFloat("speedScale", &fogConstants.speedScale, 0.0f, 0.5f, "%.4f");
            ImGui::TreePop();
        }

        if (ImGui::Button("button"))
        {
            isEntryLight = true;
        }

        ImGui::End();
    }
}

void Shader::SetDepthStencileState(int depthStencileState)
{
    Graphics::Instance().GetDeviceContext()->
        OMSetDepthStencilState(depthStencilStates[depthStencileState].Get(), 1);
}

void Shader::SetBlendState(int blendState)
{
    Graphics::Instance().GetDeviceContext()->
        OMSetBlendState(blendStates[blendState].Get(), nullptr, 0xFFFFFFFF);
}

void Shader::SetRasterizerState(int rasterizerState)
{
    Graphics::Instance().GetDeviceContext()->
        RSSetState(rasterizerStates[rasterizerState].Get());
}

void Shader::EntryLight()
{
    if (isEntryLight)
    {
        // 範囲を広げる
        if (lightConstant.spotLight.range <= 5.0f)lightConstant.spotLight.range += 0.01f;
        else lightConstant.spotLight.range = 5.0f;

        if (lightConstant.spotLight.range >= 5.0f)
        {
            // R
            lightConstant.spotLight.color.x = 1.0f;

            // G
            if (lightConstant.spotLight.color.y <= 0.2f)lightConstant.spotLight.color.y += 0.001f;
            else lightConstant.spotLight.color.y = 0.2f;

            // B
            if (lightConstant.spotLight.color.z <= 0.2f)lightConstant.spotLight.color.z += 0.001f;
            else lightConstant.spotLight.color.z = 0.2f;
        }
        else
        {
            lightConstant.spotLight.color.x = 1.0f;
            lightConstant.spotLight.color.y = 0.0f;
            lightConstant.spotLight.color.z = 0.0f;
        }

        if (lightConstant.spotLight.color.x == 1.0f &&
            lightConstant.spotLight.color.y == 0.2f &&
            lightConstant.spotLight.color.z == 0.2f)
            isEntryLight = false;
    }
    else
    {
        lightConstant.spotLight.range = 0.0f;

        // color
        lightConstant.spotLight.color.x = 0.0f;
        lightConstant.spotLight.color.y = 0.0f;
        lightConstant.spotLight.color.z = 0.0f;
    }
}

void Shader::EntryLight2()
{
    if (isEntryLight)
    {
        lightConstant.spotLight.range = 10.0f;

        // color
        // R
        if (lightConstant.spotLight.color.x <= 1.0f)lightConstant.spotLight.color.x += 0.00078f; //0.0039f;
        else lightConstant.spotLight.color.x = 1.0f;

        // G
        if (lightConstant.spotLight.color.y <= 1.0f)lightConstant.spotLight.color.y += 0.0039f; //0.0039f * 5;
        else lightConstant.spotLight.color.y = 1.0f;

        // B
        lightConstant.spotLight.color.z = 0.0f;

        if (lightConstant.spotLight.color.x >= 0.3f &&
            lightConstant.spotLight.color.y >= 1.0f &&
            lightConstant.spotLight.color.z == 0.0f)
            isEntryLight = false;
    }
    else
    {
        // color
        lightConstant.spotLight.color.x = 0.0f;
        lightConstant.spotLight.color.y = 0.0f;
        lightConstant.spotLight.color.z = 0.0f;
    }
}

void Shader::GSSetConstantBuffer()
{
    Graphics::Instance().GetDeviceContext()->GSSetConstantBuffers(1, 1, sceneConstantBuffer[0].GetAddressOf());
}
