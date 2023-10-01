#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <DirectXMath.h>

class Metamorphose
{
private:
    struct Vertex
    {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT2 texcoord;
        DirectX::XMFLOAT3 normal;
    };

    struct PrimitiveConstants
    {
        DirectX::XMFLOAT4X4 world;
        DirectX::XMFLOAT4 eyeInLocalSpace; // w : テッセレーションの最大分割数
        float verticalScale;
        float something[3];
    };

public:
    Metamorphose(ID3D11Device* device, const char* filename);
    virtual ~Metamorphose() = default;

    void Render(ID3D11DeviceContext* deviceContext);

    float verticalScale = 0.0f;
    float tesselationMaxSubdivision = 4;

private:    
    UINT vertexCount;
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;

    Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
    Microsoft::WRL::ComPtr<ID3D11HullShader> hullShader;
    Microsoft::WRL::ComPtr<ID3D11DomainShader> domainShader;
    Microsoft::WRL::ComPtr<ID3D11GeometryShader> geometryShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;

    D3D11_TEXTURE2D_DESC textture2dDesc;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceViews[3];
};