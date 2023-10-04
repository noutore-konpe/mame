#include "MagicCircleSummon.h"

#include "../Graphics/Graphics.h"

#include "../Other/Easing.h"

#include "MagicCircleState.h"

// �R���X�g���N�^
MagicCircleSummon::MagicCircleSummon()
{
    Graphics& graphics = Graphics::Instance();

    magicCircle[static_cast<UINT>(MAGIC_CIRCLE::Base)] = std::make_unique<MagicCircle>();
    magicCircle[static_cast<UINT>(MAGIC_CIRCLE::Move1)] =
        std::make_unique<MagicCircle>("./Resources/Model/Item/MagicCircle/MagicCircle1.fbx",
            L"./Resources/Model/Item/MagicCircle/planeEmissive1.png");
    magicCircle[static_cast<UINT>(MAGIC_CIRCLE::Move2)] =
        std::make_unique<MagicCircle>("./Resources/Model/Item/MagicCircle/MagicCircle1.fbx",
            L"./Resources/Model/Item/MagicCircle/planeEmissive1.png");

    // �X�e�[�g�}�V��
    {
        stateMachine.reset(new StateMachine<State<MagicCircleSummon>>);

        GetStateMachine()->RegisterState(new MagicCircleState::DummyState(this));
        GetStateMachine()->RegisterState(new MagicCircleState::AppearState(this));
        GetStateMachine()->RegisterState(new MagicCircleState::ExpandState(this));

        GetStateMachine()->SetState(static_cast<UINT>(StateMachineState::DummyState));
    }
}

MagicCircleSummon::~MagicCircleSummon()
{
}

void MagicCircleSummon::Initialize()
{
    for (int i = 0; i < 3; ++i)
    {
        magicCircle[i]->GetTransform()->SetScale(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
    }
}

void MagicCircleSummon::Finalize()
{
}

void MagicCircleSummon::Update(const float& elapsedTime)
{


    // �X�e�[�g�}�V���X�V
    GetStateMachine()->Update(elapsedTime);
}

void MagicCircleSummon::Render()
{
    // �J���[
    for (int i = 0; i < 3; ++i)
    {
        magicCircle[i]->SetEmissiveColor(DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f));
    }

    for (int i = 0; i < 3; ++i)
    {
        magicCircle[i]->Render(0.01f);
    }
}

void MagicCircleSummon::DrawDebug()
{
}
