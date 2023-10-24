#pragma once
#include "MagicCircle.h"

#include "../Other/Easing.h"

#include "StateMachine.h"

#include "StoneBall.h"

class MagicCircleGolemAttack2
{
public:
    enum class MAGIC_CIRCLE
    {
        Left,
        Center,
        Right,

        Max,
    };

    enum class StateMachineState
    {
        DummyState,
        AppearState,
        ThrowState,
    };

public:
    MagicCircleGolemAttack2(Enemy* owner);
    ~MagicCircleGolemAttack2();

    void Initialize();
    void Finalize();
    void Update(const float& elapsedTime);
    void Render(const DirectX::XMFLOAT4 color);
    void DrawDebug();

public:// �擾�E�ݒ�
    // �X�e�[�g�}�V��
    StateMachine<State<MagicCircleGolemAttack2>>* GetStateMachine() const { return stateMachine.get(); }

public:
    std::unique_ptr<MagicCircle> baseMagicCircle[static_cast<UINT>(MAGIC_CIRCLE::Max)];

    std::unique_ptr<StoneBall> stoneBalls[3][3];

    bool isAttack2 = false;

private:
    // �X�e�[�g�}�V��
    std::unique_ptr<StateMachine<State<MagicCircleGolemAttack2>>> stateMachine = nullptr;
};

