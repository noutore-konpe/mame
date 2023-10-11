#pragma once

#include "../Graphics/Model.h"

class Stone
{
public:
    Stone();
    virtual ~Stone() {}

    virtual void Initialize(){}
    virtual void Finalize() {}
    virtual void Update(const float& elapsedTime) {}
    virtual void Render(const float& scale, ID3D11PixelShader* psShader = nullptr);
    virtual void DrawDebug() {}

public:
    // ジョイント位置取得
    DirectX::XMFLOAT3 GetJointPosition(const std::string& meshName, const std::string& boneName, const float& scale);
    DirectX::XMFLOAT3 GetJointPosition(size_t meshIndex, size_t boneIndex, const float& scale);

public:// 取得・設定
    Transform* GetTransform() { return model->GetTransform(); }

public:
    std::unique_ptr<Model> model = nullptr;

public: // --- ImGui用 --- //
    const char* GetName() const { return name.c_str(); }
    void SetName(std::string n) { name = n; }
    static int nameNum;

private: // --- ImGui用 --- //
    std::string name = {};
};