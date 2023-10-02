#include "Character.h"
#include "../Graphics/Graphics.h"

#ifdef _DEBUG
#include "../Scene/SceneGame.h"
#endif

int Character::nameNum = 0;

// �R���X�g���N�^
Character::Character()
{
    Graphics& graphics = Graphics::Instance();

#ifdef _DEBUG
    debugSqhere = std::make_unique<Model>(graphics.GetDevice(),
        "./Resources/Model/Collision/sqhere.fbx");
#endif // _DEBUG
}

// ������
void Character::Initialize()
{
    //GetCollisionSqhereTransform()->SetPosition(GetTransform()->GetPosition());
}

// �X�V����
void Character::Update(const float& elapsedTime)
{
    DirectX::XMFLOAT3 position = GetTransform()->GetPosition();
    position = { position.x + debugSqhereOffset.x, position.y + debugSqhereOffset.y , position.z + debugSqhereOffset.z };
    GetCollisionSqhereTransform()->SetPosition(position);
    GetCollisionSqhereTransform()->SetScaleFactor(range);
    GetCollisionSqhereTransform()->SetRotation(GetTransform()->GetRotation());

#ifdef _DEBUG
    debugSqhere->GetTransform()->SetPosition(GetCollisionSqhereTransform()->GetPosition());
    debugSqhere->GetTransform()->SetScaleFactor(range);
    //debugSqhere->GetTransform()->SetScale(GetCollisionSqhereTransform()->GetScale());
    debugSqhere->GetTransform()->SetRotation(GetCollisionSqhereTransform()->GetRotation());
#endif // _DEBUG
}

// �`�揈��
void Character::Render(const float& scale, ID3D11PixelShader* psShader)
{
    model->Render(scale, psShader);

#ifdef _DEBUG
    if(SceneGame::isDebugRender)debugSqhere->Render(1.0f, 1);
#endif // _DEBUG
}

// ImGui�p
void Character::DrawDebug()
{
#ifdef USE_IMGUI

    //GetTransform()->DrawDebug();
    
    model->DrawDebug();

#ifdef _DEBUG
    if (ImGui::TreeNode("debugSqhere"))
    {
        debugSqhere->GetTransform()->DrawDebug();
        ImGui::DragFloat3("offset", &debugSqhereOffset.x);
        DirectX::XMFLOAT4 debugSqhereColor = debugSqhere->GetModelColor();
        ImGui::ColorEdit4("debugSqhereColor", &debugSqhereColor.x);
        debugSqhere->SetModelColor(debugSqhereColor);
        ImGui::TreePop();
    }
#endif // _DEBUG


#endif // USE_IMGUI
}

// �C���X�^���V���O�`��
void Character::RenderInstancing(const Model& m, const std::vector<Instance>& instances)
{
    UINT totalInstanceCount = static_cast<UINT>(instances.size());
    UINT startInstance = 0;
    UINT instanceCount = (totalInstanceCount < maxInstanceCount) ? totalInstanceCount : maxInstanceCount;
    while (startInstance < totalInstanceCount)
    {
        // ���_�o�b�t�@�[�ݒ�
        UINT stride[] = { sizeof(DirectX::XMFLOAT3), sizeof(Instance) };
        UINT offset[] = { 0,0 };
        ID3D11Buffer* vertexBuffers[] =
        {
            instanceBuffer.Get(),
        };
        Graphics::Instance().GetDeviceContext()->IASetVertexBuffers(0, 0, vertexBuffers, stride, offset);

        // �C���X�^���X�ҏW
        D3D11_MAPPED_SUBRESOURCE mappedSubresource;
        HRESULT hr = Graphics::Instance().GetDeviceContext()->Map(instanceBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
        FBX_ASSERT_MSG(SUCCEEDED(hr), "�C���X�^���X�o�b�t�@�̃}�b�v�Ɏ��s���܂����B\nhr=%08x", hr);

        memcpy(mappedSubresource.pData, &instances[startInstance], sizeof(Instance) * instanceCount);

        Graphics::Instance().GetDeviceContext()->Unmap(instanceBuffer.Get(), 0);

        // �`��
        //Graphics::Instance().GetDeviceContext()->DrawInstanced()
    }

}

void Character::PlayAnimation(const int& index, const bool& loop, const float& speed, const float& blendSeconds)
{
    model->PlayAnimation(index, loop, speed, blendSeconds);
}

void Character::PlayBlendAnimation(const int& index1, const int& index2, const bool& loop, const float& speed)
{
    model->PlayBlendAnimation(index1, index2, loop, speed);
}

void Character::UpdateBlendRate(float blendRate, const float& elapsedTime)
{
    model->UpdateBlendRate(blendRate, elapsedTime);
}

void Character::UpdateAnimation(const float& elapsedTime)
{
    model->UpdateAnimation(elapsedTime);
}

bool Character::IsPlayAnimation() const
{
    return model->IsPlayAnimation();
}

void Character::Turn(float elapsedTime, float vx, float vz, float rotSpeed)
{
    if (vx == 0 && vz == 0)return;

    Transform* transform = GetTransform();
    rotSpeed = DirectX::XMConvertToRadians(rotSpeed * elapsedTime);

    float length = sqrtf(vx * vx + vz * vz);
    vx /= length;
    vz /= length;

    DirectX::XMFLOAT3 front{transform->CalcForward()};

    float dot = (vx * front.x) + (vz * front.z);
    float rot = 1.0f - dot;
    if (rot < 0.005f)return;
    if (rot < 0.3f)rot = 0.3f;
    rot += 0.5f;
    float _rotSpeed = rotSpeed * rot;

    //���E����̂��߂̊O��
    float cross = (vx * front.z) - (vz * front.x);

    DirectX::XMFLOAT4 rotation{transform->GetRotation()};
    rotation.y += cross < 0.0f ? -_rotSpeed : _rotSpeed;

    transform->SetRotation(rotation);
}
