#pragma once

#include "../Graphics/Model.h"

class Character
{
public:
    Character();
    virtual ~Character() {}

    virtual void Initialize();
    virtual void Update(const float& elapsedTime);
    virtual void Render(const float& scale, ID3D11PixelShader* psShader = nullptr);
    virtual void DrawDebug();

public: // GPU_Instancing
    struct Instance
    {
        DirectX::XMFLOAT4X4 world;
        DirectX::XMFLOAT4 color;
    };

    std::vector<Instance> modelInstance;
    Microsoft::WRL::ComPtr<ID3D11Buffer> instanceBuffer;
    UINT maxInstanceCount = 0;

    void RenderInstancing(const Model& m, const std::vector<Instance>& instances);

public: // アニメーション関数関連
    // アニメーション再生設定
    // (アニメーション番号・ループするかどうか・アニメーション再生速度・スムーズ切り替え時間（速度）)
    void PlayAnimation(
        const int& index,
        const bool& loop,
        const float& speed = 1.0f,
        const float& blendSeconds = 1.0f
    );

    void PlayBlendAnimation(
        const int& index1,
        const int& index2,
        const bool& loop,
        const float& speed = 1.0f
    );

    // ブレンド率の計算更新処理
    void UpdateBlendRate(float blendRate, const float& elapsedTime);

    // アニメーション更新処理
    void UpdateAnimation(const float& elapsedTime);

    // アニメーションが再生中かどうか
    bool IsPlayAnimation() const;

public: // 取得・設定　関連
    Transform* GetTransform() { return model->GetTransform(); }
    Transform* GetCollisionSqhereTransform() { return &collisionSqhereTransform; }

    void SetDebugSqhereOffset(DirectX::XMFLOAT3 offset) { debugSqhereOffset = offset; }
    DirectX::XMFLOAT3 GetDebugSqhereOffset() { return debugSqhereOffset; }

    void SetRange(const float& r) { range = r; }
    float GetRange() { return range; }


    // emissive ※constansのやつなのでこいつを使う場所は UpdateConstansで使ってほしい
    void SetEmissiveIntensity(float intensity) { model->skinned_meshes->data.emissiveIntensity = intensity; }
    void SetEmissiveScrollDirection(DirectX::XMFLOAT2 scroll) { model->skinned_meshes->data.emissiveScrollDirection = scroll; }
    void SetEmissiveColor(DirectX::XMFLOAT4 color) { model->skinned_meshes->data.emissiveColor = color; }



public: // 関数
    void Turn(float elapsedTime, float vx, float vz, float rotSpeed);

public:
    std::unique_ptr<Model> model = nullptr;

#ifdef _DEBUG
    std::unique_ptr<Model> debugSqhere = nullptr;   // 当たり判定用＿球
#endif // _DEBUG

private:
    Transform collisionSqhereTransform{};
    DirectX::XMFLOAT3 debugSqhereOffset{};  // 当たり判定用
    float range = 1.0f;                     // 球当たり判定半径
    
    
public: // --- ImGui用 --- //
    const char* GetName() const { return name.c_str(); }
    void SetName(std::string n) { name = n; }
    static int nameNum;

private: // --- ImGui用 --- //
    std::string name = {};
};

