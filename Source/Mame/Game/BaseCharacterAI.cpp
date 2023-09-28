#include "BaseCharacterAI.h"

#include "../../Taki174/Common.h"


BaseCharacterAI::~BaseCharacterAI()
{
    SafeDelete(activeNode_);
}

void BaseCharacterAI::Update(const float elapsedTime)
{
    // 現在実行するノードがあれば、ビヘイビアツリーからノードを実行
    if (activeNode_ != nullptr)
    {
        activeNode_ = aiTree_->Run(activeNode_, behaviorData_.get(), elapsedTime);
    }
    // 現在実行されているノードがなければ、次に実行するノードを推論する
    else
    {
        activeNode_ = aiTree_->ActiveNodeInference(behaviorData_.get());
    }

}

void BaseCharacterAI::Render(const float elapsedTime, const float scale)
{
    Character::Render(elapsedTime, scale);
}

// ImGui用
void BaseCharacterAI::DrawDebug()
{
#ifdef USE_IMGUI
    if (ImGui::BeginMenu(GetName()))
    {
        std::string str = (activeNode_ != nullptr)
            ? activeNode_->GetName()
            : "";
        ImGui::Text(u8"Behavior　%s", str.c_str());

        Character::DrawDebug();

        float range = GetRange();
        ImGui::DragFloat("range", &range);
        SetRange(range);

        ImGui::EndMenu();
    }
#endif // USE_IMGUI
}
