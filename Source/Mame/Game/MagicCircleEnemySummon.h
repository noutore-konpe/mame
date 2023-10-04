#pragma once
#include "MagicCircle.h"

#include "../Other/Easing.h"

#include "StateMachine.h"

class MagicCircleEnemySummon
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
        CreateState,
    };

public:
    MagicCircleEnemySummon();
    ~MagicCircleEnemySummon();

    void Initialize();
    void Finalize();
    void Update(const float& elapsedTime);
    void Render(const DirectX::XMFLOAT4 color);
    void DrawDebug();

public:// 取得・設定
    // ステートマシン
    StateMachine<State<MagicCircleEnemySummon>>* GetStateMachine() const { return stateMachine.get(); }
    
public:
    std::unique_ptr<MagicCircle> magicCircle[2];
    std::unique_ptr<MagicCircle> magicCircleMove[2][3];

private:
    // ステートマシン
    std::unique_ptr<StateMachine<State<MagicCircleEnemySummon>>> stateMachine = nullptr;

};