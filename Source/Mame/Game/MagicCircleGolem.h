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

public:// 取得・設定
    // ステートマシン
    StateMachine<State<MagicCircleGolem>>* GetStateMachine() const { return stateMachine.get(); }

public:
    std::unique_ptr<MagicCircle> magicCircleGolem[2];

private:
    // ステートマシン
    std::unique_ptr<StateMachine<State<MagicCircleGolem>>> stateMachine = nullptr;
};

