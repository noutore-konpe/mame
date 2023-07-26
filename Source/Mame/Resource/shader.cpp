#include "shader.h"
#include "../Graphics/Camera.h"

#include <fstream>

#include "../Other/misc.h"

#include "skinned_mesh.h"


HRESULT create_vs_from_cso(ID3D11Device* device, const char* cso_name, ID3D11VertexShader** vertex_shader,
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

HRESULT create_ps_from_cso(ID3D11Device* device, const char* cso_name, ID3D11PixelShader** pixel_shader)
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
        hr = device->CreateBuffer(&bufferDesc, nullptr, sceneConstantBuffer[0].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        bufferDesc.ByteWidth = sizeof(CBParametric);
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bufferDesc.CPUAccessFlags = 0;
        bufferDesc.MiscFlags = 0;
        bufferDesc.StructureByteStride = 0;
        hr = device->CreateBuffer(&bufferDesc, nullptr, sceneConstantBuffer[1].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        {
            bufferDesc.ByteWidth = sizeof(LightConstants);
            hr = device->CreateBuffer(&bufferDesc, nullptr, lightConstantBuffer.GetAddressOf());
            _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
        }
    }

    // ブレンドステート
    {
        HRESULT hr{ S_OK };

        D3D11_BLEND_DESC blend_desc{};
        // ピクセルをレンダーターゲットに設定するときに、
        //マルチサンプリング手法としてアルファ隊カバレッジを使用するかどうかを指定します
        blend_desc.AlphaToCoverageEnable = FALSE;
        // 同時レンダーターゲットで独立したブレンドを有効にするかどうかを設定します。
        // 独立したブレンドを有効にするには、TUREに設定します
        // FALSEに設定すると、RenderTarget[0]メンバーのみが使用されます。RenderTarget[1..7]は無視されます
        blend_desc.IndependentBlendEnable = FALSE;
        // D3D11_RENDER_TARGET_BLEND_DESC[8]
        blend_desc.RenderTarget[0].BlendEnable = TRUE;
        blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
        blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        hr = device->CreateBlendState(&blend_desc, blendState.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    }

    // 深度ステンシルステート
    {
        HRESULT hr{ S_OK };

        D3D11_DEPTH_STENCIL_DESC depth_stencil_desc{};
        // 0 深度テスト：オン　深度ライト：オン
        depth_stencil_desc.DepthEnable = TRUE;	// 深度テストを有効にします
        depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;	// 深度データで変更できる深度ステンシルバッファーの一部を特定します
        depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL; // 深度データと既存データを比較する関数
        hr = device->CreateDepthStencilState(&depth_stencil_desc, depthStencilState[0].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        // 1 深度テスト：オン　深度ライト：オフ
        depth_stencil_desc.DepthEnable = TRUE;
        depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
        hr = device->CreateDepthStencilState(&depth_stencil_desc, depthStencilState[1].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        // 2 深度テスト：オフ　深度ライト：オン
        depth_stencil_desc.DepthEnable = FALSE;
        depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        hr = device->CreateDepthStencilState(&depth_stencil_desc, depthStencilState[2].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        // 3 深度テスト：オフ　深度ライト：オフ
        depth_stencil_desc.DepthEnable = FALSE;
        depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
        hr = device->CreateDepthStencilState(&depth_stencil_desc, depthStencilState[3].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    }

    // ラスタライザーステート
    {
        HRESULT hr{ S_OK };

        D3D11_RASTERIZER_DESC rasterizer_desc{};
        rasterizer_desc.FillMode = D3D11_FILL_SOLID;    // レンダリング時に使用するフィルモード
        rasterizer_desc.CullMode = D3D11_CULL_BACK;     // 指定した方向に面する三角形が描画されないことを示す
        rasterizer_desc.FrontCounterClockwise = TRUE;  // 三角形が表向きかを決定する
        rasterizer_desc.DepthBias = 0;                  // 指定されたピクセルに追加された深度値。
        rasterizer_desc.DepthBiasClamp = 0;             // ピクセルの最大深度バイアス
        rasterizer_desc.SlopeScaledDepthBias = 0;       // 指定されたピクセルの傾きのスカラー
        rasterizer_desc.DepthClipEnable = TRUE;         // 距離に基づいてクリッピングを有効にします
        rasterizer_desc.ScissorEnable = FALSE;          // ハサミ四角形カリングを有効にします。
        rasterizer_desc.MultisampleEnable = FALSE;      // マルチサンプリングアンチエイリアシング(MSAA)レンダーターゲットで
                                                        // 四角形またはアルファ線のアンチエイリアシングアルゴリズムを使用するかどうかを指定します
        rasterizer_desc.AntialiasedLineEnable = FALSE;  // 回線アンチエイリアシングを有効にするかどうか

        hr = device->CreateRasterizerState(&rasterizer_desc, rasterizerState[0].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        rasterizer_desc.CullMode = D3D11_CULL_NONE;
        hr = device->CreateRasterizerState(&rasterizer_desc, rasterizerState[3].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        rasterizer_desc.FillMode = D3D11_FILL_WIREFRAME;
        rasterizer_desc.CullMode = D3D11_CULL_BACK;
        rasterizer_desc.AntialiasedLineEnable = TRUE;
        hr = device->CreateRasterizerState(&rasterizer_desc, rasterizerState[1].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        rasterizer_desc.FillMode = D3D11_FILL_WIREFRAME;
        rasterizer_desc.CullMode = D3D11_CULL_NONE;
        rasterizer_desc.AntialiasedLineEnable = TRUE;
        hr = device->CreateRasterizerState(&rasterizer_desc, rasterizerState[2].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    }

    // サンプラステート
    {
        HRESULT hr{ S_OK };

        D3D11_SAMPLER_DESC sampler_desc{};
        sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
        sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        sampler_desc.MipLODBias = 0;
        sampler_desc.MaxAnisotropy = 16;
        sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
        sampler_desc.BorderColor[0] = 0;
        sampler_desc.BorderColor[1] = 0;
        sampler_desc.BorderColor[2] = 0;
        sampler_desc.BorderColor[3] = 0;
        sampler_desc.MinLOD = 0;
        sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
        hr = device->CreateSamplerState(&sampler_desc, samplerState[0].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        hr = device->CreateSamplerState(&sampler_desc, samplerState[1].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        sampler_desc.Filter = D3D11_FILTER_ANISOTROPIC;
        sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        hr = device->CreateSamplerState(&sampler_desc, samplerState[2].GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
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
    deviceContext->PSSetSamplers(0, 1, samplerState[0].GetAddressOf());
    deviceContext->PSSetSamplers(1, 1, samplerState[1].GetAddressOf());
    deviceContext->PSSetSamplers(2, 1, samplerState[2].GetAddressOf());

    deviceContext->OMSetDepthStencilState(depthStencilState[0].Get(), 1);

    deviceContext->OMSetBlendState(blendState.Get(), nullptr, 0xFFFFFFFF);

    deviceContext->RSSetState(rasterizerState[0].Get());

    // ビュープロジェクション変換行列を計算し、それを定数バッファにセットする
#if 0
    D3D11_VIEWPORT viewport{};
    UINT unm_viewports{ 1 };
    dc->RSGetViewports(&unm_viewports, &viewport);

    float aspect_ratio{ viewport.Width / viewport.Height };
    DirectX::XMMATRIX P{ DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(30),aspect_ratio,0.1f,100.0f) };
    //DirectX::XMMATRIX P{ DirectX::XMMatrixOrthographicLH(view.viewWitdh,view.viewHeight,view.nearZ,view.farZ) };

    DirectX::XMVECTOR eye{ DirectX::XMVectorSet(0.0f,0.0f,-10.0f,1.0f) };
    DirectX::XMVECTOR focus{ DirectX::XMVectorSet(0.0f,0.0f,0.0f,1.0f) };
    DirectX::XMVECTOR up{ DirectX::XMVectorSet(0.0f,1.0f,0.0f,0.0f) };
    DirectX::XMMATRIX V{ DirectX::XMMatrixLookAtLH(eye,focus,up) };
#else
    camera.SetPerspectiveFov(deviceContext);
#endif
    

    SceneConstants scene{};
    //DirectX::XMStoreFloat4x4(&data.viewProjection, V * P);
    DirectX::XMStoreFloat4x4(&scene.viewProjection, camera.GetViewMatrix() * camera.GetProjectionMatrix());
    
    scene.lightDirection = { view.position.x,view.position.y,view.position.z,view.position.w };
    scene.cameraPosition = { view.camera.x,view.camera.y,view.camera.z,view.camera.w };
    deviceContext->UpdateSubresource(sceneConstantBuffer[0].Get(), 0, 0, &scene, 0, 0);
    deviceContext->VSSetConstantBuffers(1, 1, sceneConstantBuffer[0].GetAddressOf());
    deviceContext->PSSetConstantBuffers(1, 1, sceneConstantBuffer[0].GetAddressOf());

    // light
    LightConstants lights{};
    lights.ambientColor = ambientColor;
    lights.directionalLightDirection = directionalLightDirection;
    lights.directionalLightColor = directionalLightColor;
    deviceContext->UpdateSubresource(lightConstantBuffer.Get(), 0, 0, &lights, 0, 0);
    deviceContext->VSSetConstantBuffers(2, 1, lightConstantBuffer.GetAddressOf());
    deviceContext->PSSetConstantBuffers(2, 1, lightConstantBuffer.GetAddressOf());


#ifdef USE_IMGUI
    //DrawDebug();
#endif
}

void Shader::Begin(ID3D11DeviceContext* dc, const RenderContext& rc, int state)
{
    Camera& camera = Camera::Instance();

    // サンプラーステート
    dc->PSSetSamplers(0, 1, samplerState[0].GetAddressOf());
    dc->PSSetSamplers(1, 1, samplerState[1].GetAddressOf());
    dc->PSSetSamplers(2, 1, samplerState[2].GetAddressOf());

    dc->OMSetDepthStencilState(depthStencilState[0].Get(), 1);

    dc->OMSetBlendState(blendState.Get(), nullptr, 0xFFFFFFFF);

    dc->RSSetState(rasterizerState[0].Get());
    
    camera.SetPerspectiveFov(dc);

    SceneConstants data{};
    DirectX::XMStoreFloat4x4(&data.viewProjection, camera.GetViewMatrix() * camera.GetProjectionMatrix());
    data.lightDirection = { view.position.x,view.position.y,view.position.z,view.position.w };
    data.cameraPosition = { view.camera.x,view.camera.y,view.camera.z,view.camera.w };
    dc->UpdateSubresource(sceneConstantBuffer[0].Get(), 0, 0, &data, 0, 0);
    dc->VSSetConstantBuffers(1, 1, sceneConstantBuffer[0].GetAddressOf());
    dc->PSSetConstantBuffers(1, 1, sceneConstantBuffer[0].GetAddressOf());
}

void Shader::SetState(ID3D11DeviceContext* dc, int RastarizeState, int DepthStencilState, int SamplerState)
{
    dc->PSSetSamplers(SamplerState, 1, samplerState[SamplerState].GetAddressOf());
    dc->RSSetState(rasterizerState[RastarizeState].Get());
    dc->OMSetDepthStencilState(depthStencilState[DepthStencilState].Get(), 1);
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

    if (ImGui::TreeNode("phong"))
    {
        
        ImGui::Separator();
        ImGui::ColorEdit3("ambientColor", &ambientColor.x);
        ImGui::SliderFloat3("directionalLightDirection", &directionalLightDirection.x, -1.0f, 1.0f);
        ImGui::ColorEdit3("directionalLightColor", &directionalLightColor.x);

        ImGui::TreePop();
    }
    
    camera.DrawDebug();

    ImGui::End();
}
