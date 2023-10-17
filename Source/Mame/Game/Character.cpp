#include "Character.h"
#include "../Graphics/Graphics.h"

#ifdef _DEBUG
#include "../Scene/SceneGame.h"
#endif

// コンストラクタ
Character::Character()
{
    Graphics& graphics = Graphics::Instance();

//#ifdef _DEBUG
//    debugSqhere = std::make_unique<Model>(graphics.GetDevice(),
//        "./Resources/Model/Collision/sqhere.fbx");
//#endif // _DEBUG
}

// 初期化
void Character::Initialize()
{
    //GetCollisionSqhereTransform()->SetPosition(GetTransform()->GetPosition());
}

// 更新処理
void Character::Update(const float elapsedTime)
{
//    DirectX::XMFLOAT3 position = GetTransform()->GetPosition();
//    position = { position.x + debugSphereOffset.x, position.y + debugSphereOffset.y , position.z + debugSphereOffset.z };
//    GetCollisionSphereTransform()->SetPosition(position);
//    GetCollisionSphereTransform()->SetScaleFactor(range);
//    GetCollisionSphereTransform()->SetRotation(GetTransform()->GetRotation());
//
//#ifdef _DEBUG
//    debugSqhere->GetTransform()->SetPosition(GetCollisionSphereTransform()->GetPosition());
//    debugSqhere->GetTransform()->SetScaleFactor(range);
//    //debugSqhere->GetTransform()->SetScale(GetCollisionSqhereTransform()->GetScale());
//    debugSqhere->GetTransform()->SetRotation(GetCollisionSphereTransform()->GetRotation());
//#endif // _DEBUG

    rotValue = 0.0f; //回転量リセット
    if (invincibleTime > 0.0f) invincibleTime -= elapsedTime;

}

// 描画処理
void Character::Render(
    const float scale, ID3D11PixelShader* psShader)
{
    Graphics::Instance().GetShader()->SetRasterizerState(static_cast<UINT>(Shader::RASTER_STATE::SOLID));
    model->Render(scale, psShader);

//#ifdef _DEBUG
//    // if(SceneDemo::isDebugRender)debugSqhere->Render(1.0f, 1);
//
//    Graphics::Instance().GetShader()->SetRasterizerState(static_cast<UINT>(Shader::RASTER_STATE::WIREFRAME));
//    if(SceneGame::isDebugRender)debugSqhere->Render(1.0f);
//#endif // _DEBUG
}

// ImGui用
void Character::DrawDebug()
{
#ifdef USE_IMGUI

    //GetTransform()->DrawDebug();

    model->DrawDebug();

//#ifdef _DEBUG
//    if (ImGui::TreeNode("debugSqhere"))
//    {
//        debugSqhere->GetTransform()->DrawDebug();
//        ImGui::DragFloat3("offset", &debugSphereOffset.x);
//        DirectX::XMFLOAT4 debugSqhereColor = debugSqhere->GetModelColor();
//        ImGui::ColorEdit4("debugSqhereColor", &debugSqhereColor.x);
//        debugSqhere->SetModelColor(debugSqhereColor);
//        ImGui::TreePop();
//    }
//#endif // _DEBUG


#endif // USE_IMGUI
}

// インスタンシング描画
void Character::RenderInstancing(const Model& m, const std::vector<Instance>& instances)
{
    UINT totalInstanceCount = static_cast<UINT>(instances.size());
    UINT startInstance = 0;
    UINT instanceCount = (totalInstanceCount < maxInstanceCount) ? totalInstanceCount : maxInstanceCount;
    while (startInstance < totalInstanceCount)
    {
        // 頂点バッファー設定
        UINT stride[] = { sizeof(DirectX::XMFLOAT3), sizeof(Instance) };
        UINT offset[] = { 0,0 };
        ID3D11Buffer* vertexBuffers[] =
        {
            instanceBuffer.Get(),
        };
        Graphics::Instance().GetDeviceContext()->IASetVertexBuffers(0, 0, vertexBuffers, stride, offset);

        // インスタンス編集
        D3D11_MAPPED_SUBRESOURCE mappedSubresource;
        HRESULT hr = Graphics::Instance().GetDeviceContext()->Map(instanceBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
        FBX_ASSERT_MSG(SUCCEEDED(hr), "インスタンスバッファのマップに失敗しました。\nhr=%08x", hr);

        memcpy(mappedSubresource.pData, &instances[startInstance], sizeof(Instance) * instanceCount);

        Graphics::Instance().GetDeviceContext()->Unmap(instanceBuffer.Get(), 0);

        // 描画
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

const bool Character::IsPlayAnimation() const
{
    return model->IsPlayAnimation();
}

// JOINT_POSITION
DirectX::XMFLOAT3 Character::GetJointPosition(const std::string& meshName, const std::string& boneName, const float& scale)
{
    DirectX::XMFLOAT4X4 transform{};
    DirectX::XMStoreFloat4x4(&transform, GetTransform()->CalcWorldMatrix(scale));

    return model->skinned_meshes->JointPosition(meshName, boneName, &model->keyframe, transform);
}

// JOINT_POSITION
DirectX::XMFLOAT3 Character::GetJointPosition(size_t meshIndex, size_t boneIndex, const float& scale)
{
    DirectX::XMFLOAT4X4 transform{};
    DirectX::XMStoreFloat4x4(&transform, GetTransform()->CalcWorldMatrix(scale));

    return model->skinned_meshes->JointPosition(meshIndex, boneIndex, &model->keyframe, transform);
}

void Character::Turn(float elapsedTime, float vx, float vz, float rotSpeed)
{
    if (vx < 0.01f && vx > -0.01f && vz < 0.01f && vz > -0.01f)return;

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

    //演算がオーバーフローしたときは処理しない
    if (_rotSpeed > 100.0f || _rotSpeed < -100.0f)
    {
        return;
    }

    //左右判定のための外積
    float cross = (vx * front.z) - (vz * front.x);

    DirectX::XMFLOAT4 rotation{transform->GetRotation()};
    rotation.y += cross < 0.0f ? -_rotSpeed : _rotSpeed;

    rotValue = cross < 0.0f ? -_rotSpeed : _rotSpeed;

   
        transform->SetRotationY(rotation.y);
}

Character::DamageResult Character::ApplyDamage(float damage, float invincibleTime)
{
    DamageResult result;

    //無敵時間か
    if (this->invincibleTime > 0.0f)
    {
        result.hit = false;
        return result;
    }

    //防御力の影響
    damage -= defence;
    result.damage = damage;

    //ダメージが０の場合は健康状態を変更する必要がない
    if (damage <= 0)
    {
        result.hit = true;
        result.damage = 0;
        return result;
    }

    //死亡している場合は健康状態を変更しない
    if (health <= 0)
    {
        result.hit = true;
        return result;
    }

    //ダメージ処理
    health -= damage;

    //無敵時間設定
    this->invincibleTime = invincibleTime;

    //死亡通知
    if (health <= 0)
    {
        OnDead();
    }
    //ダメージ通知
    else
    {
        OnDamaged();
        isDead = true;
    }

    //健康状態が変更した場合はtrueを返す
    result.hit = true;
    return result;
}

bool Character::ApplyHeal(float heal)
{
    if (heal == 0)return false;

    if (health >= maxHealth)return false;

    health += heal;

    OnHealed();

    return true;
}
