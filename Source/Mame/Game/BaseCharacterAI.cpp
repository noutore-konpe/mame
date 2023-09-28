#include "BaseCharacterAI.h"

#include "../../Taki174/Common.h"


BaseCharacterAI::~BaseCharacterAI()
{
    SafeDelete(activeNode_);
}

void BaseCharacterAI::Update(const float elapsedTime)
{
    // ���ݎ��s����m�[�h������΁A�r�w�C�r�A�c���[����m�[�h�����s
    if (activeNode_ != nullptr)
    {
        activeNode_ = aiTree_->Run(activeNode_, behaviorData_.get(), elapsedTime);
    }
    // ���ݎ��s����Ă���m�[�h���Ȃ���΁A���Ɏ��s����m�[�h�𐄘_����
    else
    {
        activeNode_ = aiTree_->ActiveNodeInference(behaviorData_.get());
    }

}

void BaseCharacterAI::Render(const float elapsedTime, const float scale)
{
    Character::Render(elapsedTime, scale);
}

// ImGui�p
void BaseCharacterAI::DrawDebug()
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
