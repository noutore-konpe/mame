#include "MagicCircleGolem.h"

#include "../Graphics/Graphics.h"
#include "../Graphics/Camera.h"

#include "../Other/Easing.h"

#include "MagicCircleGolemState.h"

// コンストラクタ
MagicCircleGolem::MagicCircleGolem()
{
    Graphics& graphics = Graphics::Instance();

    magicCircleGolem[static_cast<UINT>(MAGIC_CIRCLE::Left)] = std::make_unique<MagicCircle>();
    magicCircleGolem[static_cast<UINT>(MAGIC_CIRCLE::Right)] = std::make_unique<MagicCircle>();

    // ステートマシン
    {
        stateMachine.reset(new StateMachine<State<MagicCircleGolem>>);

        GetStateMachine()->RegisterState(new MagicCircleGolemState::DummyState(this));
        GetStateMachine()->RegisterState(new MagicCircleGolemState::AppearState(this));

        GetStateMachine()->SetState(static_cast<UINT>(StateMachineState::DummyState));
    }
}

// デストラクタ
MagicCircleGolem::~MagicCircleGolem()
{
}

// 初期化
void MagicCircleGolem::Initialize()
{
    for (int i = 0; i < 2; ++i)
    {
        magicCircleGolem[i]->GetTransform()->SetScale(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
        //magicCircleGolem[i]->GetTransform()->SetScale(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
        magicCircleGolem[i]->SetEmissiveColor(DirectX::XMFLOAT4(0.8f, 0.44f, 0.24f, 1.0f));
    }



    magicCircleGolem[0]->GetTransform()->SetPosition(DirectX::XMFLOAT3(-3.0f, 2.0f, 0.0f));
    magicCircleGolem[1]->GetTransform()->SetPosition(DirectX::XMFLOAT3(3.0f, 2.0f, 0.0f));
}

// 終了化
void MagicCircleGolem::Finalize()
{
}

// 更新
void MagicCircleGolem::Update(const float& elapsedTime)
{
    // ステートマシン更新
    GetStateMachine()->Update(elapsedTime);

#if 1
    float cameraRotX = Camera::Instance().GetTransform()->GetRotation().y;
    cameraRotX += DirectX::XMConvertToRadians(180);

    for (int i = 0; i < 2; ++i)
    {
        DirectX::XMFLOAT4 magicCircleRot = magicCircleGolem[i]->GetTransform()->GetRotation();

        magicCircleRot.x = DirectX::XMConvertToRadians(90);
        magicCircleRot.z = -cameraRotX;

        magicCircleGolem[i]->GetTransform()->SetRotation(magicCircleRot);
    }
#endif


}

// 描画
void MagicCircleGolem::Render()
{
    magicCircleGolem[0]->SetEmissiveColor(DirectX::XMFLOAT4(0.8f, 0.44f, 0.24f, 1.0f));
    magicCircleGolem[1]->SetEmissiveColor(DirectX::XMFLOAT4(0.8f, 0.44f, 0.24f, 1.0f));

    magicCircleGolem[0]->SetEmissiveIntensity(1.0f);

    magicCircleGolem[static_cast<UINT>(MAGIC_CIRCLE::Left)]->Render(0.01f);
    magicCircleGolem[static_cast<UINT>(MAGIC_CIRCLE::Right)]->Render(0.01f);
}

// ImGui用
void MagicCircleGolem::DrawDebug()
{
    ImGui::Begin("MagicCircleGolem");

    magicCircleGolem[static_cast<UINT>(MAGIC_CIRCLE::Left)]->DrawDebug();
    magicCircleGolem[static_cast<UINT>(MAGIC_CIRCLE::Right)]->DrawDebug();

    ImGui::End();
}
