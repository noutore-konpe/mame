#pragma once

#include <memory>
#include "Tutorial.h"

enum class TUTORIAL_STEP
{
    MOVE,
    ATTACK,
    AVOID,

    COUNT, // チュートリアル数
};

class TutorialManager
{
private:
    TutorialManager()  = default;
    ~TutorialManager() = default;

public:
    static TutorialManager& Instance()
    {
        static TutorialManager instance;
        return instance;
    }

    void Initialize();
    const bool Update(const float elapsedTime); // true：実行中 false：終了
    void Render();

    // 次のチュートリアルを設定(※チュートリアルが最後まで行ったらfalseを返す)
    const bool SetNextTutorial();

    // n番目のチュートリアル達成フラグをONにする
    void CompleteTutorialAt(const TUTORIAL_STEP& step) { tutorialCompleteFlags_[static_cast<int>(step)]; }

public:
    // 現在のチュートリアル番号取得
    [[nodiscard]] const int GetTutorialIndex() const { return static_cast<int>(tutorialstep_); }

public:
    // チュートリアルの数
    static constexpr int TUTORIAL_COUNT_ = static_cast<int>(TUTORIAL_STEP::COUNT);

private:
    std::unique_ptr<BaseTutorial> tutorial_;

    // 現在のチュートリアルステップ
    TUTORIAL_STEP tutorialstep_ = TUTORIAL_STEP::MOVE;

    // チュートリアル達成フラグ
    bool tutorialCompleteFlags_[static_cast<int>(TUTORIAL_STEP::COUNT)] = { false };

};

