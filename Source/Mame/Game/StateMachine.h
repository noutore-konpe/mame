#pragma once

//#ifdef USE_IMGUI
//#include "../../External/imgui/imgui.h"
//#include "../../External/imgui/imgui_internal.h"
//#include "../../External/imgui/imgui_impl_dx11.h"
//#include "../../External/imgui/imgui_impl_win32.h"
//#endif

#include <vector>
#include "State.h"

// ステートマシーン
class StateMachine
{
public:
    StateMachine() {};
    ~StateMachine();

    // 更新処理
    void Update(float elapsedTime);

    // ステートセット
    void SetState(int newState);

    // ステート変更
    void ChangeState(int newState);

    // ステート登録
    void RegisterState(State* state);

    // 現在のステート番号取得
    int GetStateIndex();

    State* GetCurrentState() { return currentState; }

    // imgui用
    void DrawDebug();

    // 仮
    void SetMoveRight(bool r) { moveRight = r; }
    bool GetMoveRight() { return moveRight; }

private:
    // 現在のステート
    State* currentState = nullptr;
    // 各ステートを保持する配列
    std::vector<State*> statePool;

    // 仮にここに置きます
    bool moveRight = false;
};

