#pragma once

#include <d3d11.h>
#include <directxmath.h>
#include <wrl.h>

class Sprite
{
public:
    Sprite(ID3D11Device* device, const wchar_t* filename, const char* psFilename = nullptr);
    ~Sprite();

public:
    struct SpriteTransform
    {
    private:
        DirectX::XMFLOAT2 pos = {};
        DirectX::XMFLOAT2 size = { 100,100 };
        DirectX::XMFLOAT4 color = { 1,1,1,1 };
        float angle = 0.0f;
        DirectX::XMFLOAT2 texPos = {};
        DirectX::XMFLOAT2 texSize = { 100,100 };

    public:
        void DrawDebug();

        void SetPos(DirectX::XMFLOAT2 p) { pos = p; }
        void SetSize(DirectX::XMFLOAT2 s) { size = s; }
        void SetColor(DirectX::XMFLOAT4 c) { color = c; }
        void SetAngle(float a) { angle = a; }
        void SetTexPos(DirectX::XMFLOAT2 texP) { texPos = texP; }
        void SetTexSize(DirectX::XMFLOAT2 texS) { texSize = texS; }

        DirectX::XMFLOAT2 GetPos() { return pos; }
        DirectX::XMFLOAT2 GetSize() { return size; }
        DirectX::XMFLOAT4 GetColor() { return color; }
        float GetAngle() { return angle; }
        DirectX::XMFLOAT2 GetTexPos() { return texPos; }
        DirectX::XMFLOAT2 GetTexSize() { return texSize; }
    };

public:
    void Render();
    void Render(ID3D11DeviceContext* deviceContext, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 size);
    void Render(ID3D11DeviceContext* deviceContext, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 size, DirectX::XMFLOAT4 color);
    void Render(ID3D11DeviceContext* deviceContext, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 size, DirectX::XMFLOAT4 color, float angle/*degree*/);    
    void Render(ID3D11DeviceContext* deviceContext, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 size, DirectX::XMFLOAT4 color, float angle/*degree*/, DirectX::XMFLOAT2 texPos, DirectX::XMFLOAT2 texSize); // –{‘Ì

    void DrawDebug();

    SpriteTransform* GetSpriteTransform() { return &spriteTransform; }

private:
    SpriteTransform spriteTransform;

private:
    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView;
    D3D11_TEXTURE2D_DESC texture2d_desc;

    struct Vertex
    {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT4 color;
        DirectX::XMFLOAT2 texcord;
    };
};