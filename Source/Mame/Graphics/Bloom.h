#pragma once

#include <memory>
#include <d3d11.h>
#include <wrl.h>

#include "../FrameBuffer.h"
#include "../FullscreenQuad.h"

class Bloom
{
public:
    Bloom(ID3D11Device* device, uint32_t width, uint32_t height);
    ~Bloom() = default;
    Bloom(const Bloom&) = delete;
    Bloom& operator =(const Bloom&) = delete;
    Bloom(Bloom&&) noexcept = delete;
    Bloom& operator =(Bloom&&) noexcept = delete;

    void Bloom::Make(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* colorMap);
    ID3D11ShaderResourceView* ShaderResourceView()const
    {
        return glowExtraction->shaderResourceViews[0].Get();
    }

public:
    float bloomExtractionThreshold = 0.85f;
    float bloomIntensity = 1.0f;

private:
    std::unique_ptr<FullscreenQuad> bitBlockTransfer;
    std::unique_ptr<FrameBuffer> glowExtraction;

    static const size_t downsampledCount = 6;
    std::unique_ptr<FrameBuffer> gaussianBlur[downsampledCount][2];

    Microsoft::WRL::ComPtr<ID3D11PixelShader> glowExtractionPs;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> gaussianBlurDownsamplingPs;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> gaussianBlurHorizontalPs;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> gaussianBlurVerticalPs;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> gaussianBlurUpsamplingPs;

    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
    Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;

    struct BloomConstants
    {
        float bloomExtractionThreshold;
        float bloomIntensity;
        float something[2];
    };
    Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;
};

