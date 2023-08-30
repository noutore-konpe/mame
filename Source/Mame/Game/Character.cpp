#include "Character.h"
#include "../Graphics/Graphics.h"

#include "../Scene/SceneDemo.h"

int Character::nameNum = 0;

// コンストラクタ
Character::Character()
{
    Graphics& graphics = Graphics::Instance();

#ifdef _DEBUG
    debugSqhere = std::make_unique<Model>(graphics.GetDevice(),
        "./Resources/Model/Collision/sqhere.fbx");
#endif // _DEBUG
}

// 初期化
void Character::Initialize()
{
    //GetCollisionSqhereTransform()->SetPosition(GetTransform()->GetPosition());
}

// 更新処理
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

// 描画処理
void Character::Render(const float& elapsedTime, const float& scale)
{
    model->Render(scale);

#ifdef _DEBUG
    if(SceneDemo::isDebugRender)debugSqhere->Render(1.0f, 1);
#endif // _DEBUG
}

// ImGui用
void Character::DrawDebug()
{
#ifdef USE_IMGUI

    GetTransform()->DrawDebug();

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

void Character::PlayAnimation(const int& index, const bool& loop, const float& speed, const float& blendSeconds)
{
    model->PlayAnimation(index, loop, speed, blendSeconds);
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
