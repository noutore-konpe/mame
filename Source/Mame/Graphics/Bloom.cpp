#include "Bloom.h"

#include <vector>

#include "../Resource/shader.h"
#include "../Other/misc.h"


Bloom::Bloom(ID3D11Device* device, uint32_t width, uint32_t height)
{
    bitBlockTransfer = std::make_unique<FullscreenQuad>(device);

    glowExtraction = std::make_unique<FrameBuffer>(device, width, height, false);

    for (size_t downsampledIndex = 0; downsampledIndex < downsampledCount; ++downsampledIndex)
    {
        gaussianBlur[downsampledIndex][0] = std::make_unique<FrameBuffer>(device, width >> downsampledIndex, height >> downsampledIndex, false);
        gaussianBlur[downsampledIndex][1] = std::make_unique<FrameBuffer>(device, width >> downsampledIndex, height >> downsampledIndex, false);
    }

    CreatePsFromCso(device, "./Resources/Shader/glowExtractionPs.cso", glowExtractionPs.GetAddressOf());
    CreatePsFromCso(device, "./Resources/Shader/gaussianBlurDownsamplingPs.cso", gaussianBlurDownsamplingPs.GetAddressOf());
    CreatePsFromCso(device, "./Resources/Shader/gaussianBlurHorizontalPs.cso", gaussianBlurHorizontalPs.GetAddressOf());
    CreatePsFromCso(device, "./Resources/Shader/gaussianBlurVerticalPs.cso", gaussianBlurVerticalPs.GetAddressOf());
    CreatePsFromCso(device, "./Resources/Shader/gaussianBlurUpsamplingPs.cso", gaussianBlurUpsamplingPs.GetAddressOf());

    HRESULT hr{ S_OK };

    D3D11_RASTERIZER_DESC rasterizerDesc{};
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_BACK;
    rasterizerDesc.FrontCounterClockwise = FALSE;
    rasterizerDesc.DepthBias = 0;
    rasterizerDesc.DepthBiasClamp = 0;
    rasterizerDesc.SlopeScaledDepthBias = 0;
    rasterizerDesc.DepthClipEnable = TRUE;
    rasterizerDesc.ScissorEnable = FALSE;
    rasterizerDesc.MultisampleEnable = FALSE;
    rasterizerDesc.AntialiasedLineEnable = FALSE;
    hr = device->CreateRasterizerState(&rasterizerDesc, rasterizerState.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
    depthStencilDesc.DepthEnable = FALSE;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
    hr = device->CreateDepthStencilState(&depthStencilDesc, depthStencilState.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    D3D11_BLEND_DESC blendDesc{};
    blendDesc.AlphaToCoverageEnable = FALSE;
    blendDesc.IndependentBlendEnable = FALSE;
    blendDesc.RenderTarget[0].BlendEnable = FALSE;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    hr = device->CreateBlendState(&blendDesc, blendState.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    D3D11_BUFFER_DESC bufferDesc{};
    bufferDesc.ByteWidth = sizeof(BloomConstants);
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;
    hr = device->CreateBuffer(&bufferDesc, nullptr, constantBuffer.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
}

void Bloom::Make(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* colorMap)
{
    // Œ»Ý‚Ìó‘Ô‚ð•Û‘¶‚·‚é
    ID3D11ShaderResourceView* nullShaderResourceView{};
    ID3D11ShaderResourceView* cachedShaderResourceViews[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT]{};
    deviceContext->PSGetShaderResources(0, downsampledCount, cachedShaderResourceViews);

    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> cachedDepthStencilState;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> cahedRasterizerState;
    Microsoft::WRL::ComPtr<ID3D11BlendState> cachedBlendState;
    FLOAT blendFactor[4];
    UINT sampleMask;
    deviceContext->OMGetDepthStencilState(cachedDepthStencilState.GetAddressOf(), 0);
    deviceContext->RSGetState(cahedRasterizerState.GetAddressOf());
    deviceContext->OMGetBlendState(cachedBlendState.GetAddressOf(), blendFactor, &sampleMask);

    Microsoft::WRL::ComPtr<ID3D11Buffer> cachedConstantBuffer;
    deviceContext->PSGetConstantBuffers(8, 1, cachedConstantBuffer.GetAddressOf());

    // Bind states
    deviceContext->OMSetDepthStencilState(depthStencilState.Get(), 0);
    deviceContext->RSSetState(rasterizerState.Get());
    deviceContext->OMSetBlendState(blendState.Get(), nullptr, 0xFFFFFFFF);

    BloomConstants data{};
    data.bloomExtractionThreshold = bloomExtractionThreshold;
    data.bloomIntensity = bloomIntensity;
    deviceContext->UpdateSubresource(constantBuffer.Get(), 0, 0, &data, 0, 0);
    deviceContext->PSSetConstantBuffers(8, 1, constantBuffer.GetAddressOf());

    // –¾‚é‚¢F‚ð’Šo‚·‚é
    glowExtraction->Clear(deviceContext, 0, 0, 0, 1);
    glowExtraction->Activate(deviceContext);
    bitBlockTransfer->Blit(deviceContext, &colorMap, 0, 1, glowExtractionPs.Get());
    glowExtraction->Deactivate(deviceContext);
    deviceContext->PSSetShaderResources(0, 1, &nullShaderResourceView);

    // Gaussian blur
    gaussianBlur[0][0]->Clear(deviceContext, 0, 0, 0, 1);
    gaussianBlur[0][0]->Activate(deviceContext);
    bitBlockTransfer->Blit(deviceContext, glowExtraction->shaderResourceViews[0].GetAddressOf(), 0, 1, gaussianBlurDownsamplingPs.Get());
    gaussianBlur[0][0]->Deactivate(deviceContext);
    deviceContext->PSSetShaderResources(0, 1, &nullShaderResourceView);

    // Ping-pong gaussian blur
    gaussianBlur[0][1]->Clear(deviceContext, 0, 0, 0, 1);
    gaussianBlur[0][1]->Activate(deviceContext);
    bitBlockTransfer->Blit(deviceContext, gaussianBlur[0][0]->shaderResourceViews[0].GetAddressOf(), 0, 1, gaussianBlurHorizontalPs.Get());
    gaussianBlur[0][1]->Deactivate(deviceContext);
    deviceContext->PSSetShaderResources(0, 1, &nullShaderResourceView);

    gaussianBlur[0][0]->Clear(deviceContext, 0, 0, 0, 1);
    gaussianBlur[0][0]->Activate(deviceContext);
    bitBlockTransfer->Blit(deviceContext, gaussianBlur[0][1]->shaderResourceViews[0].GetAddressOf(), 0, 1, gaussianBlurVerticalPs.Get());
    gaussianBlur[0][0]->Deactivate(deviceContext);
    deviceContext->PSSetShaderResources(0, 1, &nullShaderResourceView);

    for (size_t downsampledIndex = 1; downsampledIndex < downsampledCount; ++downsampledIndex)
    {
        // Downsampling
        gaussianBlur[downsampledIndex][0]->Clear(deviceContext, 0, 0, 0, 1);
        gaussianBlur[downsampledIndex][0]->Activate(deviceContext);
        bitBlockTransfer->Blit(deviceContext, gaussianBlur[downsampledIndex - 1][0]->shaderResourceViews[0].GetAddressOf(), 0, 1, gaussianBlurDownsamplingPs.Get());
        gaussianBlur[downsampledIndex][0]->Deactivate(deviceContext);
        deviceContext->PSSetShaderResources(0, 1, &nullShaderResourceView);

        // Ping-pong gaussian blur
        gaussianBlur[downsampledIndex][1]->Clear(deviceContext, 0, 0, 0, 1);
        gaussianBlur[downsampledIndex][1]->Activate(deviceContext);
        bitBlockTransfer->Blit(deviceContext, gaussianBlur[downsampledIndex][0]->shaderResourceViews[0].GetAddressOf(), 0, 1, gaussianBlurHorizontalPs.Get());
        gaussianBlur[downsampledIndex][1]->Deactivate(deviceContext);
        deviceContext->PSSetShaderResources(0, 1, &nullShaderResourceView);

        gaussianBlur[downsampledIndex][0]->Clear(deviceContext, 0, 0, 0, 1);
        gaussianBlur[downsampledIndex][0]->Activate(deviceContext);
        bitBlockTransfer->Blit(deviceContext, gaussianBlur[downsampledIndex][1]->shaderResourceViews[0].GetAddressOf(), 0, 1, gaussianBlurVerticalPs.Get());
        gaussianBlur[downsampledIndex][0]->Deactivate(deviceContext);
        deviceContext->PSSetShaderResources(0, 1, &nullShaderResourceView);
    }

    // Downsampling
    glowExtraction->Clear(deviceContext, 0, 0, 0, 1);
    glowExtraction->Activate(deviceContext);
    std::vector<ID3D11ShaderResourceView*> shaderResourceViews;
    for (size_t downsampledIndex = 0; downsampledIndex < downsampledCount; ++downsampledIndex)
    {
        shaderResourceViews.push_back(gaussianBlur[downsampledIndex][0]->shaderResourceViews[0].Get());
    }
    bitBlockTransfer->Blit(deviceContext, shaderResourceViews.data(), 0, downsampledCount, gaussianBlurUpsamplingPs.Get());
    glowExtraction->Deactivate(deviceContext);
    deviceContext->PSSetShaderResources(0, 1, &nullShaderResourceView);

    // ó‘Ô‚ð•œŒ³‚·‚é
    deviceContext->PSSetConstantBuffers(8, 1, cachedConstantBuffer.GetAddressOf());

    deviceContext->OMSetDepthStencilState(cachedDepthStencilState.Get(), 0);
    deviceContext->RSSetState(cahedRasterizerState.Get());
    deviceContext->OMSetBlendState(cachedBlendState.Get(), blendFactor, sampleMask);

    deviceContext->PSSetShaderResources(0, downsampledCount, cachedShaderResourceViews);
    for (ID3D11ShaderResourceView* cachedShaderResourceView : cachedShaderResourceViews)
    {
        if (cachedShaderResourceView) cachedShaderResourceView->Release();
    }
}