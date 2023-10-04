#pragma once
#include "MagicCircle.h"

#include "StateMachine.h"

class MagicCircleGolem
{
public:
    enum class MAGIC_CIRCLE
    {
        Left,
        Right,
    };

    enum class StateMachineState
    {
        DummyState,
        AppearState,
    };

public:
    MagicCircleGolem();
    ~MagicCircleGolem();

    void Initialize();
    void Finalize();
    void Update(const float& elapsedTime);
    void Render();
    void DrawDebug();

public:// �擾�E�ݒ�
    // �X�e�[�g�}�V��
    StateMachine<State<MagicCircleGolem>>* GetStateMachine() const { return stateMachine.get(); }

public:
    std::unique_ptr<MagicCircle> magicCircleGolem[2];

private:
    // �X�e�[�g�}�V��
    std::unique_ptr<StateMachine<State<MagicCircleGolem>>> stateMachine = nullptr;
};

