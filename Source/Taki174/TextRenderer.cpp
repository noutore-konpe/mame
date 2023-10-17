#include "TextRenderer.h"

#include "../Mame/Other/misc.h"

TextRenderer::TextRenderer(
    ID3D11Device* device, const wchar_t* const fontName)
{
    // フォントハンドルの作成
    static constexpr int fontSize   = 64;
    static constexpr int fontWeight = 1000;
    const wchar_t* font  = (0 == ::wcscmp(fontName, L"")) ? fontName : L"MS P明朝";
    const LOGFONT logFont = {
        fontSize, 0, 0, 0, fontWeight, 0, 0, 0,
        SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS,
        PROOF_QUALITY, DEFAULT_PITCH | FF_MODERN, (*font)
    };
    const HFONT hFont = ::CreateFontIndirectW(&logFont);

    // 現在のウィンドウに適用
    // デバイスにフォントを持たせないとGetGlyphOutline関数はエラーとなる
    const HDC hdc = ::GetDC(NULL);
    //HFONT oldFont = static_cast<HFONT>(::SelectObject(hdc, hFont));
    ::SelectObject(hdc, hFont);


    // フォントビットマップ取得
    const wchar_t* c        = L"S";
    const UINT     code     = (UINT) * c;
    const int      gradFlag = GGO_GRAY4_BITMAP;

    // 諧調の最大値
    int grad = 0;
    switch (gradFlag)
    {
    case GGO_GRAY2_BITMAP: grad = 4;  break;
    case GGO_GRAY4_BITMAP: grad = 16; break;
    case GGO_GRAY8_BITMAP: grad = 64; break;
    }

    // フォントの読み込み
    {
        GetTextMetrics(hdc, &textMetric_);

        const MAT2  mat  = { { 0,1 }, { 0,0 }, { 0,0 }, { 0,1 } };
        const DWORD size = ::GetGlyphOutlineW(hdc, code, gradFlag, &glyphMetrics_, 0, nullptr, &mat);

        //pMono = new BYTE[size];
        pMono_ = std::make_unique<BYTE[]>(size);
        ::GetGlyphOutlineW(hdc, code, gradFlag, &glyphMetrics_, size, pMono_.get(), &mat);
    }

    //フォントを書き込むテクスチャ作成
    {
        //フォントの幅と高さ
        const int fontWidth  = glyphMetrics_.gmCellIncX;
        const int fontHeight = textMetric_.tmHeight;

        D3D11_TEXTURE2D_DESC desc = {};
        ::memset(&desc, 0, sizeof(desc));
        desc.Width               = fontWidth;
        desc.Height              = fontHeight;
        desc.MipLevels           = 1;
        desc.ArraySize           = 1;
        desc.Format              = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count    = 1;
        desc.SampleDesc.Quality  = 0;
        desc.Usage               = D3D11_USAGE_DYNAMIC;
        desc.BindFlags           = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
        desc.MiscFlags           = 0;
        HRESULT hr = device->CreateTexture2D(&desc, nullptr, fontTexture_.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), ::hr_trace(hr));
    }

}

void TextRenderer::Render(ID3D11DeviceContext* dc)
{
    // フォント情報をテクスチャに書き込む
    HRESULT hr = S_OK;

    D3D11_MAPPED_SUBRESOURCE hMappedResource = {};
    hr = dc->Map(fontTexture_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &hMappedResource);

    // ここで書き込む
    BYTE* pBits = static_cast<BYTE*>(hMappedResource.pData);

    // フォント情報の書き込み（ビットマップデータに従いテクスチャに点を打つ)
    // iOfs_x, iOfs_y : 書き出し位置(左上)
    // iBmp_w, iBmp_h : フォントビットマップの幅高
    // Level : α値の段階 (GGO_GRAY4_BITMAPなので17段階)
    const int iOfs_x = glyphMetrics_.gmptGlyphOrigin.x;
    const int iOfs_y = textMetric_.tmAscent - glyphMetrics_.gmptGlyphOrigin.y;
    const int iBmp_w = glyphMetrics_.gmBlackBoxX + (4 - (glyphMetrics_.gmBlackBoxX % 4)) % 4;
    const int iBmp_h = glyphMetrics_.gmBlackBoxY;
    const int Level  = 17;
    DWORD Alpha = {};
    DWORD Color = {};

    // キャスト分岐
    if (textMetric_.tmHeight < 0)
    {
        const LONG size = static_cast<LONG>(hMappedResource.RowPitch) * textMetric_.tmHeight;
        ::memset(pBits, 0, size);
    }
    else
    {
        const UINT size = hMappedResource.RowPitch * static_cast<UINT>(textMetric_.tmHeight);
        ::memset(pBits, 0, size);
    }
    for (int y = iOfs_y; y < iOfs_y + iBmp_h; ++y)
    {
        for (int x = iOfs_x; x < iOfs_x + iBmp_w; ++x)
        {
            Alpha = (255 * pMono_.get()[x - iOfs_x + iBmp_w * (y - iOfs_y)]) / (Level - 1);

            Color = 0x00ffffff | (Alpha << 24);

            ::memcpy(
                pBits + (hMappedResource.RowPitch * y) + (4 * x),
                &Color, sizeof(DWORD)
            );
        }
    }
    dc->Unmap(fontTexture_.Get(), 0);

    //不要なので削除
    //delete[] pMono;
}
