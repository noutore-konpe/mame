#pragma once
#include "Item.h"

#include "StateMachine.h"

class MagicCircle : public Item
{
public:
    MagicCircle();
    MagicCircle(const char* fbxFilename, const wchar_t* spriteFilename);
    ~MagicCircle() override;

    void Initialize()                       override; // 初期化
    void Finalize()                         override; // 終了化
    void Begin()                            override; // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime)   override; // 更新処理
    void End()                              override; // 毎フレーム一番最後に呼ばれる
    void Render(const float& scale, ID3D11PixelShader* psShader = nullptr) override; // 描画処理
    void DrawDebug()                        override; // ImGui用

    void SetEmissiveIntensity(float intensity) { model->skinned_meshes->data.emissiveIntensity = intensity; }
    void SetEmissiveScrollDirection(DirectX::XMFLOAT2 scroll) { model->skinned_meshes->data.emissiveScrollDirection = scroll; }
    void SetEmissiveColor(DirectX::XMFLOAT4 color) { model->skinned_meshes->data.emissiveColor = color; }

private:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> magicCirclePS;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> emissiveTexture;
};

