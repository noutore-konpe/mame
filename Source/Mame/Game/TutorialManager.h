#pragma once

#include <memory>
#include "Tutorial.h"

enum class TUTORIAL_STEP
{
    NO_TUTORIAL = -1,   // デフォルト
    MOVE,               // 移動
    MOVE_CAMERA,        // カメラ移動
    LOCK_ON,            // ロックオン
    LOW_ATTACK,         // 弱攻撃
    HIGH_ATTACK,        // 強攻撃
    AVOID,              // 回避
    LEVEL_UP,           // レベルアップ

    COUNT,              // チュートリアル数
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
    void DrawImGui();

    // 次のチュートリアルを設定(※チュートリアルが最後まで行っていたらfalseを返す)
    const bool SetNextTutorial();

    // チュートリアル達成フラグをONにする
    void CompleteTutorial() { tutorialCompleteFlags_ = true; }

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
    TUTORIAL_STEP tutorialStep_ = TUTORIAL_STEP::NO_TUTORIAL;

    // チュートリアル達成フラグ
    bool tutorialCompleteFlags_ = false;

};

