#include "EnemyTestAI.h"

#include <memory>

#include "../Graphics/Graphics.h"
#include "JudgmentDerived.h"
#include "ActionDerived.h"

int EnemyTestAI::nameNum_ = 0;

EnemyTestAI::EnemyTestAI()
{
    using std::make_unique;
    using std::to_string;
    using SelectRule = BehaviorTree::SelectRule;

    Graphics& graphics = Graphics::Instance();

    model = {
        make_unique<Model>(graphics.GetDevice(),
        "./Resources/Model/Character/Player/P_Motion.fbx")
    };

    behaviorData_ = make_unique<BehaviorData>();
    behaviorTree_ = make_unique<BehaviorTree>(this);

    behaviorTree_->AddNode("", "Root", 0, SelectRule::Priority, nullptr, nullptr);
    behaviorTree_->AddNode("Root", "Pursuit", 1, SelectRule::Non, nullptr, new PursuitAction(this));

    // ImGuiñºëOê›íË
    SetName("EnemyTestAI_" + to_string(nameNum_++));
}


void EnemyTestAI::Initialize()
{
    moveSpeed_ = 2.0f;
    turnSpeed_ = 360.0f;
}


void EnemyTestAI::Update(const float& elapsedTime)
{
    BaseEnemyAI::Update(elapsedTime);
}


void EnemyTestAI::Render(const float& scale, ID3D11PixelShader* psShader)
{
    BaseEnemyAI::Render(scale, psShader);
}