#pragma once
#include "Stone.h"

class StoneBall : public Stone
{
public:
    StoneBall();
    ~StoneBall() override;

    void Initialize() override;
    void Finalize() override;
    void Update(const float& elapsedTime) override;
    void Render(const float& scale, ID3D11PixelShader* psShader = nullptr) override;
    void DrawDebug() override;

public:
    void UpdateConstants();

    void SetEmissiveIntensity(float intensity) { model->skinned_meshes->data.emissiveIntensity = intensity; }
    void SetEmissiveScrollDirection(DirectX::XMFLOAT2 scroll) { model->skinned_meshes->data.emissiveScrollDirection = scroll; }
    void SetEmissiveColor(DirectX::XMFLOAT4 color) { model->skinned_meshes->data.emissiveColor = color; }

    void SetRadius(float r) { radius = r; }

    float radius = 1.0f;
    float damage = 1.0f;

private:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> stoneBallPS;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> stoneBallTexture;
};

