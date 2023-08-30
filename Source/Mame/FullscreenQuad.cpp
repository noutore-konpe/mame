#include "FullscreenQuad.h"
#include "Resource/shader.h"
#include "Other/misc.h"

FullscreenQuad::FullscreenQuad(ID3D11Device* device)
{
    CreateVsFromCso(device, "./resources/Shader/fullscreen_quad_vs.cso", embeddedVertexShader.ReleaseAndGetAddressOf(),
        nullptr, nullptr, 0);
    CreatePsFromCso(device, "./resources/Shader/fullscreen_quad_ps.cso", embeddedPixelShader.ReleaseAndGetAddressOf());
}

void FullscreenQuad::Blit(ID3D11DeviceContext* deviceContext,
    ID3D11ShaderResourceView** shaderResourceView, uint32_t startSlot, uint32_t numViews, 
    ID3D11PixelShader* replacedPixelShader)
{
    deviceContext->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    deviceContext->IASetInputLayout(nullptr);

    deviceContext->VSSetShader(embeddedVertexShader.Get(), 0, 0);
    replacedPixelShader ? deviceContext->PSSetShader(replacedPixelShader, 0, 0) :
        deviceContext->PSSetShader(embeddedPixelShader.Get(), 0, 0);

    deviceContext->PSSetShaderResources(startSlot, numViews, shaderResourceView);

    deviceContext->Draw(4, 0);
}
