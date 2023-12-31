#include "EnemyTestAI.h"

#include <memory>
#include "../Graphics/Graphics.h"

int EnemyTestAI::nameNum_ = 0;

EnemyTestAI::EnemyTestAI()
{
    using std::make_unique;
    using std::to_string;
    using SelectRule = BehaviorTree::SelectRule;

    Graphics& graphics = Graphics::Instance();

    model = {
        make_unique<Model>(graphics.GetDevice(),
        "./Resources/Model/yoshiakiModel/Cube.fbx")
    };

    behaviorData_ = make_unique<BehaviorData>();
    behaviorTree_ = make_unique<BehaviorTree>(this);

    behaviorTree_->AddNode("", "Root", 0, SelectRule::Priority, nullptr, nullptr);

    // ImGui���O�ݒ�
    SetName("EnemyTestAI_" + to_string(nameNum_++));
}


void EnemyTestAI::Initialize()
{
}


void EnemyTestAI::Update(const float elapsedTime)
{
    BaseCharacterAI::Update(elapsedTime);
}


void EnemyTestAI::Render(const float elapsedTime, const float scale)
{
    BaseCharacterAI::Render(elapsedTime, scale);
}
