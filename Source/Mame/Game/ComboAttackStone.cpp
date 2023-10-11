#include "ComboAttackStone.h"

#include "../Graphics/Graphics.h"

#include "ComboAttackStoneState.h"

// �R���X�g���N�^
ComboAttackStone::ComboAttackStone()
{
    Graphics& graphics = Graphics::Instance();

    stoneBall = std::make_unique<StoneBall>();
    magicCircle = std::make_unique<MagicCircle>();


    // �X�e�[�g�}�V��
    {
        stateMachine.reset(new StateMachine<State<ComboAttackStone>>);

        GetStateMachine()->RegisterState(new ComboAttackStoneState::DummyState(this));
        GetStateMachine()->RegisterState(new ComboAttackStoneState::AppearState(this));
        GetStateMachine()->RegisterState(new ComboAttackStoneState::ThrowState(this));

        GetStateMachine()->SetState(static_cast<UINT>(StateMachineState::DummyState));
    }
}

// ������
void ComboAttackStone::Initialize()
{
    stoneBall->Initialize();

    isChangeState = false;
}

// �I����
void ComboAttackStone::Finalize()
{
}

// �X�V
void ComboAttackStone::Update(const float& elapsedTime)
{
    // �X�e�[�g�}�V���X�V
    GetStateMachine()->Update(elapsedTime);
}

// �`��
void ComboAttackStone::Render()
{
    stoneBall->Render(0.01f);

    // �G�~�b�V�u�ɂ���
    magicCircle->SetEmissiveColor(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
    magicCircle->SetEmissiveIntensity(1.5f);

    magicCircle->Render(0.01f);
}

// ImGui�p
void ComboAttackStone::DrawDebug()
{
}
