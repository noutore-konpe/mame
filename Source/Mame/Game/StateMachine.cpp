#include "StateMachine.h"


// デストラクタ
StateMachine::~StateMachine()
{
    // 登録したtステートを削除する
    for (State* state : statePool)
    {
        delete state;
    }
    statePool.clear();
}

// 更新処理
void StateMachine::Update(float elapsedTime)
{
    currentState->Execute(elapsedTime);
}

// ステートセット
void StateMachine::SetState(int newState)
{
    currentState = statePool.at(newState);
    currentState->Enter();
}

// ステート変更
void StateMachine::ChangeState(int newState)
{
    currentState->Exit();
    SetState(newState);
}

// ステート登録
void StateMachine::RegisterState(State* state)
{
    statePool.emplace_back(state);
}

// ステート番号取得
int StateMachine::GetStateIndex()
{
    int i = 0;
    for (State* state : statePool)
    {
        if (state == currentState)
        {
            // i番号目のステートをリターン
            return i;
        }
        ++i;
    }

    // ステートが見つからなかったとき
    return -1;
}

// debug用
void StateMachine::DrawDebug()
{
#if USE_IMGUI
    if (ImGui::TreeNode("stateMachine"))
    {
        ImGui::Text(currentState->GetName());

        float temp = currentState->GetTimer();
        ImGui::DragFloat("time", &temp);

        //float moveSpeed = currentState->GetMoveSpeed();
        //ImGui::DragFloat("moveSpeed", &moveSpeed);

/*        float rotSpeed = currentState->GetRotationSpeed();
        ImGui::DragFloat("rotationSpeed", &rotSpeed)*/;

        ImGui::TreePop();
    }
#endif // USE_IMGUI
}
