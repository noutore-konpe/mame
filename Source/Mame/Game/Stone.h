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
    // �W���C���g�ʒu�擾
    DirectX::XMFLOAT3 GetJointPosition(const std::string& meshName, const std::string& boneName, const float& scale);
    DirectX::XMFLOAT3 GetJointPosition(size_t meshIndex, size_t boneIndex, const float& scale);

public:// �擾�E�ݒ�
    Transform* GetTransform() { return model->GetTransform(); }

public:
    std::unique_ptr<Model> model = nullptr;

public: // --- ImGui�p --- //
    const char* GetName() const { return name.c_str(); }
    void SetName(std::string n) { name = n; }
    static int nameNum;

private: // --- ImGui�p --- //
    std::string name = {};
};