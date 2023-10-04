#pragma once
#include "Item.h"

#include "StateMachine.h"

class MagicCircle
{
public:
    MagicCircle();
    MagicCircle(const char* fbxFilename, const wchar_t* spriteFilename);
    ~MagicCircle();

    void Initialize()                       ; // 初期化
    void Finalize()                         ; // 終了化
    void Begin()                            ; // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime)   ; // 更新処理
    void End()                              ; // 毎フレーム一番最後に呼ばれる
    void Render(const float& scale, ID3D11PixelShader* psShader = nullptr) ; // 描画処理
    void DrawDebug()                        ; // ImGui用

    void SetEmissiveIntensity(float intensity) { model->skinned_meshes->data.emissiveIntensity = intensity; }
    void SetEmissiveScrollDirection(DirectX::XMFLOAT2 scroll) { model->skinned_meshes->data.emissiveScrollDirection = scroll; }
    void SetEmissiveColor(DirectX::XMFLOAT4 color) { model->skinned_meshes->data.emissiveColor = color; }

public: // 取得・設定　関連
    Transform* GetTransform() { return model->GetTransform(); }

public:
    std::unique_ptr<Model> model = nullptr;

private:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> magicCirclePS;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> emissiveTexture;

public: // --- ImGui用 --- //
    const char* GetName() const { return name.c_str(); }
    void SetName(std::string n) { name = n; }
    static int nameNum;

private: // --- ImGui用 --- //
    std::string name = {};
};

