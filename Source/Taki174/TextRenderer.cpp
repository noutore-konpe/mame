#include "TextRenderer.h"

#include "../Mame/Other/misc.h"

TextRenderer::TextRenderer(
    ID3D11Device* device, const wchar_t* const fontName)
{
    // �t�H���g�n���h���̍쐬
    static constexpr int fontSize   = 64;
    static constexpr int fontWeight = 1000;
    const wchar_t* font  = (0 == ::wcscmp(fontName, L"")) ? fontName : L"MS P����";
    const LOGFONT logFont = {
        fontSize, 0, 0, 0, fontWeight, 0, 0, 0,
        SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS,
        PROOF_QUALITY, DEFAULT_PITCH | FF_MODERN, (*font)
    };
    const HFONT hFont = ::CreateFontIndirectW(&logFont);

    // ���݂̃E�B���h�E�ɓK�p
    // �f�o�C�X�Ƀt�H���g���������Ȃ���GetGlyphOutline�֐��̓G���[�ƂȂ�
    const HDC hdc = ::GetDC(NULL);
    //HFONT oldFont = static_cast<HFONT>(::SelectObject(hdc, hFont));
    ::SelectObject(hdc, hFont);


    // �t�H���g�r�b�g�}�b�v�擾
    const wchar_t* c        = L"S";
    const UINT     code     = (UINT) * c;
    const int      gradFlag = GGO_GRAY4_BITMAP;

    // �~���̍ő�l
    int grad = 0;
    switch (gradFlag)
    {
    case GGO_GRAY2_BITMAP: grad = 4;  break;
    case GGO_GRAY4_BITMAP: grad = 16; break;
    case GGO_GRAY8_BITMAP: grad = 64; break;
    }

    // �t�H���g�̓ǂݍ���
    {
        GetTextMetrics(hdc, &textMetric_);

        const MAT2  mat  = { { 0,1 }, { 0,0 }, { 0,0 }, { 0,1 } };
        const DWORD size = ::GetGlyphOutlineW(hdc, code, gradFlag, &glyphMetrics_, 0, nullptr, &mat);

        //pMono = new BYTE[size];
        pMono_ = std::make_unique<BYTE[]>(size);
        ::GetGlyphOutlineW(hdc, code, gradFlag, &glyphMetrics_, size, pMono_.get(), &mat);
    }

    //�t�H���g���������ރe�N�X�`���쐬
    {
        //�t�H���g�̕��ƍ���
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
    // �t�H���g�����e�N�X�`���ɏ�������
    HRESULT hr = S_OK;

    D3D11_MAPPED_SUBRESOURCE hMappedResource = {};
    hr = dc->Map(fontTexture_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &hMappedResource);

    // �����ŏ�������
    BYTE* pBits = static_cast<BYTE*>(hMappedResource.pData);

    // �t�H���g���̏������݁i�r�b�g�}�b�v�f�[�^�ɏ]���e�N�X�`���ɓ_��ł�)
    // iOfs_x, iOfs_y : �����o���ʒu(����)
    // iBmp_w, iBmp_h : �t�H���g�r�b�g�}�b�v�̕���
    // Level : ���l�̒i�K (GGO_GRAY4_BITMAP�Ȃ̂�17�i�K)
    const int iOfs_x = glyphMetrics_.gmptGlyphOrigin.x;
    const int iOfs_y = textMetric_.tmAscent - glyphMetrics_.gmptGlyphOrigin.y;
    const int iBmp_w = glyphMetrics_.gmBlackBoxX + (4 - (glyphMetrics_.gmBlackBoxX % 4)) % 4;
    const int iBmp_h = glyphMetrics_.gmBlackBoxY;
    const int Level  = 17;
    DWORD Alpha = {};
    DWORD Color = {};

    // �L���X�g����
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

    //�s�v�Ȃ̂ō폜
    //delete[] pMono;
}
