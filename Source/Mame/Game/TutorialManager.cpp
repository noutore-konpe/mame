#include "TutorialManager.h"
#include "../../Taki174/Common.h"
#include "../Graphics/Graphics.h"

void TutorialManager::Initialize()
{
    // チュートリアルステップ初期化
    tutorialStep_ = TUTORIAL_STEP::NO_TUTORIAL;

    // 次のチュートリアル設定
    SetNextTutorial();

    // チュートリアル達成フラグリセット
    tutorialCompleteFlags_ = false;

}

const bool TutorialManager::Update(const float elapsedTime)
{
    // チュートリアル達成フラグが立っていたら
    // 次のチュートリアルに移る
    if (true == tutorialCompleteFlags_)
    {
        // 次のチュートリアルに移る
        const bool tutorialExecuteFlag = SetNextTutorial();

        // チュートリアルが最後まで行っていたら終了する
        if (false == tutorialExecuteFlag)
        {
            // チュートリアル終了
            return false;
        }

        // チュートリアル達成フラグをリセット
        tutorialCompleteFlags_ = false;

        // チュートリアル実行中
        return true;
    }

    // チュートリアル更新
    tutorial_->Update(elapsedTime);

    // チュートリアル実行中
    return true;
}

void TutorialManager::Render()
{
    // チュートリアル描画
    tutorial_->Render();
}

void TutorialManager::DrawImGui()
{
#if USE_IMGUI

    if (ImGui::BeginMenu("TutorialManager"))
    {
        // チュートリアルImGui描画
        tutorial_->DrawImGui();
        ImGui::EndMenu();
    }

#endif
}

const bool TutorialManager::SetNextTutorial()
{
    Graphics& graphics = Graphics::Instance();

    // チュートリアルステップ加算
    ::IncrementEnumClass(&tutorialStep_);

    // チュートリアルが最後まで行っていたらチュートリアル終了
    if (tutorialStep_ >= TUTORIAL_STEP::COUNT)
    {
        return false;
    }

    // チュートリアル代入分岐
    switch (tutorialStep_)
    {
    case TUTORIAL_STEP::MOVE:        tutorial_ = std::make_unique<TutorialMove>();       break;
    case TUTORIAL_STEP::MOVE_CAMERA: tutorial_ = std::make_unique<TutorialMoveCamera>(); break;
    case TUTORIAL_STEP::LOCK_ON:     tutorial_ = std::make_unique<TutorialLockOn>();     break;
    case TUTORIAL_STEP::LOW_ATTACK:  tutorial_ = std::make_unique<TutorialLowAttack>();  break;
    case TUTORIAL_STEP::HIGH_ATTACK: tutorial_ = std::make_unique<TutorialHighAttack>(); break;
    case TUTORIAL_STEP::AVOID:       tutorial_ = std::make_unique<TutorialAvoid>();      break;
    case TUTORIAL_STEP::LEVEL_UP:    tutorial_ = std::make_unique<TutorialLevelUp>();    break;
    }

    // チュートリアル初期化
    tutorial_->Initialize();

    return true;
}
