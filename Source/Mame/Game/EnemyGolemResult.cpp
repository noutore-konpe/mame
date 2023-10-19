#include "EnemyGolemResult.h"
#include "../Graphics/Graphics.h"

// コンストラクタ
EnemyGolemResult::EnemyGolemResult()
{
    Graphics& graphics = Graphics::Instance();

    model=std::make_unique<Model>(graphics.GetDevice(),
        "./Resources/Model/Character/Enemy/golem.fbx");

    CreatePsFromCso(graphics.GetDevice(),
        "./Resources/Shader/ResultPS.cso",
        golemPS.GetAddressOf());
}

// 初期化
void EnemyGolemResult::Initialize()
{
    // アニメーション再生
    PlayAnimation(static_cast<UINT>(Animation::Walk), true);
    GetTransform()->SetScaleFactor(0.2f);
}

// 更新
void EnemyGolemResult::Update(const float& elapsedTime)
{
    // アニメーション更新
    Character::UpdateAnimation(elapsedTime);

    GetTransform()->AddRotationY(elapsedTime);
    float rotY = GetTransform()->GetRotation().y;
    if (rotY > DirectX::XMConvertToRadians(360))
        rotY -= DirectX::XMConvertToRadians(360);
    GetTransform()->SetRotationY(rotY);
}

void EnemyGolemResult::Render(const float& scale, ID3D11PixelShader* psShader)
{
    //model->Render(scale, nullptr);
    model->Render(scale, golemPS.Get());
}


void EnemyGolemResult::DrawDebug()
{
    model->DrawDebug();
}

void EnemyGolemResult::UpdateConstants()
{
}
