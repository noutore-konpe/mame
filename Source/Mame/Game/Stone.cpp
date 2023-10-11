#include "Stone.h"

Stone::Stone()
{
}

void Stone::Render(const float& scale, ID3D11PixelShader* psShader)
{
    model->Render(scale, psShader);
}

DirectX::XMFLOAT3 Stone::GetJointPosition(const std::string& meshName, const std::string& boneName, const float& scale)
{
    DirectX::XMFLOAT4X4 transform{};
    DirectX::XMStoreFloat4x4(&transform, GetTransform()->CalcWorldMatrix(scale));

    return model->skinned_meshes->JointPosition(meshName, boneName, &model->keyframe, transform);
}

DirectX::XMFLOAT3 Stone::GetJointPosition(size_t meshIndex, size_t boneIndex, const float& scale)
{
    DirectX::XMFLOAT4X4 transform{};
    DirectX::XMStoreFloat4x4(&transform, GetTransform()->CalcWorldMatrix(scale));

    return model->skinned_meshes->JointPosition(meshIndex, boneIndex, &model->keyframe, transform);
}
