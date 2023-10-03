#pragma once
#include "MagicCircle.h"

#include "StateMachine.h"

class MagicCircleSummon
{
public:
    enum class MAGIC_CIRCLE
    {
        Base,
        Move1,
        Move2,
    };

    enum class StateMachineState
    {
        DummyState,
        AppearState,
        ExpandState,
    };

public:
    MagicCircleSummon();
    ~MagicCircleSummon();

    void Initialize();
    void Finalize();
    void Update(const float& elapsedTime);
    void Render();
    void DrawDebug();

public:// �擾�E�ݒ�
    // �X�e�[�g�}�V��
    StateMachine<State<MagicCircleSummon>>* GetStateMachine() const { return stateMachine.get(); }


public:
    std::unique_ptr<MagicCircle> magicCircle[3];

    
private:
    // �X�e�[�g�}�V��
    std::unique_ptr<StateMachine<State<MagicCircleSummon>>> stateMachine = nullptr;
};

