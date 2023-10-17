#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <memory>
#include <Windows.h>
#include <wingdi.h>

class TextRenderer
{
public:
    TextRenderer(ID3D11Device* device, const wchar_t* const fontName = L"");

    void Render(ID3D11DeviceContext* dc);

private:
    Microsoft::WRL::ComPtr<ID3D11Texture2D> fontTexture_;

    std::unique_ptr<BYTE[]> pMono_;

    TEXTMETRIC   textMetric_   = {};
    GLYPHMETRICS glyphMetrics_ = {};

};

