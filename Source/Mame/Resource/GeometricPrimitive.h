#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

#include <wrl.h>



class GeometricPrimitive
{
public:
    struct vertex
    {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT3 normal;
    };
    struct constants
    {
        DirectX::XMFLOAT4X4 world;
        DirectX::XMFLOAT4 material_color;
    };

private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> index_buffer;

    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> input_layout;
    Microsoft::WRL::ComPtr<ID3D11Buffer> constant_buffer;

public:
    GeometricPrimitive(ID3D11Device* device);
    GeometricPrimitive(ID3D11Device* device, DirectX::XMFLOAT3 center, DirectX::XMFLOAT3 range);
    virtual ~GeometricPrimitive() = default;

    void render(ID3D11DeviceContext* immediate_context,
        const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& material_color);

protected:
    void create_com_buffers(
        ID3D11Device* device,
        const vertex* vertices, const size_t& vertex_count,
        const uint32_t* indices, const size_t& index_count
    );
};


// AABB（当たり判定確認用）
class GeometricAABB : public GeometricPrimitive
{
public:
    GeometricAABB(
        ID3D11Device* device,
        const DirectX::XMFLOAT3& min,
        const DirectX::XMFLOAT3& max
    );
};