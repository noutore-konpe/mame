#include "BaseEnemyAI.h"

#include "../../Taki174/Common.h"

BaseEnemyAI::~BaseEnemyAI()
{
    SafeDelete(activeNode_);
}

void BaseEnemyAI::Update(const float elapsedTime)
{
    // ���ݎ��s����m�[�h������΁A�r�w�C�r�A�c���[����m�[�h�����s
    if (activeNode_)
    {
        activeNode_ = aiTree_->Run(activeNode_, behaviorData_.get(), elapsedTime);
    }
    // ���ݎ��s����Ă���m�[�h���Ȃ���΁A���Ɏ��s����m�[�h�𐄘_����
    else
    {
        activeNode_ = aiTree_->ActiveNodeInference(behaviorData_.get());
    }

}

void BaseEnemyAI::Render(const float elapsedTime, const float scale)
{
    Character::Render(elapsedTime, scale);
}

// ImGui�p
void BaseEnemyAI::DrawDebug()
{
#ifdef USE_IMGUI
    if (ImGui::BeginMenu(GetName()))
    {
        std::string str = (activeNode_ != nullptr)
            ? activeNode_->GetName()
            : "";
        ImGui::Text(u8"Behavior�@%s", str.c_str());

        Character::DrawDebug();

        float range = GetRange();
        ImGui::DragFloat("range", &range);
        SetRange(range);

        ImGui::EndMenu();
    }
#endif // USE_IMGUI
}
