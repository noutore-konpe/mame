#include "EnemyTestAI.h"

#include <memory>

#include "../../Taki174/Common.h"
#include "../Graphics/Graphics.h"


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
    aiTree_       = make_unique<BehaviorTree>(this);

    aiTree_->AddNode("", "Root", 0, SelectRule::Priority, nullptr, nullptr);

    // ImGuiñºëOê›íË
    SetName("EnemyTestAI" + to_string(nameNum++));
}


void EnemyTestAI::Initialize()
{
}


void EnemyTestAI::Update(const float elapsedTime)
{
    BaseEnemyAI::Update(elapsedTime);
}


void EnemyTestAI::Render(const float elapsedTime, const float scale)
{
    BaseEnemyAI::Render(elapsedTime, scale);
}
