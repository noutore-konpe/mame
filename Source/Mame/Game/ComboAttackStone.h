#pragma once
#include "StoneBall.h"
#include "MagicCircle.h"

#include "../Other/Easing.h"

#include "StateMachine.h"

class ComboAttackStone
{
public:// 定数
    enum class StateMachineState
    {
        DummyState,
        AppearState,
        ThrowState,
    };

public:
    ComboAttackStone();
    ~ComboAttackStone() {}

    void Initialize();
    void Finalize();
    void Update(const float& elapsedTime);
    void Render();
    void DrawDebug();

public:// 取得・設定
    // ステートマシン
    StateMachine<State<ComboAttackStone>>* GetStateMachine() const { return stateMachine.get(); }

    Transform* GetOwnerTransform() { return ownerTransform; }
    void SetOwnerTransform(Transform* transform) { ownerTransform = transform; }

public:
    std::unique_ptr<StoneBall> stoneBall = nullptr;
    std::unique_ptr<MagicCircle> magicCircle = nullptr;

    bool isChangeState = false;

private:
    // ステートマシン
    std::unique_ptr<StateMachine<State<ComboAttackStone>>> stateMachine = nullptr;

    Transform* ownerTransform;
};

