#include "AuraEffect.h"

#include "../Resource/shader.h"
#include "../Other/misc.h"

AuraEffect::AuraEffect(ID3D11Device* device)
{
    HRESULT hr = S_OK;

    D3D11_BUFFER_DESC bufferDesc = {};
}

void AuraEffect::Render(ID3D11DeviceContext* deviceContext, const std::vector<Emitter>& emitters, const DirectX::XMFLOAT4X4& world, ID3D11ShaderResourceView* shaderResourceView)
{
}
