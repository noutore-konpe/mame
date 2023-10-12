#include "ComboAttackStone.h"

#include "../Graphics/Graphics.h"

#include "ComboAttackStoneState.h"

// コンストラクタ
ComboAttackStone::ComboAttackStone()
{
    Graphics& graphics = Graphics::Instance();

    stoneBall = std::make_unique<StoneBall>();
    magicCircle = std::make_unique<MagicCircle>();


    // ステートマシン
    {
        stateMachine.reset(new StateMachine<State<ComboAttackStone>>);

        GetStateMachine()->RegisterState(new ComboAttackStoneState::DummyState(this));
        GetStateMachine()->RegisterState(new ComboAttackStoneState::AppearState(this));
        GetStateMachine()->RegisterState(new ComboAttackStoneState::ThrowState(this));

        GetStateMachine()->SetState(static_cast<UINT>(StateMachineState::DummyState));
    }
}

// 初期化
void ComboAttackStone::Initialize()
{
    stoneBall->Initialize();

    isChangeState = false;
}

// 終了化
void ComboAttackStone::Finalize()
{
}

// 更新
void ComboAttackStone::Update(const float& elapsedTime)
{
    // ステートマシン更新
    GetStateMachine()->Update(elapsedTime);
}

// 描画
void ComboAttackStone::Render()
{
    stoneBall->Render(0.01f);

    // エミッシブについて
    magicCircle->SetEmissiveColor(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
    magicCircle->SetEmissiveIntensity(1.5f);

    magicCircle->Render(0.01f);
}

// ImGui用
void ComboAttackStone::DrawDebug()
{
}
