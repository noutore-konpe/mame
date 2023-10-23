#include "MagicCircleGolemAttack2.h"

#include "../Graphics/Graphics.h"

#include "MagicCircleGolemAttack2State.h"

// �R���X�g���N�^
MagicCircleGolemAttack2::MagicCircleGolemAttack2(Enemy* owner)
{
    Graphics& graphics = Graphics::Instance();

    for (int i = 0; i < static_cast<UINT>(MAGIC_CIRCLE::Max); ++i)
    {
        baseMagicCircle[i] = std::make_unique<MagicCircle>
            ("./Resources/Model/Item/MagicCircle/anime_magicCircle.fbx",
                L"./Resources/Model/Item/MagicCircle/planeEmissive1.png");
    }

    for (int i = 0; i < static_cast<UINT>(MAGIC_CIRCLE::Max); ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            stoneBalls[i][j] = std::make_unique<StoneBall>(owner);
        }
    }

    // �X�e�[�g�}�V��
    {
        stateMachine.reset(new StateMachine<State<MagicCircleGolemAttack2>>);

        GetStateMachine()->RegisterState(new MagicCircleGolemAttack2State::DummyState(this));
        GetStateMachine()->RegisterState(new MagicCircleGolemAttack2State::AppearState(this));
        GetStateMachine()->RegisterState(new MagicCircleGolemAttack2State::ThrowState(this));

        GetStateMachine()->SetState(static_cast<UINT>(StateMachineState::DummyState));
    }
}

// �f�X�g���N�^
MagicCircleGolemAttack2::~MagicCircleGolemAttack2()
{
}

// ������
void MagicCircleGolemAttack2::Initialize()
{
}

// �I����
void MagicCircleGolemAttack2::Finalize()
{
}

// �X�V
void MagicCircleGolemAttack2::Update(const float& elapsedTime)
{
    // �X�e�[�g�}�V���X�V
    GetStateMachine()->Update(elapsedTime);
}

// �`��
void MagicCircleGolemAttack2::Render(const DirectX::XMFLOAT4 color)
{
    baseMagicCircle[0]->SetEmissiveColor(color);
    baseMagicCircle[0]->SetEmissiveIntensity(1.5);
    

    for (int i = 0; i < static_cast<UINT>(MAGIC_CIRCLE::Max); ++i)
    {
        baseMagicCircle[i]->Render(0.01f);
    }

    for (int i = 0; i < static_cast<UINT>(MAGIC_CIRCLE::Max); ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            stoneBalls[i][j]->Render(0.01f);
        }
    }
}

void MagicCircleGolemAttack2::DrawDebug()
{
}
