#include "AuraEffect.h"

#include "../Resource/shader.h"
#include "../Other/misc.h"

AuraEffect::AuraEffect(ID3D11Device* device)
{
    HRESULT hr = S_OK;

    D3D11_BUFFER_DESC bufferDesc = {};
    D3D11_SUBRESOURCE_DATA subresourceData = {};
    bufferDesc.ByteWidth = static_cast<UINT>(sizeof(Emitter) * MAX_EMITTERS);
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;
    hr = device->CreateBuffer(&bufferDesc, NULL, vertexBuffer.ReleaseAndGetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    D3D11_INPUT_ELEMENT_DESC inputElementDesc[]
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TYPE" , 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    CreateVsFromCso(device, "./Resources/Shader/AuraEffectVS.cso", vertexShader.GetAddressOf(), inputLayout.GetAddressOf(), inputElementDesc, ARRAYSIZE(inputElementDesc));
    CreateGsFromCso(device, "./Resources/Shader/AuraEffectGS.cso", geometryShader.GetAddressOf());
    CreatePsFromCso(device, "./Resources/Shader/AuraEffectPS.cso", pixelShader.GetAddressOf());

    bufferDesc.ByteWidth = sizeof(Constants);
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    hr = device->CreateBuffer(&bufferDesc, nullptr, constantBuffer.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
}

void AuraEffect::Render(ID3D11DeviceContext* deviceContext, const std::vector<Emitter>& emitters, const DirectX::XMFLOAT4X4& world, ID3D11ShaderResourceView* shaderResourceView)
{
    HRESULT hr{ S_OK };
    D3D11_MAPPED_SUBRESOURCE mappedSubresource = {};
    hr = deviceContext->Map(vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    size_t vertexCount = emitters.size();
    _ASSERT_EXPR(MAX_EMITTERS >= vertexCount, "Buffer overflow");
    Emitter* p = reinterpret_cast<Emitter*>(mappedSubresource.pData);

    for (size_t i = 0; i < emitters.size(); ++i)
    {
        p[i].position = emitters.at(i).position;
        p[i].type = emitters.at(i).type;
    }
    deviceContext->Unmap(vertexBuffer.Get(), 0);

    uint32_t stride{ sizeof(Emitter) };
    uint32_t offset{ 0 };
    deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
    deviceContext->IASetIndexBuffer(NULL, DXGI_FORMAT_UNKNOWN, 0);
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
    deviceContext->IASetInputLayout(inputLayout.Get());

    deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
    deviceContext->GSSetShader(geometryShader.Get(), nullptr, 0);
    deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);

    Constants data = { world };
    deviceContext->UpdateSubresource(constantBuffer.Get(), 0, 0, &data, 0, 0);
    deviceContext->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
    deviceContext->GSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
    deviceContext->PSSetShaderResources(0, 1, &shaderResourceView);
    deviceContext->Draw(static_cast<UINT>(emitters.size()), 0);

    deviceContext->VSSetShader(NULL, nullptr, 0);
    deviceContext->GSSetShader(NULL, nullptr, 0);
    deviceContext->PSSetShader(NULL, nullptr, 0);
}
