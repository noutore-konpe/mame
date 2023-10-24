#pragma once

#include <memory>
#include "Tutorial.h"

enum class TUTORIAL_STEP
{
    MOVE,           // 移動
    MOVE_CAMERA,    // カメラ移動
    LOCK_ON,        // ロックオン
    ATTACK,         // 攻撃
    AVOID,          // 回避
    LEVEL_UP,       // レベルアップ

    COUNT,          // チュートリアル数
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

    // 次のチュートリアルを設定(※チュートリアルが最後まで行っていたらfalseを返す)
    const bool SetNextTutorial();

    // n番目のチュートリアル達成フラグをONにする
    void CompleteTutorialAt(const TUTORIAL_STEP step) { tutorialCompleteFlags_[static_cast<int>(step)] = true; }

public:
    // 現在のチュートリアル番号取得
    [[nodiscard]] const int GetTutorialIndex() const { return static_cast<int>(tutorialStep_); }

public:
    // チュートリアルの数
    static constexpr int TUTORIAL_COUNT_ = static_cast<int>(TUTORIAL_STEP::COUNT);

private:
    // チュートリアル
    std::unique_ptr<BaseTutorial> tutorial_;

    // 現在のチュートリアルステップ
    TUTORIAL_STEP tutorialStep_ = TUTORIAL_STEP::MOVE;

    // チュートリアル達成フラグ
    bool tutorialCompleteFlags_[TUTORIAL_COUNT_] = {false};

};

