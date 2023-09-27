#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <memory>
#include <vector>

#define MAX_EMITTERS 256
class AuraEffect
{
public:
    struct Emitter
    {
        DirectX::XMFLOAT3 position = {};
        float type = 0;
    };

    AuraEffect(ID3D11Device* device);
    void Render(ID3D11DeviceContext* deviceContext, const std::vector<Emitter>& emitters, const DirectX::XMFLOAT4X4& world, ID3D11ShaderResourceView* shaderResourceView);

private:
    struct Constants
    {
        DirectX::XMFLOAT4X4 world;
    };

    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;

    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
    Microsoft::WRL::ComPtr<ID3D11GeometryShader> geometryShader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
    Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;
};