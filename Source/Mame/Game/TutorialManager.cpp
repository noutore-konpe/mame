#include "TutorialManager.h"
#include "../Graphics/Graphics.h"

void TutorialManager::Initialize()
{
    // チュートリアルステップ初期化
    tutorialstep_ = TUTORIAL_STEP::MOVE;

    // 次のチュートリアル設定
    SetNextTutorial();

    // チュートリアル達成フラグリセット
    for (bool& flag : tutorialCompleteFlags_) { flag = false; }

}

const bool TutorialManager::Update(const float elapsedTime)
{
    // 現在のチュートリアルの達成フラグが立っていたら
    // 次のチュートリアルに移る
    if (true == tutorialCompleteFlags_[GetTutorialIndex()])
    {
        // 次のチュートリアルに移る
        const bool tutorialExecuteFlag = SetNextTutorial();

        // チュートリアルが最後まで行っていたら終了する
        if (false == tutorialExecuteFlag)
        {
            // チュートリアル終了
            return false;
        }

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

const bool TutorialManager::SetNextTutorial()
{
    Graphics& graphics = Graphics::Instance();

    // チュートリアルが最後まで行っていたらチュートリアル終了
    if (tutorialstep_ >= TUTORIAL_STEP::COUNT)
    {
        return false;
    }

    // チュートリアル分岐
    switch (tutorialstep_)
    {
    case TUTORIAL_STEP::MOVE: tutorial_ = std::make_unique<TutorialMove>(); break;
    }

    // チュートリアル初期化
    tutorial_->Initialize();

    // チュートリアルステップ加算
    tutorialstep_ = static_cast<TUTORIAL_STEP>(static_cast<int>(tutorialstep_) + 1);

    return true;
}
