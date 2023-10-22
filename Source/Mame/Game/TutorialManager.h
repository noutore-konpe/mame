#pragma once

#include <memory>
#include "../Resource/sprite.h"

enum class TUTORIAL
{
    MOVE,
    ATTACK,
    AVOID,

    COUNT, // チュートリアル数
};

// チュートリアルのスプライトパラメータ
struct TutorialSpriteParam
{
    // ※位置は含めない
    DirectX::XMFLOAT2 size_     = {};
    DirectX::XMFLOAT2 texSize_  = {};
    DirectX::XMFLOAT2 texPos_   = {};
    DirectX::XMFLOAT4 color_    = {};
    float             angle_    = 0.0f;
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

    const bool MoveNextTutorial();    // 次のチュートリアルに移る(※チュートリアルが最後まで行ったらfalseを返す)

    // n番目のチュートリアル達成フラグをONにする
    void CompleteTutorialAt(const TUTORIAL& index) { tutorialCompleteFlag_[static_cast<int>(index)]; }

public:
    [[nodiscard]] const int GetCurrentTutorialIndex() const { return currentTutorialIndex_; }

public:
    static constexpr int TUTORIAL_COUNT_ = static_cast<int>(TUTORIAL::COUNT); // チュートリアルの数

private:
    std::unique_ptr<Sprite> text_;              // テキスト
    std::unique_ptr<Sprite> checkMark_;         // チェックマーク
    std::unique_ptr<Sprite> textBackGround_;    // テキスト背景

    int currentTutorialIndex_ = 0;              // 現在のチュートリアル番号

    // チュートリアル達成フラグ
    bool tutorialCompleteFlag_[static_cast<int>(TUTORIAL::COUNT)] = { false };

private:
    // 各スプライトパラメータ
    TutorialSpriteParam textParam_ = {
        { 10.0f, 10.0f },
        { 100.0f, 100.0f },
        { 0.0f, 0.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f },
        0.0f,
    };
    TutorialSpriteParam checkMarkParam_ = {
        { 10.0f, 10.0f },
        { 100.0f, 100.0f },
        { 0.0f, 0.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f },
        0.0f,
    };
    TutorialSpriteParam textBackGroundParam_ = {
        { 10.0f, 10.0f },
        { 100.0f, 100.0f },
        { 0.0f, 0.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f },
        0.0f,
    };

};

