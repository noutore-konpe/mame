#include "MagicCircleEnemySummon.h"

#include "../Graphics//Graphics.h"

#include "MagicCircleEnemySummonState.h"

// コンストラクタ
MagicCircleEnemySummon::MagicCircleEnemySummon()
{
    Graphics& graphics = Graphics::Instance();

    for (int i = 0; i < 2; ++i)
    {
        magicCircle[i] = std::make_unique<MagicCircle>();

        for (int j = 0; j < 3; ++j)
        {
            magicCircleMove[i][j] = std::make_unique<MagicCircle>
                ("./Resources/Model/Item/MagicCircle/MagicCircle1.fbx",
                    L"./Resources/Model/Item/MagicCircle/planeEmissive1.png");
        }
    }

    // ステートマシン
    {
        stateMachine.reset(new StateMachine<State<MagicCircleEnemySummon>>);

        GetStateMachine()->RegisterState(new MagicCircleEnemySummonState::DummyState(this));
        GetStateMachine()->RegisterState(new MagicCircleEnemySummonState::AppearState(this));
        GetStateMachine()->RegisterState(new MagicCircleEnemySummonState::CreateState(this));

        GetStateMachine()->SetState(static_cast<UINT>(StateMachineState::DummyState));
    }
}

// デストラクタ
MagicCircleEnemySummon::~MagicCircleEnemySummon()
{
}

// 初期化
void MagicCircleEnemySummon::Initialize()
{
    for (int i = 0; i < 2; ++i)
    {
        magicCircle[i]->GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 0, 0));

        for (int j = 0; j < 3; ++j)
        {
            magicCircleMove[i][j]->GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 0, 0));
        }
    }
}

// 終了化
void MagicCircleEnemySummon::Finalize()
{
}

// 更新
void MagicCircleEnemySummon::Update(const float& elapsedTime)
{
    // ステートマシン更新
    GetStateMachine()->Update(elapsedTime);
}

// 描画
void MagicCircleEnemySummon::Render(const DirectX::XMFLOAT4 color)
{
    // こいつは一回呼ぶだけですべてに適応される(個別で変えたい場合は別)
    magicCircle[0]->SetEmissiveColor(color);
    magicCircleMove[0][0]->SetEmissiveColor(color);
    magicCircle[0]->SetEmissiveIntensity(1.5);
    magicCircleMove[0][0]->SetEmissiveIntensity(1.5);

    for (int i = 0; i < 2; ++i)
    {
        magicCircle[i]->Render(0.01f);

        for (int j = 0; j < 3; ++j)
        {
            magicCircleMove[i][j]->Render(0.01f);
        }
    }
}

// ImGui用
void MagicCircleEnemySummon::DrawDebug()
{
}
